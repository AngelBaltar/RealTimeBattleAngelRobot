/*
 * hide_escape_strategy.c
 *
 *  Created on: 20/04/2013
 *      Author: angel
 */
#include <stdlib.h>

#include "rtblib.h"


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
	#define NOTHING	0
	#define GET_COOKIE	1
	#define ESCAPE 2
	static int doing=NOTHING;

	int dir=(rand()/RAND_MAX>0.6)? 1:-1;

	double rnd;


	switch(info->object_find)
	{
	case MINE:{break;}
	  case ROBOT:{shoot(info->shotminenergy);break;}
	  case WALL:{

		  if(info->dist_to_object<7){
			  brake(1.0);
			  rotate(ROTATE_ALL,info->robotmaxrotate);
		  }
		  break;}
	  case SHOT:{break;}
	  case COOKIE:{shoot(info->shotminenergy);break;}

	}

}

/**
 * handles collision
 * @param info the information about the robot
 */
void hide_escape_do_collision(robot_info * info)
{
	#define NOTHING	0
	#define GET_COOKIE	1
	#define ESCAPE 2
	static int doing=NOTHING;

	//debug("collision!!");
	int dir=(rand()/RAND_MAX>0.6)? 1:-1;

		double rnd;

		switch(info->object_find)
		{
		case MINE:{break;}
		  case ROBOT:{break;}
		  case WALL:{brake(1.0);doing!=NOTHING;break;}
		  case SHOT:{
			  	  	  if(doing!=ESCAPE){
			  	  		 rotate(ROTATE_ALL,0);
			  	  		 rotate_amount(ROTATE_ROBOT,info->robotmaxrotate,PI/4);
			  	  		 accelerate(info->maxspeed);
			  	  		 doing=ESCAPE;
			  	  	  }
			  	  	  break;
		  	  	  }
		  case COOKIE:{doing!=NOTHING;break;}

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

