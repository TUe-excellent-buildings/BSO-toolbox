#ifndef QUAD_HEXAHEDRON_HPP
#define QUAD_HEXAHEDRON_HPP

#include <bso/utilities/geometry/polyhedron.hpp>
#include <bso/utilities/geometry/tetrahedron.hpp>

namespace bso { namespace utilities { namespace geometry {
	
	class quad_hexahedron : public polyhedron
	{
	protected:
		std::vector<tetrahedron> mTetrahedrons; // decomposition into tetrahedrons
		void sortPoints(const double& tol = 1e-3);
	public:
		quad_hexahedron();
		template <typename CONTAINER>
		quad_hexahedron(const CONTAINER& l, const double& tol = 1e-3);
		quad_hexahedron(const std::initializer_list<vertex>&& l, const double& tol = 1e-3);
		quad_hexahedron(const quad_hexahedron& rhs, const double& tol = 1e-3); // copy ctor, need this for mPolygons, to be copied properly
		polyhedron* clone();
		
		double getVolume() const;
		bool isInside(const vertex& p1, const double& tol = 1e-3) const;
		bool isInsideOrOn(const vertex& p1, const double& tol = 1e-3) const;
	};
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/quad_hexahedron.cpp>

#endif // QUAD_HEXAHEDRON_HPP