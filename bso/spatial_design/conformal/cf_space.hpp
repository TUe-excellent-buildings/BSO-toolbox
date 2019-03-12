#ifndef CF_SPACE_HPP
#define CF_SPACE_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_space : public utilities::geometry::quad_hexahedron,
									 public cf_building_entity
	{
	private:

	public:
		cf_space(const utilities::geometry::quad_hexahedron& rhs, cf_building_model* buildingModel);
		
		void checkVertex(cf_vertex* pPtr);
		
		void addLine					(cf_line* 			lPtr	) = delete;
		void addRectangle			(cf_rectangle* 	recPtr) = delete;
		void removeLine				(cf_line* 			lPtr	) = delete;
		void removeRectangle	(cf_rectangle* 	recPtr) = delete;
		void addSpace					(cf_space*			spPtr	) = delete;

		const std::vector<cf_vertex*		>& cfVertices() 	const { return mCFVertices;}
		const std::vector<cf_line*			>& cfLines() 			const { return mCFLines;}
		const std::vector<cf_rectangle*	>& cfRectangles() const { return mCFRectangles;}
		const std::vector<cf_space*			>& cfSpaces() 		const { return mCFSpaces;}
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_SPACE_HPP