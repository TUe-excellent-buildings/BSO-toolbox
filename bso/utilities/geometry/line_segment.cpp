#ifndef LINE_SEGMENT_CPP
#define LINE_SEGMENT_CPP

#include <ostream>
#include <sstream>
#include <initializer_list>
#include <iterator>
#include <algorithm>

namespace bso { namespace utilities { namespace geometry {
	
	template <class CONTAINER>
	void line_segment::initFromVertices(const CONTAINER& l)
	{
		if (std::distance(l.begin(), l.end()) != 2)
		{
			std::stringstream errorMessage;
			errorMessage << "Error when initializing line segment from list.\n"
									 << "Expected two vertices, received: " << l.size()
									 << "\n(bso/utilities/geometry/line_segment.cpp)";
			
			throw std::invalid_argument(errorMessage.str());
		}
		std::copy(l.begin(), l.end(), mVertices);
	} // initFromVertices
	
	line_segment::line_segment()
	{
		mVertices[0] = vertex();
		mVertices[1] = vertex();
	} // empty ctor
	
	line_segment::line_segment(const vertex& v1, const vertex& v2)
	{
		mVertices[0] = v1;
		mVertices[1] = v2;
	} // ctor with two vertices
	
	line_segment::line_segment(const std::initializer_list<vertex>&& l)
	{
		this->initFromVertices(l);
	} // ctor with initializer list
	
	template <typename CONTAINER>
	line_segment::line_segment(const CONTAINER& l)
	{
		this->initFromVertices(l);
	} // ctor from container
	
	auto line_segment::begin()
	{
		return std::begin(mVertices);
	} // iterator begin()
	
	auto line_segment::end()
	{
		return std::end(mVertices);
	} // iterator end()
	
	auto line_segment::begin() const
	{
		return std::begin(mVertices);
	} // const iterator begin()
	
	auto line_segment::end() const
	{
		return std::end(mVertices);
	} // const iterator end()
	
	bool line_segment::isSameAs(const line_segment& rhs, const double& tol /*= 1e-3*/) const
	{
		if ((mVertices[0].isSameAs(rhs[0],tol) && mVertices[1].isSameAs(rhs[1],tol)) ||
				(mVertices[0].isSameAs(rhs[1],tol) && mVertices[1].isSameAs(rhs[0],tol))) return true;
		else return false;
	} // isSameAs()
	
	bool line_segment::isOnLine(const vertex& c, const double& tol /*= 1e-3*/) const
	{
		vector vAB = mVertices[1] - mVertices[0]; 
		vector vAC = c - mVertices[0];
		double dist = vAB.dot(vAC)/vAB.dot(vAB);
		if (!vAB.isParallel(vAC,tol)) return false;
		if (dist > tol && dist < 1 - tol) return true;
		else return false;
	} // conincidesWith()
	
	bool line_segment::intersectsWith(const line_segment& rhs, vertex& pInt, const double& tol /*= 1e-3*/) const
	{
		vector v1 = mVertices[1] - mVertices[0];
		vector v2 = rhs[1] - rhs[0];
		vector v3 = rhs[0] - mVertices[0];

		int dec = (int)(-log10(tol));
		
		if (v1.isParallel(v2,tol)) return false;
		else if (!v3.isPerpendicular(v1.cross(v2), tol)) return false;
		else
		{
			double s = v2.cross(v3).norm() / v2.cross(v1).norm();
			if (!v2.cross(v3).isCodirectional(v2.cross(v1), tol)) s = -s;
			double t = v1.cross(-v3).norm() / v1.cross(v2).norm();
			if (!v1.cross(-v3).isCodirectional(v1.cross(v2),tol)) t = -t;
			
			if ((s > 0.0+tol && s < 1.0-tol && t > 0.0-tol && t < 1.0+tol) ||
					(s > 0.0-tol && s < 1.0+tol && t > 0.0+tol && t < 1.0-tol))
			{ // if the scalars are smaller than one, the intersection point lies on both lines
			pInt = (mVertices[0] + (s * v1));
			pInt.round(dec);
			return true;
			}
			else return false;
		}
	} // intersectsWith()
	
	bool line_segment::intersectsWith(const line_segment& rhs, const double& tol /*= 1e-3*/) const
	{
		vertex dummy;
		return this->intersectsWith(rhs, dummy, tol);
	} // intersectsWith
	
	vertex line_segment::getPointClosestTo(const vertex& p) const
	{ //
		vector v1 = mVertices[1] - mVertices[0];
		vector v2 = p - mVertices[0];
		double t = v1.dot(v2)/v1.dot(v1);
		return (mVertices[0] + t * v1);
	} // getPointClosestTo()
	
	vector line_segment::getVector(const bool& direction /*= 1*/) const
	{ // 1 (true) for positive, 0 (false) for negative
		if (direction) return vector(mVertices[1] - mVertices[0]);
		else  return vector(mVertices[0] - mVertices[1]);
	} //
	
	double line_segment::getLength() const
	{
		return getVector().norm();
	}
	
	vertex line_segment::getCenter() const
	{
		return (mVertices[0] + mVertices[1])/2.0;
	}
	
	vertex& line_segment::operator [](const unsigned int& index)
	{
		if (index > 1)
		{
			std::stringstream errorMessage;
			errorMessage << "Index \"" << index
									 << "\"exceeds line segment (with size 2).\n"
									 << "\(bso/utilities/geometry/line_segment.cpp)";
			
			throw std::invalid_argument(errorMessage.str());
		}
		return mVertices[index];
	} // operator [] by reference

	const vertex& line_segment::operator [](const unsigned int& index) const
	{
		if (index > 1)
		{
			std::stringstream errorMessage;
			errorMessage << "Index \"" << index
									 << "\"exceeds line segment (with size 2).\n"
									 << "\(bso/utilities/geometry/line_segment.cpp)";
			
			throw std::invalid_argument(errorMessage.str());
		}
		return mVertices[index];
	} // operator [] by value (when the line segment is of type const)
	
	bool line_segment::operator == (const line_segment& rhs) const
	{
		return this->isSameAs(rhs,0);
	}
	
	std::ostream& operator <<(std::ostream& stream, const line_segment& l)
	{
		Eigen::IOFormat layout(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",", "{", "}", "", "");
		stream << "{" << l[0].transpose().format(layout) << "," << l[1].transpose().format(layout) << "}";
		return stream;
	}

} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // LINE_SEGMENT_CPP