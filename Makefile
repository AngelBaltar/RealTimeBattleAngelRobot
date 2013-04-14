#Makefile
#
# Created on: 05/04/2013
#     Author: angel


all: 
	gcc rtblib.c angel.c -O2 -lm -o angel.robot
        
clean:
	rm -f *.robot
