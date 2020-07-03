# specify location of libraries
BOOST = /usr/include/boost
EIGEN = /usr/include/eigen
BSO = //usr/include/BSO-toolbox
ALL_LIB = -I$(BOOST) -I$(EIGEN) -I$(BSO)

# compiler settings
CPP = g++ -std=c++14
FLAGS = -O3 -march=native -lglut -lGL -lGLU -lpthread

# specify file(s) to be compiled
MAINFILE = main.cpp

# specify name of executable
EXE = topopt_ele_wise

.PHONY: all clean

# definition of arguments for make command
# argument to call compiler and compile executable called "design_response_grammar"
all:
	$(CPP) -o topopt_ele_wise $(ALL_LIB) $(MAINFILE) $(FLAGS)
cls:
	clear
	clear
# remove previously compiled executable
clean:
	@rm -f $(EXE)
