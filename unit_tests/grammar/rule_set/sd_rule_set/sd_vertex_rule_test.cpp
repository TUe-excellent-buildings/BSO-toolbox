#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "grammar_sd_vertex_rule_test"
#endif

#include <bso/grammar/rule_set/sd_rule_set/sd_vertex_rule.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE_NO_THROW(function)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_sd_rule_set_test {
using namespace bso::grammar::rule_set;

BOOST_AUTO_TEST_SUITE( grammar_sd_vertex_rule_test )

BOOST_AUTO_TEST_CASE( initialization )
{
	namespace cf = bso::spatial_design::conformal;
	bso::spatial_design::ms_building msModel("grammar/ms_test_2.txt");
	bso::spatial_design::cf_building cfModel(msModel);
	cf::cf_geometry_model* geomModel = &cfModel;
	auto vertexPtr1 = geomModel->addVertex({0,0,3000});
	vertex_property vp1(vertexPtr1);
	
	BOOST_REQUIRE_NO_THROW(sd_vertex_rule sdv1(&vp1));
	sd_vertex_rule sdv1(&vp1);
	
	bso::structural_design::sd_model sd;
	sdv1.apply(sd);
	
	BOOST_REQUIRE(sd.getPoints().size() == 1);
	BOOST_REQUIRE(sd.getPoints().front()->isSameAs(*vertexPtr1));
}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace grammar_sd_rule_set_test