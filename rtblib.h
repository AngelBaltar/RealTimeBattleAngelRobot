/*
RealTimeBattle, a robot programming game for Unix
Copyright (C) 1998-2000  Erik Ouchterlony and Ragnar Ouchterlony

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#ifndef __RTBLIB__
#define __RTBLIB__

#include <stdio.h>


#define ROTATE_ROBOT	1
#define	ROTATE_CANNON	2
#define ROTATE_RADAR	4
#define ROTATE_ALL		7
#define PI	 3.14159265358 //979323846264338327950288419716939937510


enum message_to_robot_type 
{
  UNKNOWN_MESSAGE_TO_ROBOT = -1,
  INITIALIZE,
  YOUR_NAME,
  YOUR_COLOUR,
  //  BIN_DATA_TO,
  //  ASCII_DATA_TO,
  //  LOAD_DATA_FINISHED,
  GAME_OPTION,
  GAME_STARTS,
  RADAR,
  INFO,
  COORDINATES,
  ROBOT_INFO,
  ROTATION_REACHED,
  ENERGY,
  ROBOTS_LEFT,
  COLLISION,
  WARNING,
  DEAD,
  GAME_FINISHES,
  //  SAVE_DATA,
  EXIT_ROBOT
};

#define NUM_MESSAGES_TO_ROBOT 17

enum message_from_robot_type 
{
  UNKNOWN_MESSAGE_FROM_ROBOT = -1,
  ROBOT_OPTION,
  NAME,
  COLOUR,
  //  LOAD_DATA,
  ROTATE,
  ROTATE_TO,
  ROTATE_AMOUNT,
  SWEEP,
  ACCELERATE,
  BRAKE,
  BREAK,
  SHOOT,
  PRINT,
  DEBUG,
  DEBUG_LINE,
  DEBUG_CIRCLE

  //  BIN_DATA_FROM,
  //  ASCII_DATA_FROM,
  //  SAVE_DATA_FINISHED
};

enum argument_type
{
  NONE,
  STRING,
  DOUBLE,
  INT,
  HEX
  //  BINDATA
};

enum warning_type
{
  UNKNOWN_MESSAGE=0,
  PROCESS_TIME_LOW=1,
  //  ENERGY_LOW,
  //VARIABLE_OUT_OF_RANGE,
  MESSAGE_SENT_IN_ILLEGAL_STATE=2,
  UNKNOWN_OPTION=3,
  OBSOLETE_KEYWORD=4,
  NAME_NOT_GIVEN=5,
  COLOUR_NOT_GIVEN=6
};

enum game_option_type
{
  ROBOT_MAX_ROTATE=0,
  ROBOT_CANNON_MAX_ROTATE=1,
  ROBOT_RADAR_MAX_ROTATE=2,

  ROBOT_MAX_ACCELERATION=3,
  ROBOT_MIN_ACCELERATION=4,

  ROBOT_START_ENERGY=5,
  ROBOT_MAX_ENERGY=6,
  ROBOT_ENERGY_LEVELS=7,

  SHOT_SPEED=8,
  SHOT_MIN_ENERGY=9,
  SHOT_MAX_ENERGY=10,
  SHOT_ENERGY_INCREASE_SPEED=11,

  TIMEOUT=12,

  DEBUG_LEVEL=13,            // 0 - no debug, 5 - highest debug level

  SEND_ROBOT_COORDINATES= 14    // 0 - no coordinates, 
                                // 1 - coordniates are given relative the starting position
                                // 2 - absolute coordinates 
};

enum robot_option_type
{
  SIGNAL=2,                   // 0 - no signal, > 1 - signal to send (e.g. SIGUSR1 or SIGUSR2) 
  SEND_SIGNAL=0,              // 0 - false, 1 - true
  SEND_ROTATION_REACHED=1,    // 0 - no messages
                              // 1 - messages when RotateTo and RotateAmount finished
                              // 2 - messages also when sweep direction is changed

  USE_NON_BLOCKING=3          // 0 - false, 1 - true 
                              // This option should always be sent as soon as possible

};

enum object_type 
{ 
  NOOBJECT = -1, 
  ROBOT = 0, 
  SHOT = 1, 
  WALL = 2, 
  COOKIE = 3, 
  MINE = 4,
  LAST_OBJECT_TYPE = 5
};

static const int number_of_object_types = 5;

struct Message
{
  char* msg;
  int number_of_args;
  enum argument_type arg_type[4];
};

static const struct Message message_to_robot[25] = 
{
  {"Initialize", 1, {INT,    NONE,   NONE,   NONE}},   // arg: if 1 this is the first sequence for the robot, send name and colour!!
  {"YourName",   1, {STRING, NONE,   NONE,   NONE}},   // arg: previous name, send new name only if you don't like it
  {"YourColour", 1, {HEX,    NONE,   NONE,   NONE}},   // arg: previous colour
  //  {"BinData",    2, {INT,    BINDATA,NONE,   NONE}},
  //  {"AsciiData",  1, {STRING, NONE,   NONE,   NONE}},
  //  {"LoadDataFinished",0, {NONE,NONE, NONE,   NONE}},
  {"GameOption", 2, {INT,    DOUBLE, NONE,   NONE}},   // arg 1: OPTION_NR,  arg 2:  value 
  {"GameStarts", 0, {NONE,   NONE,   NONE,   NONE}},
  {"Radar",      3, {DOUBLE, INT,    DOUBLE, NONE}},   // first arg: distance, second arg: object_type, third arg: radar_angle
  {"Info",       3, {DOUBLE, DOUBLE, DOUBLE, NONE}},   // first arg: time, second arg: speed, third arg: cannon_angle
  {"Coordinates",3, {DOUBLE, DOUBLE, DOUBLE, NONE}},   // first arg: x, second arg: y, third arg: angle
  {"RobotInfo",  2, {DOUBLE, INT,    NONE,   NONE}},   // first arg: Other robots energylevel, second arg: enemy - 0, teammate - 1
  {"RotationReached",1,{INT, NONE,  NONE,   NONE}},    // first arg: what has finished rotation (see Rotate below)
  {"Energy",     1, {DOUBLE, NONE,   NONE,   NONE}},   // arg: energylevel
  {"RobotsLeft", 1, {INT,    NONE,   NONE,   NONE}},   // arg: robots left
  {"Collision",  2, {INT,    DOUBLE, NONE,   NONE}},   // first arg: object_type, second arg: collision angle
  {"Warning",    2, {INT,    STRING, NONE,   NONE}},   // first arg: warning_type, second arg: string
  {"Dead",       0, {NONE,   NONE,   NONE,   NONE}},   // Robot died  
  {"GameFinishes",0,{NONE,   NONE,   NONE,   NONE}},
  //  {"SaveData",   0, {NONE,   NONE,   NONE,   NONE}},

  {"ExitRobot",  0, {NONE,   NONE,   NONE,   NONE}},

  {"",           0, {}}
};

static const struct Message message_from_robot[25] = 
{
  {"RobotOption",  2, {INT, INT}},        // arg 1: OPTION_NR,  arg 2:  value 
  {"Name",         1, {STRING}},             // arg: name
  {"Colour",       2, {HEX, HEX}},           // first arg: home colour, second arg: away colour
  //  {"LoadData",     1, {INT}},                // arg: type of data, 0 - binary, 1 - ascii
  {"Rotate",       2, {INT, DOUBLE}},     // first arg: what to rotate: 1 Robot, 2 cannon, 4 radar and apropriate sums for combinations                                                             // second arg: angular velocity (rad/s)
  {"RotateTo",     3, {INT, DOUBLE, DOUBLE}},     // first and second arg: as in 'Rotate', third arg: angle to move to
  {"RotateAmount", 3, {INT, DOUBLE, DOUBLE}},     // first and second arg: as in 'Rotate', third arg: angle to rotate
  {"Sweep",        4, {INT, DOUBLE, DOUBLE, DOUBLE}}, // first and second arg: as in 'Rotate', but only cannon and radar
                                                      // third arg: left_angle, fourth arg: right_angle  (relative robot)
  {"Accelerate", 1, {DOUBLE}},             // arg: accelerate (m/s^2)  
  {"Brake",        1, {DOUBLE}},             // arg: brake precentage, 0 = no brake, 1 = full brake 
  {"Break",        1, {DOUBLE}},             // Wrong spelling, included for compabillity reasons.
  {"Shoot",        1, {DOUBLE}},             // arg: amount of energy
  {"Print",        1, {STRING}},             // arg: message to print   
  {"Debug",        1, {STRING}},             // arg: debug message
  {"DebugLine",    4, {DOUBLE, DOUBLE, DOUBLE, DOUBLE}}, // arg1,2: startpoint angle, radius (relative robot)
                                                         // arg3,4: endpoint angle, radius (relative robot)
  {"DebugCircle",  3, {DOUBLE, DOUBLE, DOUBLE}},         // arg1,2: centerpoint angle, radius (relative robot)
                                                         // arg3: circle radius
  //  {"BinData",      2, {INT, BINDATA}},    // first arg: number of bytes,  second arg: data
  //  {"AsciiData",    1, {STRING}},          // arg: data
  //  {"SaveDataFinished",0, {}},
  {"",             0, {}}
};

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

	double maxspeed,
		  robotmaxrotate,
		  cannonmaxrotate,
		  radarmaxrotate,
		  dist,
		  prev_dist;
	double robotstartenergy,
			robotmaxenergy,
			robotenergylevels,
			shotspeed,
			shotmaxenergy,
			shotminenergy,
	shotenergyincreasespeed
	,timeout;

	unsigned char exit_robot;

}robot_info;

typedef  void (type_action)(robot_info *);

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

enum message_to_robot_type name2msg_to_robot_type(char* msg_name);
void read_robot(int sig);
void set_work_info(robot_info * data_to_work);
void set_message_action(enum message_to_robot_type code, type_action *action);
void basic_initialize(robot_info * data_to_work);


#endif 
