#libraries
BOOST   = /usr/include/boost
EIGEN   = /usr/include/eigen
BSO     = ..
ALL_LIB = -I$(BOOST) -I$(EIGEN) -I$(BSO)

#compiler settings
CPP   = g++ -std=c++14
FLAGS  = -O3 -march=native -lboost_unit_test_framework -lglut -lGL -lGLU -lpthread

#test suites
MS_SPACE    = $(BSO)/unit_tests/spatial_design/ms_space_test.cpp
MS_BUILDING = $(BSO)/unit_tests/spatial_design/ms_building_test.cpp
SC_BUILDING = $(BSO)/unit_tests/spatial_design/sc_building_test.cpp
CONFORMAL 	= $(BSO)/unit_tests/spatial_design/conformal_test.cpp
TRIM_CAST   = $(BSO)/unit_tests/utilities/trim_and_cast_test.cpp
GEOMETRY		= $(BSO)/unit_tests/utilities/geometry_test.cpp
STRUCT_DES	= $(BSO)/unit_tests/structural_design/structural_design_test.cpp
BUILD_PHYS  = $(BSO)/unit_tests/building_physics/building_physics_test.cpp
VISUALIZE   = $(BSO)/unit_tests/visualization/visualization_test.cpp
BSO_ALL			= $(BSO)/unit_tests/all_test.cpp
XML				  = $(BSO)/unit_tests/spatial_design/xml/xml_test.cpp
DATA				= $(BSO)/unit_tests/utilities/data_handling_test.cpp
GRAMMAR			= $(BSO)/unit_tests/grammar/grammar_test.cpp

.PHONY: all ms_space ms_building sc_building conformal trim_cast geometry building_physics structural_design clean visualization xml data grammar

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
structural_design:
	$(CPP) -o sd_test $(ALL_LIB) $(STRUCT_DES) $(FLAGS)
building_physics:
	$(CPP) -o bp_test $(ALL_LIB) $(BUILD_PHYS) $(FLAGS)
visualization:
	$(CPP) -o vis_test $(ALL_LIB) $(VISUALIZE) $(FLAGS)
xml:
	$(CPP) -o xml_test $(ALL_LIB) $(XML) $(FLAGS)
data:
	$(CPP) -o data_test $(ALL_LIB) $(DATA) $(FLAGS)
grammar:
	$(CPP) -o grammar_test $(ALL_LIB) $(GRAMMAR) $(FLAGS)
clean:
	@rm -f ms_space_test
	@rm -f ms_building_test
	@rm -f sc_building_test
	@rm -f conformal_test
	@rm -f trim_cast_test
	@rm -f geometry_test
	@rm -f sd_test
	@rm -f bp_test
	@rm -f all_test
	@rm -f vis_test
	@rm -f xml_test
	@rm -f data_test
	@rm -f grammar_test