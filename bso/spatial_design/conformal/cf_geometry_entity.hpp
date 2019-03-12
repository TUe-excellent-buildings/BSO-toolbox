#ifndef CF_BUILDING_ENTITY_HPP
#define CF_BUILDING_ENTITY_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_geometry_entity : public cf_entity
	{
	protected:
		cf_geometry_model* mGeometryModel;
		
		bool mDeletion = false;
		bool mStructural = false;
	public:
		bool& isStructural() {return mStructural;}
		const bool& isStructural() const {return mStructural;}

		bool& deletion() {return mDeletion;}
		const bool& deletion() const {return mDeletion;}
		cf_geometry_model* getGeometryModel() const {return mGeometryModel;}
		
		virtual void split(cf_vertex* pPtr) = 0;
		virtual void checkAssociated(cf_vertex* pPtr) = 0;
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_BUILDING_ENTITY_HPP