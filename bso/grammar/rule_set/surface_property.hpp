#ifndef BSO_GRAMMAR_SURFACE_PROPERTY_HPP
#define BSO_GRAMMAR_SURFACE_PROPERTY_HPP

#include <bso/grammar/rule_set/property_base.hpp>

namespace bso { namespace grammar { namespace rule_set {

class surface_property : public property_base
{
private:
	bso::spatial_design::conformal::cf_surface* mSurface;

	bso::utilities::geometry::vector mOrientation;
	std::string mSurfaceType;
	
	bool mIsFloor = false;
	bool mIsWall = false;
public:
	surface_property(bso::spatial_design::conformal::cf_surface* surface);
	~surface_property();
	
	bso::spatial_design::conformal::cf_surface* getSurfacePtr() const {return mSurface;}
	const bso::utilities::geometry::vector& getOrientation() const {return mOrientation;}
	const std::string& getSurfaceType() const {return mSurfaceType;}
	const bool& isWall() const {return mIsWall;}
	const bool& isFloor() const {return mIsFloor;}
};
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/surface_property.cpp>

#endif // BSO_GRAMMAR_SURFACE_PROPERTY_HPP