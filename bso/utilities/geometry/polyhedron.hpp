#ifndef POLYHEDRON_HPP
#define POLYHEDRON_HPP

#include <bso/utilities/geometry/vertex.hpp>
#include <bso/utilities/geometry/vector.hpp>
#include <bso/utilities/geometry/line_segment.hpp>
#include <bso/utilities/geometry/polygon.hpp>

#include <ostream>
#include <initializer_list>
#include <vector>

namespace bso { namespace utilities { namespace geometry {
	
	class polyhedron
	{
	private:
		template <class CONTAINER>
		void initFromVertices(const CONTAINER& l, const double& tol = 1e-3);		
		
	protected:
		std::vector<vertex> mVertices;
		std::vector<line_segment> mLineSegments;
		std::vector<polygon*> mPolygons; // must be pointer because it is of abstract type
		unsigned int mSize; // number of vertices
		unsigned int mSizeLines; // number of lines
		unsigned int mSizePolygons;
		
		vertex mCenter;
		virtual void sortPoints(const double& tol = 1e-3) = 0;
	public:
		polyhedron();
		template <typename CONTAINER>
		polyhedron(const CONTAINER& l, const double& tol = 1e-3);
		polyhedron(const std::initializer_list<vertex>&& l, const double& tol = 1e-3);
		virtual ~polyhedron();
		virtual polyhedron* clone() = 0;
		
		virtual double getVolume() const = 0;
		virtual double getSurfaceArea() const;
		virtual bool isInside(const vertex& p1, const double& tol = 1e-3) const = 0;
		virtual bool isInsideOrOn(const vertex& p1, const double& tol = 1e-3) const = 0;
		
		bool isSameAs(const polyhedron& pol, const double& tol = 1e-3) const;
		
		vertex getCenter() const;
		const vertex& center() const;
		
		// only allow const accessers because we dont allow geometric member variables to be changed, to prevent sortings to be distorted
		auto begin() const {return mVertices.begin();}
		auto end() const {return mVertices.end();}
			
		const auto& getVertices() const {return mVertices;}
		const auto& getLines() const {return mLineSegments;}
		const auto& getPolygons() const {return mPolygons;}
		
		const vertex& operator [] (const unsigned int& index) const;
		
		friend std::ostream& operator << (std::ostream& stream, const polyhedron& pol);
	};
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#include <bso/utilities/geometry/polyhedron.cpp>

#endif // POLYHEDRON_HPP