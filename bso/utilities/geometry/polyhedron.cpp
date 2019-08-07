#ifndef POLYHEDRON_CPP
#define POLYHEDRON_CPP

namespace bso { namespace utilities { namespace geometry {
	
	template <class CONTAINER>
	void polyhedron::initFromVertices(const CONTAINER& l, const double& tol /*= 1e-3*/)
	{ //
		mSize = std::distance(std::begin(l), std::end(l));
		mSizeLines = 0;
		mSizePolygons = 0;
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
		
		if (mSize < 4)
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot initialize polyhedron with less than four unique vertices.\n"
									 << mSize << " vertices were provided.\n"
									 << "(bso/utilities/geometry/polyhedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		bool allVerticesOnOneplane = true;
		for (unsigned int i = 1; i < mSize-2; i++)
		{
			vector v1 = {mVertices[i]-mVertices[0]};
			vector v2 = {mVertices[i+1]-mVertices[0]};
			vector v3 = {mVertices[i+2]-mVertices[0]};
			if (v3.isZero(tol) || v2.isZero(tol) || v1.isZero(tol)) continue;
			vector v1x2 = v1.cross(v2);
			vector v1x3 = v1.cross(v3);
			if (!v1x2.isParallel(v1x3,tol))
			{
				allVerticesOnOneplane = false;
				break;
			}
		}
		
		if (allVerticesOnOneplane)
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot initialize polyhedron with vertices that are all on one plane.\n"
									 << "The following vertices were argument: \n";
			for (const auto& i : mVertices) errorMessage << i.transpose() << std::endl;
			errorMessage << "(bso/utilities/geometry/polyhedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		mCenter = {0,0,0};
		for (const auto& i : mVertices) mCenter += i;
		mCenter /= mSize;
	} // initFromVertices()

	polyhedron::polyhedron()
	{ //
		mSize = 0;
		mSizeLines = 0;
		mSizePolygons = 0;
	} // empty ctor

	template <typename CONTAINER>
	polyhedron::polyhedron(const CONTAINER& l, const double& tol /*= 1e-3*/)
	{ // this also works as a copy constructor via the .begin() and .end() functions
		this->initFromVertices(l, tol);
	} // ctor with arbitrary container of vertices

	polyhedron::polyhedron(const std::initializer_list<vertex>&& l, const double& tol /*= 1e-3*/)
	{ //
		this->initFromVertices(l, tol);
	} // ctor with initailizer list of vertices

	polyhedron::~polyhedron()
	{ //
		for (auto i : mPolygons) delete i;
	} // dtor

	double polyhedron::getSurfaceArea() const
	{ //
		double surfaceArea = 0;
		for (const auto& i: mPolygons)
		{
			surfaceArea += i->getArea();
		}
		return surfaceArea;
	} // getSurg

	vertex polyhedron::getCenter() const
	{ //
		return mCenter;
	} // getCenter()

	const vertex& polyhedron::center() const
	{ //
		return mCenter;
	} // center()
	
	bool polyhedron::isSameAs(const polyhedron& pol, const double& tol /*= 1e-3*/) const
	{
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
	} // isSameAs
	
	const vertex& polyhedron::operator [] (const unsigned int& index) const
	{
		return mVertices[index];
	}

	std::ostream& operator << (std::ostream& stream, const polyhedron& pol)
	{ //
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
	} // ofstream operator

} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // POLYHEDRON_CPP