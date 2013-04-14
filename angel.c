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
	case MINE:{shoot(info->shotminenergy);break;}
	  case ROBOT:{//lets hit that guy
		  	  	  	  brake(0.8);
		  	  	  	  if(info->dist_to_object<40){
						  shot_energy=(info->shotmaxenergy)*(info->energy/info->robotmaxenergy)/
								info->speed*info->dist_to_object*info->dist_to_object;
						 // rotate(ROTATE_ALL,0);
						  rnd=(rand()/RAND_MAX)*(2*PI);
						  shoot(shot_energy);
						  if(info->dist_to_object<12){
							  count_shot=20;
							  Print("Die coward!\n");
						  }
						  else{
							  count_shot=((rand()/RAND_MAX)*10)/info->dist_to_object;
						  }
						  i=0;
						  while(i++<count_shot){
							  rotate_amount(ROTATE_CANNON,info->cannonmaxrotate,info->object_angle+rnd);
							  rnd*=-1;
							  shoot(shot_energy);
						  }
						  if(info->enemy_found<2){
							  sweep(ROTATE_CANNON+ROTATE_RADAR,PI/2.0,PI/2.0,-PI/2.0);
							  debug("set sweep!\n");
						  }
						  info->enemy_found+=2;
						  break;
		  	  	  	  }
				  }
	  case SHOT:{//lets avoid THAT.
		  if(info->enemy_found>2){
			  info->enemy_found--;
			  break;
		  }
		  if(info->dist_to_object<10){
			  brake(1.0);
			  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,PI/2.0);
			  accelerate(info->maxspeed);
		  }
		  break;
	  }
	  case WALL:{//lets avoid THAT
		  	  	  	  brake(1/info->dist_to_object);
		  	  	  	  if(info->enemy_found>2){
		  	  	  		  info->enemy_found--;
		  	  	  		  break;
		  	  	  	  }
		  	  	  	  rnd=info->speed*5;
					  if(info->dist_to_object<rnd){
						  brake(1.0);
						  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,PI/2.0);
						  accelerate(info->maxspeed/2);
					  }
				  break;
			  }
	  case COOKIE:{//get it
		  	  	  //rotate(ROTATE_ALL,0.0);//stop
		  	  	  if(info->object_find!=last_object){
					  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
					  if(info->dist_to_object>5)
						  accelerate(info->maxspeed);
					  else
						  accelerate(info->maxspeed/1.2);

		  	  	  }
		  	  	  else{
		  	  		 if(info->dist_to_object<=5)
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
	info->enemy_found=0;
	int i,count_shot;
	double rnd;
	int direction;
	switch(info->object_find)
		{
	case ROBOT:{//lets hit that guy
					  brake(1.0);
					  shot_energy=(info->shotmaxenergy)*(info->energy/info->robotmaxenergy)/4*info->dist_to_object;
					  shoot(shot_energy);
					  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
					  if(info->dist_to_object<12){
						  count_shot=20;
						  Print("Die coward!\n");
					  }
					  else{
						  count_shot=((rand()/RAND_MAX)*5)/info->dist_to_object;
					  }
					  i=0;
					  while(i++<count_shot){
						  shoot(shot_energy);
					  }
					  info->enemy_found+=2;

						  break;
						  }
		  case SHOT:{
			  	  	  info->enemy_found-=3;
			  		  accelerate(info->maxspeed);
			  		  rotate(ROTATE_ALL,0);
			  		  if(rnd>0.5)
							direction=1;
					  else
							direction=-1;
					  rnd=rnd=(rand()/RAND_MAX)*2*PI;
					  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,rnd*direction);
			  	  	  break;}
		  case WALL:{//lets avoid THAT
			  	  	  rnd=rand()/RAND_MAX;
			  	  	  if(info->enemy_found>2){
						  info->enemy_found--;
						  break;
					  }
			  	  	if(rnd>0.5)
			  	  		direction=1;
			  	  	else
			  	  		direction=-1;
			  	  	rnd=rnd=(rand()/RAND_MAX)*2*PI;
			  	  	rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,rnd*direction);
					accelerate(-info->maxspeed);
					break;
				  }

		  case COOKIE:{//get it
			  	  	  Print("It was delicious!\n");
			  	  	  if(info->enemy_found>2){
						  info->enemy_found--;
						  break;
					  }
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

  set_work_info(&info);
  basic_initialize(&info);
  //name("Angel");
  //		colour("ff0433","aaffaa");

  signal(SIGUSR1, &read_robot);
  srand(time(NULL));

  info.object_find=-1;
  info.enemy_found=0;
  for( ;!info.exit_robot ;sleep(1));
  return(EXIT_SUCCESS);
}
