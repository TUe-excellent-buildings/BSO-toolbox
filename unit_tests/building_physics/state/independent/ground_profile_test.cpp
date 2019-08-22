#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "bp_indep_state_ground_profile_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/state/independent/ground_profile.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_state_test {
using namespace bso::building_physics::state;

BOOST_AUTO_TEST_SUITE( bp_indep_state_ground_profile_test )

	BOOST_AUTO_TEST_CASE( initialization )
	{
		BOOST_REQUIRE_NO_THROW(independent::ground_profile gp1(2,10));
		independent::ground_profile gp1(3,12);
		BOOST_REQUIRE(!gp1.isDependent());
		BOOST_REQUIRE( gp1.isIndependent());
		BOOST_REQUIRE(!gp1.isSpace());
		BOOST_REQUIRE(!gp1.isFloor());
		BOOST_REQUIRE(!gp1.isWall());
		BOOST_REQUIRE(!gp1.isWindow());
		BOOST_REQUIRE(!gp1.isWeatherProfile());
		BOOST_REQUIRE( gp1.isGroundProfile() );
		BOOST_REQUIRE(gp1.getIndex() ==3);
		BOOST_REQUIRE(gp1.getTemperature() == 12);
	}
	
	BOOST_AUTO_TEST_CASE( init_system )
	{
		boost::posix_time::ptime testTime = boost::posix_time::from_iso_string("20190814T155000");
		
		bso::building_physics::state_space_system ss(3,2);
		independent::ground_profile gp1(1,11);
		gp1.initSystem(ss);

		Eigen::VectorXd checku = Eigen::VectorXd::Ones(2);
		checku(1) = 11;

		BOOST_REQUIRE(ss.getu().isApprox(checku, 1e-9));
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_state_test