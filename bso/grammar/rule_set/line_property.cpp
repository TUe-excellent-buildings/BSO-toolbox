#ifndef BSO_GRAMMAR_LINE_PROPERTY_CPP
#define BSO_GRAMMAR_LINE_PROPERTY_CPP

namespace bso { namespace grammar { namespace rule_set {

line_property::line_property(bso::spatial_design::conformal::cf_line* line)
: mLine(line)
{
	mIsLineProperty = true;
	
	mIsVertical = mLine->getVector().isVertical();
	mIsHorizontal = mLine->getVector().isHorizontal();
	if (mLine->cfEdges().size() > 0) mIsSpaceEdge = true;
	if (mIsSpaceEdge) mIsOnSpaceSurface = true;
	else
	{
		for (const auto& i : mLine->cfRectangles())
		{
			if (i->cfSurfaces().size() > 0)
			{
				mIsOnSpaceSurface = true;
				break;
			}
		}
	}
	
	for (const auto& i : *mLine)
	{
		if (i(2) > 1e-9)
		{
			mIsAboveGroundSurface = true;
			break;
		}
	}
	if (mIsHorizontal)
	{
		if (mLine->getCenter()(2) > -1e-9 &&
				mLine->getCenter()(2) <  1e-9)
		{
			mIsBelowGroundSurface = false;
		}
		else mIsBelowGroundSurface = ! mIsAboveGroundSurface;
	}
	else mIsBelowGroundSurface = !mIsAboveGroundSurface;
} // ctor()

line_property::~line_property()
{
	
} // dtor()
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_LINE_PROPERTY_CPP