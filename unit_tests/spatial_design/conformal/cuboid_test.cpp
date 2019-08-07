#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "cf_cuboid"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/spatial_design/cf_building.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace conformal_test {
using namespace bso::spatial_design::conformal;

BOOST_AUTO_TEST_SUITE( cf_cuboid_tests )
	
	BOOST_AUTO_TEST_CASE( split_1 )
	{
		cf_geometry_model geomModel;
		geomModel.addCuboid({{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}});
		cf_vertex* splitVertex = geomModel.addVertex({0,0,0});
		
		std::vector<bso::utilities::geometry::quad_hexahedron> checks = {
			{{-1,-1,-1}, { 0,-1,-1}, { 0, 0,-1}, { 0,-1, 0}, { 0, 0, 0}, {-1, 0,-1}, {-1, 0, 0}, {-1,-1, 0}},
			{{ 1, 1, 1}, { 0, 1, 1}, { 0, 0, 1}, { 0, 1, 0}, { 0, 0, 0}, { 1, 0, 1}, { 1, 0, 0}, { 1, 1, 0}},
			{{-1,-1, 1}, { 0,-1, 1}, { 0, 0, 1}, { 0,-1, 0}, { 0, 0, 0}, {-1, 0, 1}, {-1, 0, 0}, {-1,-1, 0}},
			{{-1, 1,-1}, { 0, 1,-1}, { 0, 0,-1}, { 0, 1, 0}, { 0, 0, 0}, {-1, 0,-1}, {-1, 0, 0}, {-1, 1, 0}},
			{{-1, 1, 1}, { 0, 1, 1}, { 0, 0, 1}, { 0, 1, 0}, { 0, 0, 0}, {-1, 0, 1}, {-1, 0, 0}, {-1, 1, 0}},
			{{ 1,-1, 1}, { 0,-1, 1}, { 0, 0, 1}, { 0,-1, 0}, { 0, 0, 0}, { 1, 0, 1}, { 1, 0, 0}, { 1,-1, 0}},
			{{ 1, 1,-1}, { 0, 1,-1}, { 0, 0,-1}, { 0, 1, 0}, { 0, 0, 0}, { 1, 0,-1}, { 1, 0, 0}, { 1, 1, 0}},
			{{ 1,-1,-1}, { 0,-1,-1}, { 0, 0,-1}, { 0,-1, 0}, { 0, 0, 0}, { 1, 0,-1}, { 1, 0, 0}, { 1,-1, 0}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			cubPtr->split(splitVertex);
			if (cubPtr->deletion()) geomModel.removeCuboid(cubPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (cubPtr->isSameAs(j))
				{
					foundMatch = true;
					break;
				}
			}
			BOOST_REQUIRE(foundMatch);
		}
	}
	
	BOOST_AUTO_TEST_CASE( split_2 )
	{
		cf_geometry_model geomModel;
		geomModel.addCuboid({{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}});
		cf_vertex* splitVertex = geomModel.addVertex({0,0,0.99});
		
		std::vector<bso::utilities::geometry::quad_hexahedron> checks = {
			{{-1,-1,-1}, { 0,-1,-1}, { 0, 0,-1}, { 0,-1, 0.99}, { 0, 0, 0.99}, {-1, 0,-1}, {-1, 0, 0.99}, {-1,-1, 0.99}},
			{{ 1, 1, 1}, { 0, 1, 1}, { 0, 0, 1}, { 0, 1, 0.99}, { 0, 0, 0.99}, { 1, 0, 1}, { 1, 0, 0.99}, { 1, 1, 0.99}},
			{{-1,-1, 1}, { 0,-1, 1}, { 0, 0, 1}, { 0,-1, 0.99}, { 0, 0, 0.99}, {-1, 0, 1}, {-1, 0, 0.99}, {-1,-1, 0.99}},
			{{-1, 1,-1}, { 0, 1,-1}, { 0, 0,-1}, { 0, 1, 0.99}, { 0, 0, 0.99}, {-1, 0,-1}, {-1, 0, 0.99}, {-1, 1, 0.99}},
			{{-1, 1, 1}, { 0, 1, 1}, { 0, 0, 1}, { 0, 1, 0.99}, { 0, 0, 0.99}, {-1, 0, 1}, {-1, 0, 0.99}, {-1, 1, 0.99}},
			{{ 1,-1, 1}, { 0,-1, 1}, { 0, 0, 1}, { 0,-1, 0.99}, { 0, 0, 0.99}, { 1, 0, 1}, { 1, 0, 0.99}, { 1,-1, 0.99}},
			{{ 1, 1,-1}, { 0, 1,-1}, { 0, 0,-1}, { 0, 1, 0.99}, { 0, 0, 0.99}, { 1, 0,-1}, { 1, 0, 0.99}, { 1, 1, 0.99}},
			{{ 1,-1,-1}, { 0,-1,-1}, { 0, 0,-1}, { 0,-1, 0.99}, { 0, 0, 0.99}, { 1, 0,-1}, { 1, 0, 0.99}, { 1,-1, 0.99}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			cubPtr->split(splitVertex);
			if (cubPtr->deletion()) geomModel.removeCuboid(cubPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (cubPtr->isSameAs(j))
				{
					foundMatch = true;
					break;
				}
			}
			BOOST_REQUIRE(foundMatch);
		}
	}
	
	BOOST_AUTO_TEST_CASE( split_3 )
	{
		cf_geometry_model geomModel;
		geomModel.addCuboid({{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}});
		cf_vertex* splitVertex = geomModel.addVertex({0,0,1});
		
		std::vector<bso::utilities::geometry::quad_hexahedron> checks = {
			{{ 1, 1, 1}, { 0, 1, 1}, { 0, 0, 1}, { 1, 0, 1}, { 1, 1,-1}, { 0, 1,-1}, { 0, 0,-1}, { 1, 0,-1}},
			{{-1, 1, 1}, {-1, 0, 1}, { 0, 0, 1}, { 0, 1, 1}, {-1, 1,-1}, {-1, 0,-1}, { 0, 0,-1}, { 0, 1,-1}},
			{{-1,-1, 1}, { 0,-1, 1}, { 0, 0, 1}, {-1, 0, 1}, {-1,-1,-1}, { 0,-1,-1}, { 0, 0,-1}, {-1, 0,-1}},
			{{ 1,-1, 1}, { 1, 0, 1}, { 0, 0, 1}, { 0,-1, 1}, { 1,-1,-1}, { 1, 0,-1}, { 0, 0,-1}, { 0,-1,-1}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			cubPtr->split(splitVertex);
			if (cubPtr->deletion()) geomModel.removeCuboid(cubPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (cubPtr->isSameAs(j))
				{
					foundMatch = true;
					break;
				}
			}
			BOOST_REQUIRE(foundMatch);
		}
	}
	
	BOOST_AUTO_TEST_CASE( split_4 )
	{
		cf_geometry_model geomModel;
		geomModel.addCuboid({{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}});
		cf_vertex* splitVertex = geomModel.addVertex({0,0,1.0001});
		
		std::vector<bso::utilities::geometry::quad_hexahedron> checks = {
			{{ 1, 1, 1}, { 0, 1, 1}, { 0, 0, 1}, { 1, 0, 1}, { 1, 1,-1}, { 0, 1,-1}, { 0, 0,-1}, { 1, 0,-1}},
			{{-1, 1, 1}, {-1, 0, 1}, { 0, 0, 1}, { 0, 1, 1}, {-1, 1,-1}, {-1, 0,-1}, { 0, 0,-1}, { 0, 1,-1}},
			{{-1,-1, 1}, { 0,-1, 1}, { 0, 0, 1}, {-1, 0, 1}, {-1,-1,-1}, { 0,-1,-1}, { 0, 0,-1}, {-1, 0,-1}},
			{{ 1,-1, 1}, { 1, 0, 1}, { 0, 0, 1}, { 0,-1, 1}, { 1,-1,-1}, { 1, 0,-1}, { 0, 0,-1}, { 0,-1,-1}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			cubPtr->split(splitVertex);
			if (cubPtr->deletion()) geomModel.removeCuboid(cubPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (cubPtr->isSameAs(j))
				{
					foundMatch = true;
					break;
				}
			}
			BOOST_REQUIRE(foundMatch);
		}
	}
	
	BOOST_AUTO_TEST_CASE( split_5 )
	{
		cf_geometry_model geomModel;
		geomModel.addCuboid({{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}});
		cf_vertex* splitVertex = geomModel.addVertex({0,1,1});
		
		std::vector<bso::utilities::geometry::quad_hexahedron> checks = {
			{{ 1, 1, 1}, { 0, 1, 1}, { 0,-1, 1}, { 1,-1, 1}, { 1, 1,-1}, { 0, 1,-1}, { 0,-1,-1}, { 1,-1,-1}},
			{{-1,-1, 1}, { 0,-1, 1}, { 0, 1, 1}, {-1, 1, 1}, {-1,-1,-1}, { 0,-1,-1}, { 0, 1,-1}, {-1, 1,-1}},
		};
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			cubPtr->split(splitVertex);
			if (cubPtr->deletion()) geomModel.removeCuboid(cubPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (cubPtr->isSameAs(j))
				{
					foundMatch = true;
					break;
				}
			}
			BOOST_REQUIRE(foundMatch);
		}
	}
	
	BOOST_AUTO_TEST_CASE( split_6 )
	{
		cf_geometry_model geomModel;
		geomModel.addCuboid({{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}});
		cf_vertex* splitVertex = geomModel.addVertex({0,1,1.0001});
		
		std::vector<bso::utilities::geometry::quad_hexahedron> checks = {
			{{ 1, 1, 1}, { 0, 1, 1}, { 0,-1, 1}, { 1,-1, 1}, { 1, 1,-1}, { 0, 1,-1}, { 0,-1,-1}, { 1,-1,-1}},
			{{-1,-1, 1}, { 0,-1, 1}, { 0, 1, 1}, {-1, 1, 1}, {-1,-1,-1}, { 0,-1,-1}, { 0, 1,-1}, {-1, 1,-1}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			cubPtr->split(splitVertex);
			if (cubPtr->deletion()) geomModel.removeCuboid(cubPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (cubPtr->isSameAs(j))
				{
					foundMatch = true;
					break;
				}
			}
			BOOST_REQUIRE(foundMatch);
		}
	}
	
	BOOST_AUTO_TEST_CASE( split_7 )
	{
		cf_geometry_model geomModel;
		geomModel.addCuboid({{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}});
		std::vector<cf_vertex*> splitVertices;
		splitVertices.push_back(geomModel.addVertex({0,1,2}));
		
		std::vector<bso::utilities::geometry::quad_hexahedron> checks = {
			{{-1,-1,1},{1,-1,1},{1,1,1},{-1,1,1},{-1,-1,-1},{1,-1,-1},{1,1,-1},{-1,1,-1}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			for (const auto& j : splitVertices)
			{
				cubPtr->split(j);
			}
			if (cubPtr->deletion()) geomModel.removeCuboid(cubPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfCuboids().size(); ++i)
		{
			auto cubPtr = geomModel.cfCuboids()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (cubPtr->isSameAs(j))
				{
					foundMatch = true;
					break;
				}
			}
			BOOST_REQUIRE(foundMatch);
		}
	}
	
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace conformal_test