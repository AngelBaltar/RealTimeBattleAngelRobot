#include <stdlib.h>
#include <signal.h>

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
  robot_option(USE_NON_BLOCKING,1);
  robot_option(SIGNAL,SIGUSR1);
  robot_option(SEND_ROTATION_REACHED,1);


  set_work_info(&info);
  basic_initialize(&info);
  signal(SIGUSR1, &read_robot);

  info.object_find=-1;
  for( ;!info.exit_robot ;sleep(1)){
	  if(info.robots_left>4){
		  set_strategy(get_hide_escape_strategy());
	  }
	  else{
		  set_strategy(get_search_destroy_strategy());
	  }


  }
  return(EXIT_SUCCESS);
}
