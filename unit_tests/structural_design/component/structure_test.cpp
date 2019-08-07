#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_structure_component_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/component/structure.hpp>

#include <sstream>
#include <stdexcept>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace component_test {
using namespace bso::structural_design::component;

BOOST_AUTO_TEST_SUITE( sd_structure_component )
	
	BOOST_AUTO_TEST_CASE( truss )
	{
		structure s1("truss",{{"A",100},{"E",1e6}});
		BOOST_REQUIRE(s1.type() == "truss");
		BOOST_REQUIRE(s1.A() == 100);
		BOOST_REQUIRE(s1.E() == 1e6);
		BOOST_REQUIRE(s1.ERelativeLowerBound() == 1e-6);
		BOOST_REQUIRE(s1.isVisible());
		BOOST_REQUIRE(!s1.isGhostComponent());
		
		
		s1.isVisible() = false;
		s1.isGhostComponent() = true;
		BOOST_REQUIRE(!s1.isVisible());
		BOOST_REQUIRE(s1.isGhostComponent());
		
		structure s2("truss",{{"A",100},{"E",1e6},{"ERelativeLowerBound",1e-9}});
		BOOST_REQUIRE(s2.ERelativeLowerBound() == 1e-9);
		BOOST_REQUIRE_THROW(s2.poisson(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.width(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.height(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.thickness(), std::runtime_error);
		
		BOOST_REQUIRE_THROW(structure s3("truss",{}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("truss",{{"A",100},{"E",1e6},{"beta",2.5}}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("truss",{{"A",100},{"E",1e6},{"poisson",0.3}}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("truss",{{"A", 100},{"A",200}}) ,std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( beam )
	{
		structure s1("beam",{{"width",100},{"height",300},{"poisson",0.3},{"E",1e6}});
		BOOST_REQUIRE(s1.type() == "beam");
		BOOST_REQUIRE(s1.width() == 100);
		BOOST_REQUIRE(s1.height() == 300);
		BOOST_REQUIRE(s1.poisson() == 0.3);
		BOOST_REQUIRE(s1.E() == 1e6);
		BOOST_REQUIRE(s1.ERelativeLowerBound() == 1e-6);
		
		structure s2("beam",{{"width",100},{"height",300},{"poisson",0.3},{"E",1e6},{"ERelativeLowerBound",1e-9}});
		BOOST_REQUIRE(s2.ERelativeLowerBound() == 1e-9);
		BOOST_REQUIRE_THROW(s2.A(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.thickness(), std::runtime_error);
		
		BOOST_REQUIRE_THROW(structure s3("beam",{}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("beam",{{"width",100},{"height",300},{"poisson",0.3},{"E",1e6},{"beta",2.5}}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("beam",{{"width",100},{"height",300},{"poisson",0.3},{"E",1e6},{"A",100}}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("beam",{{"width", 100},{"width",200}}) ,std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( flat_shell )
	{
		structure s1("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e6}});
		BOOST_REQUIRE(s1.type() == "flat_shell");
		BOOST_REQUIRE(s1.thickness() == 100);
		BOOST_REQUIRE(s1.poisson() == 0.3);
		BOOST_REQUIRE(s1.E() == 1e6);
		BOOST_REQUIRE(s1.ERelativeLowerBound() == 1e-6);
		
		structure s2("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e6},{"ERelativeLowerBound",1e-9}});
		BOOST_REQUIRE(s2.ERelativeLowerBound() == 1e-9);
		BOOST_REQUIRE_THROW(s2.A(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.width(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.height(), std::runtime_error);
		
		BOOST_REQUIRE_THROW(structure s3("flat_shell",{}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e6},{"beta",2.5}}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("flat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e6},{"A",100}}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("flat_shell",{{"thickness",100},{"thickness",100}}) ,std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( quad_hexahedron )
	{
		structure s1("quad_hexahedron",{{"poisson",0.3},{"E",1e6}});
		BOOST_REQUIRE(s1.type() == "quad_hexahedron");
		BOOST_REQUIRE(s1.poisson() == 0.3);
		BOOST_REQUIRE(s1.E() == 1e6);
		BOOST_REQUIRE(s1.ERelativeLowerBound() == 1e-6);
		
		structure s2("quad_hexahedron",{{"poisson",0.3},{"E",1e6},{"ERelativeLowerBound",1e-9}});
		BOOST_REQUIRE(s2.ERelativeLowerBound() == 1e-9);
		BOOST_REQUIRE_THROW(s2.A(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.width(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.height(), std::runtime_error);
		BOOST_REQUIRE_THROW(s2.thickness(), std::runtime_error);
		
		BOOST_REQUIRE_THROW(structure s3("quad_hexahedron",{}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("quad_hexahedron",{{"poisson",0.3},{"E",1e6},{"beta",2.5}}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("quad_hexahedron",{{"poisson",0.3},{"E",1e6},{"A",100}}) ,std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s3("quad_hexahedron",{{"E",1e5},{"E",1e6}}) ,std::invalid_argument);
	}
	
	BOOST_AUTO_TEST_CASE( invalid_type )
	{
		BOOST_REQUIRE_THROW(structure s1("fat_shell",{{"thickness",100},{"poisson",0.3},{"E",1e6}}), std::invalid_argument);
		BOOST_REQUIRE_THROW(structure s1("",{}), std::invalid_argument);
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace component_test