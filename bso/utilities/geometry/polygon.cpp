#ifndef POLYGON_CPP
#define POLYGON_CPP

#include <sstream>
#include <stdexcept>

namespace bso { namespace utilities {  namespace geometry {
	
	template <class CONTAINER>
	void polygon::initFromVertices(const CONTAINER& l, const double& tol /*= 1e-3*/)
	{
		mSize = std::distance(l.begin(), l.end());
		mVertices.clear();
		mVertices.reserve(mSize);
		mVertices.insert(mVertices.end(),std::begin(l),std::end(l));
		
		// remove non unique vertices
		for (unsigned int i = 0; i < mSize ; ++i)
		{
			for (unsigned int j = i+1; j < mSize; ++j)
			{
				if ((mVertices[i] - mVertices[j]).isZero(tol))
				{
					mSize--;
					mVertices.erase(mVertices.begin() + j);
				}
			}
		}
		
		if (mSize < 3)
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot initialize polygon with less than three unique vertices.\n"
									 << mSize << " vertices were provided.\n"
									 << "(bso/utilities/geometry/polygon.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		mVertices.clear();
		mVertices.reserve(mSize);
		mVertices.insert(mVertices.end(),std::begin(l),std::end(l));
		
		bool allVerticesOnOneLine = true;
		for (unsigned int i = 1; i < mSize-1; i++)
		{
			vector v1 = {mVertices[i]-mVertices[0]};
			vector v2 = {mVertices[i+1]-mVertices[0]};
			if (v1.isZero(tol) || v2.isZero(tol)) continue;
			if (!v1.isParallel(v2,tol))
			{
				allVerticesOnOneLine = false;
				mNormal = v1.cross(v2).normalized();
				break;
			}
		}
		
		if (allVerticesOnOneLine)
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot initialize polygon with vertices that are all on one line.\n"
									 << "The following verticeswere argument: \n";
			for (const auto& i : mVertices) errorMessage << i.transpose() << std::endl;
			errorMessage << "(bso/utilities/geometry/polygon.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		if (!this->isCoplanarItself())
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot initialize polygon with vertices that are out of plane.\n"
									 << "The following points were argument: \n";
			for (const auto& i : mVertices) errorMessage << i.transpose() << std::endl;
			errorMessage << "(bso/utilities/geometry/polygon.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		mCenter = {0,0,0};
		for (const auto& i : mVertices) mCenter += i;
		mCenter /= mSize;
	}
	
	bool polygon::isCoplanarItself(const double& tol /*= 1e-3*/) const
	{
		for (unsigned int i = 1; i < mSize; i++)
		{
			vector v = {mVertices[i] - mVertices[0]};
			if (!mNormal.isPerpendicular(v, tol))
			{
				return false;
			}
		}
		return true;;
	} // isCoplanarItself()
	
	void polygon::sortPoints(const double& tol /*= 1e-3*/)
	{
		mLineSegments.clear();
		mLineSegments.reserve(mSize);
		for (unsigned int i = 0; i < mSize; i++)
		{
			mLineSegments.push_back(line_segment(mVertices[i],mVertices[(i+1)%mSize]));
		}
	}
	
	polygon::polygon()
	{
		mSize = 0;
	} // empty ctor

	template <class CONTAINER>
	polygon::polygon(const CONTAINER& l, const double& tol /*= 1e-3*/)
	{
		this->initFromVertices(l,tol);
	}
	
	polygon::polygon(const std::initializer_list<vertex>&& l, const double& tol /*= 1e-3*/)
	{
		this->initFromVertices(l, tol);
	}
	
	polygon::~polygon()
	{
		
	} // dtor
	
	bool polygon::intersectsWith(const line_segment& l1, vertex& pInt, const double& tol /*= 1e-3*/) const
	{ 
		if (l1.getVector().isPerpendicular(mNormal, tol)) return false;
		vector v1 = l1[1]-l1[0];
		vector v2 = mVertices[0]-l1[0];
		
		//check if the line intersects with the plane
		double r = (mNormal.dot(v2))/(mNormal.dot(v1));
		if (r > -tol && r < (1 + tol))
		{ // if it does, compute the point
			pInt = l1[0] + r * v1;
		}
		else return false;
		
		// check if the point of intersection lies within the polygon
		if (this->isInside(pInt,tol)) return true;
		else return false;
	} // check if line segment l intersects with the surface inside the polygon
	
	bool polygon::intersectsWith(const line_segment& l1, const double& tol /*= 1e-3*/) const
	{
		vertex dummy;
		return this->intersectsWith(l1,dummy,tol);
	}
	
	vertex polygon::getPointClosestTo(const vertex& p) const
	{
		vector v = p - mVertices[0];
		return p - mNormal.normalized().dot(v)*mNormal.normalized();
	} // getPointClosestTo()
	
	bool polygon::isSameAs(const polygon& pol, const double& tol /*= 1e-3*/) const
	{ //
		if (mSize != pol.mSize) return false;
		for (const auto& i : mVertices)
		{
			bool vertexFound = false;
			for (const auto& j : pol)
			{
				if (j.isSameAs(i,tol))
				{
					vertexFound = true;
					break;
				}
			}
			if (!vertexFound) return false;
		}
		return true;
	} // isSameAs()

	// bool polygon::intersectsWith(const polygon& rhs, line_segment lInt, const double& tol /*= 1e-3*/) const
	// { // Tolga Birdal's answer on https://math.stackexchange.com/questions/475953/how-to-calculate-the-intersection-of-two-planes
		// if (this->isParallel(rhs,tol)) return false;
		// /*find the direction of the line of intersection between the two planes*/
		// vector vInt = mNormal.cross(rhs.mNormal); 
		
		// /*find a point on the line of intersection between the two planes*/
		// Eigen::MatrixXd M(5,5);
		// M.block<3,3>(0,0) = 2*Eigen::MatrixXd::Identity(3,3);
		// M.block<3,1>(0,3) = mNormal;
		// M.block<3,1>(0,4) = rhs.mNormal;
		// M.block<1,3>(3,0) = mNormal.transpose();
		// M.block<1,3>(4,0) = rhs.mNormal.transpose();
		
		// Eigen::VectorXd b(5);
		// b << 2*(mVertices[0].transpose()), mVertices[0].dot(mNormal), rhs.mVertices[0].dot(rhs.mNormal);
		// vertex pInt = (M.inverse()*b).block<3,1>(0,0);
		
		// /*line of intersection is described by pInt and Vint*/
		// /*now find the vertices of intersection with any line segment or vertex of the polygon or rhs*/
		// std::vector<line_segment> allLineSegments;
		// allLineSegments.reserve(mLineSegments.size() + rhs.mLineSegments.size());
		// allLineSegments.insert(allLineSegments.end(),mLineSegments.begin(),mLineSegments.end());
		// allLineSegments.insert(allLineSegments.end(),rhs.mLineSegments.begin(),rhs.mLineSegments.end());
		
		// std::vector<vertex> intersectVertices;
		// for (const auto& i : allLineSegments)
		// {
			// vector v1 = i[1] - i[0];
			// vector v2 = i[0] - pInt;
			
			// if (v1.isParallel(vInt,tol)) continue;
			// else if (!v2.isPerpendicular(v1.cross(vInt), tol)) continue;
			// else
			// {
				// double s = vInt.cross(v2).norm() / vInt.cross(v1).norm();
				
				// if (s > -tol && s < 1 + tol)
				// {
					// intersectVertices.push_back(vertex(i[0] + v1 * s));
					// intersectVertices.back().round((int)(-log10(tol)));
				// }
			// }
		// }
		// if (intersectVertices.size() < 2) return false;
		// else if (intersectVertices.size() > 2)
		// {
			// std::stringstream errorMessage;
			// errorMessage << "Error checking for line segment of intersection between two polygons.\n"
									 // << "Expected a convex polygon, but more than one\n"
									 // << "line segments of intersection were found.\n"
									 // << "(bso/utilities/geometry/polygon.cpp)." << std::endl;
			// throw std::invalid_argument(errorMessage.str());
		// }
		// else
		// {
			// lInt = line_segment(intersectVertices[0], intersectVertices[1]);
			// return true;
		// }
	// } // check if the surfaces inside this polygon and rhs intersect
	
	// bool polygon::intersectsWith(const polygon& rhs, const double& tol /*= 1e-3*/) const
	// {
		// line_segment dummy;
		// return this->intersectsWith(rhs,dummy,tol);
	// }

	bool polygon::isCoplanar(const vertex& p1, const double& tol /*= 1e-3*/) const
	{
		vector v1;
		if (p1.isSameAs(mVertices[0])) v1 = {p1-mVertices[1]};
		else v1 = {p1-mVertices[0]};
		return mNormal.isPerpendicular(v1, tol);
	}
	
	bool polygon::isParallel(const polygon& pol1, const double& tol /*= 1e-3*/) const
	{
		return mNormal.isParallel(pol1.normal(), tol);
	}
	
	bool polygon::isParallel(const vector& v1, const double& tol /*= 1e-3*/) const
	{
		return mNormal.isPerpendicular(v1);
	}

	bool polygon::sameSide(const vertex& p1, const vertex& p2, const double& tol /*= 1e-3*/) const
	{
		if (this->isCoplanar(p1,tol) || this->isCoplanar(p2,tol)) return false;
		double check1 = mNormal.dot(p1 - mVertices[0]); // fill in the point in the line equation: n<dot>(x-x0)
		double check2 = mNormal.dot(p2 - mVertices[0]);
		
		return (check1 >= 0) ^ (check2 < 0);
	}

	vector polygon::getNormal() const
	{
		return mNormal;
	}

	const vector& polygon::normal() const
	{
		return mNormal;
	}
	
	vertex polygon::getCenter() const
	{
		return mCenter;
	}
	
	const vertex& polygon::center() const
	{
		return mCenter;
	}

	const vertex& polygon::operator [] (const unsigned int& index) const
	{
		if (index >= mSize)
		{
			std::stringstream errorMessage;
			errorMessage << "Error, accessing polygon's vertex with an index that is out of bound.\n"
									 << "Requested index: " << index << " is out of bounds of size: " << mSize << std::endl
									 << "(bso/utilities/geometry/polygon.cpp)" << std::endl;			
			throw std::invalid_argument(errorMessage.str());
		}
		return mVertices[index];
	}
	
	std::ostream& operator <<(std::ostream& stream, const polygon& pol)
	{
		Eigen::IOFormat layout(Eigen::StreamPrecision, Eigen::DontAlignCols, ",", ",", "{", "}", "", "");
		bool first = true;
		stream << "{";
		for (const auto& i : pol.mVertices)
		{
			if (first)
			{
				first = false;
				stream << i.transpose().format(layout);
			}
			else stream << "," << i.transpose().format(layout);
		}
		stream << "}";
		return stream;
	}

} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // POLYGON_CPP