#ifndef BSO_GRAMMAR_VERTEX_PROPERTY_HPP
#define BSO_GRAMMAR_VERTEX_PROPERTY_HPP

#include <bso/grammar/rule_set/property_base.hpp>

namespace bso { namespace grammar { namespace rule_set {

class vertex_property : public property_base
{
private:
	bso::spatial_design::conformal::cf_vertex* mVertex;
	
	bool mIsSpaceCorner = false;
	bool mIsOnSpaceSurface = false;
	bool mIsOnSpaceEdge = false;
	bool mIsAboveGroundSurface = false;
	bool mIsBelowGroundSurface = false;
public:
	vertex_property(bso::spatial_design::conformal::cf_vertex* vertex);
	~vertex_property();
	
	bso::spatial_design::conformal::cf_vertex* getVertexPtr() const {return mVertex;}
	const bool& isSpaceCorner() const {return mIsSpaceCorner;}
	const bool& isOnSpaceSurface() const {return mIsOnSpaceSurface;}
	const bool& isOnSpaceEdge() const {return mIsOnSpaceEdge;}
	const bool& isAboveGroundSurface() const {return mIsAboveGroundSurface;}
	const bool& isBelowGroundSurface() const {return mIsBelowGroundSurface;}
};
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/vertex_property.cpp>

#endif // BSO_GRAMMAR_VERTEX_PROPERTY_HPP