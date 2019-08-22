#ifndef BSO_BP_LAYER_CPP
#define BSO_BP_LAYER_CPP

#include <sstream>
#include <stdexcept>


namespace bso { namespace building_physics { namespace properties {

layer::layer(const material& mat, const double& thickness)
:	mMaterial(mat), mThickness(thickness)
{
	if (mThickness <= 0)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize a layer with an invalid\n"
								 << "thickness: " << mThickness << ".\n"
								 << "value should be non-negative and non-zero.\n"
								 << "(bso/building_physics/properties/layer.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // ctor()

layer::~layer()
{
	
} // dtor()

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_LAYER_CPP