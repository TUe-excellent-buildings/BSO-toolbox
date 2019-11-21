#ifndef QUADRILATERAL_CPP
#define QUADRILATERAL_CPP

#include <algorithm>

namespace bso { namespace utilities { namespace geometry {
	
	void quadrilateral::sortPoints(const double& tol /*= 1e-3*/)
	{ // 
		if (mSize != 4)
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot sort quadrilateral:\n"
									 << "received " << mSize << "vertices, expected 4.\n"
									 << "\n(bso/utilities/geometry/quadrilateral.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		line_segment l01 = {mVertices[1], mVertices[0]};
		line_segment l23 = {mVertices[3], mVertices[2]};
		line_segment l03 = {mVertices[3], mVertices[0]};
		line_segment l12 = {mVertices[2], mVertices[1]};
		
		if (l01.intersectsWith(l23,tol))
		{
			std::iter_swap(mVertices.begin() + 1, mVertices.begin() + 2);
		}
		else if (l03.intersectsWith(l12, tol))
		{
			std::iter_swap(mVertices.begin() + 2, mVertices.begin() + 3);
		}
		
		line_segment l02 = {mVertices[2], mVertices[0]};
		line_segment l13 = {mVertices[3], mVertices[1]};
		
		if (!l02.intersectsWith(l13, tol))
		{
			std::stringstream errorMessage;
			errorMessage << "When sorting the vertices of a quadrilateral,\n"
									 << "the quadrilateral is not convex.\n"
									 << "\n(bso/utilities/geometry/quadrilateral.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		polygon::sortPoints(tol);
	} //  sortPoints()

	quadrilateral::quadrilateral()
	{ // 
		
	} //  empty ctor

	template <class CONTAINER>
	quadrilateral::quadrilateral(const CONTAINER& l, const double& tol /*= 1e-3*/) : polygon(l,tol)
	{ // 
		try 
		{
			this->sortPoints(tol);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Could not sort points when initializing quadrilateral from container.\n"
									 << "Received the following error: \n"
									 << e.what()
									 << "\n(bso/utilities/geometry/quadrilateral.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} //  ctor with a container
	
	quadrilateral::quadrilateral(const std::initializer_list<vertex>&& l, const double& tol /*= 1e-3*/) : polygon(std::move(l),tol)
	{ // 
		try 
		{
			this->sortPoints(tol);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Could not sort points when initializing quadrilateral from initializer list.\n"
									 << "Received the following error: \n"
									 << e.what()
									 << "\n(bso/utilities/geometry/quadrilateral.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // ctor with initializer list
	
	polygon* quadrilateral::clone()
	{
		return new quadrilateral(*this);
	} // virtual copy constructor

	double quadrilateral::getArea() const
	{ // only holds for convex quadrilaterals
		vector v1 = mVertices[2] - mVertices[0];
		vector v2 = mVertices[3] - mVertices[1];
		return (v1.cross(v2)).norm()/2;
	} //  getArea()

	bool quadrilateral::isInside(const vertex& p1, const double& tol /*= 1e-3*/) const
	{ // only holds for convex quadrilaterals
		if (this->isInsideOrOn(p1,tol))
		{
			for (const auto& i : mLineSegments)
			{
				if (i.isOnLine(p1,tol)) return false;
			}
			for (const auto& i : mVertices)
			{
				if (i.isSameAs(p1,tol)) return false;
			}
			return true;
		}
		else return false;
	} //  isInside()

	bool quadrilateral::isInsideOrOn(const vertex& p1, const double& tol /*= 1e-3*/) const
	{ // only holds for convex quadrilaterals
		triangle t1 = {mVertices[0],mVertices[1],mVertices[2]};
		triangle t2 = {mVertices[0],mVertices[3],mVertices[2]};

		return (t1.isInsideOrOn(p1,tol) || t2.isInsideOrOn(p1,tol));
	} //  isInside()
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // QUADRILATERAL_CPP