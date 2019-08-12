#ifndef CF_RECTANGLE_HPP
#define CF_RECTANGLE_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_rectangle : public bso::utilities::geometry::quadrilateral,
											 public cf_geometry_entity
	{
	private:
		
	public:
		cf_rectangle(const utilities::geometry::quadrilateral& rhs, cf_geometry_model* geomModel);
		~cf_rectangle();
		
		void split(cf_vertex* pPtr);
		void checkAssociated(cf_vertex* pPtr);
		
		void addLine					(cf_line* 			lPtr	) = delete;
		void addRectangle			(cf_rectangle* 	recPtr) = delete;
		void removeLine				(cf_line* 			lPtr	) = delete;
		void removeRectangle	(cf_rectangle* 	recPtr) = delete;
		void addPoint					(cf_point*			pPtr	) = delete;
		void addEdge					(cf_edge*				ePtr	) = delete;
		void addSpace					(cf_space*			spPtr	) = delete;
		
		const std::vector<cf_rectangle*	>& cfRectangles() = delete;
		const std::vector<cf_point*			>& cfPoints() 		= delete;
		const std::vector<cf_edge*			>& cfEdges() 			= delete;
		const std::vector<cf_space*			>& cfSpaces() 		= delete;
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_RECTANGLE_HPP