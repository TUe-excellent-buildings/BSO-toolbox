#ifndef CF_SURFACE_HPP
#define CF_SURFACE_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_surface : public utilities::geometry::quadrilateral,
										 public cf_building_entity
	{
	private:
		std::string mSurfaceType;
	public:
		cf_surface(const utilities::geometry::quadrilateral& rhs, cf_building_model* buildingModel);
		
		void checkVertex(cf_vertex* pPtr);
		
		void addLine					(cf_line* 			lPtr	) = delete;
		void addCuboid				(cf_cuboid* 		cubPtr) = delete;
		void removeLine				(cf_line* 			lPtr	) = delete;
		void removeCuboid			(cf_cuboid* 		cubPtr) = delete;
		void addPoint					(cf_point*			pPtr	) = delete;
		void addEdge					(cf_edge*				ePtr	) = delete;
		void addSurface				(cf_surface*		srfPtr) = delete;
		
		void setSurfaceType(const std::string& surfaceType) {mSurfaceType = surfaceType;}
		const std::string& getSurfaceType() const {return mSurfaceType;}

		const std::vector<cf_vertex*		>& cfVertices() 	const { return mCFVertices;}
		const std::vector<cf_line*			>& cfLines() 			const { return mCFLines;}
		const std::vector<cf_cuboid*		>& cfCuboids() 		const { return mCFCuboids;}
		const std::vector<cf_surface*		>& cfSurfaces() 	const { return mCFSurfaces;}
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_SURFACE_HPP