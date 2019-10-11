#ifndef TETRAHEDRON_CPP
#define TETRAHEDRON_CPP

namespace bso { namespace utilities { namespace geometry {
	
	void tetrahedron::sortPoints(const double& tol /*= 1e-3*/)
	{ //
		if (mVertices.size() != 4)
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot sort tetrahedron:\n"
									 << "received " << mSize << "vertices, expected 4.\n"
									 << "\n(bso/utilities/geometry/tetrahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		// any permutation is ok for the vertices, line segments, and polygons
		mLineSegments.clear();
		mSizeLines = 6;
		mLineSegments.reserve(mSizeLines);
		for (unsigned int i = 0; i < mSize; ++i)
		{
			for (unsigned int j = i+1; j < mSize; ++j)
			{
				mLineSegments.push_back(line_segment({mVertices[i],mVertices[j]}));
			}
		}
		
		mPolygons.clear();
		mSizePolygons = 4;
		mPolygons.reserve(mSizeLines);
		for (unsigned int i = 0; i < mSize; ++i)
		{
			for (unsigned int j = i+1; j < mSize; ++j)
			{
				for (unsigned int k = j+1; k < mSize; ++k)
				{
					mPolygons.push_back(new triangle({mVertices[i],mVertices[j],mVertices[k]},tol));
				}
			}
		}
	} // sortPoints()

	tetrahedron::tetrahedron() : polyhedron()
	{ //
		
	} // empty ctor()

	template <typename CONTAINER>
	tetrahedron::tetrahedron(const CONTAINER& l, const double& tol /*= 1e-3*/) : polyhedron(l,tol)
	{ //
		try 
		{
			this->sortPoints(tol);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Could not sort points when initializing tetrahedron from container.\n"
									 << "Received the following error: \n"
									 << e.what()
									 << "\n(bso/utilities/geometry/tetrahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // ctor with an arbitrary list of vertices

	tetrahedron::tetrahedron(const std::initializer_list<vertex>&& l, const double& tol /*= 1e-3*/) : polyhedron(std::move(l),tol)
	{ //
		try 
		{
			this->sortPoints(tol);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Could not sort points when initializing tetrahedron from initializer list.\n"
									 << "Received the following error: \n"
									 << e.what()
									 << "\n(bso/utilities/geometry/tetrahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // ctor with an initializer list of vertices
	
	tetrahedron::tetrahedron(const tetrahedron& rhs, const double& tol /*= 1e-3*/) : polyhedron(rhs) // use default of parent class
	{ // copy ctor, need this for mPolygons, to be copied properly
		this->sortPoints(tol); // sort it without the try-catch structure, as it is initialized from a proper tetrahedron already
		// mPolygons is reinitialized in the sort points
	} // ocpy ctor
	
	polyhedron* tetrahedron::clone()
	{
		return new tetrahedron(*this);
	} // virtual copy constructor

	double tetrahedron::getVolume() const
	{ //
		Eigen::Matrix3d tempMat;
		tempMat << mVertices[1] - mVertices[0],
							 mVertices[2] - mVertices[0],
							 mVertices[3] - mVertices[0];
		return abs((tempMat.determinant()/6.0));
	} // getVolume()

	bool tetrahedron::isInside(const vertex& p1, const double& tol /*= 1e-3*/) const
	{ //
		// algorithm from: http://steve.hollasch.net/cgindex/geometry/ptintet.html
		Eigen::MatrixXd tempMat = Eigen::MatrixXd::Ones(4,4);
		std::vector<double> determinants;
		
		for (unsigned int i = 0; i < 4; i++) tempMat.block<1,3>(i,1) = mVertices[i].transpose();
		determinants.push_back(tempMat.determinant());
		
		for (unsigned int i = 0; i < 4; i++)
		{
			auto checkMat = tempMat;
			checkMat.block<1,3>(i,1) = p1.transpose();
			determinants.push_back(checkMat.determinant());	
		}

		int sameSignCount = 0;
		for (unsigned int i = 1; i < 5; i++)
		{
			if (abs(determinants[i]) < tol) // p is on a surface of the tet
			{
				return false;
			}
			if ((determinants[0] >= 0.0) ^ (determinants[i] < 0.0)) sameSignCount++;
		}
		if (sameSignCount != 4) // point lies outside of the tetrahedron
		{
			return false;
		}
		return true;
	} // isInside()
	
	bool tetrahedron::isInsideOrOn(const vertex& p1, const double& tol /*= 1e-3*/) const
	{ //
		// algorithm from: http://steve.hollasch.net/cgindex/geometry/ptintet.html
		Eigen::MatrixXd tempMat = Eigen::MatrixXd::Ones(4,4);
		std::vector<double> determinants;
		
		for (unsigned int i = 0; i < 4; i++) tempMat.block<1,3>(i,1) = mVertices[i].transpose();
		determinants.push_back(tempMat.determinant());
		
		for (unsigned int i = 0; i < 4; i++)
		{
			auto checkMat = tempMat;
			checkMat.block<1,3>(i,1) = p1.transpose();
			determinants.push_back(checkMat.determinant());	
		}

		int sameSignCount = 0;
		for (unsigned int i = 1; i < 5; i++)
		{
			if ((determinants[0] < 0)? (determinants[i] < tol) : (determinants[i] > -tol) ) sameSignCount++;
		}
		
		if (sameSignCount != 4) // point lies outside of the tetrahedron
		{
			return false;
		}
		return true;
	} // isInside()


} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // TETRAHEDRON_CPP