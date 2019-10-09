#ifndef QUADRILATERAL_HPP
#define QUADRILATERAL_HPP

#include <bso/utilities/geometry/polygon.hpp>

namespace bso { namespace utilities { namespace geometry {
	
	class quadrilateral : public polygon
	{
	protected:
		void sortPoints(const double& tol = 1e-3);
	public:
		quadrilateral();
		template <class CONTAINER>
		quadrilateral(const CONTAINER& l, const double& tol = 1e-3);
		quadrilateral(const std::initializer_list<vertex>&& l, const double& tol = 1e-3);
		polygon* clone();
		
		double getArea() const;
		bool isInside(const vertex& p1, const double& tol = 1e-3) const;
		bool isInsideOrOn(const vertex& p1, const double& tol = 1e-3) const;
	};
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/quadrilateral.cpp>

#endif // QUADRILATERAL_HPP