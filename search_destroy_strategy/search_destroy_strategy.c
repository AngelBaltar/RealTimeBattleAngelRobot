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

/**
 * starts this strategy
 * @param info the information about the robot
 */
void search_destroy_do_game_starts(robot_info * info)
{
	Print("Get ready to die guys!\n");
	sweep(ROTATE_CANNON+ROTATE_RADAR,PI/2.0,-PI/2.0,PI/2.0);
	accelerate(info->maxspeed);
	srand(time(NULL));
}

/**
 * action about the message info
 * @param info the information about the robot
 */
void search_destroy_do_info(robot_info * info)
{
	rotate_to(ROTATE_CANNON+ROTATE_RADAR,1.0,0.0);
}

/**
 * action to do when radar event received
 * @param info the information about the robot
 */
void search_destroy_do_radar(robot_info * info)
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

/**
 * action to do when a collision message is received
 * @param info the information about the robot
 */
void search_destroy_do_collision(robot_info * info)
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

