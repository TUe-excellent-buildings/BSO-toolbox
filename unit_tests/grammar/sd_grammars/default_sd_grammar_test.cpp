#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "default_sd_grammar_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/spatial_design/ms_building.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/structural_design/sd_model.hpp>
#include <bso/grammar/grammar.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace grammar_test {
using namespace bso::grammar;

BOOST_AUTO_TEST_SUITE( grammar_default_sd_test )

	BOOST_AUTO_TEST_CASE( default_sd_grammar_test_1 )
	{
		bso::spatial_design::ms_building ms("grammar/ms_test_1.txt");
		bso::spatial_design::cf_building cf(ms);

		bso::grammar::grammar gram(cf);
		bso::structural_design::sd_model sd = gram.sd_grammar<>(std::string("grammar/sd_settings_1.txt"),true);
		
		unsigned int flatShellCount = 0;
		unsigned int trussCount = 0;
		unsigned int beamCount = 0;
		unsigned int loadPanelCount = 0;
		unsigned int constraintCount = 0;
		unsigned int loadCase1Count = 0;
		unsigned int loadCase2Count = 0;
		unsigned int loadCase3Count = 0;
		unsigned int loadCase4Count = 0;
		unsigned int loadCase5Count = 0;
		unsigned int uncounted = 0;
		
		for (const auto& i : sd.getGeometries())
		{
			for (const auto& j : i->getStructures())
			{
				if (j.type() == "flat_shell" && !j.isGhostComponent()) ++flatShellCount;
				else if (j.type() == "beam" && !j.isGhostComponent()) ++beamCount;
				else if (j.type() == "truss" && !j.isGhostComponent()) ++trussCount;
				else if (j.type() == "flat_shell" && j.isGhostComponent()) ++loadPanelCount;
				else ++uncounted;
			}
			for (const auto& j : i->getLoads())
			{
				if      (j.loadCase().description() == "1") ++loadCase1Count;
				else if (j.loadCase().description() == "2") ++loadCase2Count;
				else if (j.loadCase().description() == "3") ++loadCase3Count;
				else if (j.loadCase().description() == "4") ++loadCase4Count;
				else if (j.loadCase().description() == "5") ++loadCase5Count;
				else ++uncounted;
			}
			for (const auto& j : i->getConstraints()) ++constraintCount;
		}

		BOOST_REQUIRE(flatShellCount == 3);
		BOOST_REQUIRE(trussCount == 4);
		BOOST_REQUIRE(beamCount == 2);
		BOOST_REQUIRE(loadPanelCount == 3);
		BOOST_REQUIRE(loadCase1Count == 2);
		BOOST_REQUIRE(loadCase2Count == 5);
		BOOST_REQUIRE(loadCase3Count == 5);
		BOOST_REQUIRE(loadCase4Count == 5);
		BOOST_REQUIRE(loadCase5Count == 5);
		BOOST_REQUIRE(constraintCount == 12);
		BOOST_REQUIRE(uncounted == 0);
	}

BOOST_AUTO_TEST_SUITE_END()

} // namespace grammar_test