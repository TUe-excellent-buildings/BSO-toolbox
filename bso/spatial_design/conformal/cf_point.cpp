#ifndef CF_POINT_CPP
#define CF_POINT_CPP

#include <sstream>
#include <stdexcept>

namespace bso { namespace spatial_design { namespace conformal {
	
	cf_point::cf_point(const utilities::geometry::vertex& p, cf_building_model* buildingModel)
	: utilities::geometry::vertex::vertex(p)
	{ // 
		mBuildingModel = buildingModel;
		mVertex = ((cf_geometry_model*)mBuildingModel)->addVertex(*this);
		mVertex->addPoint(this);
	} // 

	cf_vertex* cf_point::getVertexPtr() const
	{ // 
		return mVertex;
	} // 
	
	void cf_point::checkVertex(cf_vertex* pPtr)
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot call \"split()\" on a cf_vertex\n"
								 << "(bso/spatial_design/conformal/cf_vertex.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	
} // conformal
} // spatial_design
} // bso

#endif // CF_POINT_CPP