#libraries
BOOST   = /usr/include/boost
EIGEN   = /usr/include/eigen
BSO 		= /usr/include/BSO-toolbox
ALL_LIB = -I$(BOOST) -I$(EIGEN) -I$(BSO)

#compiler settings
CPP   = g++ -std=c++14
FLAGS  = -O3 -march=native -lglut -lGL -lGLU -lpthread

#test suites
MAINFILE = scdp_performance_clusters.cpp

.PHONY: all cls clean

#make arguments
cls:
	# this argument clears the screen, so that it is easier to find the beginning of the compiler output
	@clear
	@clear
all:
	$(CPP) -o scdp_performance_clusters $(ALL_LIB) $(MAINFILE) $(FLAGS)
clean:
	@rm -f scdp_performance_clusters