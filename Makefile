#Makefile
#
# Created on: 05/04/2013
#     Author: angel

C_FILES= rtblib.c \
		search_destroy_strategy/search_destroy_strategy.c \
		angel.c 
all: 
	gcc -I ./Include $(C_FILES) -O2 -lm -o angel.robot
        
clean:
	rm -f *.robot
