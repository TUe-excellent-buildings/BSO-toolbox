#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_dep_state_space_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/state/dependent/space.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_state_test {
using namespace bso::building_physics::state;

BOOST_AUTO_TEST_SUITE( bp_dep_state_space_test )

	BOOST_AUTO_TEST_CASE( intialization )
	{
		bso::utilities::geometry::quad_hexahedron qh1 ({
			{0,0,0},{1,0,0},{1,1,0},{0,1,0},{0,0,1},{1,0,1},{1,1,1},{0,1,1}});
		bso::building_physics::properties::space_settings
			setting1("space_setting_1",100,150,20,25,1.0);
		independent::weather_profile wp1(1);
		boost::posix_time::ptime t1(boost::posix_time::from_iso_string("19760702T000000"));
		boost::posix_time::ptime t2(boost::posix_time::from_iso_string("19760705T240000"));
		wp1.loadNewPeriod(t1,t2,"building_physics/test_weather_data_1.txt");
		
		BOOST_REQUIRE_NO_THROW(dependent::space space1(1,&qh1,setting1,&wp1));
		dependent::space space1(1,&qh1,setting1,&wp1);
		
		BOOST_REQUIRE(space1.getIndex() == 1);
		BOOST_REQUIRE( space1.isDependent());
		BOOST_REQUIRE(!space1.isIndependent());
		BOOST_REQUIRE( space1.isSpace());
		BOOST_REQUIRE(!space1.isFloor());
		BOOST_REQUIRE(!space1.isWall());
		BOOST_REQUIRE(!space1.isWindow());
		BOOST_REQUIRE(!space1.isWeatherProfile());
		BOOST_REQUIRE(!space1.isGroundProfile() );
		BOOST_REQUIRE(space1.getVolume() == 1e-9);
		BOOST_REQUIRE(space1.getCapacitance() == 3.6e-6);
		//BOOST_REQUIRE_THROW(space1.getGeometry(),std::runtime_error);
	}
	
	BOOST_AUTO_TEST_CASE( init_system )
	{
		bso::utilities::geometry::quad_hexahedron qh1({
			{0,0,0},{1e3,0,0},{1e3,1e3,0},{0,1e3,0},
			{0,0,1e3},{1e3,0,1e3},{1e3,1e3,1e3},{0,1e3,1e3}});
		bso::building_physics::properties::space_settings 
			setting1("space_setting_1",100,150,20,25,1.0);
		independent::weather_profile wp1(0);
		boost::posix_time::ptime t1(boost::posix_time::from_iso_string("19760702T000000"));
		boost::posix_time::ptime t2(boost::posix_time::from_iso_string("19760705T240000"));
		wp1.loadNewPeriod(t1,t2,"building_physics/test_weather_data_1.txt");
		
		dependent::space space1(1,&qh1,setting1,&wp1);
		
		bso::building_physics::state_space_system ss(3,2);
		ss.setStartTime(t1);
		wp1.initSystem(ss);
		space1.initSystem(ss);
		
		Eigen::MatrixXd checkA = Eigen::MatrixXd::Zero(3,3);
		checkA(1,1) = -9.259259e-5;
		BOOST_REQUIRE(ss.getA().isApprox(checkA, 1e-6));
		
		Eigen::MatrixXd checkB = Eigen::MatrixXd::Zero(3,2);
		checkB(1,0) = 9.259259e-5;
		BOOST_REQUIRE(ss.getB().isApprox(checkB, 1e-6));
	}
	
	BOOST_AUTO_TEST_CASE( update_system )
	{
		bso::utilities::geometry::quad_hexahedron qh1({
			{0,0,0},{1e3,0,0},{1e3,1e3,0},{0,1e3,0},
			{0,0,1e3},{1e3,0,1e3},{1e3,1e3,1e3},{0,1e3,1e3}});
		bso::building_physics::properties::space_settings 
			setting1("space_setting_1",100,150,20,25,1.0);
		independent::weather_profile wp1(1);
		boost::posix_time::ptime t1(boost::posix_time::from_iso_string("19760702T000000"));
		boost::posix_time::ptime t2(boost::posix_time::from_iso_string("19760705T240000"));
		wp1.loadNewPeriod(t1,t2,"building_physics/test_weather_data_1.txt");
		
		dependent::space space1(1,&qh1,setting1,&wp1);
		
		bso::building_physics::state_space_system ss(3,2);
		ss.setStartTime(t1);
		wp1.initSystem(ss);
		space1.initSystem(ss);
		
		Eigen::VectorXd dxdt(3);
		ss(ss.getx(),dxdt,10);
		space1.updateSystem(ss);
		
		ss.getx()(1) = 50;
		ss(ss.getx(),dxdt,20);
		space1.updateSystem(ss);
		
		BOOST_REQUIRE(abs(space1.getCumulativeHeatingEnergy()/((10)*(1.0/36.0)))-1 < 1e-9);
		BOOST_REQUIRE(abs(space1.getCumulativeCoolingEnergy()/((10)*(-1.5/36.0)))-1 < 1e-9);

		ss(ss.getx(),dxdt,40);
		space1.updateSystem(ss);
		
		ss.getx()(1) = 10;
		ss(ss.getx(),dxdt,46);
		space1.updateSystem(ss);
		
		BOOST_REQUIRE(abs(space1.getCumulativeHeatingEnergy()/((16)*(1.0/36.0)))-1 < 1e-9);
		BOOST_REQUIRE(abs(space1.getCumulativeCoolingEnergy()/((20)*(-1.5/36.0)))-1 < 1e-9);
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_state_test