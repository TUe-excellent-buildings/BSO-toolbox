#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE ms_space
#endif

#include <bso/spatial_design/ms_space.hpp>

#include <vector>
#include <stdexcept>

#include <boost/test/included/unit_test.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace spatial_design_test {
using namespace bso::spatial_design;

BOOST_AUTO_TEST_SUITE( space_initialization )

	BOOST_AUTO_TEST_CASE( empty_initialization )
	{
		Eigen::Vector3d coord1(0,0,0);
		Eigen::Vector3d dim1(0,0,0);
		ms_space s1;
		BOOST_REQUIRE(s1.getCoordinates() == coord1);
		BOOST_REQUIRE(s1.getDimensions() == dim1);
		BOOST_REQUIRE(s1.getID() == 0);
		std::string checkSpaceType;
		BOOST_REQUIRE(!s1.getSpaceType(checkSpaceType));
		BOOST_REQUIRE(checkSpaceType == "");
		std::vector<std::string> surfaceTypes;
		std::vector<std::string> checkSurfaceTypes;
		BOOST_REQUIRE(!s1.getSurfaceTypes(checkSurfaceTypes));
		BOOST_CHECK_EQUAL_COLLECTIONS(checkSurfaceTypes.begin(), checkSurfaceTypes.end(), surfaceTypes.begin(), surfaceTypes.end());
	}
	
	BOOST_AUTO_TEST_CASE( initialization_1 )
	{
		Eigen::Vector3d coord1(0,0,0);
		Eigen::Vector3d dim1(1000,1000,1000);
		ms_space s1(1, coord1, dim1);
		BOOST_REQUIRE(s1.getCoordinates() == coord1);
		BOOST_REQUIRE(s1.getDimensions() == dim1);
		BOOST_REQUIRE(s1.getID() == 1);
		std::string checkSpaceType;
		BOOST_REQUIRE(!s1.getSpaceType(checkSpaceType));
		BOOST_REQUIRE(checkSpaceType == "");
		std::vector<std::string> surfaceTypes;
		std::vector<std::string> checkSurfaceTypes;
		BOOST_REQUIRE(!s1.getSurfaceTypes(checkSurfaceTypes));
		BOOST_CHECK_EQUAL_COLLECTIONS(checkSurfaceTypes.begin(), checkSurfaceTypes.end(), surfaceTypes.begin(), surfaceTypes.end());
	}
	
	BOOST_AUTO_TEST_CASE( initialization_2 )
	{
		Eigen::Vector3d coord1(500,6500,7000);
		Eigen::Vector3d dim1(2250,3500,4000);
		std::vector<std::string> surfaceTypes = {"type_1", "type_2", "type_3", "type_4", "type_5", "type_6"};
		ms_space s1(1, coord1, dim1, "type_a", surfaceTypes);
		BOOST_REQUIRE(s1.getCoordinates() == coord1);
		BOOST_REQUIRE(s1.getDimensions() == dim1);
		BOOST_REQUIRE(s1.getID() == 1);
		std::string checkSpaceType;
		BOOST_REQUIRE(s1.getSpaceType(checkSpaceType));
		BOOST_REQUIRE(checkSpaceType == "type_a");
		std::vector<std::string> checkSurfaceTypes;
		BOOST_REQUIRE(s1.getSurfaceTypes(checkSurfaceTypes));
		BOOST_CHECK_EQUAL_COLLECTIONS(checkSurfaceTypes.begin(), checkSurfaceTypes.end(), surfaceTypes.begin(), surfaceTypes.end());
	}
	
	BOOST_AUTO_TEST_CASE( initialization_3 )
	{
		Eigen::Vector3d coord1(1000,1000,1000);
		Eigen::Vector3d dim1(3000,3000,3000);
		ms_space s1("3,3000,3000,3000,1000,1000,1000");
		BOOST_REQUIRE(s1.getCoordinates() == coord1);
		BOOST_REQUIRE(s1.getDimensions() == dim1);
		BOOST_REQUIRE(s1.getID() == 3);
		std::string checkSpaceType;
		BOOST_REQUIRE(!s1.getSpaceType(checkSpaceType));
		BOOST_REQUIRE(checkSpaceType == "");
		std::vector<std::string> surfaceTypes;
		std::vector<std::string> checkSurfaceTypes;
		BOOST_REQUIRE(!s1.getSurfaceTypes(checkSurfaceTypes));
		BOOST_CHECK_EQUAL_COLLECTIONS(checkSurfaceTypes.begin(), checkSurfaceTypes.end(), surfaceTypes.begin(), surfaceTypes.end());
	}
	
	BOOST_AUTO_TEST_CASE( initialization_4 )
	{
		Eigen::Vector3d coord1(1000,1000,1000);
		Eigen::Vector3d dim1(3000,3000,3000);
		ms_space s1("4,3000,3000,3000,1000,1000,1000,type_a");
		BOOST_REQUIRE(s1.getCoordinates() == coord1);
		BOOST_REQUIRE(s1.getDimensions() == dim1);
		BOOST_REQUIRE(s1.getID() == 4);
		std::string checkSpaceType;
		BOOST_REQUIRE(s1.getSpaceType(checkSpaceType));
		BOOST_REQUIRE(checkSpaceType == "type_a");
		std::vector<std::string> surfaceTypes;
		std::vector<std::string> checkSurfaceTypes;
		BOOST_REQUIRE(!s1.getSurfaceTypes(checkSurfaceTypes));
		BOOST_CHECK_EQUAL_COLLECTIONS(checkSurfaceTypes.begin(), checkSurfaceTypes.end(), surfaceTypes.begin(), surfaceTypes.end());
	}
	
	BOOST_AUTO_TEST_CASE( initialization_5 )
	{
		Eigen::Vector3d coord1(1000,1000,1000);
		Eigen::Vector3d dim1(3000,3000,3000);
		ms_space s1("5,3000,3000,3000,1000,1000,1000,type_b,type_1,type_2,type_3,type_4,type_5,type_6");
		BOOST_REQUIRE(s1.getCoordinates() == coord1);
		BOOST_REQUIRE(s1.getDimensions() == dim1);
		BOOST_REQUIRE(s1.getID() == 5);
		std::string checkSpaceType;
		BOOST_REQUIRE(s1.getSpaceType(checkSpaceType));
		BOOST_REQUIRE(checkSpaceType == "type_b");
		std::vector<std::string> surfaceTypes = {"type_1","type_2","type_3","type_4","type_5","type_6"};
		std::vector<std::string> checkSurfaceTypes;
		BOOST_REQUIRE(s1.getSurfaceTypes(checkSurfaceTypes));
		BOOST_CHECK_EQUAL_COLLECTIONS(checkSurfaceTypes.begin(), checkSurfaceTypes.end(), surfaceTypes.begin(), surfaceTypes.end());
	}
	
	BOOST_AUTO_TEST_CASE( initialization_6 )
	{
		Eigen::Vector3d coord1(1000,1000,1000);
		Eigen::Vector3d dim1(3000,3000,3000);
		ms_space s1("6,3000,3000,3000,1000,1000,1000,type_1,type_2,type_3,type_4,type_5,type_6");
		BOOST_REQUIRE(s1.getCoordinates() == coord1);
		BOOST_REQUIRE(s1.getDimensions() == dim1);
		BOOST_REQUIRE(s1.getID() == 6);
		std::string checkSpaceType;
		BOOST_REQUIRE(!s1.getSpaceType(checkSpaceType));
		BOOST_REQUIRE(checkSpaceType == "");
		std::vector<std::string> surfaceTypes = {"type_1","type_2","type_3","type_4","type_5","type_6"};
		std::vector<std::string> checkSurfaceTypes;
		BOOST_REQUIRE(s1.getSurfaceTypes(checkSurfaceTypes));
		BOOST_CHECK_EQUAL_COLLECTIONS(checkSurfaceTypes.begin(), checkSurfaceTypes.end(), surfaceTypes.begin(), surfaceTypes.end());
	}
	
	BOOST_AUTO_TEST_CASE( getGeometryTest )
	{
		ms_space s1("78,3000,3000,3000,1000,1000,1000");
		bso::utilities::geometry::quad_hexahedron spaceGeom = s1.getGeometry();
		bso::utilities::geometry::quad_hexahedron checkGeom = {
			{1000,1000,1000},{4000,1000,1000},{4000,4000,1000},{1000,4000,1000},
			{1000,1000,4000},{4000,1000,4000},{4000,4000,4000},{1000,4000,4000}
		};
		BOOST_CHECK_EQUAL_COLLECTIONS(spaceGeom.begin(), spaceGeom.end(), checkGeom.begin(), checkGeom.end());
	}
	
	BOOST_AUTO_TEST_CASE( initialization_by_assignment )
	{
		ms_space s1;
		Eigen::Vector3d coord1(500,6500,7000);
		Eigen::Vector3d dim1(2250,3500,4000);
		std::vector<std::string> surfaceTypes = {"type_1", "type_2", "type_3", "type_4", "type_5", "type_6"};
		ms_space s2(1, coord1, dim1, "type_a", surfaceTypes);
		s1 = s2;
		BOOST_REQUIRE(s1.getCoordinates() == coord1);
		BOOST_REQUIRE(s1.getDimensions() == dim1);
		BOOST_REQUIRE(s1.getID() == 1);
		std::string checkSpaceType;
		BOOST_REQUIRE(s1.getSpaceType(checkSpaceType));
		BOOST_REQUIRE(checkSpaceType == "type_a");
		std::vector<std::string> checkSurfaceTypes;
		BOOST_REQUIRE(s1.getSurfaceTypes(checkSurfaceTypes));
		BOOST_CHECK_EQUAL_COLLECTIONS(checkSurfaceTypes.begin(), checkSurfaceTypes.end(), surfaceTypes.begin(), surfaceTypes.end());
	}
	
	BOOST_AUTO_TEST_CASE ( initialize_invalid_space_1 )
	{
		Eigen::Vector3d coord1(500,6500,7000);
		Eigen::Vector3d dim1(2250,-3500,4000);
		BOOST_REQUIRE_THROW(ms_space s1(1, coord1, dim1), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE ( initialize_invalid_space_2 )
	{
		BOOST_REQUIRE_THROW(ms_space s1("6,3000,-3000,3000,1000,1000,1000,type_1,type_2,type_3,type_4,type_5,type_6"), std::invalid_argument);
	}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE( space_comparison )

	BOOST_AUTO_TEST_CASE( comparison_1 )
	{
		ms_space s1;
		Eigen::Vector3d coord1(500,6500,7000);
		Eigen::Vector3d dim1(2250,3500,4000);
		std::vector<std::string> surfaceTypes = {"type_1", "type_2", "type_3", "type_4", "type_5", "type_6"};
		ms_space s2(1, coord1, dim1, "type_a", surfaceTypes);
		s1 = s2;
		BOOST_REQUIRE(s1 == s2);
		BOOST_REQUIRE(s2 == s1);
		BOOST_REQUIRE(!(s1 != s2));
		BOOST_REQUIRE(!(s2 != s1));
	}
	
	BOOST_AUTO_TEST_CASE( comparison_2 )
	{
		ms_space s1("1,2250,3500,4000,500,6500,7000,type_a,type_1,type_2,type_3,type_4,type_5,type_6");
		Eigen::Vector3d coord1(500,6500,7000);
		Eigen::Vector3d dim1(2250,3500,4000);
		std::vector<std::string> surfaceTypes = {"type_1", "type_2", "type_3", "type_4", "type_5", "type_6"};
		ms_space s2(1, coord1, dim1, "type_a", surfaceTypes);
		BOOST_REQUIRE(s1 == s2);
		BOOST_REQUIRE(s2 == s1);
		BOOST_REQUIRE(!(s1 != s2));
		BOOST_REQUIRE(!(s2 != s1));
	}
	
	BOOST_AUTO_TEST_CASE( comparison_3 )
	{
		ms_space s1("1,2250,3500,4000,500,6500,7000,type_a,type_1,type_2,type_3,type_4,type_5,type_6");
		ms_space s2("1,2250,3500,4000,500,6500,7000,type_a,type_1,type_2,type_4,type_4,type_5,type_6");
		BOOST_REQUIRE(s1 != s2);
		BOOST_REQUIRE(s2 != s1);
		BOOST_REQUIRE(!(s1 == s2));
		BOOST_REQUIRE(!(s2 == s1));
	}
	
	BOOST_AUTO_TEST_CASE( comparison_4 )
	{
		ms_space s1("1,2250,3500,4000,500,6500,7000,type_a,type_1,type_2,type_3,type_4,type_5,type_6");
		ms_space s2("1,2250,3500,4000,500,6500,7000,type_a");
		BOOST_REQUIRE(s1 != s2);
		BOOST_REQUIRE(s2 != s1);
		BOOST_REQUIRE(!(s1 == s2));
		BOOST_REQUIRE(!(s2 == s1));
	}
	
	BOOST_AUTO_TEST_CASE( comparison_5 )
	{
		ms_space s1("1,2250,3500,4000,500,6500,7000,type_a,type_1,type_2,type_3,type_4,type_5,type_6");
		ms_space s2("1,2250,3500,4000,500,6500,7000,type_1,type_2,type_4,type_4,type_5,type_6");
		BOOST_REQUIRE(s1 != s2);
		BOOST_REQUIRE(s2 != s1);
		BOOST_REQUIRE(!(s1 == s2));
		BOOST_REQUIRE(!(s2 == s1));
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace spatial_design_test