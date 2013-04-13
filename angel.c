#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "math.h"
#include "string.h"
#include <time.h>

#define MESSAGE_TYPES_H "MessageTypes.h"

#define PI	3.14159265359

#include MESSAGE_TYPES_H

#define ROTATE_ROBOT	1
#define	ROTATE_CANNON	2
#define ROTATE_RADAR	4
#define ROTATE_ALL		7



typedef struct
{

	double speed;
	double energy;

	int object_find;
	double dist_to_object;
	double object_angle;

	double x_pos;
	double y_pos;
	double angle_pos;
	double cannon_angle_pos;

	int enemy_found;
	double enemy_energy;

	int robots_left;

}robot_info;


static robot_info info_robot;

double maxspeed =3.0, robotmaxrotate, cannonmaxrotate, radarmaxrotate, dist=0.0, prev_dist;
double robotstartenergy=100, robotmaxenergy=100, robotenergylevels=0, shotspeed=1, shotmaxenergy=2,shotminenergy=0.5,
shotenergyincreasespeed,timeout;

unsigned char exit_robot=0;

static inline void debug(const char * msg)
{
	printf("Debug %s\n",msg);
	fflush(stdout);
}

static inline void Print(const char * msg)
{
	printf("Print %s\n",msg);
	fflush(stdout);
}

static inline void robot_option(int option,int value)
{
	printf("RobotOption %i %i\n",option,value);
	fflush(stdout);
}

static inline void colour(const char* color1,const char* color2)
{
	printf("Colour %s %s\n",color1,color2);
	fflush(stdout);
}

static inline void name(const char* name)
{
	printf("Name %s\n",name);
	fflush(stdout);
}

static inline void accelerate(const double accel)
{
	printf("Accelerate %le\n",accel);
	fflush(stdout);
}

static inline void rotate(const int what,const double angular_speed)
{
	printf("Rotate %i %le\n",what,angular_speed);
	fflush(stdout);
}

static inline void shoot(const double shoot_energy)
{
	printf("Shoot %le\n",shoot_energy);
	fflush(stdout);
}

static inline void rotate_to(const int what,const double angular_speed,const double end_angle)
{
	printf("RotateTo %i %le %le\n",what,angular_speed,end_angle);
	fflush(stdout);
}

static inline void sweep(const int what,const double angular_speed,const double left,const double right)
{
	printf("Sweep %i %le %le %le\n",what,angular_speed,left,right);
	fflush(stdout);
}

static inline void rotate_amount(const int what,const double angular_speed,const double angle)
{
	printf("RotateAmount %i %le %le\n",what,angular_speed,angle);
	fflush(stdout);
}

static inline void brake(const double portion)
{
	printf("Brake %le\n",portion);
	fflush(stdout);
}

void print_robot_state(void)
{
	char state[255];
	snprintf(state,255,"speed=%f \t"
						"energy=%f\t"
						"robots_left=%i\t"
						"object_find=%i\t"
						"dist_to_object=%f\t"
						"object_angle=%f\t",
						info_robot.speed,
						info_robot.energy,
						info_robot.robots_left,
						info_robot.object_find,
						info_robot.dist_to_object,
						info_robot.object_angle);
	debug(state);
}

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

void action_game_start(void)
{
	Print("Get ready to die guys!\n");
	sweep(ROTATE_CANNON+ROTATE_RADAR,PI/2.0,-PI/2.0,PI/2.0);
	accelerate(maxspeed);
}
void action_info(void)
{

	rotate_to(ROTATE_CANNON+ROTATE_RADAR,1.0,0.0);
}
void action_radar(void)
{
	int i,count_shot;
	double acceleration,robot_rotate,radar_angle,sweepleft,sweepright,shot_energy;
	static enemy_found=0;
	static last_object=0;
	double rnd;
	if(info_robot.robots_left>20){
		rotate(ROTATE_ROBOT,2*PI);
		accelerate(maxspeed);
		if(info_robot.object_find==ROBOT)
			shoot(shotminenergy);
		return;
	}
	switch(info_robot.object_find)
	{
	case MINE:{shoot(shotminenergy);break;}
	  case ROBOT:{//lets hit that guy
		  	  	  	  brake(1.0);
		  	  	  	  if((info_robot.energy)>5){
						  shot_energy=(shotmaxenergy)*(info_robot.energy/robotmaxenergy)/
								info_robot.speed*info_robot.dist_to_object*info_robot.dist_to_object;
						  rotate(ROTATE_ALL,0);
						  rnd=(rand()/RAND_MAX)*(2*PI);
						  shoot(shot_energy);
						  if(info_robot.dist_to_object<12){
							  count_shot=20;
							  Print("Die coward!\n");
						  }
						  else{
							  count_shot=((rand()/RAND_MAX)*10)/info_robot.dist_to_object;
						  }
						  i=0;
						  while(i++<count_shot){
							  rotate_to(ROTATE_CANNON,robotmaxrotate,info_robot.object_angle+rnd);
							  rnd*=-1;
							  shoot(shot_energy);
						  }
						  if(info_robot.enemy_found<2){
							  sweep(ROTATE_CANNON+ROTATE_RADAR,PI/2.0,PI/2.0,-PI/2.0);
							  debug("set sweep!\n");
						  }
						  info_robot.enemy_found+=2;
						  break;
		  	  	  	  }
				  }
	  case SHOT:{//lets avoid THAT.
		  if(info_robot.enemy_found>2){
			  info_robot.enemy_found--;
			  break;
		  }
		  if(info_robot.dist_to_object<15){
			  brake(1.0);
			  rotate_amount(ROTATE_ROBOT,robotmaxrotate,PI/2.0);
			  accelerate(maxspeed);
		  }
		  break;
	  }
	  case WALL:{//lets avoid THAT
		  	  	  	  brake(1/info_robot.dist_to_object);
		  	  	  	  if(info_robot.enemy_found>2){
		  	  	  		  info_robot.enemy_found--;
		  	  	  		  break;
		  	  	  	  }
		  	  	  	  rnd=info_robot.speed*5;
					  if(info_robot.dist_to_object<rnd){
						  brake(1.0);
						  rotate_amount(ROTATE_ROBOT,robotmaxrotate,PI/2.0);
						  accelerate(maxspeed/2);
					  }
				  break;
			  }
	  case COOKIE:{//get it
		  	  	  //rotate(ROTATE_ALL,0.0);//stop
		  	  	  rotate_amount(ROTATE_ROBOT,robotmaxrotate,info_robot.object_angle);
		  	  	  if(info_robot.dist_to_object>5)
		  	  		  accelerate(maxspeed);
		  	  	  else
		  	  		  accelerate(maxspeed/1.2);
		  	  	  break;
	  }

	}
	last_object=info_robot.object_find;
}

void action_collision()
{
	//debug("collision!!");
	double acceleration,robot_rotate,radar_angle,sweepleft,sweepright,shot_energy;
	info_robot.enemy_found=0;
	int i,count_shot;
	double rnd;
	int direction;
	switch(info_robot.object_find)
		{
	case ROBOT:{//lets hit that guy
					  brake(1.0);
					  shot_energy=(shotmaxenergy)*(info_robot.energy/robotmaxenergy)/4*info_robot.dist_to_object;
					  shoot(shot_energy);
					  rotate_amount(ROTATE_ROBOT,robotmaxrotate,info_robot.object_angle);
					  if(info_robot.dist_to_object<12){
						  count_shot=20;
						  Print("Die coward!\n");
					  }
					  else{
						  count_shot=((rand()/RAND_MAX)*5)/info_robot.dist_to_object;
					  }
					  i=0;
					  while(i++<count_shot){
						  shoot(shot_energy);
					  }
					  info_robot.enemy_found+=2;

						  break;
						  }
		  case SHOT:{
			  	  	  info_robot.enemy_found-=3;
			  		  accelerate(maxspeed);
			  		  if(rnd>0.5)
							direction=1;
					  else
							direction=-1;
					  rnd=rnd=(rand()/RAND_MAX)*2*PI;
					  rotate_amount(ROTATE_ROBOT,robotmaxrotate,rnd*direction);
			  	  	  break;}
		  case WALL:{//lets avoid THAT
			  	  	  rnd=rand()/RAND_MAX;
			  	  	  if(info_robot.enemy_found>2){
						  info_robot.enemy_found--;
						  break;
					  }
			  	  	if(rnd>0.5)
			  	  		direction=1;
			  	  	else
			  	  		direction=-1;
			  	  	rnd=rnd=(rand()/RAND_MAX)*2*PI;
			  	  	rotate_amount(ROTATE_ROBOT,robotmaxrotate,rnd*direction);
					accelerate(-maxspeed);
					break;
				  }

		  case COOKIE:{//get it
			  	  	  Print("It was delicious!\n");
			  	  	  if(info_robot.enemy_found>2){
						  info_robot.enemy_found--;
						  break;
					  }
			  	  	  info_robot.energy+=10;
			  	  	  break;
		  }
		}
}

void read_robot(int sig)
{
   int err;
   int i;
   char buff[255];
   //fflushin();
   //debug("READING...");


   enum message_to_robot_type msg;

   int teammate;
   double tid,cannon_angle;
   err=scanf("%s\n",buff);
   msg=name2msg_to_robot_type(buff);
   while(err!=EOF){
		   switch(msg){
			   case INFO:
						err=scanf("%le\n%le\%le\n",&tid,&info_robot.speed,&cannon_angle);
						action_info();
						break;
			   case ROBOT_INFO:{
				   info_robot.object_find=ROBOT;
				   err=scanf("%le\%i\n",&info_robot.enemy_energy,&teammate);
				   action_radar();
				   break;
			   }

			  case ENERGY:{
						err=scanf("%le\n",&tid,&info_robot.energy);
						break;
					  }
			  case ROBOTS_LEFT:{
							err=scanf("%d\n",&info_robot.robots_left);
							 break;
						  }
			  case RADAR:
						{

						  //Radar [distance (double)] [observed object type (int)] [radar angle (double)]
						  double dist,angle;
						  int object;
						  err=scanf("%le\n",&info_robot.dist_to_object);
						  err=scanf("%d\n",&info_robot.object_find);
						  err=scanf("%le\n",&info_robot.object_angle);
						  action_radar();

						  break;
						}
			  case COORDINATES:{
						  err=scanf("%le\n",&info_robot.x_pos);
						  err=scanf("%le\n",&info_robot.y_pos);
						  err=scanf("%le\n",&info_robot.angle_pos);
							  break;
						  }

			  case COLLISION:
						{
						  err=scanf("%d\n",&info_robot.object_find);
						  err=scanf("%le\n",&info_robot.object_angle);
						  info_robot.dist_to_object=0;
						  action_collision();
						  break;
						}
			  case ROTATION_REACHED:{
										  break;
								  }
			  case GAME_STARTS:
				  	  	  action_game_start();
			            break;
			  case GAME_OPTION:{
						int optnr;
						double value;
						err=scanf("%i\n",&optnr);
						err=scanf("%le\n",&value);
						switch(optnr)
						  {
							  case ROBOT_MAX_ROTATE:
								robotmaxrotate = value;
								break;
							  case ROBOT_CANNON_MAX_ROTATE:
								cannonmaxrotate = value;
								break;
							  case ROBOT_RADAR_MAX_ROTATE:
								radarmaxrotate = value ;
								break;
							  case	ROBOT_MAX_ACCELERATION:
								  maxspeed=value;
								  break;
							  case ROBOT_START_ENERGY:
								robotstartenergy = value;
								info_robot.energy=value;
								break;
							  case ROBOT_MAX_ENERGY:
								robotmaxenergy = value;
								break;
							  case ROBOT_ENERGY_LEVELS:
								robotenergylevels = value;
								break;
							  case SHOT_SPEED:
								shotspeed = value;
								break;
							  case SHOT_MIN_ENERGY:
								shotminenergy = value;
								break;
							  case SHOT_MAX_ENERGY:
								shotmaxenergy = value;
								break;
							  case SHOT_ENERGY_INCREASE_SPEED:
								shotenergyincreasespeed = value;
								break;
							  case TIMEOUT:
								timeout = value;
								break;
						  }
						break;
					  }
			  case EXIT_ROBOT:{exit_robot=69;break;}
			  case GAME_FINISHES:{Print("I WIN!!!!!!!!!!!\n");break;}
			  default:{
							 //debug(buff);
							 break;
						  }
		   }
		   snprintf(buff,255,"");
		   err=scanf("%s\n",buff);
		   msg=name2msg_to_robot_type(buff);
   }
   signal (sig, &read_robot);
}



int main(int argc, char * argv[])
{
  robot_option(USE_NON_BLOCKING,1);
  robot_option(SIGNAL,SIGUSR1);
  robot_option(SEND_ROTATION_REACHED,1);

  signal(SIGUSR1, &read_robot);
  name("Angel");
  colour("ff0033","aaffaa");
  srand(time(NULL));

  info_robot.object_find=-1;
  info_robot.enemy_found=0;
  for( ;!exit_robot ;sleep(1));
  return(EXIT_SUCCESS);
}
