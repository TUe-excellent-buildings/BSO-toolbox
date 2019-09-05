#ifndef BSO_GRAMMAR_SPACE_PROPERTY_HPP
#define BSO_GRAMMAR_SPACE_PROPERTY_HPP

#include <bso/grammar/rule_set/property_base.hpp>

namespace bso { namespace grammar { namespace rule_set {

class space_property : public property_base
{
private:
	bso::spatial_design::conformal::cf_space* mSpace;
	
	std::string mSpaceType;
public:
	space_property(bso::spatial_design::conformal::cf_space* space);
	~space_property();
	
	bso::spatial_design::conformal::cf_space* getSpacePtr() const {return mSpace;}
	const std::string& getSpaceType() const {return mSpaceType;}
};
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/space_property.cpp>

#endif // BSO_GRAMMAR_SPACE_PROPERTY_HPP