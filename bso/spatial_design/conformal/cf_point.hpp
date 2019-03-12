#ifndef CF_POINT_HPP
#define CF_POINT_HPP

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_point : public utilities::geometry::vertex,
									 public cf_building_entity
	{
	private:
		// associated with
		cf_vertex* mVertex;

	public:
		cf_point(const utilities::geometry::vertex& p, cf_building_model* buildingModel);
		
		cf_vertex* getVertexPtr() const;
		void checkVertex(cf_vertex* pPtr);
		
		void addLine					(cf_line* 			lPtr	) = delete;
		void addRectangle			(cf_rectangle* 	recPtr) = delete;
		void addCuboid				(cf_cuboid* 		cubPtr) = delete;
		void removeLine				(cf_line* 			lPtr	) = delete;
		void removeRectangle	(cf_rectangle* 	recPtr) = delete;
		void removeCuboid			(cf_cuboid* 		cubPtr) = delete;
		void addPoint					(cf_point*			pPtr	) = delete;
		
		const std::vector<cf_vertex*		>& cfVertices() 	const { return mCFVertices;}
		const std::vector<cf_line*			>& cfLines() 			const { return mCFLines;}
		const std::vector<cf_rectangle*	>& cfRectangles() const { return mCFRectangles;}
		const std::vector<cf_cuboid*		>& cfCuboids() 		const { return mCFCuboids;}
		const std::vector<cf_point*			>& cfPoints() 		const { return mCFPoints;}
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_POINT_HPP