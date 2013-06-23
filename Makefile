CC = gcc
CCFLAGS = -Wall
SRC = tema3.c
PROGRAM = tema3
FIS1 = list.c
FIS2 = hash.c

all:
	$(CC) -o $(PROGRAM) $(SRC) $(FIS1) $(FIS2) $(CCFLAGS)


.PHONY : clean
clean :
	rm -f $(PROGRAM) core *~


