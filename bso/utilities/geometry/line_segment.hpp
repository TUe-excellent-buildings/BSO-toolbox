#ifndef LINE_SEGMENT_HPP
#define LINE_SEGMENT_HPP

#include <bso/utilities/geometry/vertex.hpp>
#include <bso/utilities/geometry/vector.hpp>

namespace bso { namespace utilities { namespace geometry {
	
	class line_segment
	{
	protected:
		template <class CONTAINER>
		void initFromVertices(const CONTAINER& l);
		vertex mVertices[2];
	public:
		line_segment();
		line_segment(const vertex& v1, const vertex& v2);
		template <typename CONTAINER>
		line_segment(const CONTAINER& l);
		line_segment(const std::initializer_list<vertex>&& l);
		
		auto begin();
		auto end();
		auto begin() const;
		auto end() const;
		
		bool isSameAs(const line_segment& rhs, const double& tol = 1e-3) const;
		bool isOnLine(const vertex& c, const double& tol = 1e-3) const;
		bool intersectsWith(const line_segment& rhs, const double& tol = 1e-3) const;
		bool intersectsWith(const line_segment& rhs, vertex& v, const double& tol = 1e-3) const;
		vertex getPointClosestTo(const vertex& p) const;
		vector getVector(const bool& direction = 1) const; // 1 (true) for positive, 0 (false) for negative
		double getLength() const;
		vertex getCenter() const;
		
    const vertex& operator [](const unsigned int& index) const;
		vertex& operator [](const unsigned int& index);
		bool operator == (const line_segment& rhs) const;

		friend std::ostream& operator << (std::ostream& stream, const line_segment& l);
	};
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/line_segment.cpp>

#endif // LINE_SEGMENT_HPP