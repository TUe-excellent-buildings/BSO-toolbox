#ifndef BSO_GRAMMAR_EDGE_PROPERTY_HPP
#define BSO_GRAMMAR_EDGE_PROPERTY_HPP

#include <bso/grammar/rule_set/property_base.hpp>

namespace bso { namespace grammar { namespace rule_set {

class edge_property : public property_base
{
private:
	bso::spatial_design::conformal::cf_edge* mEdge;
	
	bool mIsVertical = false;
	bool mIsHorizontal = false;
public:
	edge_property(bso::spatial_design::conformal::cf_edge* edge);
	~edge_property();
	
	bso::spatial_design::conformal::cf_edge* getEdgePtr() const {return mEdge;}
	const bool& isVertical() const {return mIsVertical;}
	const bool& isHorizontal() const {return mIsHorizontal;}
};
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/edge_property.cpp>

#endif // BSO_GRAMMAR_EDGE_PROPERTY_HPP