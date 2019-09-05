#ifndef BSO_GRAMMAR_RECTANGLE_PROPERTY_HPP
#define BSO_GRAMMAR_RECTANGLE_PROPERTY_HPP

#include <bso/grammar/rule_set/property_base.hpp>

namespace bso { namespace grammar { namespace rule_set {

class rectangle_property : public property_base
{
private:
	bso::spatial_design::conformal::cf_rectangle* mRectangle;

	bso::utilities::geometry::vector mOrientation;
	std::pair<std::string, std::string> mSpaceTypes;
	std::pair<std::string, std::string> mSurfaceTypes;
	
	bool mIsFloor = false;
	bool mIsWall = false;
	bool mIsSpaceSeparating	= false;
	bool mIsExternal = false;
	bool mIsRoof = false;
	bool mIsOverhang = false;
	bool mIsAboveGroundSurface = false;
	bool mIsBelowGroundSurface = false;
public:
	rectangle_property(bso::spatial_design::conformal::cf_rectangle* rectangle);
	~rectangle_property();
	
	bso::spatial_design::conformal::cf_rectangle* getRectanglePtr() const {return mRectangle;}
	const bso::utilities::geometry::vector& getOrientation() const {return mOrientation;}
	const std::pair<std::string, std::string>& getSpaceTypes() const {return mSpaceTypes;}
	const std::pair<std::string, std::string>& getSurfaceTypes() const {return mSurfaceTypes;}
	
	const bool& isWall() const {return mIsWall;}
	const bool& isFloor() const {return mIsFloor;}
	const bool& isSpaceSeparating() const {return mIsSpaceSeparating;}
	const bool& isExternal() const {return mIsExternal;}
	const bool& isRoof() const {return mIsRoof;}
	const bool& isOverhang() const {return mIsOverhang;}
	const bool& isAboveGroundSurface() const {return mIsAboveGroundSurface;}
	const bool& isBelowGroundSurface() const {return mIsBelowGroundSurface;}
};
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/rectangle_property.cpp>

#endif // BSO_GRAMMAR_RECTANGLE_PROPERTY_HPP