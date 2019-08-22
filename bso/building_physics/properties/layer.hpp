#ifndef BSO_BP_LAYER_HPP
#define BSO_BP_LAYER_HPP

#include <bso/building_physics/properties/material.hpp>

namespace bso { namespace building_physics { namespace properties {

class layer
{
private:
	material mMaterial;
	double mThickness;
public:
	layer(const material& mat, const double& thickness);
	~layer();
	
	const material& getMaterial() const {return mMaterial;}
	const double& getThickness() const {return mThickness;}
};

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#include <bso/building_physics/properties/layer.cpp>

#endif // BSO_BP_LAYER_HPP