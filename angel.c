#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "math.h"
#include "string.h"
#include <time.h>





#include "rtblib.h"










/*void print_robot_state(void)
{
	char state[255];
	snprintf(state,255,"speed=%f \t"
						"energy=%f\t"
						"robots_left=%i\t"
						"object_find=%i\t"
						"dist_to_object=%f\t"
						"object_angle=%f\t",
						info->speed,
						info->energy,
						info->robots_left,
						info->object_find,
						info->dist_to_object,
						info->object_angle);
	debug(state);
}*/



void action_game_start(robot_info * info)
{
	Print("Get ready to die guys!\n");
	sweep(ROTATE_CANNON+ROTATE_RADAR,PI/2.0,-PI/2.0,PI/2.0);
	accelerate(info->maxspeed);
}
void action_info(robot_info * info)
{
	rotate_to(ROTATE_CANNON+ROTATE_RADAR,1.0,0.0);
}
void action_radar(robot_info * info)
{
	int i,count_shot;
	double acceleration,robot_rotate,radar_angle,sweepleft,sweepright,shot_energy;
	static enemy_found=0;
	static last_object=0;
	double rnd;
	if(info->robots_left>17){
		rotate(ROTATE_ROBOT,2*PI);
		accelerate(info->maxspeed);
		if(info->object_find==ROBOT)
			shoot(info->shotminenergy);
		return;
	}
	switch(info->object_find)
	{
	case MINE:{
				if(info->dist_to_object<7){
					shoot(info->shotminenergy);
				}
				rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,PI/4);break;}
	  case ROBOT:{//lets hit that guy
		  	  	  	  brake(0.3);
		  	  	  	  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
		  	  	  	  if(info->energy<15)
		  	  	  		  break;
		  	  	  	  rotate(ROTATE_ALL,0);
		  	  	  	  if(info->dist_to_object<7){
						  shot_energy=(info->shotmaxenergy)*(info->energy/info->robotmaxenergy)/
								info->speed+info->dist_to_object;
						  rnd=(rand()/RAND_MAX)*(2*PI);
						  shoot(shot_energy);
						  if(info->dist_to_object<6){
							  count_shot=10;
							  i=0;
							  while(i++<count_shot){
								  //rotate_amount(ROTATE_CANNON,info->cannonmaxrotate,info->object_angle+rnd);
								  rnd*=-1;
								  shoot(shot_energy);
							  }
							  Print("Die coward!\n");
						  }
						  break;
		  	  	  	  }
				  }
	  case SHOT:{//lets avoid THAT.
		 // shoot(info->shotminenergy);
		  break;
	  }
	  case WALL:{//lets avoid THAT
		  	  	  	  brake(1/info->dist_to_object);
		  	  	  	  rnd=info->speed*16;
					  if(info->dist_to_object<rnd){
						  brake(1.0);
						  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,PI/2.0);
						  accelerate(info->maxspeed);
						  rotate(ROTATE_RADAR+ROTATE_CANNON,0.1);
					  }
				  break;
			  }
	  case COOKIE:{//get it
		  	  	  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
		  	  	  if((info->dist_to_object>40)&&(info->robots_left>6)){


		  	  		  shoot(info->shotminenergy);
		  	  	  }
		  	  	  else
		  	  	  {
		  	  		 if(info->dist_to_object>5)
						  accelerate(info->maxspeed);
					  else
						  accelerate(info->maxspeed/1.2);
		  	  	  }
		  	  	  break;
	  }

	}
	last_object=info->object_find;
}

void action_collision(robot_info * info)
{
	//debug("collision!!");
	double acceleration,robot_rotate,radar_angle,sweepleft,sweepright,shot_energy;
	int i,count_shot;
	double rnd;
	switch(info->object_find)
		{
	case ROBOT:{//lets hit that guy
					  brake(1.0);
					  shot_energy=(info->shotmaxenergy)*(info->energy/info->robotmaxenergy)/4*info->dist_to_object;

					  if(info->dist_to_object<50){
						  shoot(shot_energy);
						  //rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
						  if(info->dist_to_object<10){
							  count_shot=20;
							  i=0;
							  while(i++<count_shot){
								  shoot(shot_energy);
							  }
							  Print("Die coward!\n");
						  }
						  else{
							  count_shot=((rand()/RAND_MAX)*5)/info->dist_to_object;
						  }
					  }


					break;
				 }
		  case SHOT:{
			  	  	  if(info->speed<1.0){
			  		  accelerate(info->maxspeed);
			  	  	  }
			  		  rotate(ROTATE_ALL,0);
					  rnd=rnd=(rand()/RAND_MAX)*2*PI;
					  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,rnd);
			  	  	  break;}
		  case WALL:{//lets avoid THAT
			  	  	rnd=rnd=(rand()/RAND_MAX)*2*PI;
			  	  	rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,rnd);
					accelerate(info->maxspeed);
					break;
				  }

		  case COOKIE:{//get it
			  	  	  Print("It was delicious!\n");
			  	  	  info->energy+=10;
			  	  	  break;
		  }
		}
}

void action_initialize(robot_info * info)
{
	if(info->int_msg_value==1)//first round
	{
		debug("initializingggg!!!!!!!");
		name("Angel");
		colour("ff0433","aaffaa");
	}
}


void action_rotation_reach(robot_info * info)
{
	//shoot(info->shotminenergy);
}

int main(int argc, char * argv[])
{
  robot_info info;
  robot_option(USE_NON_BLOCKING,1);
  robot_option(SIGNAL,SIGUSR1);
  robot_option(SEND_ROTATION_REACHED,1);

  set_message_action(COLLISION,action_collision);
  set_message_action(RADAR,action_radar);
  set_message_action(GAME_STARTS,action_game_start);
  set_message_action(INFO,action_info);
  set_message_action(INITIALIZE,action_initialize);
  set_message_action(ROTATION_REACHED,action_rotation_reach);

  set_work_info(&info);
  basic_initialize(&info);
  signal(SIGUSR1, &read_robot);


  srand(time(NULL));

  info.object_find=-1;
  for( ;!info.exit_robot ;sleep(1));
  return(EXIT_SUCCESS);
}
