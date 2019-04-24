#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_node"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/component/load_case.hpp>

#include <sstream>

using namespace bso::structural_design::component;

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

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