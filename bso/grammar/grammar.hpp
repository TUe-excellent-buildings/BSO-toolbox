#ifndef BSO_GRAMMAR_HPP
#define BSO_GRAMMAR_HPP

#include <bso/spatial_design/cf_building.hpp>
#include <bso/grammar/rule_set/rule_sets.hpp>

namespace bso { namespace grammar {


#ifdef SD_MODEL_HPP
class DEFAULT_SD_GRAMMAR;
#endif

#ifdef BSO_BP_MODEL_HPP
class DEFAULT_BP_GRAMMAR;
#endif

class grammar
{
private:
	spatial_design::cf_building mCFBuilding;

	// rule sets
	std::map<spatial_design::conformal::cf_vertex*, rule_set::vertex_property*> mVertexProperties;
	std::map<spatial_design::conformal::cf_line*, rule_set::line_property*> mLineProperties;
	std::map<spatial_design::conformal::cf_rectangle*, rule_set::rectangle_property*> mRectangleProperties;
	std::map<spatial_design::conformal::cf_cuboid*, rule_set::cuboid_property*> mCuboidProperties;

	std::map<spatial_design::conformal::cf_point*, rule_set::point_property*> mPointProperties;
	std::map<spatial_design::conformal::cf_edge*, rule_set::edge_property*> mEdgeProperties;
	std::map<spatial_design::conformal::cf_surface*, rule_set::surface_property*> mSurfaceProperties;
	std::map<spatial_design::conformal::cf_space*, rule_set::space_property*> mSpaceProperties;

	#ifdef SD_MODEL_HPP
	std::map<spatial_design::conformal::cf_vertex*, rule_set::sd_vertex_rule*> mSDVertexRules;
	std::map<spatial_design::conformal::cf_line*, rule_set::sd_line_rule*> mSDLineRules;
	std::map<spatial_design::conformal::cf_rectangle*, rule_set::sd_rectangle_rule*> mSDRectangleRules;
	std::map<spatial_design::conformal::cf_cuboid*, rule_set::sd_cuboid_rule*> mSDCuboidRules;

	structural_design::sd_model mSDModel;
	std::vector<structural_design::sd_model> mIntermediateSDModels;
	std::map<std::pair<std::string,std::string>,
		structural_design::component::structure> mSDWallProperties;
	std::map<std::pair<std::string,std::string>,
		structural_design::component::structure> mSDFloorProperties;
    std::map<std::string, structural_design::component::structure> mSDSpaceProperties;
	structural_design::component::structure mLoadPanel;
	std::multimap<std::string, structural_design::component::load> mLoads;
	unsigned int mMeshSize = 1;

	void mReadSDSettings(const std::string& fileName);
	#endif

	#ifdef BSO_BP_MODEL_HPP
	std::map<spatial_design::conformal::cf_space*, rule_set::bp_space_rule*> mBPSpaceRules;
	std::map<spatial_design::conformal::cf_rectangle*, rule_set::bp_rectangle_rule*> mBPRectangleRules;

	boost::posix_time::time_duration mBPWarmUpDuration;
	boost::posix_time::time_duration mBPTimeStepSize;
	double mBPGroundTemperature = 0;
	std::map<std::string,boost::posix_time::time_period> mBPSimulationPeriods;
	std::map<std::string,bso::building_physics::properties::space_settings> mBPSpaceSettings;
	std::map<std::pair<std::string,std::string>,bso::building_physics::properties::construction> mBPWallConstructions;
	std::map<std::pair<std::string,std::string>,bso::building_physics::properties::construction> mBPFloorConstructions;
	std::map<std::pair<std::string,std::string>,bso::building_physics::properties::glazing> mBPWallGlazings;
	std::map<std::pair<std::string,std::string>,bso::building_physics::properties::glazing> mBPFloorGlazings;

	building_physics::bp_model mBPModel;
	std::vector<building_physics::bp_model> mIntermediateBPModels;
	void mReadBPSettings(const std::string& fileName);
	#endif

public:
	grammar(const spatial_design::cf_building& cf);
	~grammar();

	#ifdef SD_MODEL_HPP
	template <typename T = DEFAULT_SD_GRAMMAR, typename...ARGS>
	bso::structural_design::sd_model sd_grammar(const ARGS&...);
	const std::vector<structural_design::sd_model>& getIntermediateSDModels() const
		{return mIntermediateSDModels;}
	#endif

	#ifdef BSO_BP_MODEL_HPP
	template <typename T = DEFAULT_BP_GRAMMAR, typename...ARGS>
	bso::building_physics::bp_model bp_grammar(const ARGS&...);
	const std::vector<building_physics::bp_model> getIntermediateBPModels() const
		{return mIntermediateBPModels;}
	#endif
};

} // namespace grammar
} // namespace bso

#include <bso/grammar/grammar.cpp>

#ifdef SD_MODEL_HPP
#include <bso/grammar/grammar_sd_specifics.cpp>
#include <bso/grammar/sd_grammars/default_sd_grammar.cpp>
#include <bso/grammar/sd_grammars/volume_sd_grammar.cpp>
#endif

#ifdef BSO_BP_MODEL_HPP
#include <bso/grammar/grammar_bp_specifics.cpp>
#include <bso/grammar/bp_grammars/default_bp_grammar.cpp>
#endif

#endif // BSO_GRAMMAR_HPP
