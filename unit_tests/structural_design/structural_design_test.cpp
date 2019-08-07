#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "structural_design"
#endif

#include <boost/test/included/unit_test.hpp>

#include <unit_tests/structural_design/element/node_test.cpp>
#include <unit_tests/structural_design/element/truss_test.cpp>
#include <unit_tests/structural_design/element/beam_test.cpp>
#include <unit_tests/structural_design/element/flat_shell_test.cpp>
#include <unit_tests/structural_design/element/quad_hexahedron_test.cpp>
#include <unit_tests/structural_design/component/structure_test.cpp>
#include <unit_tests/structural_design/component/load_test.cpp>
#include <unit_tests/structural_design/component/constraint_test.cpp>
#include <unit_tests/structural_design/component/point_test.cpp>
#include <unit_tests/structural_design/component/line_segment_test.cpp>
#include <unit_tests/structural_design/component/quadrilateral_test.cpp>
#include <unit_tests/structural_design/component/quad_hexahedron_test.cpp>
#include <unit_tests/structural_design/fea_test.cpp>
#include <unit_tests/structural_design/sd_model_test.cpp>