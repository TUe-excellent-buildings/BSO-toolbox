#ifndef BSO_GRAMMAR_PROPERTY_BASE_HPP
#define BSO_GRAMMAR_PROPERTY_BASE_HPP

#include <bso/spatial_design/cf_building.hpp>

namespace bso { namespace grammar { namespace rule_set {

class property_base
{
protected:
	bool mIsOccupied = false;
	
	bool mIsVertexProperty = false;
	bool mIsLineProperty = false;
	bool mIsRectangleProperty = false;
	bool mIsCuboidProperty = false;
	bool mIsPointProperty = false;
	bool mIsEdgeProperty = false;
	bool mIsSurfaceProperty = false;
	bool mIsSpaceProperty = false;
public:
	property_base();
	virtual ~property_base() = 0;
	
	virtual void setOccupation(const bool& occupied) {mIsOccupied = occupied;}
	
	virtual const bool& isOccupied() const {return mIsOccupied;}
	virtual const bool& isVertexProperty() const {return mIsVertexProperty;}
	virtual const bool& isLineProperty() const {return mIsLineProperty;}
	virtual const bool& isRectangleProperty() const {return mIsRectangleProperty;}
	virtual const bool& isCuboidProperty() const {return mIsCuboidProperty;}
	virtual const bool& isPointProperty() const {return mIsPointProperty;}
	virtual const bool& isEdgeProperty() const {return mIsEdgeProperty;}
	virtual const bool& isSurfaceProperty() const {return mIsSurfaceProperty;}
	virtual const bool& isSpaceProperty() const {return mIsSpaceProperty;}
};
	
} // namespace rule_set
} // namespace grammar
} // namespace bso

#include <bso/grammar/rule_set/property_base.cpp>

#endif // BSO_GRAMMAR_PROPERTY_BASE_HPP