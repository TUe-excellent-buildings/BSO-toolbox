#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "sd_fea"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/structural_design/fea.hpp>
#include <bso/structural_design/component/point.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace structural_design_test {
using namespace bso::structural_design;

BOOST_AUTO_TEST_SUITE( sd_fea_test )
	
	BOOST_AUTO_TEST_CASE( initialize )
	{
		BOOST_REQUIRE_NO_THROW(fea testFEA);
	}

	BOOST_AUTO_TEST_CASE( add_node )
	{
		fea testFEA;
		bso::utilities::geometry::vertex v1 = {0,0,0};
		component::point p1(1,{0,0,0});
		component::point p2(2,{1,0,0});
		
		element::node* n1 = testFEA.addNode(v1);
		element::node* n2 = testFEA.addNode(p1);
		element::node* n3 = testFEA.addNode(p2);
		
		BOOST_REQUIRE(n1->isSameAs({0,0,0}));
		BOOST_REQUIRE(n1 == n2);
		BOOST_REQUIRE(n3->isSameAs({1,0,0}));
		BOOST_REQUIRE(n1->ID() == 1);
		BOOST_REQUIRE(n3->ID() == 2);
	}
	
	BOOST_AUTO_TEST_CASE( add_element )
	{
		fea testFEA;
		element::node* n1 = testFEA.addNode({0,0,0});
		element::node* n2 = testFEA.addNode({1,0,0});
		
		testFEA.addElement(new element::truss(0,1e5,1e3,{n1,n2}));
		
		BOOST_REQUIRE(testFEA.getElements().size() == 1);
		BOOST_REQUIRE(testFEA.getElements()[0]->isTruss());
		BOOST_REQUIRE(testFEA.getElements()[0]->ID() == 0);
		BOOST_REQUIRE(testFEA.getElements()[0]->getProperty("E") == 1e5);
		BOOST_REQUIRE(testFEA.getElements()[0]->getProperty("A") == 1e3);
	}
	
	BOOST_AUTO_TEST_CASE( generate_GSM )
	{
		fea testFEA;
		element::node* n1 = testFEA.addNode({0,0,0});
		element::node* n2 = testFEA.addNode({1,0,0});
		
		n1->addConstraint(0);
		n2->addConstraint(1);
		
		testFEA.addElement(new element::truss(0,1e5,1e3,{n1,n2}));
		testFEA.generateGSM();

		BOOST_REQUIRE_THROW(n1->getGlobalDOF(0), std::invalid_argument);
		BOOST_REQUIRE_THROW(n1->getGlobalDOF(3), std::invalid_argument);
		BOOST_REQUIRE(n1->getGlobalDOF(1) == 0);
		BOOST_REQUIRE(n1->getGlobalDOF(2) == 1);
		
		BOOST_REQUIRE_THROW(n2->getGlobalDOF(1), std::invalid_argument);
		BOOST_REQUIRE_THROW(n2->getGlobalDOF(3), std::invalid_argument);
		BOOST_REQUIRE(n2->getGlobalDOF(0) == 2);
		BOOST_REQUIRE(n2->getGlobalDOF(2) == 3);
	}
	
	BOOST_AUTO_TEST_CASE( solve )
	{
		fea testFEA;
		element::node* n1 = testFEA.addNode({0,0,0});
		element::node* n2 = testFEA.addNode({1,0,0});
		
		n1->addConstraint(0);
		n1->addConstraint(1);
		n1->addConstraint(2);
		n2->addConstraint(1);
		n2->addConstraint(2);
		
		element::load_case lc1("test_case");
		element::load l1(lc1,1e9,0);
		n2->addLoad(l1);

		testFEA.addElement(new element::truss(0,1e5,1e3,{n1,n2}));
		testFEA.generateGSM();
		
		testFEA.solve();
		
		BOOST_REQUIRE(abs(n1->getDisplacements(lc1)(1)) < 1e-9);
		BOOST_REQUIRE(abs(n1->getDisplacements(lc1)(2)) < 1e-9);
		BOOST_REQUIRE(abs(n2->getDisplacements(lc1)(0)/10-1) < 1e-9);
		
		testFEA.clearResponse();
		testFEA.solve("BiCGSTAB");

		BOOST_REQUIRE(abs(n1->getDisplacements(lc1)(1)) < 1e-3);
		BOOST_REQUIRE(abs(n1->getDisplacements(lc1)(2)) < 1e-3);
		BOOST_REQUIRE(abs(n2->getDisplacements(lc1)(0)/10-1) < 1e-3);
		
		testFEA.clearResponse();
		testFEA.solve("scaledBiCGSTAB");

		BOOST_REQUIRE(abs(n1->getDisplacements(lc1)(1)) < 1e-3);
		BOOST_REQUIRE(abs(n1->getDisplacements(lc1)(2)) < 1e-3);
		BOOST_REQUIRE(abs(n2->getDisplacements(lc1)(0)/10-1) < 1e-3);
		
		testFEA.clearResponse();
		BOOST_REQUIRE_THROW(testFEA.solve("notASolver"), std::invalid_argument);
	}

BOOST_AUTO_TEST_SUITE_END()
} // namespace structural_design_test