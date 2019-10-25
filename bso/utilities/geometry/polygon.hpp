#ifndef POLYGON_HPP
#define POLYGON_HPP

#include <bso/utilities/geometry/line_segment.hpp>

#include <initializer_list>
#include <vector>

namespace bso { namespace utilities { namespace geometry {
	
	
	class polygon
	{
	private:
		template <class CONTAINER>
		void initFromVertices(const CONTAINER& l, const double& tol = 1e-3);		
		
		bool isCoplanarItself(const double& tol = 1e-3) const;
	protected:
		std::vector<vertex> mVertices;
		std::vector<line_segment> mLineSegments;
		unsigned int mSize; // number of vertices (and by definition line segments)
		
		vector mNormal;
		vertex mCenter;
		virtual void sortPoints(const double& tol = 1e-3);
	public:
		polygon();
		template <class CONTAINER>
		polygon(const CONTAINER& l, const double& tol = 1e-3);
		polygon(const std::initializer_list<vertex>&& l, const double& tol = 1e-3);
		virtual ~polygon();
		virtual polygon* clone() = 0;
		
		virtual double getArea() const = 0;
		virtual bool isInside(const vertex& p1, const double& tol = 1e-3) const = 0;
		virtual bool isInsideOrOn(const vertex& p1, const double& tol = 1e-3) const = 0;
		
		bool isSameAs(const polygon& pol, const double& tol = 1e-3) const;
		bool intersectsWith(const line_segment& l1, vertex& pInt, const double& tol = 1e-3) const;
		bool intersectsWith(const line_segment& l1, const double& tol = 1e-3) const;
		vertex getPointClosestTo(const vertex& p) const;
		// not using this function because it does not generalize well for arbitrary polygons
		// bool intersectsWith(const polygon& rhs, line_segment lInt, const double& tol = 1e-3) const;
		// bool intersectsWith(const polygon& rhs, const double& tol = 1e-3) const;
		bool isCoplanar(const vertex& p1, const double& tol = 1e-3) const;
		bool isParallel(const polygon& pol1, const double& tol = 1e-3) const;
		bool isParallel(const vector& v1, const double& tol = 1e-3) const;
		bool sameSide(const vertex& p1, const vertex& p2, const double& tol = 1e-3) const;
		vector getNormal() const;
		const vector& normal() const;
		
		vertex getCenter() const;
		const vertex& center() const;
		
		// only allow const accessers because we dont allow geometric member variables to be changed, to prevent sortings to be distorted
		auto begin() const {return mVertices.begin();}
		auto end() const {return mVertices.end();}
		
		const auto& getLines() const {return mLineSegments;}

		const vertex& operator [] (const unsigned int& index) const;
		
		friend std::ostream& operator << (std::ostream& stream, const polygon& pol);
	};

} // namespace geometry
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/polygon.cpp>

#endif // POLYGON_HPP