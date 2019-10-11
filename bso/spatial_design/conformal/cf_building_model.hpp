#ifndef CF_BUILDING_MODEL_HPP
#define CF_BUILDING_MODEL_HPP

#include <bso/spatial_design/ms_building.hpp>

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_building_model : public cf_geometry_model
	{
	private:
		std::vector<cf_point*> mCFPoints;
		std::vector<cf_edge*> mCFEdges;
		std::vector<cf_surface*> mCFSurfaces;
		std::vector<cf_space*> mCFSpaces;
		ms_building mMSModel; // safe it, in case copy consttructor is called
		double mTol;
		
		void addSpace(const ms_space& msSpace);
		void makeConformal();
		
		cf_building_model& operator = (cf_building_model& rhs) = default;
		friend class cf_geometry_entity;
	public:
		cf_building_model(const cf_building_model& rhs);
		cf_building_model(const ms_building& msModel, const double& tol = 1e-3);
		~cf_building_model();

		const std::vector<cf_point*			>& cfPoints() 		const { return mCFPoints;}
		const std::vector<cf_edge*			>& cfEdges() 			const { return mCFEdges;}
		const std::vector<cf_surface*		>& cfSurfaces() 	const { return mCFSurfaces;}
		const std::vector<cf_space*			>& cfSpaces() 		const { return mCFSpaces;}
		
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_BUILDING_MODEL_HPP