#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "cf_vertex"
#endif

#include <boost/test/included/unit_test.hpp>
#include <sstream>

#include <bso/spatial_design/cf_building.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace conformal_test {
using namespace bso::spatial_design::conformal;

BOOST_AUTO_TEST_SUITE( cf_vertex_tests )
	
	BOOST_AUTO_TEST_CASE( empty_init )
	{
		cf_vertex p;
		std::stringstream check;
		check << p.transpose();
		BOOST_REQUIRE(check.str() == "0 0 0");
	}
	
	BOOST_AUTO_TEST_CASE( eigen_vector_init )
	{
		Eigen::Vector3d eigenVec;
		eigenVec << 1.5,2.5,3.5;
		cf_vertex p = eigenVec;
		std::stringstream check;
		check << p.transpose();
		BOOST_REQUIRE(check.str() == "1.5 2.5 3.5");
	}
	
	BOOST_AUTO_TEST_CASE( initializer_list_init )
	{
		cf_vertex p = {1.5,2.5,3.5};
		std::stringstream check;
		check << p.transpose();
		BOOST_REQUIRE(check.str() == "1.5 2.5 3.5");
	}
	
	BOOST_AUTO_TEST_CASE( is_structural )
	{
		cf_vertex p;
		BOOST_REQUIRE(!p.isStructural(/*1*/));
		p.isStructural() = true;
		BOOST_REQUIRE(p.isStructural(/*2*/));
		p.isStructural() = false;
		BOOST_REQUIRE(!p.isStructural(/*3*/));
	}
	
	BOOST_AUTO_TEST_CASE( add_point )
	{
		cf_point* pPtr;
		cf_vertex p;
		BOOST_REQUIRE(p.cfPoints().size() == 0);
		p.addPoint(pPtr);
		BOOST_REQUIRE(p.cfPoints().size() == 1);
		for (auto& i : p.cfPoints())
		{
			BOOST_REQUIRE(i == pPtr);
		}
	}
	
	BOOST_AUTO_TEST_CASE( add_remove_line )
	{
		cf_line* lPtr;
		cf_vertex p;
		BOOST_REQUIRE(p.cfLines().size() == 0);
		p.addLine(lPtr);
		BOOST_REQUIRE(p.cfLines().size() == 1);
		for (auto& i : p.cfLines())
		{
			BOOST_REQUIRE(i == lPtr);
		}
		p.removeLine(lPtr);
		BOOST_REQUIRE(p.cfLines().size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( add_remove_rectangle )
	{
		cf_rectangle* recPtr;
		cf_vertex p;
		BOOST_REQUIRE(p.cfRectangles().size() == 0);
		p.addRectangle(recPtr);
		BOOST_REQUIRE(p.cfRectangles().size() == 1);
		for (auto& i : p.cfRectangles())
		{
			BOOST_REQUIRE(i == recPtr);
		}
		p.removeRectangle(recPtr);
		BOOST_REQUIRE(p.cfRectangles().size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( add_remove_cuboid )
	{
		cf_cuboid* cubPtr;
		cf_vertex p;
		BOOST_REQUIRE(p.cfCuboids().size() == 0);
		p.addCuboid(cubPtr);
		BOOST_REQUIRE(p.cfCuboids().size() == 1);
		for (auto& i : p.cfCuboids())
		{
			BOOST_REQUIRE(i == cubPtr);
		}
		p.removeCuboid(cubPtr);
		BOOST_REQUIRE(p.cfCuboids().size() == 0);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace conformal_test