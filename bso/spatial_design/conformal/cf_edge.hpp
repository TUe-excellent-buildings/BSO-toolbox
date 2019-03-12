#ifndef CF_EDGE_HPP
#define CF_EDGE_HPP

#include <string>
#include <vector>

namespace bso { namespace spatial_design { namespace conformal {
	
	class cf_edge : public utilities::geometry::line_segment,
									public cf_building_entity
	{
	private:

	public:
		cf_edge(const utilities::geometry::line_segment& rhs, cf_building_model* geomModel);
		
		void checkVertex(cf_vertex* pPtr);
		
		void addRectangle			(cf_rectangle* 	recPtr) = delete;
		void addCuboid				(cf_cuboid* 		cubPtr) = delete;
		void removeRectangle	(cf_rectangle* 	recPtr) = delete;
		void removeCuboid			(cf_cuboid* 		cubPtr) = delete;
		void addPoint					(cf_point*			pPtr	) = delete;
		void addEdge					(cf_edge*				ePtr	) = delete;

		const std::vector<cf_vertex*		>& cfVertices() 	const { return mCFVertices;}
		const std::vector<cf_rectangle*	>& cfRectangles() const { return mCFRectangles;}
		const std::vector<cf_cuboid*		>& cfCuboids() 		const { return mCFCuboids;}
		const std::vector<cf_edge*			>& cfEdges() 			const { return mCFEdges;}
	};
	
} // conformal
} // spatial_design
} // bso

#endif // CF_EDGE_HPP