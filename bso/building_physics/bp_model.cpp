#ifndef BSO_BP_MODEL_CPP
#define BSO_BP_MODEL_CPP

#include <boost/numeric/odeint.hpp>
#include <boost/numeric/odeint/external/eigen/eigen_algebra.hpp>

namespace bso { namespace building_physics {

void bp_model::mInitSystem()
{
	mSystem = state_space_system(mDependentCount, mIndependentCount);
	mIsInitialized = true;
} // intitializes the state space system

void bp_model::mPrintObserverHead(std::ostream& out)
{
	out << "\ntime";
	for (const auto& i : mStates)
	{
		if (i->isSpace()) 
		{
			out << (",t_space_" + i->getDescription());
			out << (",Q_heat_space_" + i->getDescription());
			out << (",Q_cool_space_" + i->getDescription());
		}
		else if (i->isWall()) out << (",t_wall_" + i->getDescription());
		else if (i->isFloor()) out << (",t_floor_" + i->getDescription());
		else if (i->isWindow()) out << (",t_window_" + i->getDescription());
		else if (i->isWeatherProfile()) out << (",t_weather_profile_" + i->getDescription());
		else if (i->isGroundProfile()) out << (",t_ground_profile_" + i->getDescription());
	}
	out << std::endl;
} // mPrintObserverHead()

void bp_model::mPrintSystemState(std::ostream& out)
{
	out << mSystem.getCurrentTime();
	for (const auto& i : mStates)
	{
		if (i->isDependent())
		{
			out << "," << mSystem.getx()(i->getIndex());
			if (i->isSpace())
			{ // also print out the heating and cooling loads
				auto spacePtr = dynamic_cast<state::space*>(i);
				double energyFlow = mSystem.getB()(i->getIndex()) * spacePtr->getCapacitance();
				if (energyFlow > 0) out << "," << energyFlow;
				else out << "," << 0.0;
				if (energyFlow < 0) out << "," << -energyFlow;
				else out << "," << 0.0;
			}
		}
		else if (i->isIndependent())
		{
			out << "," << mSystem.getu()(i->getIndex());
		}
	}
	out << std::endl;
} // mPrintSystemState()


template <class STEPPER_TYPE>
void bp_model::mSimulate(const boost::posix_time::time_period& period, std::ostream& out,
	const double& absError /* = 0.0*/, const double& relError /* = 0.0*/)
{
	namespace odeint = boost::numeric::odeint;
	STEPPER_TYPE stepper;
	boost::posix_time::ptime simulationTime;

	boost::posix_time::ptime warmUpEnd = period.begin() + mWarmUpDuration;
	if (mWarmUpDuration > period.length())
	{
		mWeatherProfile->loadNewPeriod(period.begin(),warmUpEnd,mSimulationPeriods[period]);
	}
	else mWeatherProfile->loadNewPeriod(period.begin(),period.last(),mSimulationPeriods[period]);

	// warm up period
	simulationTime = warmUpEnd;
	mSystem.setStartTime(simulationTime);
	mSystem.resetSystem();
	for (auto& i : mDependentStates) mSystem.getx()(i->getIndex()) = mInitialStateTemperatures;
	for (auto& i : mStates) i->initSystem(mSystem);

	while(simulationTime > period.begin())
	{
		simulationTime -= mTimeStepSize;
		mSystem.updateTime(simulationTime);
		for (auto& j : mIndependentStates) j->updateSystem(mSystem);
		for (auto& j : mDependentStates) j->updateSystem(mSystem);
		if (absError == 0 && relError == 0)
		{
			stepper.do_step(mSystem,mSystem.getx(),
				(double)(simulationTime - warmUpEnd).total_seconds(),
				-(double)(mTimeStepSize.total_seconds()));
		}
		else
		{
			odeint::integrate_const(odeint::make_controlled(absError,relError,stepper),
				std::ref(mSystem), mSystem.getx(),0.0,
				-(double)(mTimeStepSize).total_seconds(),-(double)(mTimeStepSize).total_seconds());
		}
	}
	for (auto& i : mSpaces) i->resetCumulativeEnergies();

	// if observer is on, print head
	if (mObserveSimulation)
	{
		out << "results for period: " << period << std::endl;
		this->mPrintObserverHead(out);
		this->mPrintSystemState(out);
	}

	// actual simulation
	simulationTime = period.begin();
	mSystem.setStartTime(simulationTime);
	for (auto& i : mSpaces) i->resetCumulativeEnergies();
	while (simulationTime < period.last())
	{
		simulationTime += mTimeStepSize;
		mSystem.updateTime(simulationTime);
		for (auto& j : mIndependentStates) j->updateSystem(mSystem);
		for (auto& j : mDependentStates) j->updateSystem(mSystem);
		if (absError == 0 && relError == 0)
		{
			stepper.do_step(mSystem,mSystem.getx(),
				(double)(simulationTime-period.begin()).total_seconds(),
				(double)(mTimeStepSize.total_seconds()));
			// odeint::integrate_const(stepper, std::ref(mSystem), mSystem.getx(),0.0,
				// (double)(mTimeStepSize).total_seconds(),(double)(mTimeStepSize).total_seconds());
		}
		else
		{
			odeint::integrate_const(odeint::make_controlled(absError,relError,stepper),
				std::ref(mSystem), mSystem.getx(),0.0,
				(double)(mTimeStepSize).total_seconds(),(double)(mTimeStepSize).total_seconds());
		}
		if (mObserveSimulation)
		{
			this->mPrintSystemState(out);
		}
	}

} // stepper()

bp_model::bp_model() : mSystem(state_space_system(0,0)),
	mWarmUpDuration(boost::posix_time::time_duration(92,0,0,0)), // default four warm up days
	mTimeStepSize(boost::posix_time::time_duration(0,15,0,0)) // default 4 time steps per hour
{
	mWeatherProfile = nullptr;
	mGroundProfile = nullptr;
} // ctor()

bp_model::bp_model(const bp_model& rhs) : mSystem(state_space_system(0,0))
{
	mWeatherProfile = nullptr;
	mGroundProfile = nullptr;
	std::map<state::state*, state::state*> stateCopies;
	auto weatherPtr = new state::weather_profile(this->getNextIndependentIndex());
	stateCopies.emplace(rhs.mWeatherProfile, weatherPtr);
	this->addState(weatherPtr);

	auto groundPtr = new state::ground_profile(this->getNextIndependentIndex(),
		rhs.mGroundProfile->getTemperature());
	stateCopies.emplace(rhs.mGroundProfile, groundPtr);
	this->addState(groundPtr);

	for (const auto& i : rhs.mSpaces)
	{
		auto spacePtr = new state::space(this->getNextDependentIndex(), i->getGeometry(),
			i->getSettings(), mWeatherProfile);
		stateCopies.emplace(i,spacePtr);
		this->addState(spacePtr);
	}
	for (const auto& i : rhs.mWalls)
	{
		auto side1Search = stateCopies.find(i->getSide1());
		auto side2Search = stateCopies.find(i->getSide2());
		
		if (side1Search == stateCopies.end() || side2Search == stateCopies.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, trying to copy a bp_wall\n"
									 << "cannot find the state on one of its sides.\n"
									 << "(bso/building_physics/bp_model.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		
		this->addState(new state::wall(this->getNextDependentIndex(), i->getGeometry(),
			i->getConstruction(),side1Search->second, side2Search->second));
	}
	for (const auto& i : rhs.mFloors)
	{
		auto side1Search = stateCopies.find(i->getSide1());
		auto side2Search = stateCopies.find(i->getSide2());
		
		if (side1Search == stateCopies.end() || side2Search == stateCopies.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, trying to copy a bp_floor\n"
									 << "cannot find the state on one of its sides.\n"
									 << "(bso/building_physics/bp_model.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		
		this->addState(new state::floor(this->getNextDependentIndex(), i->getGeometry(),
			i->getConstruction(),side1Search->second, side2Search->second));
	}
	for (const auto& i : rhs.mWindows)
	{
		auto side1Search = stateCopies.find(i->getSide1());
		auto side2Search = stateCopies.find(i->getSide2());
		
		if (side1Search == stateCopies.end() || side2Search == stateCopies.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, trying to copy a bp_window\n"
									 << "cannot find the state on one of its sides.\n"
									 << "(bso/building_physics/bp_model.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}

		this->addState(new state::window(this->getNextDependentIndex(), i->getGeometry(),
			i->getGlazing(),side1Search->second, side2Search->second));
	}

	mSimulationPeriods = rhs.mSimulationPeriods;
	mWarmUpDuration = rhs.mWarmUpDuration;
	mTimeStepSize = rhs.mTimeStepSize;
	mInitialStateTemperatures = rhs.mInitialStateTemperatures;
}

bp_model::~bp_model()
{
	for (auto& i : mStates)
	{
		delete i;
		i = nullptr;
	}
	mStates.clear();
	mDependentStates.clear();
	mIndependentStates.clear();
} // dtor()

void bp_model::addState(state::state* s)
{
	if (mIsInitialized) mIsInitialized = false;
	if (s->isDependent())
	{
		mStates.insert(mStates.begin(),s); // insert it at the beginning, so that these are updated before independent states are
		mDependentStates.push_back(dynamic_cast<state::dependent_state*>(s));
		
		if 			(s->isSpace())  mSpaces.push_back( dynamic_cast<state::space* >(s));
		else if (s->isWall())   mWalls.push_back(  dynamic_cast<state::wall*  >(s));
		else if (s->isFloor())  mFloors.push_back( dynamic_cast<state::floor* >(s));
		else if (s->isWindow()) mWindows.push_back(dynamic_cast<state::window*>(s));
	}
	else
	{
		mStates.push_back(s);
		mIndependentStates.push_back(dynamic_cast<state::independent_state*>(s));
		if (s->isWeatherProfile())
		{
			if (mWeatherProfile == nullptr)
			{
				mWeatherProfile = dynamic_cast<state::weather_profile*>(s);
			}
			else
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, trying to add more than one weather profile\n"
										 << "to a building physics model.\n"
										 << "(bso/building_physics/bp_model.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
		}
		if (s->isGroundProfile())
		{
			if (mGroundProfile == nullptr)
			{
				mGroundProfile = dynamic_cast<state::ground_profile*>(s);
			}
				else
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, trying to add more than one ground profile\n"
										 << "to a building physics model.\n"
										 << "(bso/building_physics/bp_model.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
		}
	}
} // addState()

void bp_model::addSimulationPeriod(const std::string& fileName,
	const boost::posix_time::time_period& simPeriod)
{
	if (mSimulationPeriods.find(simPeriod) != mSimulationPeriods.end())
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to add twice the same simulation\n"
								 << "period to a building physics model.\n"
								 << "(bso/building_physics/bp_model.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	mSimulationPeriods[simPeriod] = fileName;
} // addSimulationPeriod()

void bp_model::setWarmUpDuration(const boost::posix_time::time_duration& warmUpDuration)
{
	mWarmUpDuration = warmUpDuration;
} // setWarmUpDuration()

void bp_model::setTimeStepSize(const boost::posix_time::time_duration& timeStepSize)
{
	mTimeStepSize = timeStepSize;
} // setTimeStepSize()

void bp_model::setInitialStateTemperatures(const double& temperature)
{
	mInitialStateTemperatures = temperature;
} // setInitialStateTemperatures()

void bp_model::simulatePeriods(std::ostream& out,
	const std::string& stepperType /*= "runge_kutta_dopri5"*/, 
	const double& relError /*= 0.0*/, const double& absError /*= 0.0*/)
{
	this->mInitSystem();
	namespace odeint = boost::numeric::odeint;
	typedef odeint::runge_kutta_dopri5<Eigen::VectorXd,double,Eigen::VectorXd,
		double,odeint::vector_space_algebra> stepper_rkd5;
	typedef odeint::runge_kutta_cash_karp54<Eigen::VectorXd,double,Eigen::VectorXd,
		double,odeint::vector_space_algebra> stepper_rkck54;
	typedef odeint::runge_kutta_fehlberg78<Eigen::VectorXd,double,Eigen::VectorXd,
		double,odeint::vector_space_algebra> stepper_rkf78;

	for (const auto& i : mSimulationPeriods)
	{// simulate the period
		// define warm up period and load the weather data
		
		if (stepperType == "runge_kutta_dopri5")
		{
			this->mSimulate<stepper_rkd5>(i.first,out,absError,relError);
		}
		else if (stepperType == "runge_kutta_cash_karp54")
		{
			this->mSimulate<stepper_rkck54>(i.first,out,absError,relError);
		}
		else if (stepperType == "runge_kutta_fehlberg78")
		{
			this->mSimulate<stepper_rkf78>(i.first,out,absError,relError);
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, trying to simulate bp_model with an unknown\n"
									 << "stepper type: " << stepperType
									 << "(bso/building_physics/bp_model.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		// save the cumulative energies for both cooling and heating for this simulation period
		std::map<state::space*, double> tempHeatingEnergies;
		std::map<state::space*, double> tempCoolingEnergies;
		for (auto& j : mSpaces)
		{
			tempHeatingEnergies[j] = j->getCumulativeHeatingEnergy();
			tempCoolingEnergies[j] = j->getCumulativeCoolingEnergy();
		}
		mHeatingEnergies[i.first] = tempHeatingEnergies;
		mCoolingEnergies[i.first] = tempCoolingEnergies;
	}
	mObserveSimulation = true; // set it back to default in case simulation has not been observed
} // simulatePeriods()

void bp_model::simulatePeriods(const std::string& stepperType /*= "runge_kutta_dopri5"*/,
	const double& relError /*= 0.0*/, const double& absError /*= 0.0*/)
{
	mObserveSimulation = false;
	std::ostream dummyStream(0);
	this->simulatePeriods(dummyStream, stepperType, relError, absError);
} // simulatePeriods()

// double mTotalHeatingEnergy = 0.0;
// double mTotalCoolingEnergy = 0.0;
// double mTotalEnergy = 0.0;

bp_results bp_model::getTotalResults()
{
	bp_results results;
	
	for (const auto& i : mSpaces)
	{
		for (const auto& j : mSimulationPeriods)
		{
			results.mTotalHeatingEnergy += mHeatingEnergies[j.first][i];
			results.mTotalCoolingEnergy += mCoolingEnergies[j.first][i];
			results.mTotalEnergy				+= mHeatingEnergies[j.first][i];
			results.mTotalEnergy				+= mCoolingEnergies[j.first][i];
		}
	}
	
	return results;
} // getTotalResults()

bp_results bp_model::getPartialResults(bso::utilities::geometry::polyhedron* geom)
{
	bp_results results;
	
	for (const auto& i : mSpaces)
	{
		bool allPointsInsideOrOn = true;
		for (const auto& j : *(i->getGeometry()))
		{
			if (!geom->isInsideOrOn(j))
			{
				allPointsInsideOrOn = false;
				break;
			}
		}
		if (!allPointsInsideOrOn) continue;
		
		for (const auto& j : mSimulationPeriods)
		{
			results.mTotalHeatingEnergy += mHeatingEnergies[j.first][i];
			results.mTotalCoolingEnergy += mCoolingEnergies[j.first][i];
			results.mTotalEnergy				+= mHeatingEnergies[j.first][i];
			results.mTotalEnergy				+= mCoolingEnergies[j.first][i];
		}
	}
	
	return results;
} // getPartialResults()

} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_MODEL_CPP