#ifndef CF_VERTEX_CPP
#define CF_VERTEX_CPP

#include <algorithm>
#include <sstream>
#include <stdexcept>

namespace bso { namespace spatial_design { namespace conformal {

	void cf_vertex::setGeometryModel(cf_geometry_model* geometryModel)
	{
		mGeometryModel = geometryModel;
	}
	
	void cf_vertex::split(cf_vertex* pPtr)
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot call \"split()\" on a cf_vertex\n"
								 << "(bso/spatial_design/conformal/cf_vertex.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	
	void cf_vertex::checkAssociated(cf_vertex* pPtr)
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot call \"checkAssociated()\" on a cf_vertex\n"
								 << "(bso/spatial_design/conformal/cf_vertex.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	}
	
} // conformal
} // spatial_design
} // bso

#endif // CF_VERTEX_CPP