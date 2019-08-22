#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_indep_state_weather_profile_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/state/independent/weather_profile.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_state_test {
using namespace bso::building_physics::state;

BOOST_AUTO_TEST_SUITE( bp_indep_state_weather_profile_test )

	BOOST_AUTO_TEST_CASE( initialization )
	{
		BOOST_REQUIRE_NO_THROW(independent::weather_profile wp1(3));
		independent::weather_profile wp1(2);
		BOOST_REQUIRE(!wp1.isDependent());
		BOOST_REQUIRE( wp1.isIndependent());
		BOOST_REQUIRE(!wp1.isSpace());
		BOOST_REQUIRE(!wp1.isFloor());
		BOOST_REQUIRE(!wp1.isWall());
		BOOST_REQUIRE(!wp1.isWindow());
		BOOST_REQUIRE( wp1.isWeatherProfile());
		BOOST_REQUIRE(!wp1.isGroundProfile() );
		BOOST_REQUIRE(wp1.getIndex() == 2);
		BOOST_REQUIRE(wp1.getWeatherData().size() == 0);
	}
	
	BOOST_AUTO_TEST_CASE( load_weather_data )
	{
		independent::weather_profile wp1(2);
		boost::posix_time::ptime t1(boost::posix_time::from_iso_string("19760702T000000"));
		boost::posix_time::ptime t2(boost::posix_time::from_iso_string("19760703T150000"));
		boost::posix_time::ptime t3(boost::posix_time::from_iso_string("19760705T240000"));
		boost::posix_time::ptime t4(boost::posix_time::from_iso_string("19760706T240000"));
		
		BOOST_REQUIRE_THROW(wp1.loadNewPeriod(t1,t3,"invalid_file_name"), std::invalid_argument);
		BOOST_REQUIRE_THROW(wp1.loadNewPeriod(t1,t4,"building_physics/test_weather_data_1.txt"),
			std::runtime_error);
		BOOST_REQUIRE_THROW(wp1.loadNewPeriod(t3,t1,"building_physics/test_weather_data_1.txt"),
			std::invalid_argument);
		
		wp1.loadNewPeriod(t1,t3,"building_physics/test_weather_data_1.txt");
		
		BOOST_REQUIRE(wp1.getWeatherData().size() == 97); 
		BOOST_REQUIRE(wp1.getWeatherData().find(t1)->second == 16.1);
		BOOST_REQUIRE(wp1.getWeatherData().find(t2)->second == 34.5);
		BOOST_REQUIRE(wp1.getWeatherData().find(t3)->second == 20.3);
	}
	
	BOOST_AUTO_TEST_CASE( update_system )
	{
		independent::weather_profile wp1(1);
		boost::posix_time::ptime t1(boost::posix_time::from_iso_string("19760702T000000"));
		boost::posix_time::ptime t2(boost::posix_time::from_iso_string("19760703T153000"));
		boost::posix_time::ptime t3(boost::posix_time::from_iso_string("19760705T240000"));
		boost::posix_time::ptime t4(boost::posix_time::from_iso_string("19760706T240000"));
		
		bso::building_physics::state_space_system ss(3,2);
		Eigen::VectorXd checku1 = Eigen::VectorXd::Ones(2);
		auto checku2 = checku1, checku3 = checku1;
		
		BOOST_REQUIRE_THROW(wp1.updateSystem(ss),std::runtime_error);
		
		wp1.loadNewPeriod(t1,t3,"building_physics/test_weather_data_1.txt");
		BOOST_REQUIRE_THROW(wp1.updateSystem(ss),std::runtime_error);
	
		ss.resetSystem();
		ss.setStartTime(t1);
		wp1.updateSystem(ss);
		checku1(1) = 16.1;
		BOOST_REQUIRE(ss.getu().isApprox(checku1, 1e-9));
		
		ss.resetSystem();
		ss.setStartTime(t2);
		wp1.updateSystem(ss);
		checku2(1) = 33.8;
		BOOST_REQUIRE(ss.getu().isApprox(checku2, 1e-9));
		
		ss.resetSystem();
		ss.setStartTime(t3);
		wp1.updateSystem(ss);
		checku3(1) = 20.3;
		BOOST_REQUIRE(ss.getu().isApprox(checku3, 1e-9));
	}

	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_state_test