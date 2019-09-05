#ifndef BSO_GRAMMAR_SPACE_PROPERTY_CPP
#define BSO_GRAMMAR_SPACE_PROPERTY_CPP

namespace bso { namespace grammar { namespace rule_set {

space_property::space_property(bso::spatial_design::conformal::cf_space* space)
:	mSpace(space), mSpaceType(space->getSpaceType())
{
	mIsSpaceProperty = true;

} // ctor()

space_property::~space_property()
{
	
}
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_SPACE_PROPERTY_CPP