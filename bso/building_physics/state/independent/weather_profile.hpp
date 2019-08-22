#ifndef BSO_BP_WEATHER_PROFILE_HPP
#define BSO_BP_WEATHER_PROFILE_HPP

#include <bso/building_physics/state/independent/independent_state.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace bso { namespace building_physics { namespace state { namespace independent {

class weather_profile : public independent_state
{
private:
	std::map<boost::posix_time::ptime,double> mWeatherData; // contains only one double now, but can in future be container
public:
	weather_profile(const unsigned int& index);
	~weather_profile();
	
	void loadNewPeriod(const boost::posix_time::ptime& start, 
										 const boost::posix_time::ptime& end, const std::string& inputFile);
	void initSystem(bso::building_physics::state_space_system& system);
	void updateSystem(bso::building_physics::state_space_system& system);
	
	const std::map<boost::posix_time::ptime,double>& getWeatherData() const {return mWeatherData;}
};

} // namespace independent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/state/independent/weather_profile.cpp>

#endif // BSO_BP_WEATHER_PROFILE_HPP