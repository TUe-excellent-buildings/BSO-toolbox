#ifndef CF_VERTEX_HPP
#define CF_VERTEX_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_vertex : public utilities::geometry::vertex,
										public cf_geometry_entity
	{
	private:
		
	public:
		using utilities::geometry::vertex::vertex;
		
		void setGeometryModel(cf_geometry_model* geometryModel);
		
		void split(cf_vertex* pPtr);
		void checkAssociated(cf_vertex* pPtr);
		
		void addEdge					(cf_edge*				ePtr	) = delete;
		void addSurface				(cf_surface*		srfPtr) = delete;
		void addSpace					(cf_space*			spPtr	) = delete;
		
		const std::vector<cf_vertex*		>& cfVertices() 	const { return mCFVertices;}
		const std::vector<cf_edge*			>& cfEdges() 			const { return mCFEdges;}
		const std::vector<cf_surface*		>& cfSurfaces() 	const { return mCFSurfaces;}
		const std::vector<cf_space*			>& cfSpaces() 		const { return mCFSpaces;}
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_VERTEX_HPP