#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "cf_line"
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

BOOST_AUTO_TEST_SUITE( cf_line_tests )
	
	BOOST_AUTO_TEST_CASE( split_1 )
	{
		cf_geometry_model geomModel;
		geomModel.addLine({{0,0,0},{2,0,0}});
		cf_vertex* splitVertex = geomModel.addVertex({1,0,0});
		
		std::vector<bso::utilities::geometry::line_segment> checks = {
			{{0,0,0},{1,0,0}},
			{{1,0,0},{2,0,0}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfLines().size(); ++i)
		{
			auto lPtr = geomModel.cfLines()[i];
			lPtr->split(splitVertex);
			if (lPtr->deletion()) geomModel.removeLine(lPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfLines().size(); ++i)
		{
			auto lPtr = geomModel.cfLines()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (lPtr->isSameAs(j))
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
		geomModel.addLine({{0,0,0},{2,0,0}});
		cf_vertex* splitVertex = geomModel.addVertex({1,0,0.0001});
		
		std::vector<bso::utilities::geometry::line_segment> checks = {
			{{0,0,0},{1,0,0}},
			{{1,0,0},{2,0,0}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfLines().size(); ++i)
		{
			auto lPtr = geomModel.cfLines()[i];
			lPtr->split(splitVertex);
			if (lPtr->deletion()) geomModel.removeLine(lPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfLines().size(); ++i)
		{
			auto lPtr = geomModel.cfLines()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (lPtr->isSameAs(j))
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
		geomModel.addLine({{0,0,0},{2,0,0}});
		std::vector<cf_vertex*> splitVertices;
		splitVertices.push_back(geomModel.addVertex({1,0,0.1}));
		splitVertices.push_back(geomModel.addVertex({0,0,0}));
		splitVertices.push_back(geomModel.addVertex({2,0,0}));
		splitVertices.push_back(geomModel.addVertex({-1,0,0}));
		splitVertices.push_back(geomModel.addVertex({3,0,0}));
		
		std::vector<bso::utilities::geometry::line_segment> checks = {
			{{0,0,0},{2,0,0}}
		};
		
		for (unsigned int i = 0; i < geomModel.cfLines().size(); ++i)
		{
			auto lPtr = geomModel.cfLines()[i];
			for (auto& j : splitVertices)
			{
				lPtr->split(j);
			}
			if (lPtr->deletion()) geomModel.removeLine(lPtr);
		}
		
		for (unsigned int i = 0; i < geomModel.cfLines().size(); ++i)
		{
			auto lPtr = geomModel.cfLines()[i];
			bool foundMatch = false;
			for (const auto& j : checks)
			{
				if (lPtr->isSameAs(j))
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