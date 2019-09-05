#ifndef BSO_GRAMMAR_POINT_PROPERTY_HPP
#define BSO_GRAMMAR_POINT_PROPERTY_HPP

#include <bso/grammar/rule_set/property_base.hpp>

namespace bso { namespace grammar { namespace rule_set {

class point_property : public property_base
{
private:
	bso::spatial_design::conformal::cf_point* mPoint;
public:
	point_property(bso::spatial_design::conformal::cf_point* point);
	~point_property();
	
	bso::spatial_design::conformal::cf_point* getPointPtr() const {return mPoint;}
};
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/point_property.cpp>

#endif // BSO_GRAMMAR_POINT_PROPERTY_HPP