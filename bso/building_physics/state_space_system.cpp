#ifndef BSO_BP_STATE_SPACE_SYSTEM_CPP
#define BSO_BP_STATE_SPACE_SYSTEM_CPP

namespace bso { namespace building_physics {

state_space_system::state_space_system(const unsigned int& dependentCount,
																			 const unsigned int& independentCount)
: mA(Eigen::MatrixXd::Zero(dependentCount,dependentCount)),
	mB(Eigen::MatrixXd::Zero(dependentCount,independentCount)),
	mx(Eigen::VectorXd::Zero(dependentCount)),
	mu(Eigen::VectorXd::Ones(independentCount))
{

} // ctor()

state_space_system::~state_space_system()
{
	
} // dtor()

void state_space_system::resetSystem()
{
	mA.setZero();
	mB.setZero();
	mx.setZero();
	mu.setOnes();
}

void state_space_system::setStartTime(const boost::posix_time::ptime& startTime)
{
	mStartTime = startTime;
	mPreviousTime = startTime;
	mCurrentTime = startTime;
} // setStartTime()

void state_space_system::updateTime(const boost::posix_time::ptime& newTime)
{
	mPreviousTime = mCurrentTime;
	mCurrentTime = newTime;
} // updateTime()

void state_space_system::operator()(const Eigen::VectorXd& x,	Eigen::VectorXd& dxdt,
	const double& t)
{
	dxdt = mA*x + mB*mu;
	if (t < 0) dxdt *= -1;
} // ODE function

} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_STATE_SPACE_SYSTEM_CPP