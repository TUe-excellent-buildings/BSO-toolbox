#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE ms_building
#endif

#include <bso/spatial_design/ms_building.hpp>

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

BOOST_AUTO_TEST_SUITE( building_initialization )

	BOOST_AUTO_TEST_CASE( empty_initialization )
	{
		ms_building b1;
		std::stringstream sstr;
		sstr << b1;
		
		BOOST_REQUIRE(sstr.str() == "");
		BOOST_REQUIRE(b1.getSpacePtrs().size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_file_1 )
	{
		ms_building b1("spatial_design/ms_test_1.txt");
		std::stringstream sstr;
		sstr << b1;
		
		ms_space* s1 = b1.getSpacePtrs()[0];
		
		Eigen::Vector3d coord = s1->getCoordinates(), coordCheck;
		coordCheck << 0,0,0;
		Eigen::Vector3d dims  = s1->getDimensions(), dimCheck;
		dimCheck << 1000,2000,3000;

		BOOST_REQUIRE(sstr.str() == "R,1,1000,2000,3000,0,0,0");
		BOOST_REQUIRE(b1.getSpacePtrs().size() == 1);
		BOOST_REQUIRE(coord == coordCheck);
		BOOST_REQUIRE(dims == dimCheck);
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_file_2 )
	{
		ms_building b1("spatial_design/ms_test_2.txt");
		std::stringstream sstr, check;
		sstr << b1;
		check << "R,1,3000,3000,3000,0,0,0" << std::endl
					<< "R,2,3000,3000,3000,3000,0,0" << std::endl
					<< "R,3,3000,3000,3000,0,3000,0" << std::endl
					<< "R,4,3000,3000,3000,3000,3000,0";
		
		ms_space* s1 = b1.getSpacePtrs()[0];

		BOOST_REQUIRE(sstr.str() == check.str());
		BOOST_REQUIRE(b1.getSpacePtrs().size() == 4);
		
		sstr.str(std::string()); // clear the string stream
		ms_building b2 = b1; // copy contructor
		sstr << b2;

		BOOST_REQUIRE(sstr.str() == check.str());
		BOOST_REQUIRE(b2.getSpacePtrs().size() == 4);
	}
	
	BOOST_AUTO_TEST_CASE( initialize_from_file_3 )
	{
		ms_building b1;

		BOOST_REQUIRE_THROW(b1 = ms_building("non_existent_file.txt"), std::invalid_argument);
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( building_functions )

	BOOST_AUTO_TEST_CASE( getSpacePtr_1 )
	{
		ms_building b1("spatial_design/ms_test_1.txt");
		ms_space* s1 = b1.getSpacePtrs()[0];
		ms_space s2 = ms_space(*s1);
			
		BOOST_REQUIRE(s1 == b1.getSpacePtr(s2));
		b1.deleteSpace(s1);
		BOOST_REQUIRE_THROW(s1 == b1.getSpacePtr(s2), std::runtime_error);
	}
	
		BOOST_AUTO_TEST_CASE( getSpacePtr_2 )
	{
		ms_building b1;
		
		ms_space s1(1,{0,0,0},{1000,2000,3000});
		ms_space s2(2,{1000,0,0},{3000,3000,3000});
		ms_space s3(3,{0,0,4303},{3000,3000,3000});
		
		std::vector<ms_space*> testPtrs;
		testPtrs.push_back(&s1);
		testPtrs.push_back(&s2);
		testPtrs.push_back(&s3);
		
		b1.addSpace(s1);
		b1.addSpace(s2);
		
		std::vector<ms_space*> addedPtrs = b1.getSpacePtrs();
		bool found[2] = {false};
		for (int i = 0; i < 2; i++)
		{
			for (int j = 0; j < 2; j++)
			{
				if (*(addedPtrs[i]) == *(testPtrs[i]))
				{
					found[i] = true;
					break;
				}
			}
		}
		
		BOOST_REQUIRE(addedPtrs[0] == b1.getSpacePtr(s1));
		BOOST_REQUIRE(found[0] && found[1]);
		BOOST_REQUIRE_THROW(b1.getSpacePtr(s3), std::runtime_error);
	}
	
	BOOST_AUTO_TEST_CASE( getLastSpaceID )
	{
		ms_building b1;
		BOOST_REQUIRE(b1.getLastSpaceID() == 0);
		b1.addSpace(ms_space(1,{0,0,0},{1000,1000,1000}));
		BOOST_REQUIRE(b1.getLastSpaceID() == 1);
		b1.addSpace(ms_space(131,{0,0,3000},{1000,1000,1000}));
		BOOST_REQUIRE(b1.getLastSpaceID() == 131);
	}
	
	BOOST_AUTO_TEST_CASE( getVolume )
	{
		ms_building b1("spatial_design/ms_test_1.txt");
		
		BOOST_REQUIRE(b1.getVolume() == 6);
		
		b1.scale();
		BOOST_REQUIRE(round(b1.getVolume()) == 12);
	}
	
	BOOST_AUTO_TEST_CASE( setZZero_1 )
	{
		ms_building b1("spatial_design/ms_test_3.txt");
		b1.setZZero();
		std::stringstream sstr;
		sstr << b1;

		BOOST_REQUIRE(sstr.str() == "R,1,3000,3000,3000,0,0,0\nR,2,3000,3000,3000,0,0,4303");
	}
	
	BOOST_AUTO_TEST_CASE( setZZero_2 )
	{
		ms_building b1("spatial_design/ms_test_4.txt");
		b1.setZZero();
		std::stringstream sstr;
		sstr << b1;

		BOOST_REQUIRE(sstr.str() == "R,1,3000,3000,3000,0,0,-697\nR,2,3000,3000,3000,0,0,5000");
	}
	
	BOOST_AUTO_TEST_CASE( addSpace_and_Comparison )
	{
		ms_building b1;
		ms_building b2("spatial_design/ms_test_3.txt");
		
		b1.addSpace(ms_space(1,{0,0,0},{3000,3000,3000}));
		b1.addSpace(ms_space(2,{0,0,4303},{3000,3000,3000}));

		BOOST_REQUIRE(b1 != b2);
		BOOST_REQUIRE(b2 != b1);

		b2.setZZero();
		
		BOOST_REQUIRE(b1 == b2);
		BOOST_REQUIRE(b2 == b1);
	}
	
	BOOST_AUTO_TEST_CASE( deleteSpace )
	{
		ms_building b1("spatial_design/ms_test_1.txt");
		ms_building b2;
		
		ms_space s1(1,{0,0,0},{1000,2000,3000});
		ms_space s2(2,{1000,0,0},{3000,3000,3000});
		ms_space s3(3,{0,0,4303},{3000,3000,3000});
		
		b2.addSpace(s1);
		b2.addSpace(s2);
		b2.addSpace(s3);
		
		b2.deleteSpace(s2);
		b2.deleteSpace(s3);
		BOOST_REQUIRE(b1 == b2);
		
		b2.deleteSpace(&s1);
		BOOST_REQUIRE(b2.getSpacePtrs().empty());
		BOOST_REQUIRE_THROW(b2.deleteSpace(&s2), std::runtime_error);
		BOOST_REQUIRE_THROW(b2.deleteSpace(s1), std::runtime_error);
	}
	
	BOOST_AUTO_TEST_CASE( sweep )
	{
		ms_building b1("spatial_design/ms_test_2.txt");
		std::stringstream sstr;
		
		b1.sweep({3000,1500,0.0},{{0,300},{1,500},{2,1000}});
		sstr << b1;
		BOOST_REQUIRE(sstr.str() == "R,1,3000,3500,4000,0,0,0\nR,2,3300,3500,4000,3000,0,0\nR,3,3000,3000,4000,0,3500,0\nR,4,3300,3000,4000,3000,3500,0");
		sstr.str(std::string());

		BOOST_REQUIRE_THROW(b1.sweep({3000,1500,0.0},{{3,1.5}}), std::invalid_argument);
		BOOST_REQUIRE_THROW(b1.sweep({3000,1500,0.0},{{1,1.0},{1,1.0}}), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( scale )
	{
		ms_building b1("spatial_design/ms_test_3.txt");
		std::stringstream sstr;
		
		b1.scale({{0,1.5},{1,2},{2,1.23}});
		sstr << b1;
		BOOST_REQUIRE(sstr.str() == "R,1,4500,6000,3690,0,0,857.31\nR,2,4500,6000,3690,0,0,6150");
		sstr.str(std::string());
		
		b1.scale();
		sstr << b1;
		BOOST_REQUIRE(sstr.str() == "R,1,6363.96,8485.28,3690,0,0,857.31\nR,2,6363.96,8485.28,3690,0,0,6150");
		
		BOOST_REQUIRE_THROW(b1.scale({{3,1.5}}), std::invalid_argument);
		BOOST_REQUIRE_THROW(b1.scale({{1,1.0},{1,1.0}}), std::invalid_argument);

		//BOOST_REQUIRE_THROW();
	}
	
	BOOST_AUTO_TEST_CASE( snapOn )
	{
		ms_building b1("spatial_design/ms_test_3.txt");
		std::stringstream sstr;
		
		b1.snapOn({{2,10}});
		sstr << b1;
		BOOST_REQUIRE(sstr.str() == "R,1,3000,3000,3000,0,0,700\nR,2,3000,3000,3000,0,0,5000");
		sstr.str(std::string());
		
		b1.scale();
		b1.snapOn();
		sstr << b1;
		BOOST_REQUIRE(sstr.str() == "R,1,4240,4240,3000,0,0,700\nR,2,4240,4240,3000,0,0,5000");
		
		BOOST_REQUIRE_THROW(b1.snapOn({{3,1.5}}), std::invalid_argument);
		BOOST_REQUIRE_THROW(b1.snapOn({{1,1.0},{1,1.0}}), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( splitSpace )
	{
		ms_building b1("spatial_design/ms_test_1.txt");
		std::stringstream sstr;
		std::stringstream check;
		
		ms_space* spacePtr = b1.getSpacePtrs()[0];
		
		b1.splitSpace(spacePtr,{{1,2},{2,3}});
		sstr << b1;
		check << "R,2,1000,1000,1000,0,0,0" << std::endl
					<< "R,3,1000,1000,1000,0,0,1000" << std::endl
					<< "R,4,1000,1000,1000,0,0,2000" << std::endl
					<< "R,5,1000,1000,1000,0,1000,0" << std::endl
					<< "R,6,1000,1000,1000,0,1000,1000" << std::endl
					<< "R,7,1000,1000,1000,0,1000,2000";
		BOOST_REQUIRE(sstr.str() == check.str());

		BOOST_REQUIRE_THROW(b1.splitSpace(b1.getSpacePtrs()[0],{{3,2},{2,3}}), std::invalid_argument); // non-existent axis
		BOOST_REQUIRE_THROW(b1.splitSpace(b1.getSpacePtrs()[0],{{1,2},{1,3}}), std::invalid_argument); // splitting twice over same axis
		BOOST_REQUIRE_THROW(b1.splitSpace(b1.getSpacePtrs()[0],{{1,1},{2,0}}), std::invalid_argument); // splitting space over an axis into less than two new spaces
		BOOST_REQUIRE_THROW(b1.getSpacePtr(spacePtr), std::runtime_error); // spacePtr is deleted after the split
		BOOST_REQUIRE_THROW(b1.splitSpace(spacePtr,{{1,2},{2,3}}), std::runtime_error); // spacePtr does not belong to b1 anymore and cannot be split
	}
	
	BOOST_AUTO_TEST_CASE( has_overlapping_spaces )
	{
		std::multimap<ms_space*, ms_space*> overlaps;
		ms_building ms1;
		ms1.addSpace(ms_space("1,1000,1000,1000,0,0,0"));
		ms1.addSpace(ms_space("2,2000,2000,2000,-500,-500,-500"));
		BOOST_REQUIRE(ms1.hasOverlappingSpaces(overlaps));
		BOOST_REQUIRE(overlaps.size() == 1);

		overlaps.clear();
		ms_building ms2;
		ms2.addSpace(ms_space("1,1000,1000,3000,0,0,-1000"));
		ms2.addSpace(ms_space("2,2000,2000,2000,-500,-500,-500"));
		BOOST_REQUIRE(ms2.hasOverlappingSpaces(overlaps));
		BOOST_REQUIRE(overlaps.size() == 1);
		
		overlaps.clear();
		ms_building ms3;
		ms3.addSpace(ms_space("1,1000,1000,1000,0,0,0"));
		ms3.addSpace(ms_space("2,1000,1000,1000,500,500,0"));
		BOOST_REQUIRE(ms3.hasOverlappingSpaces(overlaps));
		BOOST_REQUIRE(overlaps.size() == 2);
		
		overlaps.clear();
		ms_building ms4;
		ms4.addSpace(ms_space("1,1000,1000,1000,0,0,500"));
		ms4.addSpace(ms_space("2,1000,1000,1000,500,500,0"));
		BOOST_REQUIRE(ms4.hasOverlappingSpaces(overlaps));
		BOOST_REQUIRE(overlaps.size() == 2);
		
		overlaps.clear();
		ms_building ms5;
		ms5.addSpace(ms_space("1,1000,1000,1000,0,0,1000"));
		ms5.addSpace(ms_space("2,1000,1000,1000,500,500,0"));
		BOOST_REQUIRE(!ms5.hasOverlappingSpaces(overlaps));
		BOOST_REQUIRE(overlaps.size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( has_floating_spaces )
	{
		std::vector<ms_space*> floatingSpaces;
		ms_building ms1;
		ms1.addSpace(ms_space("1,1000,1000,1000,0,0,0"));
		ms1.addSpace(ms_space("2,1000,1000,1000,0,0,1500"));
		BOOST_REQUIRE(ms1.hasFloatingSpaces(floatingSpaces));

		floatingSpaces.clear();
		ms_building ms2;
		ms2.addSpace(ms_space("1,1000,1000,1000,0,0,0"));
		ms2.addSpace(ms_space("2,1000,1000,1000,1000,1000,1000"));
		BOOST_REQUIRE(!ms2.hasFloatingSpaces(floatingSpaces));
		
		floatingSpaces.clear();
		ms_building ms3;
		ms3.addSpace(ms_space("1,1000,1000,1000,0,0,0"));
		ms3.addSpace(ms_space("2,1000,1000,1000,1000,0,1000"));
		BOOST_REQUIRE(!ms3.hasFloatingSpaces(floatingSpaces));
		
		floatingSpaces.clear();
		ms_building ms4;
		ms4.addSpace(ms_space("1,1000,1000,1000,0,0,0"));
		ms4.addSpace(ms_space("2,1000,1000,1000,1000,500,500"));
		BOOST_REQUIRE(!ms4.hasFloatingSpaces(floatingSpaces));
		
		floatingSpaces.clear();
		ms_building ms5;
		ms5.addSpace(ms_space("1,1000,1000,1000,0,0,0"));
		ms5.addSpace(ms_space("2,1000,1000,1000,0,0,1000"));
		BOOST_REQUIRE(!ms5.hasFloatingSpaces(floatingSpaces));
	}
	
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( ms_sc_conversion )

	BOOST_AUTO_TEST_CASE( sc_to_ms )
	{
		sc_building sc1("spatial_design/sc_test_4.txt");
		ms_building ms1("spatial_design/ms_test_1.txt");
		ms_building ms2 = sc1;
		
		BOOST_REQUIRE(ms1 == ms2);
	}
	
	BOOST_AUTO_TEST_CASE( ms_to_sc )
	{
		sc_building sc1("spatial_design/sc_test_4.txt");
		ms_building ms1("spatial_design/ms_test_1.txt");
		sc_building sc2 = sc1;
		BOOST_REQUIRE(sc1 == sc2);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace spatial_design_test