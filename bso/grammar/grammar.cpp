#ifndef BSO_GRAMMAR_CPP
#define BSO_GRAMMAR_CPP

namespace bso { namespace grammar {

grammar::grammar(const spatial_design::cf_building& cf)
:	mCFBuilding(cf),
	mLoadPanel(structural_design::component::structure())
{
	for (const auto& i : mCFBuilding.cfVertices())
	{
		mVertexProperties[i] = new rule_set::vertex_property(i);
	}
	for (const auto& i : mCFBuilding.cfLines())
	{
		mLineProperties[i] = new rule_set::line_property(i);
	}
	for (const auto& i : mCFBuilding.cfRectangles())
	{
		mRectangleProperties[i] = new rule_set::rectangle_property(i);
	}
	for (const auto& i : mCFBuilding.cfCuboids())
	{
		mCuboidProperties[i] = new rule_set::cuboid_property(i);
	}
	for (const auto& i : mCFBuilding.cfPoints())
	{
		mPointProperties[i] = new rule_set::point_property(i);
	}
	for (const auto& i : mCFBuilding.cfEdges())
	{
		mEdgeProperties[i] = new rule_set::edge_property(i);
	}
	for (const auto& i : mCFBuilding.cfSurfaces())
	{
		mSurfaceProperties[i] = new rule_set::surface_property(i);
	}
	for (const auto& i : mCFBuilding.cfSpaces())
	{
		mSpaceProperties[i] = new rule_set::space_property(i);
	}
	
} // ctor()

grammar::~grammar()
{
	for (auto& i : mVertexProperties) delete i.second;
	for (auto& i : mLineProperties) delete i.second;
	for (auto& i : mRectangleProperties) delete i.second;
	for (auto& i : mCuboidProperties) delete i.second;
	for (auto& i : mPointProperties) delete i.second;
	for (auto& i : mEdgeProperties) delete i.second;
	for (auto& i : mSurfaceProperties) delete i.second;
	for (auto& i : mSpaceProperties) delete i.second;
} // dtor()
	
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_CPP