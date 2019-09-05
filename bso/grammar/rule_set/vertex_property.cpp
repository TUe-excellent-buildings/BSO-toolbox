#ifndef BSO_GRAMMAR_VERTEX_PROPERTY_CPP
#define BSO_GRAMMAR_VERTEX_PROPERTY_CPP

namespace bso { namespace grammar { namespace rule_set {

vertex_property::vertex_property(bso::spatial_design::conformal::cf_vertex* vertex)
:	mVertex(vertex)
{
	mIsVertexProperty = true;
	
	if (mVertex->cfPoints().size() > 0) mIsSpaceCorner = true;
	if (mIsSpaceCorner)
	{
		mIsOnSpaceSurface = true;
		mIsOnSpaceEdge = true;
	}
	else
	{
		for (const auto& i : mVertex->cfLines())
		{
			if (i->cfEdges().size() > 0)
			{
				mIsOnSpaceEdge = true;
				break;
			}
		}
	}
	if (mIsOnSpaceEdge)
	{
		mIsOnSpaceSurface = true;
	}
	else
	{
		for (const auto& i : mVertex->cfRectangles())
		{
			if (i->cfSurfaces().size() > 0)
			{
				mIsOnSpaceSurface = true;
				break;
			}
		}
	}
	
	if ((*vertex)(2) > 1e-9) mIsAboveGroundSurface = true;
	if ((*vertex)(2) < -1e-9) mIsBelowGroundSurface = true;
} // ctor()

vertex_property::~vertex_property()
{
	
} // dtor()

} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_VERTEX_PROPERTY_CPP