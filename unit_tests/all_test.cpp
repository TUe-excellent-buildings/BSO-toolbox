#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "all_bso"
#endif

#include <boost/test/included/unit_test.hpp>

#include <unit_tests/utilities/trim_and_cast_test.cpp>
#include <unit_tests/utilities/geometry_test.cpp>
#include <unit_tests/utilities/data_handling_test.cpp>
#include <unit_tests/spatial_design/ms_space_test.cpp>
#include <unit_tests/spatial_design/ms_building_test.cpp>
#include <unit_tests/spatial_design/sc_building_test.cpp>
#include <unit_tests/spatial_design/conformal_test.cpp>
#include <unit_tests/spatial_design/xml/xml_test.cpp>
#include <unit_tests/structural_design/structural_design_test.cpp>
#include <unit_tests/building_physics/building_physics_test.cpp>
#include <unit_tests/grammar/grammar_test.cpp>