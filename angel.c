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
	#define NOTHING	0
	#define GET_COOKIE	1


	int i,count_shot;
	double acceleration,robot_rotate,radar_angle,sweepleft,sweepright,shot_energy;
	static unsigned char enemy_found=0;
	static unsigned char last_object=0;
	static unsigned char doing=NOTHING;
	int dir=(rand()/RAND_MAX>0.6)? 1:-1;

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
				rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,(PI/2)*dir);
				break;
		}
	  case ROBOT:{//lets hit that guy
		  	  	  	  brake(0.8);
		  	  	  	  rotate(ROTATE_ALL,0);
		  	  	  	  if(info->energy<15)
		  	  	  		  break;
		  	  	  	  if(info->dist_to_object<12){
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
		  	  	  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
		  	  	  rotate_to(ROTATE_CANNON+ROTATE_RADAR,info->cannonmaxrotate,info->object_angle+rand()*dir);
		  	  	  rnd=40;
				  if(info->dist_to_object<rnd)
					  brake(10/info->dist_to_object);
				  else
					  accelerate(info->maxspeed);
		  	  	  break;
				  }
	  case WALL:{//lets avoid THAT

					  rnd=40;
					  if(info->dist_to_object<rnd){
						  brake(10/info->dist_to_object);
						  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,2*PI*rand()*dir);
						  accelerate(info->maxspeed);
						  rotate(ROTATE_RADAR+ROTATE_CANNON,0.1);
					  }
				  break;
	 			 }
	  case SHOT:{//lets avoid THAT.
		  if((info->dist_to_object<20)&&(info->dist_to_object<10))
			  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,rand()*2*PI*dir);
		  break;
	  }
	  case COOKIE:{//get it
		  	  	  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
		  	  	  if((info->dist_to_object>40)&&(info->robots_left>6)){


		  	  		  shoot(info->shotminenergy);
		  	  	  }
		  	  	  else
		  	  	  {
		  	  		  if(doing!=GET_COOKIE){
		  	  			  doing=GET_COOKIE;
						 if(info->dist_to_object>5)
							  accelerate(info->maxspeed);
						  else
							  accelerate(info->maxspeed/1.2);
						  }
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
	int dir=(rand()/RAND_MAX>0.6)? 1:-1;
	switch(info->object_find)
		{
	case ROBOT:{//lets hit that guy
		 	 	  brake(0.8);
				  rotate(ROTATE_ALL,0);
				  if(info->energy<15)
					  break;
				  if(info->dist_to_object<12){
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
			  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
			  rotate_to(ROTATE_CANNON+ROTATE_RADAR,info->cannonmaxrotate,info->object_angle+rand()*dir);
			  rnd=40;
			  if(info->dist_to_object<rnd)
				  brake(10/info->dist_to_object);
			  else
				  accelerate(info->maxspeed);
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
