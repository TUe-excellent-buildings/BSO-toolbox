#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_sd_rectangle_rule_test"
#endif

#include <bso/grammar/rule_set/sd_rule_set/sd_rectangle_rule.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_sd_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_sd_rectangle_rule_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,0,3000},{0,0,3000}});
	rectangle_property rp1(rectPtr);
	
	BOOST_REQUIRE_NO_THROW(sd_rectangle_rule sdr1(&rp1));
	sd_rectangle_rule sdr1(&rp1);
	BOOST_REQUIRE(sdr1.getStructure().type() == "none");
	
	bso::structural_design::sd_model sd;
	sdr1.apply(sd);
	
	BOOST_REQUIRE(sd.getGeometries().size() == 0);
}

BOOST_AUTO_TEST_CASE( apply_rule_1 )
{ // no load only structure
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,0,3000},{0,0,3000}});
	rectangle_property rp1(rectPtr);
	sd_rectangle_rule sdr1(&rp1);
	
	bso::structural_design::component::structure struct1("flat_shell",{{"thickness",100},{"E",1e5},{"poisson",0.3}});
	std::multimap<std::string, bso::structural_design::component::load> loads;
	sdr1.addLoads(&loads);
	sdr1.assignStructure(struct1);
	
	bso::structural_design::sd_model sd;
	sdr1.apply(sd);
	
	BOOST_REQUIRE(sd.getGeometries().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().front()->getLoads().size() == 0);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().type() == "flat_shell");
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().thickness() == 100);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().E() == 1e5);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().poisson() == 0.3);
	auto sdQuadGeomPtr = dynamic_cast<bso::structural_design::component::quadrilateral*>
		(sd.getGeometries().front());
	BOOST_REQUIRE(sdQuadGeomPtr->isSameAs(*rectPtr));
}

BOOST_AUTO_TEST_CASE( apply_rule_2 )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,0,3000},{0,0,3000}});
	std::vector<bso::utilities::geometry::line_segment> diagonals;
	diagonals.push_back({{0,0,0},{3000,0,3000}});
	diagonals.push_back({{3000,0,0},{0,0,3000}});
	rectangle_property rp1(rectPtr);
	sd_rectangle_rule sdr1(&rp1);
	
	bso::structural_design::component::structure struct1("truss",{{"A",1e5},{"E",1e6}});
	std::multimap<std::string, bso::structural_design::component::load> loads;
	sdr1.addLoads(&loads);
	sdr1.assignStructure(struct1);
	
	bso::structural_design::sd_model sd;
	sdr1.apply(sd);

	BOOST_REQUIRE(sd.getGeometries().size() == 2);
	BOOST_REQUIRE(sd.getGeometries().front()->getLoads().size() == 0);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().type() == "truss");
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().A() == 1e5);
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().front().E() == 1e6);

	bool allTrussesFound = true;
	for (const auto& i : diagonals)
	{
		bool diagonalFound = false;
		for (const auto& j : sd.getGeometries())
		{
			auto sdLinePtr = dynamic_cast<bso::structural_design::component::line_segment*>(j);
			if(sdLinePtr->isSameAs(i))
			{
				diagonalFound = true;
				break;
			}
		}
		if (!diagonalFound)
		{
			allTrussesFound = false;
			break;
		}
	}
	BOOST_REQUIRE(allTrussesFound);
}

BOOST_AUTO_TEST_CASE( apply_rule_3 )
{ // no structure but with load
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto rectPtr1 = geomModel->addRectangle({{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0}});
	auto rectPtr2 = geomModel->addRectangle({{0,0,0},{0,3000,0},{0,3000,3000},{0,0,3000}});

	rectangle_property rp1(rectPtr1);
	rectangle_property rp2(rectPtr2);
	sd_rectangle_rule sdr1(&rp1);
	sd_rectangle_rule sdr2(&rp2);
	
	bso::structural_design::component::structure loadPanel("flat_shell",{{"thickness",100},{"E",1},{"poisson",0}});
	bso::structural_design::component::load_case lc1("load case 1");
	bso::structural_design::component::load_case lc2("load case 2");
	bso::structural_design::component::load_case lc3("load case 3");
	bso::structural_design::component::load_case lc4("load case 4");
	bso::structural_design::component::load l1(lc1,-10,2);
	bso::structural_design::component::load l2(lc2,2,1);
	bso::structural_design::component::load l3(lc3,4,0);
	bso::structural_design::component::load l4(lc4,-2,0);
	std::multimap<std::string, bso::structural_design::component::load> loads;
	loads.insert({"live_load",l1});
	loads.insert({"wind_shear",l2});
	loads.insert({"wind_pressure",l3});
	loads.insert({"wind_suction",l4});
	sdr1.addLoads(&loads);
	sdr1.assignLoadPanel(loadPanel);
	sdr2.addLoads(&loads);
	sdr2.assignLoadPanel(loadPanel);
	
	bso::structural_design::sd_model sd;
	sdr1.apply(sd);
	sdr2.apply(sd);

	BOOST_REQUIRE(sd.getGeometries().size() == 2); 
	BOOST_REQUIRE(sd.getGeometries().front()->getStructures().size() == 1);
	BOOST_REQUIRE(sd.getGeometries().back()->getStructures().size() == 1);
	
	for (const auto& i : sd.getGeometries())
	{
		if (i->isQuadrilateral())
		{
			auto quadPtr = dynamic_cast<bso::structural_design::component::quadrilateral*>(i);
			if (quadPtr->isSameAs(*rectPtr1))
			{
				BOOST_REQUIRE(i->getLoads().size()/*1*/ == 1);
				BOOST_REQUIRE(i->getLoads().front().loadCase() == lc1);
				BOOST_REQUIRE(i->getLoads().front().magnitude() == -10);
				BOOST_REQUIRE(i->getLoads().front().DOF() == 2);
			}
			else if (quadPtr->isSameAs(*rectPtr2))
			{
				BOOST_REQUIRE(i->getLoads().size()/*2*/ == 3);
				std::vector<bso::structural_design::component::load> expectedLoads = {l2,l3,l4};
				bool allLoadsFound = true;
				for (const auto& j : expectedLoads)
				{
					bool loadFound = false;
					for (const auto& k : i->getLoads())
					{
						if (j.loadCase() == k.loadCase() && 
								j.DOF() == k.DOF() && 
								j.magnitude() == k.magnitude())
						{
							loadFound = true;
							break;
						}
					}
					if (!loadFound)
					{
						allLoadsFound = false;
						break;
					}
				}
				BOOST_REQUIRE(allLoadsFound);
			}
		}
	}
}

BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_sd_rule_set_test