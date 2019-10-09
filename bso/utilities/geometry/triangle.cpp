#ifndef TRIANGLE_CPP
#define TRIANGLE_CPP

namespace bso { namespace utilities { namespace geometry {
	
	void triangle::sortPoints(const double& tol /*= 1e-3*/)
	{
		if (mVertices.size() != 3)
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot sort triangle:\n"
									 << "received " << mSize << "vertices, expected 3.\n"
									 << "\n(bso/utilities/geometry/triangle.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		// nothing to be sorted any permutation is ok
		polygon::sortPoints(tol);
		
	} // sortPoints()
	
	triangle::triangle() : polygon()
	{

	} // empty ctor()
	
	template <class CONTAINER>
	triangle::triangle(const CONTAINER& l, const double& tol/*= 1e-3*/) : polygon(l, tol)
	{
		try 
		{
			this->sortPoints(tol);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Could not sort points when initializing triangle from container.\n"
									 << "Received the following error: \n"
									 << e.what()
									 << "\n(bso/utilities/geometry/triangle.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // 
	
	triangle::triangle(const std::initializer_list<vertex>&& l, const double& tol /*= 1e-3*/) : polygon(std::move(l), tol)
	{
		try 
		{
			this->sortPoints(tol);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Could not sort points when initializing triangle from initializer list.\n"
									 << "Received the following error: \n"
									 << e.what()
									 << "\n(bso/utilities/geometry/triangle.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // 
	
	polygon* triangle::clone()
	{
		return new triangle(*this);
	} // virtual copy constructor

	double triangle::getArea() const
	{
		vector v1 = mLineSegments[0].getVector();
		vector v2 = mLineSegments[1].getVector();
		vector v3 = v1.cross(v2)/2;
		return v3.norm();
	} // getArea()

	bool triangle::isInside(const vertex& p1, const double& tol /*= 1e-3*/) const
	{ // solution by Michael Ulm on: https://math.stackexchange.com/questions/4322/check-whether-a-point-is-within-a-3d-triangle
		if (!this->isCoplanar(p1,tol)) return false;
		for (const auto& i : mVertices) if (i.isSameAs(p1,tol)) return false;
		for (const auto& i : mLineSegments) if (i.isOnLine(p1,tol)) return false;
		
		Eigen::MatrixXd A(3,2);
		A.col(0) = mVertices[1] - mVertices[0];
		A.col(1) = mVertices[2] - mVertices[0];
		Eigen::Vector3d B = p1 - mVertices[0];
		// solve overdetermined system with normal equations (AT*A = AT*bref: Eigen manual)
		Eigen::Vector2d C;
		C << (A.transpose() * A).ldlt().solve(A.transpose() * B);
		
		if (C.sum() > 1) return false;
		if (C[0] < 0) return false;
		if (C[1] < 0) return false;
		return true;
	} // isInside()
	
	bool triangle::isInsideOrOn(const vertex& p1, const double& tol /*= 1e-3*/) const
	{ // solution by Michael Ulm on: https://math.stackexchange.com/questions/4322/check-whether-a-point-is-within-a-3d-triangle
		if (!this->isCoplanar(p1,tol)) return false;
		for (const auto& i : mVertices) if (i.isSameAs(p1,tol)) return true; // its on
		for (const auto& i : mLineSegments) if (i.isOnLine(p1,tol)) return true; // it on
		
		return isInside(p1,tol); // check if it is ins
	} // isInsideOrOn()
	
} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // TRIANGLE_CPP