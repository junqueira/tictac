# Compiler binary:
CPP= g++-8

# Recommended compiler flags for speed:
#	OpenMP enabled
#	full binary code optimization
#	full error and warning reports
#	no range checking within BRKGA:
CFLAGS= -O3 -fopenmp -Wextra -Wall -Weffc++ -ansi -Woverloaded-virtual -Wno-deprecated -Wcast-align -Wpointer-arith -std=c++11

# Compiler flags for debugging; uncomment if needed:
#	range checking enabled in the BRKGA API
#	OpenMP disabled
#	no binary code optimization
#CFLAGS= -DRANGECHECK -Wextra -Wall -Weffc++ -ansi -pedantic -Woverloaded-virtual -Wcast-align -Wpointer-arith

OBJECTS= LoggiInstance.o LoggiSolver.o LoggiDecoder.o Population.o brkga-loggi.o

# Targets:
all: brkga-loggi

brkga-loggi: $(OBJECTS)
	$(CPP) $(CFLAGS) $(OBJECTS) -o brkga-loggi

brkga-loggi.o:
	$(CPP) $(CFLAGS) -c brkga-loggi.cpp

LoggiInstance.o:
	$(CPP) $(CFLAGS) -c LoggiInstance.cpp

LoggiSolver.o:
	$(CPP) $(CFLAGS) -c LoggiSolver.cpp

LoggiDecoder.o:
	$(CPP) $(CFLAGS) -c LoggiDecoder.cpp

Population.o:
	$(CPP) $(CFLAGS) -c brkgaAPI/Population.cpp

# Remove:
clean:
	rm -f brkga-stp $(OBJECTS)
