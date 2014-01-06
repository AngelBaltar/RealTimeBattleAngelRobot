#include <stdlib.h>
#include <signal.h>
#include <time.h>

#include "rtblib.h"
#include "search_destroy_strategy/search_destroy_strategy.h"
#include "hide_escape_strategy/hide_escape_strategy.h"


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



int main(int argc, char * argv[])
{
  robot_info info;
  #define HIDE_ESCAPE 0
  #define SEARCH_DESTROY 1
  type_action ** all_strategies[]={
 		  	  	  	  	  	  	  	  get_hide_escape_strategy(),
 		  	  	  	  	  	  	  	  get_search_destroy_strategy(),
   	  	  	  	  	  	  	  	  };
  unsigned int select_strategy=0;
  unsigned int select_strategy_ant=0;

  robot_option(USE_NON_BLOCKING,1);
  robot_option(SIGNAL,SIGUSR1);
  robot_option(SEND_ROTATION_REACHED,1);


  set_work_info(&info);
  basic_initialize(&info);
  signal(SIGUSR1, &read_robot);
  set_strategy(all_strategies[1],&info);

  for( ;!info.exit_robot ;sleep(1)){

	  select_strategy_ant=select_strategy;

	  select_strategy=(info.robots_left>4)*HIDE_ESCAPE
			  	  	  +(info.robots_left<=4)*SEARCH_DESTROY;

	  if(select_strategy!=select_strategy_ant){
		  set_strategy(all_strategies[select_strategy],&info);
	  }
  }
  return(EXIT_SUCCESS);
}


//PROTOTYPE OF DYNAMIC STRATEGY CHANGING ROBOT
//int main(int argc, char * argv[])
//{
//  #define DEBUG_MSG_SIZE 255
//  robot_info info;
//  char debug_msg[DEBUG_MSG_SIZE];
//  type_action ** all_strategies[]={
//		  	  	  	  	  	  	  	  get_hide_escape_strategy(),
//		  	  	  	  	  	  	  	  get_search_destroy_strategy(),
//		  	  	  	  	  	  	  	  0
//  	  	  	  	  	  	  	  	  };
//
//  unsigned int actual_strategy=0;
//  unsigned step_bullet_hits=0;
//  unsigned last_bullet_hits=0;
//  unsigned total_robots;
//  unsigned steps=0;
//
//
//
//  robot_option(USE_NON_BLOCKING,1);
//  robot_option(SIGNAL,SIGUSR1);
//  robot_option(SEND_ROTATION_REACHED,1);
//
//  set_work_info(&info);
//  basic_initialize(&info);
//  signal(SIGUSR1, &read_robot);
//
//
//  set_strategy(all_strategies[actual_strategy],&info);
// // set_strategy(get_search_destroy_strategy());
//  total_robots=info.robots_left;
//  last_bullet_hits=info.bullet_hits;
//
//
//
//  for( ;!info.exit_robot ;sleep(1)){
//
////	  snprintf(debug_msg,DEBUG_MSG_SIZE,"time_lapse %i",steps);
////	  		debug(debug_msg);
//	if(steps>=100){  //check every 100 steps
//		step_bullet_hits=info.bullet_hits-last_bullet_hits;
//		last_bullet_hits=info.bullet_hits;
//
//		snprintf(debug_msg,DEBUG_MSG_SIZE,"step bullet hits was %i",step_bullet_hits);
//		debug(debug_msg);
//
//		if(step_bullet_hits>10){
//			actual_strategy++;
//			if(all_strategies[actual_strategy]==0)actual_strategy=0;
//			set_strategy(all_strategies[actual_strategy],&info);
//			snprintf(debug_msg,DEBUG_MSG_SIZE,"changing strategy to %i",actual_strategy);
//			debug(debug_msg);
//		}
//		steps=0;
//	}
//	steps++;
//  }
//  return(EXIT_SUCCESS);
//}
