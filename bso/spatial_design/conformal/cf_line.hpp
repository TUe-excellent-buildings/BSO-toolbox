#ifndef CF_LINE_HPP
#define CF_LINE_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_line : public utilities::geometry::line_segment,
									public cf_geometry_entity
	{
	private:
		
	public:
		cf_line(const utilities::geometry::line_segment& l, cf_geometry_model* geomModel);
		~cf_line();
		
		void split(cf_vertex* pPtr);
		void checkAssociated(cf_vertex* pPtr);

		void addLine					(cf_line* 			lPtr	) = delete;
		void removeLine				(cf_line* 			lPtr	) = delete;
		void addPoint					(cf_point*			pPtr	) = delete;
		void addSurface				(cf_surface*		srfPtr) = delete;
		void addSpace					(cf_space*			spPtr	) = delete;
		
		const std::vector<cf_line*			>& cfLines() 			const = delete;
		const std::vector<cf_point*			>& cfPoints() 		const = delete;
		const std::vector<cf_surface*		>& cfSurfaces() 	const = delete;
		const std::vector<cf_space*			>& cfSpaces() 		const = delete;
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_LINE_HPP