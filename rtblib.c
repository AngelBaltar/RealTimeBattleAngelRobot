/*
 * rtblib.c
 *
 *  Created on: 14/04/2013
 *      Author: angel
 */

#include "rtblib.h"
#include <string.h>



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
	data_to_work->maxspeed=2.0;
	data_to_work->minspeed=-0.5;
	data_to_work->energy=100;
	data_to_work->maxenergy=120;
	data_to_work->robotmaxrotate=0.785398;
	data_to_work->radarmaxrotate=2.0944;
	data_to_work->exit_robot=0;
	data_to_work->cannonmaxrotate=1.5708;
	data_to_work->shotmaxenergy=30;
	data_to_work->shotminenergy=0.5;
	data_to_work->shotspeed=10;
	data_to_work->shootpenalty=0.075;
	data_to_work->bullet_hits=0;
	data_to_work->object_find=-1;
}

/**
 * sets the robot_info variable where the data will be read
 */
void set_work_info(robot_info * data_to_work)
{
	work_info=data_to_work;
}

/**
 * sets the default action to do when message_to_robot_type code is received
 */
void set_message_action(enum message_to_robot_type code, type_action *action)
{
	actions[code]=action;
}

/**
 * sets the strategy strategy_actions, the strategy defines an action to do for any message that can be
 * received
 */
void set_strategy(type_action ** strategy_actions,robot_info * info)
{
	unsigned i;
	for(i=0;i<NUM_MESSAGES_TO_ROBOT;i++){
		actions[i]=strategy_actions[i];
	}
	//actions[INITIALIZE](info);
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
						err=scanf("%le\n",&work_info->energy);
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
						  switch(work_info->object_find){
						  	  case SHOT:{work_info->bullet_hits++;break;}
						  	  default:{break;}
						  }
						  break;
						}
			  case ROTATION_REACHED:{
										  break;
								  }
			  case GAME_STARTS:{
			            break;}
			  case WARNING:{

				  	  	  	  err=scanf("%d\n",&i);
				  	  	  	  err=scanf("%s\n",aux);
				  	  	  	  switch(i){
								  case UNKNOWN_MESSAGE:{Print("warining unknown message");break;}
								  case PROCESS_TIME_LOW:{Print("warining process time low");break;}
								  case MESSAGE_SENT_IN_ILLEGAL_STATE:{Print("warining sent in illegal state");break;}
								  case UNKNOWN_OPTION:{Print("warining unknown option");break;}
								  case OBSOLETE_KEYWORD:{Print("warining obsolete keyword");break;}
								  case NAME_NOT_GIVEN:{Print("warining name not given");break;}
								  case COLOUR_NOT_GIVEN:{Print("warining colour not given");break;}
				  	  	  	  }
				  	  	  	  Print(aux);
				  	  	  	  break;}
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
			   snprintf(aux,255,"action for msg: %i\n",msg);
			   debug(aux);
			   actions[msg](work_info);
		   }
		   //snprintf(buff,255,"");
		   memset(buff,0,255);
		   err=scanf("%s\n",buff);
		   msg=name2msg_to_robot_type(buff);
   }
   signal (sig, &read_robot);
}
