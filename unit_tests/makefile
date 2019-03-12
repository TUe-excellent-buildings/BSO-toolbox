#libraries
BOOST   = /usr/include/boost
EIGEN   = /usr/include/eigen
BSO     = /home/sjonnie/5_hobby_and_learning/toolbox
ALL_LIB = -I$(BOOST) -I$(EIGEN) -I$(BSO)

#compiler settings
CPP   = g++ -std=c++14
FLAGS  = -O3 -march=native -lboost_unit_test_framework

#test suites
MS_SPACE    = $(BSO)/unit_tests/spatial_design/ms_space_test.cpp
MS_BUILDING = $(BSO)/unit_tests/spatial_design/ms_building_test.cpp
SC_BUILDING = $(BSO)/unit_tests/spatial_design/sc_building_test.cpp
CONFORMAL 	= $(BSO)/unit_tests/spatial_design/conformal_test.cpp
TRIM_CAST   = $(BSO)/unit_tests/utilities/trim_and_cast_test.cpp
GEOMETRY		= $(BSO)/unit_tests/utilities/geometry_test.cpp
BSO_ALL			= $(BSO)/unit_tests/all_test.cpp

.PHONY: 
	all
	ms_space
	ms_building
	sc_building
	conformal
	trim_cast
	bso_vectors
	geometry
	clean 


#make arguments
cls:
	# this argument clears the screen, so that it is easier to find the beginning of the compiler output
	@clear
	@clear
all:
	$(CPP) -o all_test $(ALL_LIB) $(BSO_ALL) $(FLAGS)
ms_space:
	$(CPP) -o ms_space_test $(ALL_LIB) $(MS_SPACE) $(FLAGS)
ms_building:
	$(CPP) -o ms_building_test $(ALL_LIB) $(MS_BUILDING) $(FLAGS)
sc_building:
	$(CPP) -o sc_building_test $(ALL_LIB) $(SC_BUILDING) $(FLAGS)
conformal:
	$(CPP) -o conformal_test $(ALL_LIB) $(CONFORMAL) $(FLAGS)
trim_cast:
	$(CPP) -o trim_cast_test $(ALL_LIB) $(TRIM_CAST) $(FLAGS)
geometry:
	$(CPP) -o geometry_test $(ALL_LIB) $(GEOMETRY) $(FLAGS)
clean:
	@rm -f ms_space_test
	@rm -f ms_building_test
	@rm -f sc_building_test
	@rm -f conformal_test
	@rm -f trim_cast_test
	@rm -f geometry_test
	@rm -f all_test