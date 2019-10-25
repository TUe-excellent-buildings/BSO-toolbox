#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "quad_hexahedron"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/utilities/geometry.hpp>

#include <stdexcept>
#include <iostream>
#include <iterator>
#include <vector>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace geometry_test {
using namespace bso::utilities::geometry;

BOOST_AUTO_TEST_SUITE( quad_hexahedron_tests )

	BOOST_AUTO_TEST_CASE( empty_init )
	{
		quad_hexahedron qhex;
		std::stringstream check;
		check << qhex;
		BOOST_REQUIRE(check.str() == "{}");
	}
	
	BOOST_AUTO_TEST_CASE( initializer_list_init )
	{
		quad_hexahedron qhex1 = {{-1,-1,-1},{-1,-1, 1},{-1, 1,-1},{-1, 1, 1},
														 { 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1}};
		std::stringstream check1;
		check1 << qhex1;
		BOOST_REQUIRE(check1.str() == "{{-1,-1,-1},{-1,1,-1},{-1,1,1},{-1,-1,1},{1,-1,-1},{1,1,-1},{1,1,1},{1,-1,1}}");
	  
		vertex p1 = { 1, 1, 1};
		vertex p2 = { 1, 1,-1};
		vertex p3 = { 1,-1,-1};
		vertex p4 = { 1,-1, 1};
		vertex p5 = {-1, 1, 1};
		vertex p6 = {-1, 1,-1};
		vertex p7 = {-1,-1,-1};
		vertex p8 = {-1,-1, 1};
		vertex p9 = {-1,-1, 0};
		
		quad_hexahedron qhex2 = {p1,p2,p3,p4,p5,p6,p7,p8};
		std::stringstream check2;
		check2 << qhex2;
		BOOST_REQUIRE(check2.str() == "{{1,1,1},{1,1,-1},{1,-1,-1},{1,-1,1},{-1,1,1},{-1,1,-1},{-1,-1,-1},{-1,-1,1}}");
	  BOOST_REQUIRE_THROW(quad_hexahedron({p1,p2,p3}),std::invalid_argument);
		BOOST_REQUIRE_THROW(quad_hexahedron({p1,p2,p3,p4,p5,p6,p7,p8,p9}),std::invalid_argument);
		BOOST_REQUIRE_THROW(quad_hexahedron({p1,p2,p3,p4,p4,p6,p7,p8}),std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( container_init )
	{
		std::vector<vertex> cont1 = {{-1,-1,-1},{-1,-1, 1},{-1, 1,-1},{-1, 1, 1},
																 { 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1}};
		quad_hexahedron qhex1(cont1);
		std::stringstream check1;
		check1 << qhex1;
		BOOST_REQUIRE(check1.str() == "{{-1,-1,-1},{-1,1,-1},{-1,1,1},{-1,-1,1},{1,-1,-1},{1,1,-1},{1,1,1},{1,-1,1}}");
	  BOOST_REQUIRE_THROW(quad_hexahedron(std::vector<vertex>({{0,0,0},{1,1,1}})), std::invalid_argument);
		
		vertex cont2[8] = {{-1,-1,-1},{-1,-1, 1},{-1, 1,-1},{-1, 1, 1},
											 { 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1}};
		quad_hexahedron qhex2(cont2);
		std::stringstream check2;
		check2 << qhex2;
		BOOST_REQUIRE(check2.str() == "{{-1,-1,-1},{-1,1,-1},{-1,1,1},{-1,-1,1},{1,-1,-1},{1,1,-1},{1,1,1},{1,-1,1}}");
	}
	
	BOOST_AUTO_TEST_CASE( copy_init )
	{
		std::vector<vertex> cont = {{-1,-1,-1},{-1,-1, 1},{-1, 1,-1},{-1, 1, 1},
																{ 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1}};
		quad_hexahedron* qhex1 = new quad_hexahedron(cont);
		quad_hexahedron qhex2 = *qhex1;
		delete qhex1;
		std::stringstream check;
		for (const auto& i : qhex2.getPolygons())
		{
			BOOST_REQUIRE_NO_THROW(check << *i); // this will throw an error if the copy constructor is not working properly
		}
	}
	
	BOOST_AUTO_TEST_CASE( get_volume )
	{
		quad_hexahedron qhex1 = {{-1,-1,-1},{-1,-1, 1},{-1, 1,-1},{-1, 1, 1},
														 { 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1}};
		BOOST_REQUIRE(abs(qhex1.getVolume()-8) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( is_inside_or_on )
	{
		quad_hexahedron qhex = {{-1,-1,-1},{-1,-1, 1},{-1, 1,-1},{-1, 1, 1},
														{ 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1}};
		vertex p1 = {0,0,0};
		vertex p2 = {1,0,0};
		vertex p3 = {0.9999,0,0};
		vertex p4 = {0.99,0,0};
		vertex p5 = {0.5,0.5,0};
		vertex p6 = {-0.5,0.5,0};
		vertex p7 = {1.0001,0,0};
		vertex p8 = {1.01,0,0};
		vertex p9 = {1,1,1};

		BOOST_REQUIRE( qhex.isInside(p1));
		BOOST_REQUIRE(!qhex.isInside(p2));
		BOOST_REQUIRE(!qhex.isInside(p3));
		BOOST_REQUIRE( qhex.isInside(p4));
		BOOST_REQUIRE(!qhex.isInside(p4,1e-1));
		BOOST_REQUIRE( qhex.isInside(p5));
		BOOST_REQUIRE( qhex.isInside(p6));
		
		BOOST_REQUIRE( qhex.isInsideOrOn(p1));
		BOOST_REQUIRE( qhex.isInsideOrOn(p2));
		BOOST_REQUIRE( qhex.isInsideOrOn(p3));
		BOOST_REQUIRE( qhex.isInsideOrOn(p4));
		BOOST_REQUIRE( qhex.isInsideOrOn(p4,1e-1));
		BOOST_REQUIRE( qhex.isInsideOrOn(p5));
		BOOST_REQUIRE( qhex.isInsideOrOn(p6));
		BOOST_REQUIRE( qhex.isInsideOrOn(p7));
		BOOST_REQUIRE(!qhex.isInsideOrOn(p8));
		BOOST_REQUIRE( qhex.isInsideOrOn(p9));
	}
	
	BOOST_AUTO_TEST_CASE( get_surface_area )
	{
		quad_hexahedron qhex1 = {{-1,-1,-1},{-1,-1, 1},{-1, 1,-1},{-1, 1, 1},
														 { 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1}};
		BOOST_REQUIRE(abs(qhex1.getSurfaceArea()-24) < 1e-9);
	}
	
	BOOST_AUTO_TEST_CASE( get_center )
	{
		quad_hexahedron qhex1 = {{-1,-1,-1},{-1,-1, 1},{-1, 1,-1},{-1, 1, 1},
														 { 1, 1, 1},{ 1, 1,-1},{ 1,-1, 1},{ 1,-1,-1}};
		vertex pCheck = {0,0,0};
		vertex pCenter = qhex1.getCenter();
		BOOST_REQUIRE((pCenter-pCheck).isZero(1e-9));
		BOOST_REQUIRE((qhex1.center()-pCheck).isZero(1e-9));
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace geometry_test