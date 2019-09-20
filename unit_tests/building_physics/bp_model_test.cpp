#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_model_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/bp_model.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_test {
using namespace bso::building_physics;

BOOST_AUTO_TEST_SUITE( bp_model_test )

	BOOST_AUTO_TEST_CASE( initialize_empty )
	{
		BOOST_REQUIRE_NO_THROW(bp_model bp1);
		
		bp_model bp1;
		BOOST_REQUIRE(bp1.getStates().size() == 0);
		BOOST_REQUIRE(bp1.getDependentStates().size() == 0);
		BOOST_REQUIRE(bp1.getIndependentStates().size() == 0);
		BOOST_REQUIRE(bp1.getWalls().size() == 0);
		BOOST_REQUIRE(bp1.getFloors().size() == 0);
		BOOST_REQUIRE(bp1.getWindows().size() == 0);
		BOOST_REQUIRE(bp1.getNextDependentIndex() == 0);
		BOOST_REQUIRE(bp1.getNextDependentIndex() == 1);
		BOOST_REQUIRE(bp1.getNextIndependentIndex() == 1);
		BOOST_REQUIRE(bp1.getNextIndependentIndex() == 2);
	}
	
	BOOST_AUTO_TEST_CASE( concrete_box_with_heat_with_vent )
	{ // example take from MSc thesis S. Boonstra, although heating has been implemented differently
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
		
		// walls
		bso::building_physics::properties::material m1("mat1","concrete",2400,850,1.8);
		bso::building_physics::properties::material m2("mat2","insulation",60,850,0.04);
		std::vector<bso::building_physics::properties::layer> layers = {
			bso::building_physics::properties::layer(m1,100),
			bso::building_physics::properties::layer(m2,50)};
		bso::building_physics::properties::construction wallConstruction("testWall",layers);
		
		unsigned int counter = 0;
		for (const auto& i : bpGeom.getPolygons())
		{
			if (counter == 0)
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, gp));
			}
			else
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, wp));
			}
			counter++;
		}
		
		// add a period to simulate
		boost::posix_time::ptime start(boost::posix_time::from_iso_string("19850901T000000"));
		boost::posix_time::ptime end(boost::posix_time::from_iso_string("19851001T000000"));
		bp.addSimulationPeriod("building_physics/test_weather_data_3.txt",
			boost::posix_time::time_period(start,end));
		
		// set the timestep duration
		boost::posix_time::time_duration timeStepSize(0,15,0,0);
		bp.setTimeStepSize(timeStepSize);
		
		// set the warm up period duration
		boost::posix_time::time_duration warmUpDuration(6*24,0,0,0);
		bp.setWarmUpDuration(warmUpDuration);
		bp.setInitialStateTemperatures(0);
		
		// do the simulating
		bp.simulatePeriods("runge_kutta_dopri5",1e-6,1e-6);
		
		//check results
		Eigen::VectorXd checkx(7);
		checkx << 20,18.8,19.5,19.5,19.5,19.5,19.5;
		BOOST_REQUIRE(abs(bp.getHeatingEnergies().begin()->second.begin()->second/204.486-1) < 1e-5);
		BOOST_REQUIRE(bp.getStateSpaceSystem().getx().isApprox(checkx,1e-2));
	}
	
	BOOST_AUTO_TEST_CASE( concrete_box_with_heat_no_vent )
	{
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
		bso::building_physics::properties::space_settings spaceSettings("testSpace",100,0,20,22,0.0);
		auto spacePtr = new state::space(bp.getNextDependentIndex(),&bpGeom,
					spaceSettings, wp); 
		bp.addState(spacePtr);
		
		// walls
		bso::building_physics::properties::material m1("mat1","concrete",2400,850,1.8);
		bso::building_physics::properties::material m2("mat2","insulation",60,850,0.04);
		std::vector<bso::building_physics::properties::layer> layers = {
			bso::building_physics::properties::layer(m1,100),
			bso::building_physics::properties::layer(m2,50)};
		bso::building_physics::properties::construction wallConstruction("testWall",layers);
		
		unsigned int counter = 0;
		for (const auto& i : bpGeom.getPolygons())
		{
			if (counter == 0)
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, gp));
			}
			else
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, wp));
			}
			counter++;
		}
		
		// add a period to simulate
		boost::posix_time::ptime start(boost::posix_time::from_iso_string("19850901T000000"));
		boost::posix_time::ptime end(boost::posix_time::from_iso_string("19851001T000000"));
		bp.addSimulationPeriod("building_physics/test_weather_data_3.txt",
			boost::posix_time::time_period(start,end));
		
		// set the timestep duration
		boost::posix_time::time_duration timeStepSize(0,15,0,0);
		bp.setTimeStepSize(timeStepSize);
		
		// set the warm up period duration
		boost::posix_time::time_duration warmUpDuration(6*24,0,0,0);
		bp.setWarmUpDuration(warmUpDuration);
		bp.setInitialStateTemperatures(0);
		
		// do the simulating
		bp.simulatePeriods("runge_kutta_dopri5",1e-6,1e-6);
		
		//check results
		Eigen::VectorXd checkx(7);
		checkx << 20,18.8,19.5,19.5,19.5,19.5,19.5;
		BOOST_REQUIRE(abs(bp.getHeatingEnergies().begin()->second.begin()->second/168.24-1) < 1e-5);
		BOOST_REQUIRE(bp.getStateSpaceSystem().getx().isApprox(checkx,1e-2));
	}
	
	BOOST_AUTO_TEST_CASE( concrete_box_no_heat_with_vent )
	{
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
		bso::building_physics::properties::space_settings spaceSettings("testSpace",0,0,20,22,1.0);
		auto spacePtr = new state::space(bp.getNextDependentIndex(),&bpGeom,
					spaceSettings, wp); 
		bp.addState(spacePtr);
		
		// walls
		bso::building_physics::properties::material m1("mat1","concrete",2400,850,1.8);
		bso::building_physics::properties::material m2("mat2","insulation",60,850,0.04);
		std::vector<bso::building_physics::properties::layer> layers = {
			bso::building_physics::properties::layer(m1,100),
			bso::building_physics::properties::layer(m2,50)};
		bso::building_physics::properties::construction wallConstruction("testWall",layers);
		
		unsigned int counter = 0;
		for (const auto& i : bpGeom.getPolygons())
		{
			if (counter == 0)
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, gp));
			}
			else
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, wp));
			}
			counter++;
		}
		
		// add a period to simulate
		boost::posix_time::ptime start(boost::posix_time::from_iso_string("19850901T000000"));
		boost::posix_time::ptime end(boost::posix_time::from_iso_string("19851001T000000"));
		bp.addSimulationPeriod("building_physics/test_weather_data_3.txt",
			boost::posix_time::time_period(start,end));
		
		// set the timestep duration
		boost::posix_time::time_duration timeStepSize(0,15,0,0);
		bp.setTimeStepSize(timeStepSize);
		
		// set the warm up period duration
		boost::posix_time::time_duration warmUpDuration(6*24,0,0,0);
		bp.setWarmUpDuration(warmUpDuration);
		bp.setInitialStateTemperatures(0);
		
		// do the simulating
		bp.simulatePeriods("runge_kutta_dopri5",1e-6,1e-6);
		
		//check results
		Eigen::VectorXd checkx(7);
		checkx << 13.7,13.1,13.8,13.8,13.8,13.8,13.8;
		BOOST_REQUIRE(bp.getHeatingEnergies().begin()->second.begin()->second == 0.0);
		BOOST_REQUIRE(bp.getStateSpaceSystem().getx().isApprox(checkx,1e-2));
	}
	
	BOOST_AUTO_TEST_CASE( concrete_box_no_heat_no_vent )
	{
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
		bso::building_physics::properties::space_settings spaceSettings("testSpace",0,0,20,22,0.0);
		auto spacePtr = new state::space(bp.getNextDependentIndex(),&bpGeom,
					spaceSettings, wp); 
		bp.addState(spacePtr);
		
		// walls
		bso::building_physics::properties::material m1("mat1","concrete",2400,850,1.8);
		bso::building_physics::properties::material m2("mat2","insulation",60,850,0.04);
		std::vector<bso::building_physics::properties::layer> layers = {
			bso::building_physics::properties::layer(m1,100),
			bso::building_physics::properties::layer(m2,50)};
		bso::building_physics::properties::construction wallConstruction("testWall",layers);
		
		unsigned int counter = 0;
		for (const auto& i : bpGeom.getPolygons())
		{
			if (counter == 0)
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, gp));
			}
			else
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, wp));
			}
			counter++;
		}
		
		// add a period to simulate
		boost::posix_time::ptime start(boost::posix_time::from_iso_string("19850901T000000"));
		boost::posix_time::ptime end(boost::posix_time::from_iso_string("19851001T000000"));
		bp.addSimulationPeriod("building_physics/test_weather_data_3.txt",
			boost::posix_time::time_period(start,end));
		
		// set the timestep duration
		boost::posix_time::time_duration timeStepSize(0,15,0,0);
		bp.setTimeStepSize(timeStepSize);
		
		// set the warm up period duration
		boost::posix_time::time_duration warmUpDuration(6*24,0,0,0);
		bp.setWarmUpDuration(warmUpDuration);
		bp.setInitialStateTemperatures(0);
		
		// do the simulating
		bp.simulatePeriods("runge_kutta_dopri5",1e-6,1e-6);
		
		//check results
		Eigen::VectorXd checkx(7);
		checkx << 13.6,13.0,13.7,13.7,13.7,13.7,13.7;
		BOOST_REQUIRE(bp.getHeatingEnergies().begin()->second.begin()->second == 0.0);
		BOOST_REQUIRE(bp.getStateSpaceSystem().getx().isApprox(checkx,1e-2));
	}
	
	BOOST_AUTO_TEST_CASE( concrete_box_with_cool_no_vent )
	{
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
		bso::building_physics::properties::space_settings spaceSettings("testSpace",0,100,0,10,0.0);
		auto spacePtr = new state::space(bp.getNextDependentIndex(),&bpGeom,
					spaceSettings, wp); 
		bp.addState(spacePtr);
		
		// walls
		bso::building_physics::properties::material m1("mat1","concrete",2400,850,1.8);
		bso::building_physics::properties::material m2("mat2","insulation",60,850,0.04);
		std::vector<bso::building_physics::properties::layer> layers = {
			bso::building_physics::properties::layer(m1,100),
			bso::building_physics::properties::layer(m2,50)};
		bso::building_physics::properties::construction wallConstruction("testWall",layers);
		
		unsigned int counter = 0;
		for (const auto& i : bpGeom.getPolygons())
		{
			if (counter == 0)
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, gp));
			}
			else
			{
				bp.addState(new state::wall(bp.getNextDependentIndex(),i,wallConstruction,
					spacePtr, wp));
			}
			counter++;
		}
		
		// add a period to simulate
		boost::posix_time::ptime start(boost::posix_time::from_iso_string("19850901T000000"));
		boost::posix_time::ptime end(boost::posix_time::from_iso_string("19851001T000000"));
		bp.addSimulationPeriod("building_physics/test_weather_data_3.txt",
			boost::posix_time::time_period(start,end));
		
		// set the timestep duration
		boost::posix_time::time_duration timeStepSize(0,15,0,0);
		bp.setTimeStepSize(timeStepSize);
		
		// set the warm up period duration
		boost::posix_time::time_duration warmUpDuration(6*24,0,0,0);
		bp.setWarmUpDuration(warmUpDuration);
		bp.setInitialStateTemperatures(0);
		
		// do the simulating
		bp.simulatePeriods("runge_kutta_dopri5",1e-6,1e-6);
		
		//check results
		Eigen::VectorXd checkx(7);
		checkx << 10,10,10.7,10.7,10.7,10.7,10.7;
		BOOST_REQUIRE(abs(bp.getCoolingEnergies().begin()->second.begin()->second/96.5259-1) < 1e-5);
		BOOST_REQUIRE(bp.getStateSpaceSystem().getx().isApprox(checkx,1e-2));
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_test