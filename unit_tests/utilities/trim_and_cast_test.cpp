#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE trim_and_cast
#endif

#include <bso/utilities/trim_and_cast.hpp>

#include <vector>
#include <stdexcept>
#include <iostream>

#include <boost/test/included/unit_test.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace utilities_test {
using namespace bso::utilities;

BOOST_AUTO_TEST_SUITE( trim_and_cast_tests )

	BOOST_AUTO_TEST_CASE( integer_1 ) {
		std::string s_1 = "1";
		int i_1 = 1;
		BOOST_CHECK(i_1 == trim_and_cast_int(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( integer_2 ) {
		std::string s_1 = "-0";
		int i_1 = 0;
		BOOST_CHECK(i_1 == trim_and_cast_int(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( integer_3 ) {
		std::string s_1 = "-1";
		int i_1 = -1;
		BOOST_CHECK(i_1 == trim_and_cast_int(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( integer_failure ) {
		std::string s_1 = "hey";
		BOOST_REQUIRE_THROW(trim_and_cast_int(s_1), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( unsigned_integer_1 ) {
		std::string s_1 = "1";
		int i_1 = 1;
		BOOST_CHECK(i_1 == trim_and_cast_uint(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( unsigned_integer_2 ) {
		std::string s_1 = "0";
		int i_1 = 0;
		BOOST_CHECK(i_1 == trim_and_cast_uint(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( unsigned_integer_failure_1 ) {
		std::string s_1 = "-1";
		BOOST_REQUIRE_THROW(trim_and_cast_uint(s_1), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( unsigned_integer_failure_2 ) {
		std::string s_1 = "hey";
		BOOST_REQUIRE_THROW(trim_and_cast_uint(s_1), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( unsigned_long_1 ) {
		std::string s_1 = "1";
		int i_1 = 1;
		BOOST_CHECK(i_1 == trim_and_cast_ulong(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( unsigned_long_2 ) {
		std::string s_1 = "0";
		int i_1 = 0;
		BOOST_CHECK(i_1 == trim_and_cast_ulong(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( unsigned_long_failure_1 ) {
		std::string s_1 = "-1";
		BOOST_REQUIRE_THROW(trim_and_cast_uint(s_1), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( unsigned_long_failure_2 ) {
		std::string s_1 = "hey";
		BOOST_REQUIRE_THROW(trim_and_cast_ulong(s_1), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( double_1 ) {
		std::string s_1 = "1.234";
		double d_1 = 1.234;
		BOOST_TEST(d_1 == trim_and_cast_double(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( double_2 ) {
		std::string s_1 = "-1.234";
		double d_1 = -1.234;
		BOOST_CHECK(d_1 == trim_and_cast_double(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( double_3 ) {
		std::string s_1 = "1";
		double d_1 = 1.0;
		BOOST_CHECK(d_1 == trim_and_cast_double(s_1));
	}
	
	BOOST_AUTO_TEST_CASE( double_failure ) {
		std::string s_1 = "hey";
		BOOST_REQUIRE_THROW(trim_and_cast_double(s_1), std::invalid_argument);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace utilities_test