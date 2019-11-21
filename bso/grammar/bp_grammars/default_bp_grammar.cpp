#ifndef BSO_GRAMMAR_DEFAULT_BP_GRAMMAR_CPP
#define BSO_GRAMMAR_DEFAULT_BP_GRAMMAR_CPP

#include <stdexcept>
#include <sstream>

namespace bso { namespace grammar {


	
template <>
bso::building_physics::bp_model grammar::bp_grammar<DEFAULT_BP_GRAMMAR>(const std::string& fileName,
	const bool& useSurfaceTypeInsteadOfSpaceType)
{
	// initialize a new BP model
	bso::building_physics::bp_model mBPModel;
	// read the building physics settings
	this->mReadBPSettings(fileName);

	// create weather profile state
	auto weatherProfile = new bso::building_physics::state::weather_profile(
		mBPModel.getNextIndependentIndex());
	mBPModel.addState(weatherProfile);

	// create ground profile state
	auto groundProfile = new bso::building_physics::state::ground_profile(
		mBPModel.getNextIndependentIndex(), mBPGroundTemperature);
	mBPModel.addState(groundProfile);

	// add simulation periods
	for (const auto& i : mBPSimulationPeriods)
	{
		mBPModel.addSimulationPeriod(i.first,i.second);
	}

	// set time step size
	mBPModel.setTimeStepSize(mBPTimeStepSize);

	// set warm up duration
	mBPModel.setWarmUpDuration(mBPWarmUpDuration);

	// set initial state temperature
	mBPModel.setInitialStateTemperatures(20);

	std::map<bso::spatial_design::conformal::cf_space*, bso::building_physics::state::space*> spaceStates;

	// create space rules
	for (const auto& i : mSpaceProperties)
	{
		// get space type
		std::string usedType = i.second->getSpaceType();
		
		// find the space settings
		auto spaceSettingSearch = mBPSpaceSettings.find(usedType);
		if (spaceSettingSearch == mBPSpaceSettings.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, in default bp grammar could not find the specified\n"
									 << "space setting for type: \"" << usedType << "\"\n"
									 << "Has this type set been specified in the inputfile?: \""
									 << fileName << "\"\n(bso/grammar/bp_grammars/default_bp_grammar.cpp)" 
									 << std::endl;
			throw std::runtime_error(errorMessage.str());
		}

		auto spaceRuleSet = new rule_set::bp_space_rule(i.second);
		// assign space settings
		spaceRuleSet->assignSettings(spaceSettingSearch->second);
		
		// assign weather profile
		spaceRuleSet->assignWeatherProfile(weatherProfile);
		
		// assign space ptr map
		spaceRuleSet->assignSpaceStatesMap(&spaceStates);
		
		mBPSpaceRules[i.first] = spaceRuleSet;
	}

	// create rectangle rules
	for (const auto& i : mRectangleProperties)
	{
		if (i.second->isSpaceSeparating())
		{
			// get the space or surface types, depending on the settings
			std::pair<std::string, std::string> usedTypes = i.second->getSpaceTypes();
			if (useSurfaceTypeInsteadOfSpaceType) usedTypes = i.second->getSurfaceTypes();

			// determine the structural type and properties of the rectangle
			bool constructionFound = false;
			auto constuctionSearch = mBPFloorConstructions.find(usedTypes);
			bool glazingFound = false;
			auto glazingSearch = mBPFloorGlazings.find(usedTypes);
			if (i.second->isFloor())
			{
				if (constuctionSearch != mBPFloorConstructions.end())
				{
					constructionFound = true;
				}
				if (glazingSearch != mBPFloorGlazings.end())
				{
					glazingFound = true;
				}
			}
			else if (i.second->isWall())
			{
				constuctionSearch = mBPWallConstructions.find(usedTypes);
				glazingSearch = mBPWallGlazings.find(usedTypes);
				if (constuctionSearch != mBPWallConstructions.end())
				{
					constructionFound = true;
				}
				if (glazingSearch != mBPWallGlazings.end())
				{
					glazingFound = true;
				}
			}
			if (!constructionFound && ! glazingFound)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, in default bp grammar could not find the specified\n"
										 << "type settings: \"" << usedTypes.first << "\" - \"" 
										 << usedTypes.second << "\" ("
										 << ((i.second->isFloor())? "floor" : "wall") << ")\n"
										 << "Has this type set been specified in the inputfile?: \""
										 << fileName << "\"\n(bso/grammar/bp_grammars/default_bp_grammar.cpp)" 
										 << std::endl;
				throw std::runtime_error(errorMessage.str());
			}

			// create a rectangle rule set
			auto rectangleRuleSet = new rule_set::bp_rectangle_rule(i.second);
			if (constructionFound)
			{
				rectangleRuleSet->assignConstruction(constuctionSearch->second);
			}
			if (glazingFound)
			{
				rectangleRuleSet->assignGlazing(glazingSearch->second);
			}
			// assign space ptr map
			rectangleRuleSet->assignSpaceStatesMap(&spaceStates);

			// assign ground profile
			rectangleRuleSet->assignGroundProfile(groundProfile);

			// assign weather profile
			rectangleRuleSet->assignWeatherProfile(weatherProfile);
			mBPRectangleRules[i.first] = rectangleRuleSet;
		}
	}

	// apply rules
	for (const auto& i : mBPSpaceRules) i.second->apply(mBPModel);
	for (const auto& i : mBPRectangleRules) i.second->apply(mBPModel);

	// delete the rules
	for (auto& i : mBPSpaceRules) delete i.second;
	for (auto& i : mBPRectangleRules) delete i.second;

	return mBPModel;
} // default_bp_grammar

template <>
building_physics::bp_model grammar::bp_grammar<DEFAULT_BP_GRAMMAR>(const std::string& fileName)
{
	return this->bp_grammar<DEFAULT_BP_GRAMMAR>(fileName,false);
}

} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_DEFAULT_BP_GRAMMAR_CPP