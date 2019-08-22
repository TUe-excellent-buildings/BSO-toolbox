#ifndef BSO_BP_WEATHER_PROFILE_CPP
#define BSO_BP_WEATHER_PROFILE_CPP

#include <sstream>
#include <fstream>
#include <stdexcept>

#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string/trim.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>

#include <bso/utilities/trim_and_cast.hpp>

namespace bso { namespace building_physics { namespace state { namespace independent {

weather_profile::weather_profile(const unsigned int& index)
: independent_state(index)
{
	mIsWeatherProfile = true;
} //ctor()

weather_profile::~weather_profile()
{
	
} // dtor()

void weather_profile::loadNewPeriod(const boost::posix_time::ptime& start, 
	const boost::posix_time::ptime& end, const std::string& fileName)
{
	if (start >= end)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, when loading weather data, received starting time\n"
								 << "that is at or after the ending time:\n"
								 << "start: " << start << "\nend: " << end << "\n"
								 << "(bso/building_physics/independent/weather_profile.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	
	std::ifstream input(fileName.c_str());
	if (!input.is_open())
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, when loading weather data, could not open file:\n"
								 << fileName 
								 << "(bso/building_physics/independent/weather_profile.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	
	mWeatherData.clear();
	std::string inputLine;
	boost::char_separator<char> sep(",");
	typedef boost::tokenizer< boost::char_separator<char> > tTokenizer;
	
	boost::posix_time::ptime currentTime = start;
	tTokenizer tokens(inputLine,sep);
	while (end > currentTime)
	{
		try 
		{
			if (input.eof())
			{
				throw std::invalid_argument("Reached end of file");
			}
			getline(input,inputLine);
			tokens = tTokenizer(inputLine, sep);
		}
		catch(std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, trying to read weather data from file:" << fileName << "\n"
									 << "for period: " << start << " until " << end << "\n"
									 << "received the following error:\n" << e.what()
									 << "(bso/building_physics/state/independent/weather_profile.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		auto token = tokens.begin();
		
		if (*token != "260") continue; // station identification code
		
		++token; // skip STN = station identification
		boost::gregorian::date currentDate = boost::gregorian::from_undelimited_string(*token);
		++token; // skip date
		currentTime = boost::posix_time::ptime(currentDate, boost::posix_time::hours(bso::utilities::trim_and_cast_int(*token)));
		if (currentTime < start) continue;
		++token; // skip hour
		++token; // skip DD = Mean wind direction (in degrees) during the 10-minute period preceding the time of observation (360=north, 90=east, 180=south, 270=west, 0=calm 990=variable)
		++token; // skip FH = Hourly mean wind speed (in 0.1 m/s)
		++token; // skip FF = Mean wind speed (in 0.1 m/s) during the 10-minute period preceding the time of observation
		++token; // skip FX = Maximum wind gust (in 0.1 m/s) during the hourly division
		mWeatherData[currentTime] = bso::utilities::trim_and_cast_double(*token)/10.0;
	/*++token; // skip temperature (in tenths of degrees Celsius)
		++token; // skip T10N = Minimum temperature (in 0.1 degrees Celsius) at 0.1 m in the preceding 6-hour period
		++token; // skip TD = Dew point temperature (in 0.1 degrees Celsius) at 1.50 m at the time of observation
		++token; // skip SQ = Sunshine duration (in 0.1 hour) during the hourly division, calculated from global radiation (-1 for <0.05 hour)
		++token; // skip Q = Global radiation (in J/cm2) during the hourly division
		++token; // skip DR = Precipitation duration (in 0.1 hour) during the hourly division
		++token; // skip RH = Hourly precipitation amount (in 0.1 mm) (-1 for <0.05 mm)
		++token; // skip P = Air pressure (in 0.1 hPa) reduced to mean sea level, at the time of observation
		++token; // skip VV = Horizontal visibility at the time of observation (0=less than 100m, 1=100-200m, 2=200-300m,..., 49=4900-5000m, 50=5-6km, 56=6-7km, 57=7-8km, ..., 79=29-30km, 80=30-35km, 81=35-40km,..., 89=more than 70km)
		++token; // skip N = Cloud cover (in octants), at the time of observation (9=sky invisible)
		++token; // skip U = Relative atmospheric humidity (in percents) at 1.50 m at the time of observation
		++token; // skip WW = Present weather code (00-99), description for the hourly division. See http://www.knmi.nl/klimatologie/achtergrondinformatie/ww_lijst_engels.pdf
		++token; // skip IX = Indicator present weather code (1=manned and recorded (using code from visual observations), 2,3=manned and omitted (no significant weather phenomenon to report, not available), 4=automatically recorded (using code from visual observations), 5,6=automatically omitted (no significant weather phenomenon to report, not available), 7=automatically set (using code from automated observations)
		++token; // skip M = Fog 0=no occurrence, 1=occurred during the preceding hour and/or at the time of observation
		++token; // skip R = Rainfall 0=no occurrence, 1=occurred during the preceding hour and/or at the time of observation
		++token; // skip S = Snow 0=no occurrence, 1=occurred during the preceding hour and/or at the time of observation
		++token; // skip O = Thunder  0=no occurrence, 1=occurred during the preceding hour and/or at the time of observation
		++token; // skip final token: Y = Ice formation 0=no occurrence, 1=occurred during the preceding hour and/or at the time of observation
	*/
	}
} // loadNewPeriod()

void weather_profile::initSystem(bso::building_physics::state_space_system& system)
{

} // initSystem()

void weather_profile::updateSystem(bso::building_physics::state_space_system& system)
{
	boost::posix_time::ptime currentTime = system.getCurrentTime();

	if (mWeatherData.empty())
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to update state space system, but weather\n"
								 << "data has not been loaded\n"
								 << "(bso/building_physics/state/independent/weather_profile.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	
	if ((mWeatherData.begin()->first  > currentTime) ||
			(mWeatherData.rbegin()->first < currentTime))
	{
		std::stringstream errorMessage;
		errorMessage << "\nError trying to update weather but missing data\n"
								 << "to interpolate. Loaded period between\n"
								 << mWeatherData.begin()->first << " untill " << mWeatherData.rbegin()->first
								 << "\n requested data for: " << currentTime << "\n"
								 << "(bso/building_physics/state/independent/weather_profile.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	
	auto dataSearch = mWeatherData.lower_bound(currentTime);
	boost::posix_time::ptime prevTimeStamp, nextTimeStamp;
	double prevT, nextT;
	
	nextTimeStamp = dataSearch->first;
	nextT	= dataSearch->second;
	
	if (nextTimeStamp == currentTime)
	{
		system.getu()(mIndex) = nextT;
		return;
	}
	--dataSearch;
	
	prevTimeStamp = dataSearch->first;
	prevT	= dataSearch->second;
	
	system.getu()(mIndex) = prevT + (nextT-prevT) *
													((double)(currentTime-prevTimeStamp).total_seconds() /
													 (double)(nextTimeStamp-prevTimeStamp).total_seconds());
} // updateSystem()


} // namespace independent
} // namespace state 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_WEATHER_PROFILE_CPP