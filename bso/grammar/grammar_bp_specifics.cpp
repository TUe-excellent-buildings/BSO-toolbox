#ifndef BSO_GRAMMAR_BP_SPECIFICS_CPP
#define BSO_GRAMMAR_BP_SPECIFICS_CPP

#include <bso/utilities/trim_and_cast.hpp>

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <stdexcept>
#include <sstream>

namespace bso { namespace grammar {

void grammar::mReadBPSettings(const std::string& fileName)
{
	std::fstream input(fileName.c_str());
	if (!input.is_open())
	{
		std::stringstream errorMessage;
		errorMessage << "\nCould not read SD grammar settings, failed to open file:"
								 << fileName << "\n(bso/grammar/grammar_sd_specifics.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}

	mBPSimulationPeriods.clear();
	mBPSpaceSettings.clear();
	mBPWallConstructions.clear();
	mBPFloorConstructions.clear();
	mBPWallGlazings.clear();
	mBPFloorGlazings.clear();
	mBPWarmUpDuration = boost::posix_time::time_duration(0,0,0,0);
	mBPTimeStepSize = boost::posix_time::time_duration(0,0,0,0);
	
	std::map<std::string,std::string> spaceTypeSettingIDs;
	std::map<std::string,bso::building_physics::properties::space_settings> spaceSettings;
	std::map<std::pair<std::string,std::string>, std::pair<std::string,std::string> > wallTypeIDs;
	std::map<std::pair<std::string,std::string>, std::pair<std::string,std::string> > floorTypeIDs;
	std::map<std::string,bso::building_physics::properties::material> materials;
	std::map<std::pair<std::string,std::string>,std::vector<std::pair<std::string, double> > > constructionLayers;
	std::map<std::pair<std::string,std::string>,bso::building_physics::properties::glazing> glazings;
	std::map<std::string,Eigen::Vector4d> visualizationProps;

	std::string line;
	boost::char_separator<char> sep(","); // defines what separates tokens in a string
	typedef boost::tokenizer< boost::char_separator<char> > t_tokenizer; // settings for the boost::tokenizer
	char type_ID; // holds information about what type of information is described by the line currently read

	while(!input.eof())
	{
		getline(input, line); // get the next line from the file
		boost::algorithm::trim(line); // remove white space from start and end of line (to see if it is an empty line, remove any incidental white space)
		if (line == "") //skip empty lines (tokenizer does not like it)
		{
			continue; // continue to next line
		}
		t_tokenizer tok(line, sep); // tokenize the line
		t_tokenizer::iterator token = tok.begin(); // set iterator to first token

		try
		{
			type_ID = bso::utilities::trim_and_cast_char(*token);	++token; // interpret first token as type ID
		}
		catch (std::exception& e)
		{
			continue;
		}

		try
		{
			switch (type_ID)
			{
			case 'A':
			{ // space settings
				std::string spaceType = *token; ++token;
				boost::algorithm::trim(spaceType);
				std::string spaceSettingID = *token; ++token;
				boost::algorithm::trim(spaceSettingID);
				spaceTypeSettingIDs[spaceType] = spaceSettingID;
				break;
			}
			case 'B':
			{ // rectangle settings
				std::string type1 = *token; ++token;
				boost::algorithm::trim(type1);
				std::string type2 = *token; ++token;
				boost::algorithm::trim(type2);
				auto typePair = std::minmax(type1,type2);
				
				std::string constructionType = *token; ++token;
				boost::algorithm::trim(constructionType);
				std::string constructionID = *token; ++token;
				boost::algorithm::trim(constructionID);
				wallTypeIDs[typePair] = {constructionType,constructionID};
				
				constructionType = *token; ++token;
				boost::algorithm::trim(constructionType);
				constructionID = *token; ++token;
				boost::algorithm::trim(constructionID);
				floorTypeIDs[typePair] = {constructionType,constructionID};
				break;
			}
			case 'C':
			{ // warm up time
				unsigned int hours = bso::utilities::trim_and_cast_uint(*token); ++token;
				unsigned int minutes = bso::utilities::trim_and_cast_uint(*token); ++token;
				unsigned int seconds = bso::utilities::trim_and_cast_uint(*token); ++token;
				
				mBPWarmUpDuration = boost::posix_time::time_duration(hours,minutes,seconds,0);
				break;
			}
			case 'D':
			{ // time step size
				unsigned int hours = bso::utilities::trim_and_cast_uint(*token); ++token;
				unsigned int minutes = bso::utilities::trim_and_cast_uint(*token); ++token;
				unsigned int seconds = bso::utilities::trim_and_cast_uint(*token); ++token;
				
				mBPTimeStepSize = boost::posix_time::time_duration(hours,minutes,seconds,0);
				break;
			}
			case 'E':
			{ // space settings
				std::string ID = *token; ++token;
				boost::algorithm::trim(ID);
				double heatCap = bso::utilities::trim_and_cast_double(*token); ++token;
				double coolCap = bso::utilities::trim_and_cast_double(*token); ++token;
				double heatSetPoint = bso::utilities::trim_and_cast_double(*token); ++token;
				double coolSetPoint = bso::utilities::trim_and_cast_double(*token); ++token;
				double ACH = bso::utilities::trim_and_cast_double(*token); ++token;
				
				spaceSettings.emplace(ID,bso::building_physics::properties::space_settings(
					ID, heatCap, coolCap, heatSetPoint, coolSetPoint, ACH));
				break;
			}
			case 'F':
			{ // simulation time
				std::string fileName = *token; ++token;
				boost::algorithm::trim(fileName);
				
				unsigned int year = bso::utilities::trim_and_cast_uint(*token); ++token;
				unsigned int month = bso::utilities::trim_and_cast_uint(*token); ++token;
				unsigned int day = bso::utilities::trim_and_cast_uint(*token); ++token;
				unsigned int hour = bso::utilities::trim_and_cast_uint(*token); ++token;
				boost::posix_time::ptime start(boost::gregorian::date(year,month,day),boost::posix_time::hours(hour));
				year = bso::utilities::trim_and_cast_uint(*token); ++token;
				month = bso::utilities::trim_and_cast_uint(*token); ++token;
				day = bso::utilities::trim_and_cast_uint(*token); ++token;
				hour = bso::utilities::trim_and_cast_uint(*token); ++token;
				boost::posix_time::ptime end(boost::gregorian::date(year,month,day),boost::posix_time::hours(hour));
				
				mBPSimulationPeriods.emplace(fileName,boost::posix_time::time_period(start, end));
				break;
			}
			case 'G':
			{ // ground profile
				mBPGroundTemperature = bso::utilities::trim_and_cast_double(*token); ++token;
				break;
			}
			case 'H':
			{ // material
				std::string ID = *token; ++token;
				boost::algorithm::trim(ID);
				std::string description = *token; ++token;
				boost::algorithm::trim(description);
				double specWeight = bso::utilities::trim_and_cast_double(*token); ++token;
				double specHeat = bso::utilities::trim_and_cast_double(*token); ++token;
				double thermConduct = bso::utilities::trim_and_cast_double(*token); ++token;
				
				materials.emplace(ID,bso::building_physics::properties::material(
					ID, description, specWeight, specHeat, thermConduct));
				break;
			}
			case 'I':
			{ // construction settings
				std::string ID = *token; ++token;
				boost::algorithm::trim(ID);
				std::string visID = *token; ++token;
				boost::algorithm::trim(visID);
				std::pair<std::string, std::string> constructionPropIDs = {ID,visID};
				
				std::vector<std::pair<std::string,double> > layers;
				try {
					while (token != tok.end())
					{
						std::string matID = *token; ++token;
						boost::algorithm::trim(matID);
						double matThickness = bso::utilities::trim_and_cast_double(*token); ++token;
						layers.push_back({matID,matThickness});
					}
				}
				catch(std::exception& e)
				{
					std::stringstream errorMessage;
					errorMessage << "\nError, while reading layers of a construction.\n"
											 << "received the following exception:\n" << e.what()
											 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
					throw std::invalid_argument(errorMessage.str());
				}
				constructionLayers.emplace(constructionPropIDs,layers);
				break;
			}
			case 'J':
			{ // glazing settings
				std::string ID = *token; ++token;
				boost::algorithm::trim(ID);
				std::string visID = *token; ++token;
				boost::algorithm::trim(visID);
				std::pair<std::string, std::string> glazingPropIDs = {ID,visID};
				
				double U = bso::utilities::trim_and_cast_double(*token); ++token;
				double capPerArea = bso::utilities::trim_and_cast_double(*token); ++token;
				
				glazings.emplace(glazingPropIDs,bso::building_physics::properties::glazing(
					ID, U, capPerArea));
				break;
			}
			case 'K':
			{ // visualization settings
				std::string ID = *token; ++token;
				boost::algorithm::trim(ID);
				Eigen::Vector4d visSettings;
				visSettings << bso::utilities::trim_and_cast_double(*(token++)),
											 bso::utilities::trim_and_cast_double(*(token++)),
											 bso::utilities::trim_and_cast_double(*(token++)),
											 bso::utilities::trim_and_cast_double(*(token++));
				visualizationProps.emplace(ID,visSettings);
				break;
			}
			default:
				break;
			}
		}
		catch(std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, while reading BP grammar settings.\n"
									 << "could not parse the following line: " << line
									 << "\nreceived the following exception:\n" << e.what()
									 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // end of file
	input.close();

	for (const auto& i : spaceTypeSettingIDs)
	{
		auto settingSearch = spaceSettings.find(i.second);
		if (settingSearch == spaceSettings.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, after reading BP grammar settings.\n"
									 << "could not find the space settings for the following space type: \n"
									 << i.first
									 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		else
		{
			mBPSpaceSettings.emplace(i.first, settingSearch->second);
		}
	}

	std::map<std::string,bso::building_physics::properties::construction> assembledConstructions;
	std::map<std::string,bso::building_physics::properties::glazing> assembledGlazings;

	// assemble constructions
	for (const auto& i : constructionLayers)
	{
		std::vector<bso::building_physics::properties::layer> tempLayers;
		std::pair<std::string, Eigen::Vector4d> tempVisProps;
		for (const auto& j : i.second)
		{
			auto matSearch = materials.find(j.first);
			if (matSearch == materials.end())
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, after reading BP grammar settings.\n"
										 << "could not find the material with the following ID:\n"
										 << j.first
										 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			else
			{
				tempLayers.push_back(bso::building_physics::properties::layer(
					matSearch->second,j.second));
			}
		}
		auto visSearch = visualizationProps.find(i.first.second);
		if (visSearch == visualizationProps.end())
		{ 
			std::stringstream errorMessage;
			errorMessage << "\nError, after reading BP grammar settings.\n"
									 << "could not find the visualization settings for\n"
									 << "the following construction ID: \n"
									 << i.first.first
									 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		else 
		{
			tempVisProps = std::pair<std::string, Eigen::Vector4d>(visSearch->first, visSearch->second);
		}

		assembledConstructions.emplace(i.first.first,bso::building_physics::properties::construction(
			i.first.first, tempLayers, 0.5, tempVisProps));
	}

	// assemble glazings
	for (const auto& i : glazings)
	{
		std::pair<std::string, Eigen::Vector4d> tempVisProps;
		auto visSearch = visualizationProps.find(i.first.second);
		if (visSearch == visualizationProps.end())
		{ 
			std::stringstream errorMessage;
			errorMessage << "\nError, after reading BP grammar settings.\n"
									 << "could not find the visualization settings for\n"
									 << "the following glazing ID: \n"
									 << i.first.first
									 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		else 
		{
			tempVisProps = std::pair<std::string, Eigen::Vector4d>(visSearch->first, visSearch->second);
		}
		assembledGlazings.emplace(i.first.first,bso::building_physics::properties::glazing(
			i.first.first,i.second.getU(), i.second.getCapacitancePerArea(), tempVisProps));
	}

	for (const auto& i : wallTypeIDs)
	{
		if (i.second.first == "construction")
		{
			auto constructSearch = assembledConstructions.find(i.second.second);
			if (constructSearch == assembledConstructions.end())
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, after reading BP grammar settings.\n"
										 << "could not find the construction for a wall with ID:\n"
										 << i.second.second
										 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			else
			{
				mBPWallConstructions.emplace(i.first,constructSearch->second);
			}
		}
		else if (i.second.first == "glazing")
		{
			auto glazingSearch = assembledGlazings.find(i.second.second);
			if (glazingSearch == assembledGlazings.end())
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, after reading BP grammar settings.\n"
										 << "could not find the glazing for a wall with ID:\n"
										 << i.second.second
										 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			else
			{
				mBPWallGlazings.emplace(i.first,glazingSearch->second);
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, after reading BP grammar settings.\n"
									 << "did not recognize the type assigned to a wall:\n"
									 << i.second.first
									 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	}

	for (const auto& i : floorTypeIDs)
	{
		if (i.second.first == "construction")
		{
			auto constructSearch = assembledConstructions.find(i.second.second);
			if (constructSearch == assembledConstructions.end())
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, after reading BP grammar settings.\n"
										 << "could not find the construction for a floor with ID:\n"
										 << i.second.second
										 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			else
			{
				mBPFloorConstructions.emplace(i.first,constructSearch->second);
			}
		}
		else if (i.second.first == "glazing")
		{
			auto glazingSearch = assembledGlazings.find(i.second.second);
			if (glazingSearch == assembledGlazings.end())
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, after reading BP grammar settings.\n"
										 << "could not find the glazing for a floor with ID:\n"
										 << i.second.second
										 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			else
			{
				mBPFloorGlazings.emplace(i.first,glazingSearch->second);
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, after reading BP grammar settings.\n"
									 << "did not recognize the type assigned to a wall:\n"
									 << i.second.first
									 << "\n(bso/grammar/grammar_bp_specifics.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	}

} // mReadBPSettings()
	
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_BP_SPECIFICS_CPP