#ifndef CF_GEOMETRY_ENTITY_HPP
#define CF_GEOMETRY_ENTITY_HPP

#include <string>

namespace bso { namespace spatial_design { namespace conformal {

	class cf_building_entity : public cf_entity
	{
	protected:
		cf_building_model* mBuildingModel;
		
		std::string mType;
	public:
		std::string& type() {return mType;}
		const std::string& type() const {return mType;}

		cf_building_model* getBuildingModel() const {return mBuildingModel;}
		
		virtual void checkVertex(cf_vertex* pPtr) = 0;
		
	};

} // conformal
} // spatial_design
} // bso

#endif // CF_GEOMETRY_ENTITY_HPP