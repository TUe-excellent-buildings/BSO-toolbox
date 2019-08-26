#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE data_point_test
#endif

#include <bso/utilities/data_point.hpp>

#include <boost/test/included/unit_test.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace utilities_test {
using namespace bso::utilities;

BOOST_AUTO_TEST_SUITE( data_point_tests )

	BOOST_AUTO_TEST_CASE( empty_init )
	{
		data_point p1;
		BOOST_REQUIRE(p1.size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( eigen_vector_initialization )
	{
		Eigen::Vector3d data;
		data << 1.0, 2.0, 3.5;
		data_point p1(data);
		BOOST_REQUIRE(p1.size() == 3);
		BOOST_CHECK_EQUAL_COLLECTIONS(data.data(), data.data()+data.size(),p1.begin(),p1.end());
		BOOST_REQUIRE(p1(0) == 1.0);
		BOOST_REQUIRE(p1(1) == 2.0);
		BOOST_REQUIRE(p1(2) == 3.5);
	}
	
	BOOST_AUTO_TEST_CASE( initializer_list_initialization )
	{
		data_point p1 = {2.3,3.4,4.5};
		BOOST_REQUIRE(p1.size() == 3);
		BOOST_REQUIRE(p1(0) == 2.3);
		BOOST_REQUIRE(p1(1) == 3.4);
		BOOST_REQUIRE(p1(2) == 4.5);
	}
	
	BOOST_AUTO_TEST_CASE( std_vector_initialization )
	{
		std::vector<double> data = {8.7,6.5};
		data_point p1(data);
		BOOST_REQUIRE(p1.size() == 2);
		BOOST_CHECK_EQUAL_COLLECTIONS(data.begin(), data.end(),p1.begin(),p1.end());
		BOOST_REQUIRE(p1(0) == 8.7);
		BOOST_REQUIRE(p1(1) == 6.5);
	}
	
	BOOST_AUTO_TEST_CASE( calculate_distance )
	{
		data_point p1 = {0.0,1.0,3.0};
		data_point p2 = {-1.0,5.0,-2.5};
		data_point mismatchedData = {1,2,3,4};
		
		BOOST_REQUIRE_THROW(p1.calcDistanceTo(mismatchedData),std::invalid_argument);
		BOOST_REQUIRE(abs(p1.calcDistanceTo(p2)/6.87386354243 - 1) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( normalize_data )
	{
		data_point p1 = {1,1,1};
		data_point pStart = {0.5,0.0,1.5};
		data_point pEnd = {1.5,2.0,0.0};
		data_point mismatchedEnd = {1.5,2.0};
		BOOST_REQUIRE_THROW(p1.normalize(pStart,mismatchedEnd),std::invalid_argument)
		p1.normalize(pStart,pEnd);

		std::vector<double> checkValues = {0.5,0.5,1/3.0};
		BOOST_CHECK_EQUAL_COLLECTIONS(checkValues.begin(), checkValues.end(),p1.begin(),p1.end());
	}
	
	BOOST_AUTO_TEST_CASE( set_to_utopia_point )
	{
		std::vector<data_point> dataCollection = {{0.1,5,30},{10,3,40},{2,7,-1}};
		data_point utopia;
		std::vector<data_point> mismatchedData = {{1,2},{1,2,3}};
		BOOST_REQUIRE_THROW(p1.setToUtopia(mismatchedData),std::invalid_argument);
		utopia.setToUtopia(dataCollection);
		std::vector<double> checkValues = {0.1,3,-1};
		BOOST_CHECK_EQUAL_COLLECTIONS(checkValues.begin(), checkValues.end(),
																	utopia.begin(),utopia.end());
	}
	
	BOOST_AUTO_TEST_CASE( set_to_distopia_point )
	{
		std::vector<data_point> dataCollection = {{0.1,5,30},{10,3,40},{2,7,-1}};
		data_point distopia;
		std::vector<data_point> mismatchedData = {{1,2},{1,2,3}};
		BOOST_REQUIRE_THROW(p1.setToDistopia(mismatchedData),std::invalid_argument);
		distopia.setToDistopia(dataCollection);
		std::vector<double> checkValues = {10,7,40};
		BOOST_CHECK_EQUAL_COLLECTIONS(checkValues.begin(), checkValues.end(),
																	distopia.begin(),distopia.end());
	}
	
	BOOST_AUTO_TEST_CASE( find_closest_in_collection )
	{
		std::vector<data_point> dataCollection = {{0.1,5,30},{10,3,40},{2,7,-1},{2,7,1.0001}};
		data_point p1 = {0,0,0};
		std::vector<data_point> mismatchedData = {{1,1}};
		BOOST_REQUIRE_THROW(p1.findClosestIn(mismatchedData),std::invalid_argument);
		std::vector<data_point> closest = p1.findClosestIn(dataCollection);
		std::vector<std::vector<double> > checkValues = {{2,7,-1},{2,7,1.0001}};
		for (unsigned int i = 0; i < 2; ++i)
		{
			BOOST_CHECK_EQUAL_COLLECTIONS(checkValues[i].begin(), checkValues[i].end(),
																		closest[i].begin(), closest[i].end());
		}
	}
	
	BOOST_AUTO_TEST_CASE( find_furthest_in_collection )
	{
		std::vector<data_point> dataCollection = {{0.1,5,30},{10,3,40},{10,-2.99999,40},{2,7,-1}};
		data_point p1 = {0,0,0};
		std::vector<data_point> mismatchedData = {{1,1}};
		BOOST_REQUIRE_THROW(p1.findFurthestIn(mismatchedData),std::invalid_argument);
		std::vector<data_point> furthest = p1.findFurthestIn(dataCollection);
		std::vector<std::vector<double> > checkValues = {{10,3,40},{10,-2.99999,40}};
		for (unsigned int i = 0; i < 2; ++i)
		{
			BOOST_CHECK_EQUAL_COLLECTIONS(checkValues[i].begin(), checkValues[i].end(),
																		furthest[i].begin(), furthest[i].end());
		}
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace utilities_test