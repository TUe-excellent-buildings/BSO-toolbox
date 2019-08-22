#ifndef BOOST_TEST_MODULE
#define BOOST_TEST_MODULE "state_space_system_test"
#endif

#include <boost/test/included/unit_test.hpp>

#include <bso/building_physics/state_space_system.hpp>

#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/eigen/eigen_algebra.hpp>

/*
BOOST_TEST()
BOOST_REQUIRE_THROW(function, std::domain_error)
BOOST_REQUIRE(!s[8].dominates(s[9]) && !s[9].dominates(s[8]))
BOOST_CHECK_EQUAL_COLLECTIONS(a.begin(), a.end(), b.begin(), b.end());
*/

namespace building_physics_test {
using namespace bso::building_physics;

BOOST_AUTO_TEST_SUITE( state_space_system_test )

	BOOST_AUTO_TEST_CASE( initialization_and_getters )
	{
		state_space_system ss(3,2);
		Eigen::MatrixXd checkA = Eigen::MatrixXd::Zero(3,3);
		Eigen::MatrixXd checkB = Eigen::MatrixXd::Zero(3,2);
		Eigen::VectorXd checkx = Eigen::VectorXd::Zero(3);
		Eigen::VectorXd checku = Eigen::VectorXd::Ones(2);
		
		BOOST_REQUIRE(ss.getA().isApprox(checkA, 1e-9));
		BOOST_REQUIRE(ss.getB().isApprox(checkB, 1e-9));
		BOOST_REQUIRE(ss.getx().isApprox(checkx, 1e-9));
		BOOST_REQUIRE(ss.getu().isApprox(checku, 1e-9));
	}

	BOOST_AUTO_TEST_CASE( simple_ODE )
	{ // from MSc. thesis S. Boonstra pp. 43-47
		state_space_system ss(3,2);
		
		ss.getA()(0,0) = (-1/(0.13*2400) - 1/(3.0*2400)); //  -0.00334401709
		ss.getA()(0,1) = 1/(3.0*2400); // 0.000138888888
		ss.getA()(1,0) = 1/(3.0*104400); // 0.00000319284
		ss.getA()(1,1) = -1/(3.0*104400)-1/(0.056*104400); //  -0.00017423827
		ss.getA()(1,2) = 1/(0.056*104400); // 0.00017104542
		ss.getA()(2,1) = 1/(0.056*102000); // 0.00017507002
		ss.getA()(2,2) = -1/(0.056*102000)-1/(0.04*102000); // -0.00042016806
		ss.getB()(0,0) = 1/(0.13*2400); // 0.0032051282
		ss.getB()(2,1) = 1/(0.04*102000); // 0.00024509803
		ss.getx()(0)   = 5;
		ss.getx()(1)   = 5;
		ss.getx()(2)   = 5;
		ss.getu()(0)   = 20;
		ss.getu()(1)   = -10;
		
		Eigen::VectorXd checkTemps(3);
		checkTemps << 18.79, -9.10, -9.62;
		double duration = 24*60*60;
		double timeStep = 10;
		double timeStart = 0;

		namespace odeint = boost::numeric::odeint;
		odeint::runge_kutta4<Eigen::VectorXd,double,Eigen::VectorXd,double,
												 odeint::vector_space_algebra> stepper_rk4;
		odeint::runge_kutta_cash_karp54<Eigen::VectorXd,double,Eigen::VectorXd,double,
																		odeint::vector_space_algebra> stepper_rkck54;

		BOOST_REQUIRE_NO_THROW(odeint::integrate_const(stepper_rk4, boost::ref(ss), ss.getx(),
			timeStart, duration, timeStep));
		BOOST_REQUIRE(checkTemps.isApprox(ss.getx(), 1e-3));
		
		ss.getx()(0)   = 5;
		ss.getx()(1)   = 5;
		ss.getx()(2)   = 5;
		
		BOOST_REQUIRE_NO_THROW(odeint::integrate_adaptive(
			odeint::make_controlled(1.0e-10, 1.0e-6, stepper_rkck54),
			boost::ref(ss), ss.getx(), timeStart, duration, timeStep));
		BOOST_REQUIRE(checkTemps.isApprox(ss.getx(), 1e-3));
	}
	
BOOST_AUTO_TEST_SUITE_END()
} // namespace building_physics_test