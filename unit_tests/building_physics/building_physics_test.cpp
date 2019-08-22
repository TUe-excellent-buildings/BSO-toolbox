#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "building_physics"
#endif

#include <boost/test/included/unit_test.hpp>

#include <unit_tests/building_physics/state_space_system_test.cpp>

#include <unit_tests/building_physics/properties/material_test.cpp>
#include <unit_tests/building_physics/properties/layer_test.cpp>
#include <unit_tests/building_physics/properties/construction_test.cpp>
#include <unit_tests/building_physics/properties/glazing_test.cpp>
#include <unit_tests/building_physics/properties/space_settings_test.cpp>

#include <unit_tests/building_physics/state/independent/ground_profile_test.cpp>
#include <unit_tests/building_physics/state/independent/weather_profile_test.cpp>

#include <unit_tests/building_physics/state/dependent/wall_test.cpp>
#include <unit_tests/building_physics/state/dependent/floor_test.cpp>
#include <unit_tests/building_physics/state/dependent/window_test.cpp>
#include <unit_tests/building_physics/state/dependent/space_test.cpp>

#include <unit_tests/building_physics/bp_model_test.cpp>