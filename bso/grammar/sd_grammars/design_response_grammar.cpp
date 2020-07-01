#ifndef BSO_GRAMMAR_DESIGN_RESPONSE_GRAMMAR_CPP
#define BSO_GRAMMAR_DESIGN_RESPONSE_GRAMMAR_CPP

#include <bso/utilities/clustering.hpp>
#include <stdexcept>
#include <sstream>

namespace bso { namespace grammar {
class DESIGN_RESPONSE;

template <>
bso::structural_design::sd_model grammar::sd_grammar<DESIGN_RESPONSE>(
	const std::string& fileName,
	const double& etaBend,
	const double& etaAx,
	const double& etaShear,
	const double& etaNoise,
	const int& etaConverge,
	const std::string& checkingOrder,
	const bso::structural_design::component::structure& trussStructure,
	const bso::structural_design::component::structure& beamStructure,
	const bso::structural_design::component::structure& flatShellStructure,
	const bso::structural_design::component::structure& substituteStructure)
{
	// initialize a new SD model
	mSDModel = bso::structural_design::sd_model();
	
	//variable to store initial mean of the total design response
	double initMeanTot;

	// read the structural design settings
	this->mReadSDSettings(fileName);

	// assign substitute type to rectangles
	for (const auto& i : mRectangleProperties)
	{
		if (i.second->isSpaceSeparating())
		{
			// create a rectangle rule set
			auto sdRectangleRuleSet = new rule_set::sd_rectangle_rule(i.second);
			mSDRectangleRules[i.first] = sdRectangleRuleSet;
			sdRectangleRuleSet->assignStructure(substituteStructure);
			sdRectangleRuleSet->assignLoadPanel(mLoadPanel);
			sdRectangleRuleSet->addLoads(&mLoads);
		}
	}
	auto subRectangles = mSDRectangleRules;
	int initSubNumber = subRectangles.size();
	
	// find each line that is associated to a rectangle that has a rectangle rule set
	for (const auto& i : mSDRectangleRules)
	{ // for each sd rectangle rule
		for (const auto& j : i.first->cfLines())
		{ // for each line associated to the rectangle associated to the rectangle rule
			if (mSDLineRules.find(j) == mSDLineRules.end());
			{ // if it has not yet been added to the vector, add it
				mSDLineRules[j] = new rule_set::sd_line_rule(mLineProperties[j]);
			}
		}
	}
	
	// initialize vertex rules
	for (const auto& i : mVertexProperties)
	{
		if (i.second->isSpaceCorner())
		{
			mSDVertexRules[i.first] = new rule_set::sd_vertex_rule(i.second);
		}
	}

	for (int i = 0; i < etaConverge + 1; ++i)
	{
		// execute rectangle and line rules
		for (const auto& j : mSDVertexRules) j.second->apply(mSDModel);
		for (const auto& j : mSDLineRules) j.second->apply(mSDModel);
		for (const auto& j : mSDRectangleRules) j.second->apply(mSDModel);
		mSDModel.setMeshSize(mMeshSize);
		
		// check if the grammar has finished, if so, further evaluation is not needed
		if (i == etaConverge) break;
		
		// evaluate SD model
		mSDModel.analyze();
		
		std::map<bso::spatial_design::conformal::cf_rectangle*, bso::structural_design::sd_results> subResults;
		std::vector<bso::utilities::data_point> resultData;
		// obtain design responses of substitute rectangles
		int count = 0;
		for (auto j : subRectangles)
		{
			auto sdResult = mSDModel.getPartialResults(j.first);
			subResults[j.first] = sdResult;
			resultData.push_back(bso::utilities::data_point({sdResult.mTotalStrainEnergy}));
		}
		
		if (i == 0)
		{
			for (auto j : subResults)
			{
				initMeanTot += (j.second.mTotalStrainEnergy / j.second.mTotalStructuralVolume);
			}
			initMeanTot /= subResults.size();
		}
		
		// check substitution convergence
		int targetSubNumber;
		bool initCheck = true;
		do 
		{ // compute the target number of substitute rectangles, and if already reached, skip to next iteration
			if (initCheck) initCheck = false;
			else ++i;
			targetSubNumber = std::max(0,initSubNumber - (int)std::ceil((i+1)*initSubNumber/(double)etaConverge));

		} while (targetSubNumber > subRectangles.size());
		
		// cluster substitute rectangles
		bool doClustering = false;
		unsigned int kMin = 4; // min number of clusters
		unsigned int kMax = subRectangles.size(); // max number of clusters
		kMax = (kMax < 10) ? kMax : 10; // set upper limit to max number of clusters
		std::vector<bso::utilities::cluster> clusteredData;
		if (i < etaConverge && kMax >= kMin)
		{
			doClustering = true;
			bso::utilities::clustering subClustering(resultData,kMin,kMax,50);
			clusteredData = subClustering.getClusters();
		}
		
		// substitute the substitute rectangles
		while (targetSubNumber < subRectangles.size())
		{
			// inventory the rectangles that must be substituted
			std::vector<bso::spatial_design::conformal::cf_rectangle*> mustSubstitute;
			if (doClustering)
			{ // if clustering has been carried out
		
				// inventory centroid of each cluster
				std::vector<bso::utilities::data_point> centroids;
				for (const auto& j : clusteredData)
				{
					centroids.push_back(j.getCentroid());
				}
				
				// find the cluster with a centroid closest to the dystopian centroid
				bso::utilities::data_point distopianCentroid;
				distopianCentroid.setToDistopia(centroids);
				auto worstCentroid = distopianCentroid.findClosestIn(centroids).back();
				unsigned int worstCluster;
				for (unsigned int j = 0; j < clusteredData.size(); ++j)
				{
					if (worstCentroid == clusteredData[j].getCentroid())
					{
						worstCluster = j;
					}
				}
				
				// find the rectangles with a response in the worst cluster, and add it to the list of rectangles that must be substituted
				for (const auto& j : subResults)
				{
					for (const auto& k : clusteredData[worstCluster].getData())
					{
						if (j.second.mTotalStrainEnergy == (*k)(0))
						{
							mustSubstitute.push_back(j.first);
							break;
						}
					}
				}
				
				// remove the worst cluster from the clustering
				clusteredData.erase(clusteredData.begin() + worstCluster);
			}
			else 
			{ // if no clustering has been carried out
		
				// add all rectangles to the list of rectangles that must be substituted.
				for (const auto& j : subRectangles)
				{
					mustSubstitute.push_back(j.first);
				}
			}
			
			// substitute the rectangles in the list
			for (const auto& j : mustSubstitute)
			{
				bool substituted = false;
				double volume = subResults[j].mTotalStructuralVolume;
				double totalStrain = subResults[j].mTotalStrainEnergy/volume;
				if (totalStrain >= etaNoise * initMeanTot)
				{
					for (const auto& k : checkingOrder)
					{
						switch(k)
						{
							case '1':
							{ // shear
								double shearStrain = subResults[j].mShearStrainEnergy/volume;
								if (shearStrain/totalStrain >= etaShear && shearStrain != 0)
								{
									subRectangles[j]->assignStructure(trussStructure);
									substituted = true;
								}
								break;
							}
							case '2':
							{ // axial
								double axialStrain = subResults[j].mAxialStrainEnergy/volume;
								if (axialStrain/totalStrain >= etaAx && axialStrain != 0)
								{
									subRectangles[j]->assignStructure(beamStructure);
									substituted = true;
								}
								break;
							}
							case '3':
							{ // bending
								double bendStrain = subResults[j].mBendStrainEnergy/volume;
								if (bendStrain/totalStrain >= etaBend && bendStrain != 0)
								{
									subRectangles[j]->assignStructure(flatShellStructure);
									substituted = true;
								}
								break;
							}
							default:
							{ // error
								std::stringstream errorMessage;
								errorMessage << "\nError, when running the design response grammar.\n"
														 << "Found an invalid character in the checking order.\n"
														 << "(bso/grammar/sd_grammar/design_response_grammar.cpp)" << std::endl;
				
								throw std::runtime_error(errorMessage.str());
							}
						}
						if (substituted) break;
					}
				}
				if (!substituted)
				{
					subRectangles[j]->assignStructure(bso::structural_design::component::structure());
				}
			}
			
			// remove the rectangles that have been substituted from subRectangles and subResults
			for (const auto& j : mustSubstitute)
			{
				subRectangles.erase(j);
				subResults.erase(j);
			}
			mustSubstitute.clear();

		}
		
		// check consequences of substitution for line rules
		for (auto& i : mSDLineRules)
		{ // for each sd line rule
			// find the potential structural types from all the possible sd rectangle rules this
			// line rule may be associated with
			std::vector<structural_design::component::structure> potentialStructures;
			for (const auto& j : i.first->cfRectangles())
			{ // for each rectangle that this line is associated with
				auto SDRectangleRuleSearch = mSDRectangleRules.find(j);
				if (SDRectangleRuleSearch != mSDRectangleRules.end())
				{ // if that rectangle is associated with an sd rectangle rule, store its structural type
					potentialStructures.push_back(SDRectangleRuleSearch->second->getStructure());
				}
			}

			// pass the potential structural types to the sd line rule
			i.second->assignStructure(potentialStructures);
		}
		
		// check convergence
		if (i != etaConverge) 
		{ // if there is still an iteration coming, store the model as intermediate
			mIntermediateSDModels.push_back(mSDModel);
			mSDModel = bso::structural_design::sd_model();
		}
	}
	
	// delete the rules
	for (auto& i : mSDVertexRules) delete i.second;
	for (auto& i : mSDLineRules) delete i.second;
	for (auto& i : mSDRectangleRules) delete i.second;
	
	return mSDModel;
} // default_sd_grammar

} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_DESIGN_RESPONSE_GRAMMAR_CPP