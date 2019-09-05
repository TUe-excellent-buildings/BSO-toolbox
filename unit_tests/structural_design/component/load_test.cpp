#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_load_component_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/component/load.hpp>

#include <sstream>
#include <stdexcept>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace component_test {
using namespace bso::structural_design::component;

BOOST_AUTO_TEST_SUITE( sd_load_case_test )
	
	BOOST_AUTO_TEST_CASE( empty_init )
	{
		load_case lc;
		BOOST_REQUIRE(lc.description() == "");
		lc = load_case("test_case");
		BOOST_REQUIRE(lc.description() == "test_case");
		std::stringstream checkStream;
		checkStream << lc;
		BOOST_REQUIRE(checkStream.str() == "test_case");
	}
	
	BOOST_AUTO_TEST_CASE( description_init )
	{
		load_case lc("test_case");
		BOOST_REQUIRE(lc.description() == "test_case");
		std::stringstream checkStream;
		checkStream << lc;
		BOOST_REQUIRE(checkStream.str() == "test_case");
	}

	BOOST_AUTO_TEST_CASE( offstream_operator )
	{
		load_case lc("test_case");
		std::stringstream checkStream;
		checkStream << lc;
		BOOST_REQUIRE(checkStream.str() == "test_case");
	}
	
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( sd_load_component )
	
	BOOST_AUTO_TEST_CASE( load_test )
	{
		load_case lc("test_case");
		load l1(lc,300,0);
		
		BOOST_REQUIRE(l1.loadCase() == lc);
		BOOST_REQUIRE(l1.magnitude() == 300);
		BOOST_REQUIRE(l1.DOF() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( invalid_DOF )
	{
		load_case lc("test_case");
		BOOST_REQUIRE_THROW(load l1(lc,300,-1), std::invalid_argument);
		BOOST_REQUIRE_THROW(load l1(lc,300, 6), std::invalid_argument);
		BOOST_REQUIRE_NO_THROW(load l1(lc,300,0));
		BOOST_REQUIRE_NO_THROW(load l1(lc,300,3));
		BOOST_REQUIRE_NO_THROW(load l1(lc,300,5));
	}
	
	BOOST_AUTO_TEST_CASE( arithmetic )
	{
		load_case lc("test_case");
		load l1(lc,100,0);
		
		l1 += 100;
		BOOST_REQUIRE(l1.magnitude() == 200);
		
		l1 = l1 + 100;
		BOOST_REQUIRE(l1.magnitude() == 300);
		
		l1 -= 50;
		BOOST_REQUIRE(l1.magnitude() == 250);
		
		l1 = l1 - 50;
		BOOST_REQUIRE(l1.magnitude() == 200);
		
		l1 *= 2;
		BOOST_REQUIRE(l1.magnitude() == 400);
		
		l1 = l1 * 1.5;
		BOOST_REQUIRE(l1.magnitude() == 600);
		
		l1 /= 3;
		BOOST_REQUIRE(l1.magnitude() == 200);
		
		l1 = l1 / 2;
		BOOST_REQUIRE(l1.magnitude() == 100);
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace component_test