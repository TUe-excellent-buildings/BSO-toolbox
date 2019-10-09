#ifndef TETRAHEDRON_HPP
#define TETRAHEDRON_HPP

#include <bso/utilities/geometry/polyhedron.hpp>

namespace bso { namespace utilities { namespace geometry {
	
	class tetrahedron : public polyhedron
	{
	protected:
		void sortPoints(const double& tol = 1e-3);
	public:
		tetrahedron();
		template <typename CONTAINER>
		tetrahedron(const CONTAINER& l, const double& tol = 1e-3);
		tetrahedron(const std::initializer_list<vertex>&& l, const double& tol = 1e-3);
		tetrahedron(const tetrahedron& rhs, const double& tol = 1e-3); // copy ctor, need this for mPolygons, to be copied properly
		polyhedron* clone();
		
		double getVolume() const;
		bool isInside(const vertex& p1, const double& tol = 1e-3) const;
		bool isInsideOrOn(const vertex& p1, const double& tol = 1e-3) const;
	};
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/tetrahedron.cpp>

#endif // TETRAHEDRON_HPP