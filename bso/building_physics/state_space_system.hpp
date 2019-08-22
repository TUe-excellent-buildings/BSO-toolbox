#ifndef BSO_BP_STATE_SPACE_SYSTEM_HPP
#define BSO_BP_STATE_SPACE_SYSTEM_HPP

#include <sstream>
#include <ostream> 

#include <Eigen/Dense>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <bso/building_physics/state/state.hpp>

namespace bso { namespace building_physics {

class state_space_system
{
private:
	Eigen::MatrixXd mA, mB;
	Eigen::VectorXd mx, mu;
	
	boost::posix_time::ptime mPreviousTime;
	boost::posix_time::ptime mCurrentTime;
	boost::posix_time::ptime mStartTime;
public:
	state_space_system(const unsigned int& dependentCount,
										 const unsigned int& independentCount);
	~state_space_system();
	
	Eigen::MatrixXd& getA() {return mA;}
	Eigen::MatrixXd& getB() {return mB;}
	Eigen::VectorXd& getx() {return mx;}
	Eigen::VectorXd& getu() {return mu;}
	
	const Eigen::MatrixXd& getA() const {return mA;}
	const Eigen::MatrixXd& getB() const {return mB;}
	const Eigen::VectorXd& getx() const {return mx;}
	const Eigen::VectorXd& getu() const {return mu;}
	
	void resetSystem();
	void setStartTime(const boost::posix_time::ptime& startTime);
	void updateTime(const boost::posix_time::ptime& newTime);
	const boost::posix_time::ptime& getStartTime() const {return mStartTime;}
	const boost::posix_time::ptime& getPreviousTime() const {return mPreviousTime;}
	const boost::posix_time::ptime& getCurrentTime() const {return mCurrentTime;}
	
	void operator()(const Eigen::VectorXd& x, Eigen::VectorXd& dxdt, const double& t);
};

} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state_space_system.cpp>

#endif // BSO_BP_STATE_SPACE_SYSTEM_HPP