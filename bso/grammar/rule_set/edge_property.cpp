#ifndef BSO_GRAMMAR_EDGE_PROPERTY_CPP
#define BSO_GRAMMAR_EDGE_PROPERTY_CPP

namespace bso { namespace grammar { namespace rule_set {

edge_property::edge_property(bso::spatial_design::conformal::cf_edge* edge)
: mEdge(edge)
{
	mIsEdgeProperty = true;
	
	mIsVertical = mEdge->getVector().isVertical();
	mIsHorizontal = mEdge->getVector().isHorizontal();
} // ctor()

edge_property::~edge_property()
{
	
} // dtor()
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_EDGE_PROPERTY_CPP