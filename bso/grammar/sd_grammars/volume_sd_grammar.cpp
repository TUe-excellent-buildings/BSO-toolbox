#ifndef BSO_GRAMMAR_VOLUME_SD_GRAMMAR_CPP
#define BSO_GRAMMAR_VOLUME_SD_GRAMMAR_CPP

#include <stdexcept>
#include <sstream>

namespace bso { namespace grammar {
class VOLUME_SD_GRAMMAR;

template <>
bso::structural_design::sd_model grammar::sd_grammar<VOLUME_SD_GRAMMAR>(const std::string& fileName)
{
	// initialize a new SD model
	mSDModel = bso::structural_design::sd_model();
	// read the structural design settings
	this->mReadSDSettings(fileName);

	std::map<spatial_design::conformal::cf_cuboid*, 
					 structural_design::component::structure> cuboidStructures;
	for (const auto& i : mCuboidProperties)
	{
		// get the space type depending on the settings
		std::string usedType = i.second->getSpaceType();

		// determine the structural type and properties of the cuboid
		bool structureFound = false;
		auto structureSearch = mSDSpaceProperties.find(usedType);
		if (i.second->isInSpace())
		{
			if (structureSearch != mSDSpaceProperties.end())
			{
				structureFound = true;
			}
		}

		if (!structureFound)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, in volume grammar could not find the specified\n"
									 << "type settings: \"" << usedType << "\""
									 << "Has this type set been specified in the inputfile?: \""
									 << fileName << "\"\n(bso/grammar/sd_grammars/volume_sd_grammar.cpp)"
									 << std::endl;
			throw std::runtime_error(errorMessage.str());
		}

		// create a cuboid rule set
		auto sdCuboidRuleSet = new rule_set::sd_cuboid_rule(i.second);
		mSDCuboidRules[i.first] = sdCuboidRuleSet;
		if (structureSearch->second.type() != "none")
		{
			sdCuboidRuleSet->assignStructure(structureSearch->second);
			cuboidStructures.emplace(i.first,structureSearch->second);
		}
	}
	
	for (const auto& i : mRectangleProperties)
	{
		if (i.second->isSpaceSeparating())
		{
			// create a rectangle rule set
			auto sdRectangleRuleSet = new rule_set::sd_rectangle_rule(i.second);
			mSDRectangleRules[i.first] = sdRectangleRuleSet;
			sdRectangleRuleSet->assignLoadPanel(mLoadPanel); // add a load panel, in case a load is assigned but no structure can carry it
			sdRectangleRuleSet->addLoads(&mLoads); // add the loads
			
			std::vector<structural_design::component::structure> potentialStructures;
			for (const auto& j : i.second->getRectanglePtr()->cfCuboids())
			{
				auto structureSearch = cuboidStructures.find(j);
				if (structureSearch != cuboidStructures.end())
				{
					potentialStructures.push_back(structureSearch->second);
				}
			}
			sdRectangleRuleSet->assignStructure(potentialStructures);
		}
	}
	
	// find each line that is associated to a rectangle that has a rectangle rule set
	for (const auto& i : mSDRectangleRules)
	{ // for each sd rectangle rule
		for (const auto& j : i.first->cfLines())
		{ // for each line associated to the rectangle associated to the rectangle rule
			if (mSDLineRules.find(j) == mSDLineRules.end());
			{ // if it has not yet been added to the vector, add it
				mSDLineRules[j] = new rule_set::sd_line_rule(mLineProperties[j]); // create line rules, in this case only to generate boundary conditions
			}
		}
	}

	for (const auto& i : mVertexProperties)
	{
		if (i.second->isSpaceCorner())
		{
			mSDVertexRules[i.first] = new rule_set::sd_vertex_rule(i.second); // create nodes at space corners
		}
	}

	// apply the rules
	for (const auto& i : mSDVertexRules) i.second->apply(mSDModel);
	for (const auto& i : mSDLineRules) i.second->apply(mSDModel);
	for (const auto& i : mSDRectangleRules) i.second->apply(mSDModel);
	for (const auto& i : mSDCuboidRules) i.second->apply(mSDModel);
	mSDModel.setMeshSize(mMeshSize);

	// delete the rules
	for (auto& i : mSDVertexRules) delete i.second;
	for (auto& i : mSDLineRules) delete i.second;
	for (auto& i : mSDRectangleRules) delete i.second;
	for (auto& i : mSDCuboidRules) delete i.second;

	return mSDModel;
} // volume_sd_grammar

} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_VOLUME_SD_GRAMMAR_CPP
