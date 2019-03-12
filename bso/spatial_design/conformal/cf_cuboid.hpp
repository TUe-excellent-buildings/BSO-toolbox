#ifndef CF_CUBOID_HPP
#define CF_CUBOID_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_cuboid : public bso::utilities::geometry::quad_hexahedron,
										public cf_geometry_entity
	{
	private:
		
	public:
		cf_cuboid(const utilities::geometry::quad_hexahedron& rhs, cf_geometry_model* geomModel);
		~cf_cuboid();
		
		void split(cf_vertex* pPtr);
		void checkAssociated(cf_vertex* pPtr);
		
		void addLine					(cf_line* 			lPtr	) = delete ;
		void addRectangle			(cf_rectangle* 	recPtr) = delete ;
		void addCuboid				(cf_cuboid* 		cubPtr) = delete ;
		void removeLine				(cf_line* 			lPtr	) = delete ;
		void removeRectangle	(cf_rectangle* 	recPtr) = delete ;
		void removeCuboid			(cf_cuboid* 		cubPtr) = delete ;
		void addPoint					(cf_point*			pPtr	) = delete ;
		void addEdge					(cf_edge*				ePtr	) = delete ;
		void addSurface				(cf_surface*		srfPtr) = delete ;
		
		const std::vector<cf_cuboid*		>& cfCuboids() 		const { return mCFCuboids;}
		const std::vector<cf_point*			>& cfPoints() 		const { return mCFPoints;}
		const std::vector<cf_edge*			>& cfEdges() 			const { return mCFEdges;}
		const std::vector<cf_surface*		>& cfSurfaces() 	const { return mCFSurfaces;}
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_CUBOID_HPP