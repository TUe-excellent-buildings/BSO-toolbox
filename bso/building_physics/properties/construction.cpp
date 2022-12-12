#ifndef BSO_BP_CONSTRUCTION_CPP
#define BSO_BP_CONSTRUCTION_CPP

namespace bso { namespace building_physics { namespace properties {

construction::construction(const std::string& ID, const std::vector<layer> layers,
	const double& relativeMeasurePoint /*= 0.5*/,
	const std::pair<std::string,Eigen::Vector4d>& visualizationProps /*= {"",Eigen::Vector4d::Ones()}*/)
: mID(ID), mLayers(layers), mRelativeMeasurePoint(relativeMeasurePoint),
	mVisualizationProperties(visualizationProps)
{
	if (mRelativeMeasurePoint < 0 || mRelativeMeasurePoint > 1)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, trying to initialize construction with invalid\n"
								 << "relative measurement point: " << mRelativeMeasurePoint << "\n"
								 << "this value should be greater or equal to 0 and lesser or equal to 1\n"
								 << "(bso/building_physics/properties/construction.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	mThickness = 0;
	mCapacitancePerArea = 0;
	for (const auto& i : mLayers)
	{
		mThickness += i.getThickness();
		mCapacitancePerArea += (i.getThickness()*1e-3) * i.getMaterial().getSpecificHeat() *
													 i.getMaterial().getSpecificWeight();
	}
	
	double absoluteMeasurePoint = mRelativeMeasurePoint * mThickness;
	mResistanceSide1 = 0;
	mResistanceSide2 = 0;
	for (const auto& i : mLayers)
	{
		if (i.getThickness() <= absoluteMeasurePoint)
		{
			mResistanceSide1 += (i.getThickness()*1e-3)/i.getMaterial().getThermalConductivity();
			absoluteMeasurePoint -= i.getThickness();
		}
		else if (absoluteMeasurePoint > 0)
		{
			mResistanceSide1 += (absoluteMeasurePoint*1e-3)/i.getMaterial().getThermalConductivity();
			absoluteMeasurePoint -= i.getThickness();
			mResistanceSide2 += (absoluteMeasurePoint*-1e-3)/i.getMaterial().getThermalConductivity();
		}
		else
		{
			mResistanceSide2 += (i.getThickness()*1e-3)/i.getMaterial().getThermalConductivity();
		}
	}
	
} // ctor()

construction::~construction()
{
	
} // dtor()

construction& construction::operator = (const construction& rhs)
{
	mID = rhs.mID;
	mVisualizationProperties.first = rhs.mVisualizationProperties.first;
	mVisualizationProperties.second << rhs.mVisualizationProperties.second;
	mLayers = rhs.mLayers;
	mThickness = rhs.mThickness;
	mCapacitancePerArea = rhs.mCapacitancePerArea;
	mResistanceSide1 = rhs.mResistanceSide1;
	mResistanceSide2 = rhs.mResistanceSide2;
	mRelativeMeasurePoint = rhs.mRelativeMeasurePoint;
	return *this;
}

} // namespace properties 
} // namespace building_physics 
} // namespace bso

#endif // BSO_BP_CONSTRUCTION_CPP
