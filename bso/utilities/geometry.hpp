#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

namespace bso { namespace utilities { namespace geometry {
	
	class vertex; // [x,y,z]
	class vector; // [dx,dy,dz]
	class line_segment; // 2 vertices {a,b}
	class triangle; // 3 vertices {a,b,c}
	class tetrahedron; // 4 vertices {a,b,c,d}
	class quadrilateral; // 4 vertices {a,b,c,d} see geometry quadrilateral for order specification
	class quad_hexahedron; // 8 vertices {a,b,c,d} see geometry/quad_hexahedron for order specification

} // namespace geometry
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/vertex.hpp>
#include <bso/utilities/geometry/vector.hpp>
#include <bso/utilities/geometry/line_segment.hpp>
#include <bso/utilities/geometry/triangle.hpp>
#include <bso/utilities/geometry/tetrahedron.hpp>
#include <bso/utilities/geometry/quadrilateral.hpp>
#include <bso/utilities/geometry/quad_hexahedron.hpp>

#endif // GEOMETRY_HPP