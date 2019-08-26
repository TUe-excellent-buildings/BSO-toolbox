#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE xml_tests
#endif

#include <bso/spatial_design/xml/xml_model.hpp>

#include <fstream>
#include <vector>

#include <boost/test/included/unit_test.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace spatial_design_test {
using namespace bso::spatial_design;

BOOST_AUTO_TEST_SUITE( xml_building_spatial_design )
	
	BOOST_AUTO_TEST_CASE( write_to_file )
	{
		ms_building ms("spatial_design/ms_test_1.txt");
		xml::xml_model x(ms,"1.0","dutch");
		x.writeToFile("spatial_design/xml_test_1.xml");

		std::ifstream in("spatial_design/xml_test_1.xml");
		BOOST_REQUIRE(in.is_open());
		std::string writtenString((std::istreambuf_iterator<char>(in)),
																std::istreambuf_iterator<char>());
		std::stringstream checkStream;
		checkStream << "<?xml version=\"1.0\" encoding=\"utf-8\"?>" << "\n"
								<< "<gebouwmodel versie=\"1.0\" naam=\"\">" << "\n"
								<< "  <constructies>" << "\n"
								<< "    <constructie naam=\"rechthoek_1\" id=\"1\">" << "\n"
								<< "      <geometrie type=\"vlak\">0 0 0 1000 0 0 1000 0 3000 0 0 3000</geometrie>" << "\n"
								<< "    </constructie>" << "\n"
								<< "    <constructie naam=\"rechthoek_2\" id=\"2\">" << "\n"
								<< "      <geometrie type=\"vlak\">1000 0 0 1000 2000 0 1000 2000 3000 1000 0 3000</geometrie>" << "\n"
								<< "    </constructie>" << "\n"
								<< "    <constructie naam=\"rechthoek_3\" id=\"3\">" << "\n"
								<< "      <geometrie type=\"vlak\">1000 2000 0 0 2000 0 0 2000 3000 1000 2000 3000</geometrie>" << "\n"
								<< "    </constructie>" << "\n"
								<< "    <constructie naam=\"rechthoek_4\" id=\"4\">" << "\n"
								<< "      <geometrie type=\"vlak\">0 2000 0 0 0 0 0 0 3000 0 2000 3000</geometrie>" << "\n"
								<< "    </constructie>" << "\n"
								<< "    <constructie naam=\"rechthoek_5\" id=\"5\">" << "\n"
								<< "      <geometrie type=\"vlak\">0 0 0 1000 0 0 1000 2000 0 0 2000 0</geometrie>" << "\n"
								<< "    </constructie>" << "\n"
								<< "    <constructie naam=\"rechthoek_6\" id=\"6\">" << "\n"
								<< "      <geometrie type=\"vlak\">0 0 3000 1000 0 3000 1000 2000 3000 0 2000 3000</geometrie>" << "\n"
								<< "    </constructie>" << "\n"
								<< "  </constructies>" << "\n"
								<< "  <ruimtes>" << "\n"
								<< "    <ruimte naam=\"simpel\" id=\"1\">" << "\n"
								<< "      <constructieids>1 2 3 4 5 6</constructieids>" << "\n"
								<< "    </ruimte>" << "\n"
								<< "  </ruimtes>" << "\n"
								<< "</gebouwmodel>" << std::endl;
		BOOST_REQUIRE(checkStream.str() == writtenString);
	}
	
	BOOST_AUTO_TEST_CASE( read_from_file )
	{
		xml::xml_model x("spatial_design/xml_test_2.xml");
		ms_building ms = x.initMSBuilding();
		ms_building msCheck("spatial_design/ms_test_1.txt");
		BOOST_REQUIRE(ms == msCheck);
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace spatial_design_test