# define some Makefile variables for the compiler and compiler flags
# to use Makefile variables later in the Makefile: $()
#
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
#
# for C++ define  CC = g++
CC = g++
CFLAGS  = -g -Wall 

# typing 'make' will invoke the first target entry in the file 
# (in this case the default target entry)
# you can name this target entry anything, but "default" or "all"
# are the most commonly used names by convention
#
default: UndeadInfection

# To create the executable file UndeadInfection we need the object files
# UndeadInfection.o Model/RandomClass.o Model/Grid.o Agents/Agent.o:
#
UndeadInfection:  UndeadInfection.o Model/RandomClass.o Model/Grid.o Agents/Agent.o 
	$(CC) $(CFLAGS) -o -fopenmp -std=c++11 UndeadInfection.exe Model/RandomClass.o Model/Grid.o Agents/Agent.o

RandomClass.o:  Model/RandomClass.cpp 
	$(CC) $(CFLAGS) -c -fopenmp -std=c++11 Model/RandomClass.cpp

Grid.o:  Model/Grid.cpp 
	$(CC) $(CFLAGS) -c -fopenmp -std=c++11 Model/Grid.o

Agent.o:  Agents/Agent.cpp 
	$(CC) $(CFLAGS) -c -fopenmp -std=c++11 Agents/Agent.cpp

# To start over from scratch, type 'make clean'.  This
# removes the executable file, as well as old .o object
# files and *~ backup files:
#
clean: 
	$(RM) UndeadInfection *.o *~