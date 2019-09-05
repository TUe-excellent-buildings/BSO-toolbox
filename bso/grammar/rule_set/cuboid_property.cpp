#ifndef BSO_GRAMMAR_CUBOID_PROPERTY_CPP
#define BSO_GRAMMAR_CUBOID_PROPERTY_CPP

namespace bso { namespace grammar { namespace rule_set {

cuboid_property::cuboid_property(bso::spatial_design::conformal::cf_cuboid* cuboid)
:	mCuboid(cuboid)
{
	mIsCuboidProperty = true;
	
	if (mCuboid->cfSpaces().size() > 0)
	{
		mIsInSpace = true;
		mSpaceType = mCuboid->cfSpaces().front()->getSpaceType();
	}
	
	for (const auto& i : *mCuboid)
	{
		if (i(2) > 1e-9)
		{
			mIsAboveGroundSurface = true;
			break;
		}
	}
	mIsBelowGroundSurface = !mIsAboveGroundSurface;

} // ctor()

cuboid_property::~cuboid_property()
{
	
}
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_CUBOID_PROPERTY_CPP