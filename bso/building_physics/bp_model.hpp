#ifndef BSO_BP_MODEL_HPP
#define BSO_BP_MODEL_HPP

#include <bso/building_physics/state_space_system.hpp>
#include <bso/building_physics/state/states.hpp>

#include <vector>
#include <string>
#include <ostream>

namespace bso { namespace building_physics {

struct bp_results;

class bp_model
{
private:
	state_space_system mSystem;
	
	std::vector<state::state*						 > mStates;
	std::vector<state::dependent_state*  > mDependentStates;
	std::vector<state::independent_state*> mIndependentStates;
	std::vector<state::space* 					 > mSpaces;
	std::vector<state::wall*  					 > mWalls;
	std::vector<state::floor* 					 > mFloors;
	std::vector<state::window*					 > mWindows;
	state::weather_profile* 							 mWeatherProfile;
	state::ground_profile* 								 mGroundProfile;
	
	std::map<boost::posix_time::time_period, std::string> mSimulationPeriods;
	boost::posix_time::time_duration mWarmUpDuration;
	boost::posix_time::time_duration mTimeStepSize;
	
	std::map<boost::posix_time::time_period,std::map<state::space*,double>>
		mHeatingEnergies, mCoolingEnergies;
	
	double mInitialStateTemperatures = 0.0;
	bool mIsInitialized = false;
	bool mObserveSimulation = true;
	
	unsigned int mDependentCount = 0;
	unsigned int mIndependentCount = 1;
	void mInitSystem();
	void mPrintObserverHead(std::ostream& out);
	void mPrintSystemState(std::ostream& out);
	
	template <class STEPPER_TYPE>
	void mSimulate(const boost::posix_time::time_period& period, std::ostream& out,
		const double& absError /* = 0.0*/, const double& relError /* = 0.0*/);
	bp_model& operator = (bp_model& rhs) = default;
public:
	bp_model();
	bp_model(const bp_model& rhs);
	~bp_model();

	void addState(state::state*);
	void addSimulationPeriod(const std::string& fileName,
													 const boost::posix_time::time_period& simPeriod);
	void setWarmUpDuration(const boost::posix_time::time_duration& warmUpDuration);
	void setTimeStepSize(const boost::posix_time::time_duration& timeStepSize);
	void setInitialStateTemperatures(const double& temperature);
	
	void simulatePeriods(std::ostream& out,const std::string& stepperType = "runge_kutta_dopri5",
					const double& relError = 0.0, const double& absError = 0.0);
	void simulatePeriods(const std::string& stepperType = "runge_kutta_dopri5",
					const double& relError = 0.0, const double& absError = 0.0);
					
	bp_results getTotalResults();
	bp_results getPartialResults(bso::utilities::geometry::polyhedron* geom);
	
	const state_space_system& getStateSpaceSystem() const {return mSystem;}
	const unsigned int getNextDependentIndex() {return mDependentCount++;}
	const unsigned int getNextIndependentIndex() {return mIndependentCount++;}
	const std::vector<state::state*>& getStates() const {return mStates;}
	const std::vector<state::dependent_state*>& getDependentStates() const
				{return mDependentStates;}
	const std::vector<state::independent_state*>& getIndependentStates() const
				{return mIndependentStates;}
	const std::vector<state::space*>  getSpaces()  const {return mSpaces;}
	const std::vector<state::wall*>   getWalls()   const {return mWalls;}
	const std::vector<state::floor*>  getFloors()  const {return mFloors;}
	const std::vector<state::window*> getWindows() const {return mWindows;}
	const std::map<boost::posix_time::time_period,std::map<state::space*,double>>& getHeatingEnergies() const {return mHeatingEnergies;}
	const std::map<boost::posix_time::time_period,std::map<state::space*,double>>& getCoolingEnergies() const {return mCoolingEnergies;}
};

struct bp_results
{
	double mTotalHeatingEnergy = 0.0;
	double mTotalCoolingEnergy = 0.0;
	double mTotalEnergy = 0.0;
	// heating/cooling per period?
};

} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/bp_model.cpp>

#endif // BSO_BP_MODEL_HPP