#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "tetrahedron"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/utilities/geometry.hpp>



/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace geometry_test {
using namespace bso::utilities::geometry;

BOOST_AUTO_TEST_SUITE( tetrahedron_tests )

	BOOST_AUTO_TEST_CASE( empty_init )
	{
		tetrahedron tet1;
		std::stringstream check;
		check << tet1;
		BOOST_REQUIRE(check.str() == "{}");
	}
	
	BOOST_AUTO_TEST_CASE( initializer_list_init )
	{
		tetrahedron tet1 = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		std::stringstream check1;
		check1 << tet1;
		BOOST_REQUIRE(check1.str() == "{{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}}");
		BOOST_REQUIRE_THROW(tetrahedron({{1,1,1},{3,1,1},{1,3,1}}), std::invalid_argument);
		
		vertex p1 = {1,1,1};
		vertex p2 = {3,1,1};
		vertex p3 = {1,3,1};
		vertex p4 = {1.5,1.5,3};
		vertex p5 = {3,3,3};
		tetrahedron tet2 = {p1,p2,p3,p4};
		std::stringstream check2;
		check2 << tet2;
		BOOST_REQUIRE(check2.str() == "{{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}}");
		BOOST_REQUIRE_THROW(tetrahedron({p1,p2,p3}), std::invalid_argument);
		BOOST_REQUIRE_THROW(tetrahedron({p1,p2,p3,p3}), std::invalid_argument);
		BOOST_REQUIRE_THROW(tetrahedron({p1,p2,p3,p4,p5}), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( container_init )
	{
		std::vector<vertex> cont1 = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		tetrahedron tet1(cont1);
		std::stringstream check1;
		check1 << tet1;
	BOOST_REQUIRE(check1.str() == "{{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}}");
		BOOST_REQUIRE_THROW(tetrahedron(std::vector<vertex>({{0,0,0},{1,1,1}})), std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( copy_init )
	{
		std::vector<vertex> cont1 = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		tetrahedron* tet1 = new tetrahedron(cont1);
		tetrahedron tet2 = *tet1;
		delete tet1;
		std::stringstream check;
		for (const auto& i : tet2.getPolygons())
		{
			BOOST_REQUIRE_NO_THROW(check << *i); // this will throw an error if the copy constructor is not working properly
		}
	}
	
	BOOST_AUTO_TEST_CASE( get_volume )
	{
		tetrahedron tet1 = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		BOOST_REQUIRE(abs(tet1.getVolume()-(4.0/3.0)) < 1e-3);
	}
	
	BOOST_AUTO_TEST_CASE( is_inside_or_on )
	{
		tetrahedron tet = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		vertex p1 = {1.3,1.3,1.5};
		vertex p2 = {1.5,1.5,1};
		vertex p3 = {1.5,1.5,1.0001};
		vertex p4 = {1.5,1.5,1.1};
		vertex p5 = {2.5,2,2};
		vertex p6 = {1,2,1};
		vertex p7 = {0.9999,1,1};
		vertex p8 = {0.99,1,1};
		vertex p9 = {1,1,1};
		vertex p10= {3,1,1};
		vertex p11= {1.5,1.5,3};
		
		BOOST_REQUIRE( tet.isInside(p1));
		BOOST_REQUIRE(!tet.isInside(p2));
		BOOST_REQUIRE(!tet.isInside(p3));
		BOOST_REQUIRE( tet.isInside(p4));
		BOOST_REQUIRE(!tet.isInside(p4,1e1));
		BOOST_REQUIRE(!tet.isInside(p5));
		
		BOOST_REQUIRE( tet.isInsideOrOn(p1));
		BOOST_REQUIRE( tet.isInsideOrOn(p2));
		BOOST_REQUIRE( tet.isInsideOrOn(p3));
		BOOST_REQUIRE( tet.isInsideOrOn(p4));
		BOOST_REQUIRE( tet.isInsideOrOn(p4,1e1));
		BOOST_REQUIRE(!tet.isInsideOrOn(p5));
		BOOST_REQUIRE( tet.isInsideOrOn(p6));
		BOOST_REQUIRE( tet.isInsideOrOn(p7));
		BOOST_REQUIRE(!tet.isInsideOrOn(p8));
		BOOST_REQUIRE( tet.isInsideOrOn(p9));
		BOOST_REQUIRE( tet.isInsideOrOn(p10));
		BOOST_REQUIRE( tet.isInsideOrOn(p11));
	}
	
	BOOST_AUTO_TEST_CASE( is_same_as )
	{
		tetrahedron tet1 = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		tetrahedron tet2 = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		tetrahedron tet3 = {{1.01,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		
		BOOST_REQUIRE( tet1.isSameAs(tet2));
		BOOST_REQUIRE(!tet1.isSameAs(tet3));
		BOOST_REQUIRE( tet1.isSameAs(tet3,1e-1));
	}
	
	BOOST_AUTO_TEST_CASE( get_surface_area )
	{
		tetrahedron tet1 = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		double check = 0;
		check += 0.5*2*2; // triangle abc
		check += 0.5*2*sqrt(4.25); // triangle abd
		check += 0.5*2*sqrt(4.25); // triangle acd
		check += 0.5*sqrt(8)*sqrt(4.5); // triangle cbd
		BOOST_REQUIRE(abs(tet1.getSurfaceArea() - check) < 1e-3);
	}
	
	BOOST_AUTO_TEST_CASE( get_center )
	{
		tetrahedron tet1 = {{1,1,1},{3,1,1},{1,3,1},{1.5,1.5,3}};
		vertex pCheck = {6.5,6.5,6};
		pCheck /= 4;
		vertex pCenter = tet1.getCenter();
		BOOST_REQUIRE((pCenter - pCheck).isZero(1e-9));
		BOOST_REQUIRE((tet1.center() - pCheck).isZero(1e-9));
	}
	
	BOOST_AUTO_TEST_CASE( offstream )
	{
		tetrahedron tet({{1,sqrt(2),0},{1,4.1234e5,0},{0,53.45678,0},{100,0,100}},1e-9);
		std::stringstream check1, check2;
		check1 << std::setprecision(3) << tet;
		check2 << std::setprecision(8) << tet;
		
		BOOST_REQUIRE(check1.str() == "{{1,1.41,0},{1,4.12e+05,0},{0,53.5,0},{100,0,100}}");
		BOOST_REQUIRE(check2.str() == "{{1,1.4142136,0},{1,412340,0},{0,53.45678,0},{100,0,100}}");
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace geometry_test