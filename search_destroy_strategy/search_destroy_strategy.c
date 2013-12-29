/*
 * search_destroy_strategy.c
 *
 *  Created on: 20/04/2013
 *      Author: angel
 */
#include <stdlib.h>

#include "rtblib.h"


/*
 * does noting
 */
void search_destroy_do_nothing(robot_info * info){
	debug("search destroy doing nothing\n");
	return;
}

void perform_basics(robot_info * info)
{
	rotate_to(ROTATE_CANNON+ROTATE_RADAR,1.0,0.0);
	sweep(ROTATE_RADAR,info->radarmaxrotate,-PI/4,PI/4);

}

/**
 * starts this strategy
 * @param info the information about the robot
 */
void search_destroy_do_game_starts(robot_info * info)
{
	Print("Get ready to die guys!\n");
	accelerate(info->maxspeed/2);
	perform_basics(info);
	rotate(ROTATE_ALL,info->robotmaxrotate);
	srand(time(NULL));
	basic_initialize(info);
}

/**
 * action about the message info
 * @param info the information about the robot
 */
void search_destroy_do_info(robot_info * info)
{
	perform_basics(info);
}

/**
 * action to do when radar event received
 * @param info the information about the robot
 */
void search_destroy_do_radar(robot_info * info)
{
	debug("search destroy radar");
	#define NOTHING	0
	#define GET_COOKIE	1


	int i,count_shot;
	double acceleration,robot_rotate,radar_angle,sweepleft,sweepright,shot_energy;
	static unsigned char enemy_found=0;
	static unsigned char last_object=0;
	static unsigned char doing=NOTHING;
	double rnd=rand()/RAND_MAX;
	static int dir=-1;

	shot_energy=0;
	if(info->robotmaxenergy>0){
		shot_energy=(info->shotmaxenergy)*(info->energy/info->robotmaxenergy);
		if((info->speed+info->dist_to_object)!=0){
			 shot_energy/=info->speed+info->dist_to_object;
		}
	}
	if(shot_energy<info->shotminenergy){
		shot_energy=info->shotminenergy;
	}

	dir=dir*-1;

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
				rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,(PI/2));
				break;
		}
	  case ROBOT:{//lets hit that guy
				  brake(1);
				  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
//				  if((info->energy<10) && (info->dist_to_object>15) )
//					  break;
				  Print("Die coward!");
				  shoot(shot_energy);
				  accelerate(info->maxspeed);
				  perform_basics(info);
		  	  	  break;
				  }
	  case WALL:{//lets avoid THAT

					  if(info->dist_to_object<=10){
						  brake(10);
						  rotate(ROTATE_ROBOT+ROTATE_RADAR+ROTATE_CANNON,info->robotmaxrotate);
						  accelerate(info->maxspeed/4);
					  }
				  break;
	 			 }
	  case SHOT:{//lets avoid THAT.
		  	  accelerate(info->maxspeed);
		  	  //rotate(ROTATE_ROBOT,0.1);
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
							  accelerate(info->maxspeed);
					  }
					  break;
		  	  	  }
	  }

	}
//	//info->object_find=-1;
//	last_object=info->object_find;
}

/**
 * action to do when a collision message is received
 * @param info the information about the robot
 */
void search_destroy_do_collision(robot_info * info)
{
	debug("search destroy collision");
	double acceleration,robot_rotate,radar_angle,sweepleft,sweepright,shot_energy;
	int i,count_shot;
	double rnd=rand()/RAND_MAX;
	static int dir=-1;
	shot_energy=0;
	if(info->robotmaxenergy>0){
		shot_energy=(info->shotmaxenergy)*(info->energy/info->robotmaxenergy);
		if((info->speed+info->dist_to_object)!=0){
			 shot_energy/=info->speed+info->dist_to_object;
		}
	}
	if(shot_energy<info->shotminenergy){
		shot_energy=info->shotminenergy;
	}


	dir=dir*-1;
	switch(info->object_find)
		{
	case ROBOT:{//lets hit that guy
		 	 	  brake(0.8);
				  shoot(shot_energy);
				  if(info->dist_to_object<6){
					  count_shot=10;
					  i=0;
					  while(i++<count_shot){
						  shoot(shot_energy);
					  }
					  Print("Die coward!\n");
				  }
				  break;
				 }
		  case SHOT:{
			  		  accelerate(info->maxspeed);
			  	  	  break;}
		  case WALL:{//lets avoid THAT
			  	  	rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,rnd*8*PI);
					accelerate(info->maxspeed/3);
					break;
				  }

		  case COOKIE:{//get it
			  	  	  Print("It was delicious!\n");
			  	  	  info->energy+=10;
			  	  	  break;
		  }
		}
	//info->object_find=-1;
}

/**
 * handles the initialize message
 * @param info the information about the robot
 */
void search_destroy_do_initialize(robot_info * info)
{
	if(info->int_msg_value==1)//first round
	{
		debug("initializingggg!!!!!!!");
		name("Angel");
		colour("ff0433","aaffaa");
	}
}

/**
 * handles rotation reached message
 * @param info the information about the robot
 */
void search_destroy_do_rotation_reached(robot_info * info)
{
	//shoot(info->shotminenergy);
}

static type_action * search_destroy_strategy[NUM_MESSAGES_TO_ROBOT];

/**
 * gets the search destroy strategy
 */
type_action **get_search_destroy_strategy(void)
{

	search_destroy_strategy[INITIALIZE]=search_destroy_do_initialize;

	search_destroy_strategy[YOUR_NAME]=search_destroy_do_nothing;

	search_destroy_strategy[YOUR_COLOUR]=search_destroy_do_nothing;

	search_destroy_strategy[GAME_OPTION]=search_destroy_do_nothing;

	search_destroy_strategy[GAME_STARTS]=search_destroy_do_game_starts;

	search_destroy_strategy[RADAR]=search_destroy_do_radar;

	search_destroy_strategy[INFO]=search_destroy_do_info;

	search_destroy_strategy[COORDINATES]=search_destroy_do_nothing;

	search_destroy_strategy[ROBOT_INFO]=search_destroy_do_nothing;

	search_destroy_strategy[ROTATION_REACHED]=search_destroy_do_rotation_reached;

	search_destroy_strategy[ENERGY]=search_destroy_do_nothing;

	search_destroy_strategy[ROBOTS_LEFT]=search_destroy_do_nothing;

	search_destroy_strategy[COLLISION]=search_destroy_do_collision;

	search_destroy_strategy[WARNING]=search_destroy_do_nothing;

	search_destroy_strategy[DEAD]=search_destroy_do_nothing;

	search_destroy_strategy[GAME_FINISHES]=search_destroy_do_nothing;

	search_destroy_strategy[EXIT_ROBOT]=search_destroy_do_nothing;

	return (type_action**)search_destroy_strategy;
}

