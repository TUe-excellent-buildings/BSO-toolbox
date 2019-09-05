#ifndef BSO_GRAMMAR_SURFACE_PROPERTY_CPP
#define BSO_GRAMMAR_SURFACE_PROPERTY_CPP

namespace bso { namespace grammar { namespace rule_set {

surface_property::surface_property(bso::spatial_design::conformal::cf_surface* surface)
:	mSurface(surface), mOrientation(surface->getNormal().normalized())
{
	mIsSurfaceProperty = true;

	bso::utilities::geometry::vertex spaceCenter = 
		mSurface->cfSpaces().front()->getCenter();
	bso::utilities::geometry::vertex checkVertex = mSurface->getCenter() +
		(sqrt(mSurface->getArea()) * mOrientation);
	if (!(mSurface->sameSide(spaceCenter,checkVertex,1e-9))) mOrientation *= -1;
	
	if (abs(mOrientation.getHeading().second) > 45) mIsFloor = true;
	else mIsWall = true;
	
	mSurfaceType = mSurface->getSurfaceType();

} // ctor()

surface_property::~surface_property()
{

} // dtor()

} // namespace rule_set
} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_SURFACE_PROPERTY_CPP