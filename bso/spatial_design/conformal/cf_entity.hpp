#ifndef CF_ENTITY_HPP
#define CF_ENTITY_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_entity
	{
	protected:
		std::vector<cf_vertex*> mCFVertices;
		std::vector<cf_line*> mCFLines;
		std::vector<cf_rectangle*> mCFRectangles;
		std::vector<cf_cuboid*> mCFCuboids;
		
		std::vector<cf_point*> mCFPoints;
		std::vector<cf_edge*> mCFEdges;
		std::vector<cf_surface*> mCFSurfaces;
		std::vector<cf_space*> mCFSpaces;
	public:
		void addLine					(cf_line* 			lPtr	);
		void addRectangle			(cf_rectangle* 	recPtr);
		void addCuboid				(cf_cuboid* 		cubPtr);
		void removeLine				(cf_line* 			lPtr	);
		void removeRectangle	(cf_rectangle* 	recPtr);
		void removeCuboid			(cf_cuboid* 		cubPtr);
		void addPoint					(cf_point*			pPtr	);
		void addEdge					(cf_edge*				ePtr	);
		void addSurface				(cf_surface*		srfPtr);
		void addSpace					(cf_space*			spPtr	);
		
		const std::vector<cf_vertex*		>& cfVertices() 	const { return mCFVertices;}
		const std::vector<cf_line*			>& cfLines() 			const { return mCFLines;}
		const std::vector<cf_rectangle*	>& cfRectangles() const { return mCFRectangles;}
		const std::vector<cf_cuboid*		>& cfCuboids() 		const { return mCFCuboids;}
		const std::vector<cf_point*			>& cfPoints() 		const { return mCFPoints;}
		const std::vector<cf_edge*			>& cfEdges() 			const { return mCFEdges;}
		const std::vector<cf_surface*		>& cfSurfaces() 	const { return mCFSurfaces;}
		const std::vector<cf_space*			>& cfSpaces() 		const { return mCFSpaces;}
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_ENTITY_HPP