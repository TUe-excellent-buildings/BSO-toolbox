#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "visualization"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/spatial_design/ms_building.hpp>
#include <bso/spatial_design/sc_building.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/structural_design/sd_model.hpp>
#include <bso/building_physics/bp_model.hpp>

#include <bso/visualization/visualization.hpp>

namespace visualization_test {
using namespace bso::visualization;

BOOST_AUTO_TEST_SUITE( visualize_ms_building )
		
	BOOST_AUTO_TEST_CASE( init_visualization )
	{
		bso::visualization::initVisualization(
				boost::unit_test::framework::master_test_suite().argc, 
				boost::unit_test::framework::master_test_suite().argv);
	}

	BOOST_AUTO_TEST_CASE( ms_building_spaces )
	{
		bso::spatial_design::ms_building ms("visualization/ms_test_1.txt");
		visualize(ms,"spaces","MS visualization with space IDs only");
	}
	
	BOOST_AUTO_TEST_CASE( ms_building_space_type )
	{
		bso::spatial_design::ms_building ms("visualization/ms_test_2.txt");
		visualize(ms,"space_type","MS visualization with space IDs and space type");
	}
	
	BOOST_AUTO_TEST_CASE( ms_building_surface_type )
	{
		bso::spatial_design::ms_building ms("visualization/ms_test_3.txt");
		visualize(ms,"surface_type","MS visualization with surface types only");
	}
	
	BOOST_AUTO_TEST_CASE( sc_building_visualization )
	{
		bso::spatial_design::sc_building sc("visualization/sc_test_1.txt");
		visualize(sc,"SC visualization with active space's ID indicated per cell");
	}
	
	BOOST_AUTO_TEST_CASE( conformal_model_visualization )
	{
		bso::spatial_design::ms_building ms("visualization/cf_test_3.txt");
		bso::spatial_design::cf_building cf(ms);

		visualize(cf,"line_segment","CF visualization of line segments in building");
		visualize(cf,"rectangle","CF visualization of rectangles in building");
		visualize(cf,"cuboid","CF visualization of cuboids in building");
	}
	
	BOOST_AUTO_TEST_CASE( sd_model_components_visualization )
	{
		bso::structural_design::sd_model sd;
		auto geom1 = sd.addGeometry(bso::utilities::geometry::line_segment({{0,0,0},{1,0,0}}));
		auto geom2 = sd.addGeometry(bso::utilities::geometry::line_segment({{1,0,0},{2,0,0}}));
		auto geom3 = sd.addGeometry(bso::utilities::geometry::quadrilateral(
			{{0,0,1},{1,0,1},{1,1,1},{0,1,1}}));
		auto geom4 = sd.addGeometry(bso::utilities::geometry::quadrilateral(
			{{0,0,2},{1,0,2},{1,1,2},{0,1,2}}));
		auto geom5 = sd.addGeometry(bso::utilities::geometry::quad_hexahedron(
			{{-1,0,0},{0,0,0},{0,1,0},{-1,1,0},{-1,0,1},{0,0,1},{0,1,1},{-1,1,1}}));
		auto geom6 = sd.addGeometry(bso::utilities::geometry::quad_hexahedron(
			{{-2,0,0},{-1,0,0},{-1,1,0},{-2,1,0},{-2,0,1},{-1,0,1},{-1,1,1},{-2,1,1}}));
		
		namespace comp = bso::structural_design::component;
		
		comp::structure str1("truss",{{"E",1e5},{"A",1e-2}});
		geom1->addStructure(str1);
		comp::structure str2("beam",{{"E",1e5},{"width",0.1},{"height",0.1},{"poisson",0.3}});
		geom2->addStructure(str2);
		comp::structure str3("flat_shell",{{"E",1e5},{"thickness",0.1},{"poisson",0.3}});
		geom3->addStructure(str3);
		str3.isVisible() = false;
		geom4->addStructure(str3);
		comp::structure str4("quad_hexahedron",{{"E",1e5},{"poisson",0.3}});
		geom5->addStructure(str4);
		str4.isVisible() = false;
		geom6->addStructure(str4);

		visualize(sd,"component","Visualization of sd model without ghost components",false);
		visualize(sd,"component","Visualization of sd model with ghost components",true);
		visualize(sd,"component","Visualization of sd model with ghost components",true,{{{1,0,0},{1,1,10}},{{-0.5,0,0},{1,0,0}}});
		
		sd.mesh(2);
		visualize(sd,"element","Visualization of meshed sd model without ghost elements",false);
		visualize(sd,"element","Visualization of meshed sd model with ghost elements",true);
	}
	
	BOOST_AUTO_TEST_CASE( sd_model_strain_energy_visualization )
	{
		bso::structural_design::sd_model sd;

		namespace geom = bso::utilities::geometry;
		namespace comp = bso::structural_design::component;
		
		comp::constraint c0(0);
		comp::constraint c1(1);
		comp::constraint c2(2);

		comp::load_case lc1("vertical load");
		comp::load l1(lc1,-1e5,2);
		
		comp::structure str1("quad_hexahedron",{{"E",1e5},{"poisson",0.3}});
		
		auto p1 = sd.addPoint({0,0,10});
		p1->addLoad(l1);
		
		auto quad1 = sd.addGeometry(geom::quadrilateral({{0,0,0},{0,10,0},{10,10,0},{10,0,0}}));
		auto quad2 = sd.addGeometry(geom::quadrilateral({{0,0,0},{0,0,10},{10,0,10},{10,0,0}}));
		auto quad3 = sd.addGeometry(geom::quadrilateral({{0,0,0},{0,0,10},{0,10,10},{0,10,0}}));
		auto quadHex1 = sd.addGeometry(geom::quad_hexahedron({
					{0,0,0},{10,0,0},{10,10,0},{0,10,0},{0,0,10},{10,0,10},{10,10,10},{0,10,10}}));
		
		quadHex1->addStructure(str1);
		quad1->addConstraint(c2);
		quad2->addConstraint(c1);
		quad3->addConstraint(c0);
		
		sd.mesh(5);
		sd.analyze();
		visualize(sd,"strain_energy","Visualization of strain energy in analyzed sd model");
	}
	
	BOOST_AUTO_TEST_CASE( sd_model_topopt_visualization )
	{
		bso::structural_design::sd_model sd;

		namespace geom = bso::utilities::geometry;
		namespace comp = bso::structural_design::component;
		
		comp::constraint c0(0);
		comp::constraint c1(1);
		comp::constraint c2(2);
		comp::constraint c3(3);
		comp::constraint c4(4);
		comp::constraint c5(5);
		
		comp::load_case lc1("vertical load");
		comp::load l1(lc1, 1,2);
		
		comp::structure str1("flat_shell",{{"E",1},{"thickness",1},{"poisson",0.3}});
		
		auto p1 = sd.addPoint({0,0,20});
		auto p2 = sd.addPoint({60,0,0});
		p2->addConstraint(c2);
		p1->addLoad(l1);

		auto line1 = sd.addGeometry(geom::line_segment({{0,0,0},{0,0,20}}));
		line1->addConstraint(c0);
		
		auto quad1 = sd.addGeometry(geom::quadrilateral({{0,0,0},{0,0,20},{20,0,20},{20,0,0}}));
		auto quad2 = sd.addGeometry(geom::quadrilateral({{20,0,0},{20,0,20},{40,0,20},{40,0,0}}));
		auto quad3 = sd.addGeometry(geom::quadrilateral({{40,0,0},{40,0,20},{60,0,20},{60,0,0}}));
		
		quad1->addStructure(str1);
		quad2->addStructure(str1);
		quad3->addStructure(str1);
		quad1->addConstraint(c1); quad1->addConstraint(c3); quad1->addConstraint(c4);
		quad2->addConstraint(c1); quad2->addConstraint(c3); quad2->addConstraint(c4);
		quad3->addConstraint(c1); quad3->addConstraint(c3); quad3->addConstraint(c4);
		
		sd.mesh(20);
		sd.analyze();
		visualize(sd,"strain_energy","Visualization of strain energy in analyzed sd model");
		sd.topologyOptimization<bso::structural_design::topology_optimization::SIMP>(0.5,1.5,3.0,0.2,1e-2);
		visualize(sd,"density","Visualization of densities in topology optimized sd model using SIMP algorithm");
		sd.topologyOptimization<bso::structural_design::topology_optimization::ROBUST>(0.5,1.5,3.0,0.2,1e-2);
		visualize(sd,"density","Visualization of densities in topology optimized sd model using robust algorithm");
	}
	
	BOOST_AUTO_TEST_CASE( bp_model_visualization )
	{
		using namespace bso::building_physics;
		bp_model bp;
		bso::utilities::geometry::quad_hexahedron bpGeom({
			{0,0,0},{3000,0,0},{3000,3000,0},{0,3000,0},
			{0,0,3000},{3000,0,3000},{3000,3000,3000},{0,3000,3000}
		});
		
		// weather profile
		state::weather_profile* wp = new  state::weather_profile(bp.getNextIndependentIndex());
		bp.addState(wp);
		
		// ground profile
		state::ground_profile* gp = new state::ground_profile(bp.getNextIndependentIndex(),10);
		bp.addState(gp);
		
		// space
		bso::building_physics::properties::space_settings spaceSettings("testSpace",100,0,20,22,1.0);
		auto spacePtr = new state::space(bp.getNextDependentIndex(),&bpGeom,
					spaceSettings, wp); 
		bp.addState(spacePtr);
		
		// walls and windows
		Eigen::Vector4d wallVisProp, floorVisProp, windowVisProp;
		windowVisProp << 0.1,		0.5,		0.71,		0.35;
		floorVisProp << 0.35,		0.72,		0.35,		1.0;
		wallVisProp << 0.72,		0.35,		0.06,		1.0;
		bso::building_physics::properties::material m1("mat1","concrete",2400,850,1.8);
		bso::building_physics::properties::material m2("mat2","insulation",60,850,0.04);
		std::vector<bso::building_physics::properties::layer> layers = {
			bso::building_physics::properties::layer(m1,100),
			bso::building_physics::properties::layer(m2,50)};
		bso::building_physics::properties::construction wallConstruction("testWall",layers,0.5,{"wallVis",wallVisProp});
		bso::building_physics::properties::construction floorConstruction("testFloor",layers,0.5,{"floorVis",floorVisProp});
		bso::building_physics::properties::glazing windowGlazing("testGlass",1.2,3e4,{"windowVis",windowVisProp});
		
		unsigned int counter = 0;
		for (const auto& i : bpGeom.getPolygons())
		{
			if (counter == 5)
			{
				bp.addState(new state::floor(bp.getNextDependentIndex(),i,floorConstruction,
					spacePtr, gp));
			}
			else if (counter == 4)
			{
				bp.addState(new state::floor(bp.getNextDependentIndex(),i,floorConstruction,
					spacePtr, wp));
			}
			else if (counter == 0)
			{
				bp.addState(new state::window(bp.getNextDependentIndex(),i,windowGlazing,
					spacePtr, wp));
			}
			else
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, wp));
			}
			counter++;
		}
		bso::visualization::visualize(bp,"test_model_building_physics_visualization");
	}
	
	BOOST_AUTO_TEST_CASE( end_visualization )
	{
		bso::visualization::endVisualization();
	}

BOOST_AUTO_TEST_SUITE_END()

} // namespace visualization_test