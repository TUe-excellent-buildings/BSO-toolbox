#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE sc_building
#endif

#include <bso/spatial_design/sc_building.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <stdexcept>
#include <sstream>

#include <boost/test/included/unit_test.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace spatial_design_test {
using namespace bso::spatial_design;

bool compare_txt_files(std::string f1, std::string f2)
{
	std::ifstream in1(f1.c_str()), in2(f2.c_str());
	if (!in1.is_open() || !in2.is_open()) { std::cout << "a" << std::endl;return false;}
	std::string line1,line2;
	while (!in1.eof())
	{
		if (in2.eof()) {std::cout << "b" << std::endl;return false;}
		getline(in1,line1);
		getline(in2,line2);
		if (line1 != line2) {std::cout << "c" << std::endl;return false;}
		
	}
	if (!in2.eof()) { std::cout << "d" << std::endl;return false;}

	return true;
}

BOOST_AUTO_TEST_SUITE( sc_building_initialization )

	BOOST_AUTO_TEST_CASE( empty_initialization )
	{
		sc_building sc1 = sc_building();
		BOOST_REQUIRE(sc1.isEmpty());
		sc1.writeToFile("spatial_design/test_file_initalize_1");
		BOOST_REQUIRE(compare_txt_files("spatial_design/test_file_initalize_1", "spatial_design/sc_test_1.txt"));
		remove("spatial_design/test_file_initalize_1");
		BOOST_REQUIRE(sc1.writeToString() == "0,0,0,0");
		BOOST_REQUIRE(sc1.getWSize() == 0);
		BOOST_REQUIRE(sc1.getDSize() == 0);
		BOOST_REQUIRE(sc1.getHSize() == 0);
		BOOST_REQUIRE(sc1.getBSize() == 0);
		BOOST_REQUIRE(sc1.getBRowSize() == 0);
		BOOST_REQUIRE_THROW(sc1.getWIndex(1) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getDIndex(1) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getHIndex(1) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getCellIndex(0,0,0,0) ,std::invalid_argument);
		unsigned int dummy = 0;
		BOOST_REQUIRE_THROW(sc1.getSpaceIndex(1,dummy) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getWValue(0), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getDValue(0), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getHValue(0), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getBValue(0,1), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_file_1 )
	{
		sc_building sc1("spatial_design/sc_test_1.txt");
		BOOST_REQUIRE(sc1.isEmpty());
		BOOST_REQUIRE(sc1.writeToString() == "0,0,0,0");
		BOOST_REQUIRE(sc1.getWSize() == 0);
		BOOST_REQUIRE(sc1.getDSize() == 0);
		BOOST_REQUIRE(sc1.getHSize() == 0);
		BOOST_REQUIRE(sc1.getBSize() == 0);
		BOOST_REQUIRE(sc1.getBRowSize() == 0);
		BOOST_REQUIRE_THROW(sc1.getWIndex(1) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getDIndex(1) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getHIndex(1) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getCellIndex(0,0,0,0) ,std::invalid_argument);
		unsigned int dummy = 0;
		BOOST_REQUIRE_THROW(sc1.getSpaceIndex(1,dummy) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getWValue(0), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getDValue(0), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getHValue(0), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getBValue(0,1), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc_building sc2 = sc_building("invalid_file.txt"), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc_building sc3 = sc_building("spatial_design/sc_test2.txt"), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_file_2 )
	{
		sc_building sc1("spatial_design/sc_test_3.txt");
		BOOST_REQUIRE(!sc1.isEmpty());
		BOOST_REQUIRE(sc1.writeToString() == "1,2,3,5,1,1,2,1,2,3,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1");
		BOOST_REQUIRE(sc1.getWSize() == 1);
		BOOST_REQUIRE(sc1.getDSize() == 2);
		BOOST_REQUIRE(sc1.getHSize() == 3);
		BOOST_REQUIRE(sc1.getBSize() == 5);
		BOOST_REQUIRE(sc1.getBRowSize() == 6);
		BOOST_REQUIRE(sc1.getWIndex(1) == 0);
		BOOST_REQUIRE(sc1.getWIndex(2) == 0);
		BOOST_REQUIRE(sc1.getWIndex(3) == 0);
		BOOST_REQUIRE(sc1.getWIndex(4) == 0);
		BOOST_REQUIRE(sc1.getWIndex(5) == 0);
		BOOST_REQUIRE(sc1.getWIndex(6) == 0);
		BOOST_REQUIRE(sc1.getDIndex(1) == 0);
		BOOST_REQUIRE(sc1.getDIndex(2) == 0);
		BOOST_REQUIRE(sc1.getDIndex(3) == 0);
		BOOST_REQUIRE(sc1.getDIndex(4) == 1);
		BOOST_REQUIRE(sc1.getDIndex(5) == 1);
		BOOST_REQUIRE(sc1.getDIndex(6) == 1);
		BOOST_REQUIRE(sc1.getHIndex(1) == 0);
		BOOST_REQUIRE(sc1.getHIndex(2) == 1);
		BOOST_REQUIRE(sc1.getHIndex(3) == 2);
		BOOST_REQUIRE(sc1.getHIndex(4) == 0);
		BOOST_REQUIRE(sc1.getHIndex(5) == 1);
		BOOST_REQUIRE(sc1.getHIndex(6) == 2);
		BOOST_REQUIRE_THROW(sc1.getWIndex(8) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getDIndex(7) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getHIndex(9) ,std::invalid_argument);
		BOOST_REQUIRE(sc1.getCellIndex(0,0,0) == 1);
		BOOST_REQUIRE(sc1.getCellIndex(0,0,1) == 2);
		BOOST_REQUIRE(sc1.getCellIndex(0,0,2) == 3);
		BOOST_REQUIRE(sc1.getCellIndex(0,1,0) == 4);
		BOOST_REQUIRE(sc1.getCellIndex(0,1,1) == 5);
		BOOST_REQUIRE(sc1.getCellIndex(0,1,2) == 6);
		unsigned int test_uint = 0;
		BOOST_REQUIRE(sc1.getSpaceIndex(1,test_uint));
		BOOST_REQUIRE(test_uint == 0);
		BOOST_REQUIRE(sc1.getSpaceIndex(2,test_uint));
		BOOST_REQUIRE(test_uint == 1);
		BOOST_REQUIRE(sc1.getSpaceIndex(3,test_uint));
		BOOST_REQUIRE(test_uint == 2);
		BOOST_REQUIRE(sc1.getSpaceIndex(4,test_uint));
		BOOST_REQUIRE(test_uint == 3);
		BOOST_REQUIRE(sc1.getSpaceIndex(5,test_uint));
		BOOST_REQUIRE(test_uint == 4);
		BOOST_REQUIRE(sc1.getSpaceIndex(6,test_uint));
		BOOST_REQUIRE(test_uint == 4);
		BOOST_REQUIRE_THROW(sc1.getSpaceIndex(7,test_uint) ,std::invalid_argument);
		BOOST_REQUIRE(sc1.getWValue(0) == 1000);
		BOOST_REQUIRE(sc1.getDValue(1) == 2000);
		BOOST_REQUIRE(sc1.getHValue(2) == 3000);
		BOOST_REQUIRE_THROW(sc1.getWValue(1), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getDValue(2), std::invalid_argument);
		BOOST_REQUIRE_THROW(sc1.getHValue(3), std::invalid_argument);
		BOOST_REQUIRE(sc1.getBValue(0,1) == 1);
		BOOST_REQUIRE(sc1.getBValue(0,2) == 0);
		BOOST_REQUIRE(sc1.getBValue(4,5) == 1);
		BOOST_REQUIRE_THROW(sc1.getBValue(5,1), std::invalid_argument);
	}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE( sc_building_comparison )

	BOOST_AUTO_TEST_CASE( comparison_empty )
	{
		sc_building sc1("spatial_design/sc_test_1.txt");
		sc_building sc2("0,0,0,0");
		sc_building sc3("1,0,0,0,3");
		
		BOOST_REQUIRE(sc1 == sc2);
		BOOST_REQUIRE(!(sc1 != sc2));
		BOOST_REQUIRE(sc2 != sc3);
		BOOST_REQUIRE(!(sc2 == sc3));
	}
	
	BOOST_AUTO_TEST_CASE( comparison_1 )
	{
		sc_building sc1("spatial_design/sc_test_3.txt");
		sc_building sc2("1,2,3,5,1,1,2,1,2,3,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1");
		sc_building sc3("1,2,3,5,2,2,2,2,2,2,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,1,1");
		
		BOOST_REQUIRE(sc1 == sc2);
		BOOST_REQUIRE(!(sc1 != sc2));
		BOOST_REQUIRE(sc2 != sc3);
		BOOST_REQUIRE(!(sc2 == sc3));
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace spatial_design_test