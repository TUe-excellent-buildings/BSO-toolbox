#ifndef BSO_GRAMMAR_POINT_PROPERTY_CPP
#define BSO_GRAMMAR_POINT_PROPERTY_CPP

namespace bso { namespace grammar { namespace rule_set {

point_property::point_property(bso::spatial_design::conformal::cf_point* point)
:	mPoint(point)
{
	mIsPointProperty = true;
} // ctor()

point_property::~point_property()
{
	
} // dtor()

} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_POINT_PROPERTY_CPP