#ifndef BSO_GRAMMAR_LINE_PROPERTY_HPP
#define BSO_GRAMMAR_LINE_PROPERTY_HPP

#include <bso/grammar/rule_set/property_base.hpp>

namespace bso { namespace grammar { namespace rule_set {

class line_property : public property_base
{
private:
	bso::spatial_design::conformal::cf_line* mLine;
	
	bool mIsVertical = false;
	bool mIsHorizontal = false;
	bool mIsSpaceEdge = false;
	bool mIsOnSpaceSurface = false;
	bool mIsAboveGroundSurface = false;
	bool mIsBelowGroundSurface = false;
public:
	line_property(bso::spatial_design::conformal::cf_line* line);
	~line_property();
	
	bso::spatial_design::conformal::cf_line* getLinePtr() const {return mLine;}
	const bool& isVertical() const {return mIsVertical;}
	const bool& isHorizontal() const {return mIsHorizontal;}
	const bool& isSpaceEdge() const {return mIsSpaceEdge;}
	const bool& isOnSpaceSurface() const {return mIsOnSpaceSurface;}
	const bool& isAboveGroundSurface() const {return mIsAboveGroundSurface;}
	const bool& isBelowGroundSurface() const {return mIsBelowGroundSurface;}
};
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/line_property.cpp>

#endif // BSO_GRAMMAR_LINE_PROPERTY_HPP