/*
 * rtblib.c
 *
 *  Created on: 14/04/2013
 *      Author: angel
 */

#include "rtblib.h"



void nothing_action(robot_info * info){debug("NOTHING ACTION!!\n");return;}


static type_action  *actions[NUM_MESSAGES_TO_ROBOT]={nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action,
		nothing_action
};

static robot_info  *work_info=0;

/**
 * initializes basic info values
 */
void basic_initialize(robot_info * data_to_work)
{
	data_to_work->maxspeed=3.0;
	data_to_work->energy=100;
	data_to_work->radarmaxrotate=2*PI;
	data_to_work->exit_robot=0;
	data_to_work->radarmaxrotate=2*PI;
	data_to_work->shotmaxenergy=3;
	data_to_work->shotminenergy=1;
}

/**
 * sets the robot_info variable where the data will be read
 */
void set_work_info(robot_info * data_to_work)
{
	work_info=data_to_work;
}

/**
 * sets the default action to do when message_to_robot_type
 */
void set_message_action(enum message_to_robot_type code, type_action *action)
{
	actions[code]=action;
}

/**
 * converts a protocol message string into the code of the message
 */
enum message_to_robot_type name2msg_to_robot_type(char* msg_name)
{
  int i;
  for(i=0; message_to_robot[i].msg[0] != '\0'; i++)
    {
      if( strcmp(message_to_robot[i].msg, msg_name) == 0 )
        return (enum message_to_robot_type)i;
    }
  return UNKNOWN_MESSAGE_TO_ROBOT;
}

/**
 * reads from the robot protocol
 * use it to capture the read signal
 */
void read_robot(int sig)
{
   int err;
   int i;
   char buff[255];
   char aux[255];
   //fflushin();
   //debug("READING...");


   enum message_to_robot_type msg;

   int teammate;
   double tid,cannon_angle;
   err=scanf("%s\n",buff);
   msg=name2msg_to_robot_type(buff);
   while(err!=EOF){
	   snprintf(aux,255,"action for msg: %i\n",msg);
	   debug(aux);
		   switch(msg){
		   case INITIALIZE:{
			   	   	   err=scanf("%i\n",&work_info->int_msg_value);
			   	   	   break;}
			   case INFO:
						err=scanf("%le\n%le\%le\n",&tid,&work_info->speed,&cannon_angle);
						break;
			   case ROBOT_INFO:{
				   work_info->object_find=ROBOT;
				   err=scanf("%le\%i\n",&work_info->enemy_energy,&teammate);
				   break;
			   }

			  case ENERGY:{
						err=scanf("%le\n",&tid,&work_info->energy);
						break;
					  }
			  case ROBOTS_LEFT:{
							err=scanf("%d\n",&work_info->robots_left);
							 break;
						  }
			  case RADAR:
						{

						  //Radar [distance (double)] [observed object type (int)] [radar angle (double)]
						  double dist,angle;
						  int object;
						  err=scanf("%le\n",&work_info->dist_to_object);
						  err=scanf("%d\n",&work_info->object_find);
						  err=scanf("%le\n",&work_info->object_angle);

						  break;
						}
			  case COORDINATES:{
						  err=scanf("%le\n",&work_info->x_pos);
						  err=scanf("%le\n",&work_info->y_pos);
						  err=scanf("%le\n",&work_info->angle_pos);
							  break;
						  }

			  case COLLISION:
						{
						  err=scanf("%d\n",&work_info->object_find);
						  err=scanf("%le\n",&work_info->object_angle);
						  work_info->dist_to_object=0;
						  break;
						}
			  case ROTATION_REACHED:{
										  break;
								  }
			  case GAME_STARTS:{
			            break;}
			  case WARNING:{Print("ups i get a warning\n");break;}
			  case DEAD:{Print("im dead but i can kill someone form the other side\n");break;}
			  case GAME_OPTION:{
						int optnr;
						double value;
						err=scanf("%i\n",&optnr);
						err=scanf("%le\n",&value);
						switch(optnr)
						  {
							  case ROBOT_MAX_ROTATE:
								  work_info->robotmaxrotate = value;
								break;
							  case ROBOT_CANNON_MAX_ROTATE:
								  work_info->cannonmaxrotate = value;
								break;
							  case ROBOT_RADAR_MAX_ROTATE:
								  work_info->radarmaxrotate = value ;
								break;
							  case	ROBOT_MAX_ACCELERATION:
								  work_info->maxspeed=value;
								  break;
							  case ROBOT_START_ENERGY:
								  work_info->robotstartenergy = value;
								work_info->energy=value;
								break;
							  case ROBOT_MAX_ENERGY:
								  work_info->robotmaxenergy = value;
								break;
							  case ROBOT_ENERGY_LEVELS:
								  work_info->robotenergylevels = value;
								break;
							  case SHOT_SPEED:
								  work_info->shotspeed = value;
								break;
							  case SHOT_MIN_ENERGY:
								  work_info->shotminenergy = value;
								break;
							  case SHOT_MAX_ENERGY:
								  work_info->shotmaxenergy = value;
								break;
							  case SHOT_ENERGY_INCREASE_SPEED:
								  work_info->shotenergyincreasespeed = value;
								break;
							  case TIMEOUT:
								  work_info->timeout = value;
								break;
						  }
						break;
					  }
			  case EXIT_ROBOT:{work_info->exit_robot=69;break;}
			  case GAME_FINISHES:{Print("I WIN!!!!!!!!!!!\n");break;}
			  default:{
							 //debug(buff);
							 break;
						  }
		   }
		   if((msg>=0)&&(msg<NUM_MESSAGES_TO_ROBOT)){
			   actions[msg](work_info);
		   }
		   snprintf(buff,255,"");
		   err=scanf("%s\n",buff);
		   msg=name2msg_to_robot_type(buff);
   }
   signal (sig, &read_robot);
}
