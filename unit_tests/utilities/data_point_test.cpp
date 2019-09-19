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
	
	BOOST_AUTO_TEST_CASE( unsigned_int_initialization )
	{
		data_point p1(2);
		BOOST_REQUIRE(p1.size() == 2);
		BOOST_REQUIRE(p1(0) == 0.0);
		BOOST_REQUIRE(p1(1) == 0.0);
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
		BOOST_REQUIRE(abs(p1.calcSquaredDistanceTo(p2)/47.25 - 1) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( normalize_data )
	{
		data_point p1 = {1,1,1};
		data_point pStart = {0.5,0.0,1.5};
		data_point pEnd = {1.5,2.0,0.0};
		data_point mismatchedEnd = {1.5,2.0};
		BOOST_REQUIRE_THROW(p1.normalize(pStart,mismatchedEnd),std::invalid_argument);
		p1.normalize(pStart,pEnd);

		std::vector<double> checkValues = {0.5,0.5,1/3.0};
		BOOST_CHECK_EQUAL_COLLECTIONS(checkValues.begin(), checkValues.end(),p1.begin(),p1.end());
	}
	
	BOOST_AUTO_TEST_CASE( set_to_utopia_point )
	{
		std::vector<data_point> dataCollection = {{0.1,5,30},{10,3,40},{2,7,-1}};
		data_point utopia;
		std::vector<data_point> mismatchedData = {{1,2},{1,2,3}};
		BOOST_REQUIRE_THROW(utopia.setToUtopia(mismatchedData),std::invalid_argument);
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
		BOOST_REQUIRE_THROW(distopia.setToDistopia(mismatchedData),std::invalid_argument);
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
	
	BOOST_AUTO_TEST_CASE( aggregate_to_sum )
	{
		data_point dp1 = {1.0,1.0,1.0};
		data_point dp2 = {1.0,0.5,1.0};
		data_point dp3 = {1.0,1.0,3.0};
		
		BOOST_REQUIRE(dp1.aggregateToSum() == 3.0);
		BOOST_REQUIRE(dp2.aggregateToSum() == 2.5);
		BOOST_REQUIRE(dp3.aggregateToSum() == 5.0);
		
		data_point weights = {1.0,2.0,3.0};
		
		BOOST_REQUIRE(dp1.aggregateToSum(weights) == 6.0);
		BOOST_REQUIRE(dp2.aggregateToSum(weights) == 5.0);
		BOOST_REQUIRE(dp3.aggregateToSum(weights) == 12.0);
	}
	
	BOOST_AUTO_TEST_CASE( aggregate_to_product )
	{
		data_point dp1 = {1.0,1.0,1.0};
		data_point dp2 = {1.0,0.5,1.0};
		data_point dp3 = {1.0,1.0,3.0};
		
		BOOST_REQUIRE(dp1.aggregateToProduct() == 1.0);
		BOOST_REQUIRE(dp2.aggregateToProduct() == 0.5);
		BOOST_REQUIRE(dp3.aggregateToProduct() == 3.0);
		
		data_point weights = {1.0,2.0,3.0};
		
		BOOST_REQUIRE(dp1.aggregateToProduct(weights) == 6.0);
		BOOST_REQUIRE(dp2.aggregateToProduct(weights) == 3.0);
		BOOST_REQUIRE(dp3.aggregateToProduct(weights) == 18.0);
	}
	
	BOOST_AUTO_TEST_CASE( aggregate_to_distance )
	{
		data_point dp1 = {1.0,1.0,1.0};
		data_point dp2 = {1.0,0.5,1.0};
		data_point dp3 = {1.0,1.0,3.0};
		data_point measurePoint = {1.0,1.0,1.0};
		
		BOOST_REQUIRE(dp1.aggregateToDistance(measurePoint) == 0.0);
		BOOST_REQUIRE(dp2.aggregateToDistance(measurePoint) == 0.5);
		BOOST_REQUIRE(dp3.aggregateToDistance(measurePoint) == 2.0);
		
		data_point weights = {1.0,2.0,3.0};
		
		BOOST_REQUIRE(dp1.aggregateToDistance(measurePoint,weights) == 0.0);
		BOOST_REQUIRE(dp2.aggregateToDistance(measurePoint,weights) == 1.0);
		BOOST_REQUIRE(dp3.aggregateToDistance(measurePoint,weights) == 6.0);
	}
	
	BOOST_AUTO_TEST_CASE( addition_operator )
	{
		data_point p1 = {1,2,3};
		data_point p2 = {4,5,-6};
		data_point p3 = {7,8,9};
		data_point p4 = {1,2};

		BOOST_REQUIRE_NO_THROW(data_point p5 = p1+p2);;
		BOOST_REQUIRE_THROW(data_point p5 = p1+p4, std::invalid_argument);;
		data_point p5 = p1+p2;
		data_point p6 = p1+p3;
		BOOST_REQUIRE_THROW(p1+=p4, std::invalid_argument);
		p1+=p2;
		
		std::vector<double> check1 = {5,7,-3};
		std::vector<double> check2 = {8,10,12};
		
		BOOST_CHECK_EQUAL_COLLECTIONS(check1.begin(), check1.end(),
																	p1.begin(),p1.end());
		BOOST_CHECK_EQUAL_COLLECTIONS(check1.begin(), check1.end(),
																	p5.begin(),p5.end());
		BOOST_CHECK_EQUAL_COLLECTIONS(check2.begin(), check2.end(),
																	p6.begin(),p6.end());
	}
	
	BOOST_AUTO_TEST_CASE( subtraction_operator )
	{
		data_point p1 = {1,2,3};
		data_point p2 = {4,5,-6};
		data_point p3 = {7,8,9};
		data_point p4 = {1,2};

		BOOST_REQUIRE_NO_THROW(data_point p5 = p1-p2);;
		BOOST_REQUIRE_THROW(data_point p5 = p1-p4, std::invalid_argument);;
		data_point p5 = p1-p2;
		data_point p6 = p1-p3;
		BOOST_REQUIRE_THROW(p1-=p4, std::invalid_argument);
		p1-=p2;
		
		std::vector<double> check1 = {-3,-3,9};
		std::vector<double> check2 = {-6,-6,-6};
		
		BOOST_CHECK_EQUAL_COLLECTIONS(check1.begin(), check1.end(),
																	p1.begin(),p1.end());
		BOOST_CHECK_EQUAL_COLLECTIONS(check1.begin(), check1.end(),
																	p5.begin(),p5.end());
		BOOST_CHECK_EQUAL_COLLECTIONS(check2.begin(), check2.end(),
																	p6.begin(),p6.end());
	}
	
	BOOST_AUTO_TEST_CASE( multiplication_operator )
	{
		data_point p1 = {2.5,0.0,-1.2};
		BOOST_REQUIRE_NO_THROW(data_point p2 = p1*3);
		data_point p2 = p1*3;
		p1*=-2.5;
		
		std::vector<double> check1 = {-6.25,0.0,3.0};
		std::vector<double> check2 = {7.5,0.0,-1.2*3};
		
		BOOST_CHECK_EQUAL_COLLECTIONS(check1.begin(), check1.end(),
																	p1.begin(),p1.end());
		BOOST_CHECK_EQUAL_COLLECTIONS(check2.begin(), check2.end(),
																	p2.begin(),p2.end());
	}
	
	BOOST_AUTO_TEST_CASE( division_operator )
	{
		data_point p1 = {2.5,0.0,-1.2};
		BOOST_REQUIRE_NO_THROW(data_point p2 = p1/3);
		data_point p2 = p1/3;
		p1/=-2;
		
		std::vector<double> check1 = {-1.25,0.0,0.6};
		std::vector<double> check2 = {2.5/3,0.0,-1.2/3};
		
		BOOST_CHECK_EQUAL_COLLECTIONS(check1.begin(), check1.end(),
																	p1.begin(),p1.end());
		BOOST_CHECK_EQUAL_COLLECTIONS(check2.begin(), check2.end(),
																	p2.begin(),p2.end());
	}
	
	BOOST_AUTO_TEST_CASE( less_than_operator )
	{
		data_point p1 = {0,0,0};
		data_point p2 = {0,0,1};
		data_point p3 = {0,1,1};
		data_point p4 = {1,1,1};
		data_point p5 = {0,0,-1};
		data_point p6 = {0,0,0};
		data_point p7 = {1,1,1,1,1};
		
		BOOST_REQUIRE(p1 < p2);
		BOOST_REQUIRE(p1 < p3);
		BOOST_REQUIRE(p1 < p4);
		BOOST_REQUIRE(!(p1 < p5));
		BOOST_REQUIRE(!(p1 < p6));
		BOOST_REQUIRE_THROW(p1 < p7, std::invalid_argument);
		
		BOOST_REQUIRE(p1 <= p2);
		BOOST_REQUIRE(p1 <= p3);
		BOOST_REQUIRE(p1 <= p4);
		BOOST_REQUIRE(!(p1 <= p5));
		BOOST_REQUIRE(p1 <= p6);
		BOOST_REQUIRE_THROW(p1 <= p7, std::invalid_argument);
	}

	BOOST_AUTO_TEST_CASE( greater_than_operator )
	{
		data_point p1 = {0,0,0};
		data_point p2 = {0,0,-1};
		data_point p3 = {0,-1,-1};
		data_point p4 = {-1,-1,-1};
		data_point p5 = {0,0,1};
		data_point p6 = {0,0,0};
		data_point p7 = {1,1,1,1,1};
		
		BOOST_REQUIRE(p1 > p2);
		BOOST_REQUIRE(p1 > p3);
		BOOST_REQUIRE(p1 > p4);
		BOOST_REQUIRE(!(p1 > p5));
		BOOST_REQUIRE(!(p1 > p6));
		BOOST_REQUIRE_THROW(p1 > p7, std::invalid_argument);
		
		BOOST_REQUIRE(p1 >= p2);
		BOOST_REQUIRE(p1 >= p3);
		BOOST_REQUIRE(p1 >= p4);
		BOOST_REQUIRE(!(p1 >= p5));
		BOOST_REQUIRE(p1 >= p6);
		BOOST_REQUIRE_THROW(p1 >= p7, std::invalid_argument);
	}

	BOOST_AUTO_TEST_CASE( equality_operator )
	{
		data_point p1 = {1,2,3};
		data_point p2 = {1,2,3};
		data_point p3 = {1,2,3.5};
		data_point p4 = {0,0,0,0,0};
		
		BOOST_REQUIRE(p1 == p2);
		BOOST_REQUIRE(!(p1 == p3));
		BOOST_REQUIRE_THROW(p1 == p4, std::invalid_argument);
	}

	BOOST_AUTO_TEST_CASE( is_not_equality_operator )
	{
		data_point p1 = {1,2,3};
		data_point p2 = {1,2,3};
		data_point p3 = {1,2,3.5};
		data_point p4 = {0,0,0,0,0};
		
		BOOST_REQUIRE(!(p1 != p2));
		BOOST_REQUIRE(p1 != p3);
		BOOST_REQUIRE_THROW(p1 != p4, std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( is_approximately )
	{
		data_point p1 = {1,1,1};
		data_point p2 = {1,1,1};
		data_point p3 = {1.001,1,1};
		data_point p4 = {1,1.0000001,1};
		data_point p5 = {1,1};
		
		BOOST_REQUIRE( p1.isApproximately(p2));
		BOOST_REQUIRE( p1.isApproximately(p2,1e-9));
		BOOST_REQUIRE(!p1.isApproximately(p3,1e-9));
		BOOST_REQUIRE( p1.isApproximately(p3,1e-3));
		BOOST_REQUIRE( p3.isApproximately(p1,1e-3));
		BOOST_REQUIRE(!p1.isApproximately(p4,1e-9));
		BOOST_REQUIRE( p1.isApproximately(p4,1e-6));
		BOOST_REQUIRE( p4.isApproximately(p1,1e-6));
		BOOST_REQUIRE_THROW(p1.isApproximately(p5),std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( dominates_or_is_dominated_by )
	{
		data_point p1 = {1,1,1};
		data_point p2 = {1,1,1};
		data_point p3 = {1,2,1};
		data_point p4 = {1,1.0000001,1};
		data_point p5 = {1,1,0};
		data_point p6 = {1.5,1,0};
		data_point p7 = {1,1,1,1};
		
		BOOST_REQUIRE(!p1.dominates(p2));
		BOOST_REQUIRE(!p1.isDominatedBy(p2));
		BOOST_REQUIRE( p1.dominates(p3));
		BOOST_REQUIRE(!p1.isDominatedBy(p3));
		BOOST_REQUIRE( p1.dominates(p4));
		BOOST_REQUIRE(!p1.isDominatedBy(p4));
		BOOST_REQUIRE(!p1.dominates(p5));
		BOOST_REQUIRE( p1.isDominatedBy(p5));
		BOOST_REQUIRE(!p1.dominates(p6));
		BOOST_REQUIRE(!p1.isDominatedBy(p6));
		BOOST_REQUIRE_THROW(p1.dominates(p7), std::invalid_argument);
		BOOST_REQUIRE_THROW(p1.isDominatedBy(p7), std::invalid_argument);
		
		BOOST_REQUIRE(!p2.dominates(p1));
		BOOST_REQUIRE(!p2.isDominatedBy(p1));
		BOOST_REQUIRE(!p3.dominates(p1));
		BOOST_REQUIRE( p3.isDominatedBy(p1));
		BOOST_REQUIRE(!p4.dominates(p1));
		BOOST_REQUIRE( p4.isDominatedBy(p1));
		BOOST_REQUIRE( p5.dominates(p1));
		BOOST_REQUIRE(!p5.isDominatedBy(p1));
		BOOST_REQUIRE(!p6.dominates(p1));
		BOOST_REQUIRE(!p6.isDominatedBy(p1));
		BOOST_REQUIRE_THROW(p7.dominates(p1), std::invalid_argument);
		BOOST_REQUIRE_THROW(p7.isDominatedBy(p1), std::invalid_argument);
	}	

BOOST_AUTO_TEST_SUITE_END()
} // namespace utilities_test