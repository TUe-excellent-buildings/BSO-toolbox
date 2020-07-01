#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE cluster_test
#endif

#include <bso/utilities/cluster.hpp>

#include <boost/test/included/unit_test.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace utilities_test {
using namespace bso::utilities;

BOOST_AUTO_TEST_SUITE( cluster_tests )

	BOOST_AUTO_TEST_CASE( intitialization )
	{
		data_point p1 = {1.5,2,6.7};
		cluster c1(p1);
		BOOST_REQUIRE(c1.getData().size() == 0);
		BOOST_REQUIRE(c1.getCentroid()(0) == 1.5);
		BOOST_REQUIRE(c1.getCentroid()(1) == 2.0);
		BOOST_REQUIRE(c1.getCentroid()(2) == 6.7);
	}
	
	BOOST_AUTO_TEST_CASE( calculate_mean )
	{
		data_point p1 = {0.0,0.0,0.0};
		cluster c1(p1);
		auto p2 = std::make_shared<data_point>(data_point({5.4,93,-4.2}));
		c1.addDataPoint(p2);
		auto p3 = std::make_shared<data_point>(data_point({8.3,50,-1.2}));
		c1.addDataPoint(p3);
		auto p4 = std::make_shared<data_point>(data_point({0.1,75,2}));
		c1.addDataPoint(p4);
		
		BOOST_REQUIRE(c1.getData().size() == 3);
		BOOST_REQUIRE(abs(c1.calculateMean()(0)/4.6-1) < 1e-9);
		BOOST_REQUIRE(abs(c1.calculateMean()(1)/72.6666666667-1) < 1e-9);
		BOOST_REQUIRE(abs(c1.calculateMean()(2)/-1.1333333333-1) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( calculate_variance )
	{
		data_point p1 = {0.0,0.0,0.0};
		cluster c1(p1);
		auto p2 = std::make_shared<data_point>(data_point({5.4,93,-4.2}));
		c1.addDataPoint(p2);
		auto p3 = std::make_shared<data_point>(data_point({8.3,50,-1.2}));
		c1.addDataPoint(p3);
		auto p4 = std::make_shared<data_point>(data_point({0.1,75,2}));
		c1.addDataPoint(p4);
		
		BOOST_REQUIRE(c1.getData().size() == 3);
		BOOST_REQUIRE(abs(c1.calculateVariance()/ 328.82444444-1) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( calculate_distance_to_centroid )
	{
		data_point p1 = {0.0,0.0,0.0};
		cluster c1(p1);
		data_point p2 = {5.4,93,-4.2};

		BOOST_REQUIRE(abs(c1.calculateDistanceToCentroid(p2)/93.25127345 - 1) < 1e-9);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace utilities_test