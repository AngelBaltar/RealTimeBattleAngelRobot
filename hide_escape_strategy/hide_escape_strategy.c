/*
 * hide_escape_strategy.c
 *
 *  Created on: 20/04/2013
 *      Author: angel
 */
#include <stdlib.h>
#include <time.h>
#include "rtblib.h"


#define NOTHING	0
#define GET_COOKIE	1
#define ESCAPE 2
static int doing=NOTHING;

/*
 * does noting
 */
void hide_escape_do_nothing(robot_info * info){
	debug("hide scape doing nothing\n");
	return;
}

/**
 * handles game start
 * @param info the information about the robot
 */
void hide_escape_do_game_starts(robot_info * info)
{
	Print("Get ready to die guys!\n");
	brake(1.0);
	rotate(ROTATE_ALL,info->robotmaxrotate);
	srand(time(NULL));
}

/**
 * handles info message
 * @param info the information about the robot
 */
void hide_escape_do_info(robot_info * info)
{
	rotate_to(ROTATE_CANNON+ROTATE_RADAR,1.0,0.0);
}

/**
 * handles radar event message
 * @param info the information about the robot
 */
void hide_escape_do_radar(robot_info * info)
{

	int dir=(rand()/RAND_MAX>0.6)? 1:-1;

	double rnd;

	if((doing==ESCAPE)
		&&(info->object_find!=WALL)
		&&(info->object_find!=MINE)){//do nothing, continue scaping
		return;
	}

	switch(info->object_find)
	{
	case MINE:{
				if(info->dist_to_object<7){
					shoot(info->shotminenergy);
				}
				rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle+PI);
				break;
				}
	  case ROBOT:{shoot(info->shotminenergy);break;}
	  case WALL:{

				  if(info->dist_to_object<10){
					  brake(10/info->dist_to_object);
					  rotate(ROTATE_ALL,info->robotmaxrotate);
				  }
				  doing=NOTHING;
		  break;}
	  case SHOT:{break;}
	  case COOKIE:{
					  if(info->dist_to_object>12){//hit that cookie
							  shoot(info->shotminenergy);
					  }
					  else{//get that cookie
						  rotate(ROTATE_ALL,0);
						  rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,info->object_angle);
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

}

/**
 * handles collision
 * @param info the information about the robot
 */
void hide_escape_do_collision(robot_info * info)
{


	//debug("collision!!");
	int dir=(rand()/RAND_MAX>0.6)? 1:-1;

	double rnd;
	static clock_t last_shoot;
	double lapse_between_shoots;


	if(doing==ESCAPE){//do nothing, continue scaping
			return;
	}

	switch(info->object_find)
	{
	case MINE:{break;}
	  case ROBOT:{break;}
	  case WALL:{brake(1.0);doing=NOTHING;break;}
	  case SHOT:{
		  	  	  lapse_between_shoots=((double)clock() - last_shoot) / CLOCKS_PER_SEC;
				  if((doing!=ESCAPE)&&(lapse_between_shoots<2)){
					 rotate(ROTATE_ALL,0);
					 rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,PI/2);
					 accelerate(info->maxspeed);
					 doing=ESCAPE;
					 rotate(ROTATE_ALL,PI/8);
				  }
				  last_shoot=clock();
				  break;
			  }
	  case COOKIE:{break;}

	}
}

/**
 * handles initialize
 * @param info the information about the robot
 */
void hide_escape_do_initialize(robot_info * info)
{
	if(info->int_msg_value==1)//first round
	{
		debug("initializingggg!!!!!!!");
		name("Angel");
		colour("ff0433","aaffaa");
	}
}


void hide_escape_do_rotation_reached(robot_info * info)
{
	//shoot(info->shotminenergy);
}

static type_action * hide_escape_strategy[NUM_MESSAGES_TO_ROBOT];

/**
 * gets the search destroy strategy
 */
type_action **get_hide_escape_strategy(void)
{

	hide_escape_strategy[INITIALIZE]=hide_escape_do_initialize;

	hide_escape_strategy[YOUR_NAME]=hide_escape_do_nothing;

	hide_escape_strategy[YOUR_COLOUR]=hide_escape_do_nothing;

	hide_escape_strategy[GAME_OPTION]=hide_escape_do_nothing;

	hide_escape_strategy[GAME_STARTS]=hide_escape_do_game_starts;

	hide_escape_strategy[RADAR]=hide_escape_do_radar;

	hide_escape_strategy[INFO]=hide_escape_do_info;

	hide_escape_strategy[COORDINATES]=hide_escape_do_nothing;

	hide_escape_strategy[ROBOT_INFO]=hide_escape_do_nothing;

	hide_escape_strategy[ROTATION_REACHED]=hide_escape_do_rotation_reached;

	hide_escape_strategy[ENERGY]=hide_escape_do_nothing;

	hide_escape_strategy[ROBOTS_LEFT]=hide_escape_do_nothing;

	hide_escape_strategy[COLLISION]=hide_escape_do_collision;

	hide_escape_strategy[WARNING]=hide_escape_do_nothing;

	hide_escape_strategy[DEAD]=hide_escape_do_nothing;

	hide_escape_strategy[GAME_FINISHES]=hide_escape_do_nothing;

	hide_escape_strategy[EXIT_ROBOT]=hide_escape_do_nothing;

	return (type_action**)hide_escape_strategy;
}

