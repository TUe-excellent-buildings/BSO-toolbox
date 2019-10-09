#ifndef QUAD_HEXAHEDRON_CPP
#define QUAD_HEXAHEDRON_CPP

namespace bso { namespace utilities { namespace geometry {

	void quad_hexahedron::sortPoints(const double& tol /*= 1e-3*/)
	{ // 
		if (mSize != 8)
		{
			std::stringstream errorMessage;
			errorMessage << "Cannot sort quadrilaterl faced hexahedron:\n"
										<< "received " << mSize << "vertices, expected 8.\n"
									 << "(bso/utilities/geoemtry/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		quadrilateral current, opposing;
		bool opposingFound = false;
		bool currentFound = false;

		for (unsigned int i = 1; i < 6; ++i) {
			for (unsigned int j = i+1; j < 7; ++j) {
				for (unsigned int k = j+1; k < 8; ++k)
				{
					try
					{
						current = quadrilateral({mVertices[0],mVertices[i],mVertices[j],mVertices[k]},tol);
						currentFound = true;
					}
					catch (std::exception& e) { /* do nothing */}
					
					std::vector<vertex> tempOpposing;
					tempOpposing.reserve(4);
					for (unsigned int l = 1; l < 8; ++l)
					{
						if (l != 0 && l != i && l != j && l != k)
						{
							tempOpposing.push_back(mVertices[l]);
						}
					}
					
					try
					{
						opposing = quadrilateral(tempOpposing,tol);
						opposingFound = true;
					}
					catch(std::exception& e) { /* do nothing */ }
					bool intersection = false;
					if (opposingFound)
					{ // check if any of the line segments between the two quadrilaterals intersect
						for (const auto& l : current.getLines())
						{
							for (const auto& m : opposing.getLines())
							{
								if (l.intersectsWith(m, tol))
								{
									intersection = true;
									break;
								}
							}
							if (intersection) break;
						}
					}

					if (intersection) opposingFound = false;
					if (!opposingFound) currentFound = false;
					if (currentFound && opposingFound) break;
				}
				if (currentFound && opposingFound) break;
			}
			if (currentFound && opposingFound) break;
		}
		if (!currentFound && ! opposingFound)
		{
			std::stringstream errorMessage;
			errorMessage << "When initializing a quadrilateral faced hexahedron.\n"
									 << "Could not find two opposing quadrilaterals.\n"
									 << "The following vertices were argument:\n";
			for (const auto& i : mVertices) errorMessage << i.transpose() << std::endl;
			errorMessage << "Do the vertices define six planes?\n"
									 << "Do the vertices define six convex quadrilaterals?\n"
									 << "(bso/utilities/geoemtry/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		vector curVc0 = current[0] - current.getCenter();
		vector curVc1 = current[1] - current.getCenter();
		vertex rotationCheckVertex = (curVc1.cross(curVc0) * sqrt(current.getArea()) + current.getCenter());
		vertex quadHexCenter = {0,0,0};
		for (const auto& i : mVertices) quadHexCenter += i;
		quadHexCenter /= 8;
		if (current.sameSide(quadHexCenter,rotationCheckVertex))
		{
			current = quadrilateral({current[0],current[3],current[1],current[2]},tol);
		}
		
		unsigned int indexFive;
		bool indexFound = false;
		bool orderedCounterDirection = false;
		// find the indices of the opposing quadrilateral with which we can make a quadrilateral together with indices One and Two of the current quadrilateral
		for (unsigned int i = 0; i < 4; ++i)
		{
			try
			{
				quadrilateral quad1({current[0],current[1],opposing[i],opposing[(i+1)%4]},tol);
				quadrilateral quad2({current[2],current[3],opposing[(i+2)%4],opposing[(i+3)%4]},tol);

				bool intersection = false;
				for (const auto& j : quad1.getLines())
				{
					for (const auto& k : quad2.getLines())
					{
						if (j.intersectsWith(k,tol))
						{
							intersection = true;
							break;
						}
					}
					if (intersection) break;
				}
				if (intersection) continue;

				line_segment l1 = {current[0], opposing[i]};
				line_segment l2 = {current[1], opposing[(i+1)%4]};
				if (!l1.intersectsWith(l2,tol)) 
				{
					indexFive = i;
				}
				else
				{
					indexFive = (i+1)%4;
					orderedCounterDirection = true;
				}
				indexFound = true;
				break;
			}
			catch(std::exception& e) { /* do nothing */}
		}
		if (!indexFound)
		{
			std::stringstream errorMessage;
			errorMessage << "When initializing a quadrilateral faced hexahedron.\n"
									 << "Could not correspond the vertices of two opposing quadrilateral faces. \n"
									 << "The two faces at hand are \n" << current << "\nand\n" << opposing << std::endl
									 << "Do the vertices define six planes?\n"
									 << "Do the vertices define six convex quadrilaterals?\n"
									 << "indexFive: " << indexFive << " - " << orderedCounterDirection << std::endl
									 << "(bso/utilities/geoemtry/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		// now all points can be sorted into mVertices
		mVertices.clear();
		mVertices.reserve(8);
		for (const auto& i : current)
		{
			mVertices.push_back(i);
		}
		for (unsigned int i = 0; i < 4; i++)
		{
			if (!orderedCounterDirection) mVertices.push_back(opposing[(i+indexFive)%4]);
			else mVertices.push_back(opposing[(indexFive-i)%4]);
		}
		
		// now initialize all the edges of the quadrilateral faced hexahedron
		try
		{
			mLineSegments.clear();
			mLineSegments.reserve(12);
			mSizeLines = 12;
			for (unsigned int i = 0; i < 4; ++i)
				mLineSegments.push_back(line_segment({mVertices[i], mVertices[(i+1)%4]}));
			for (unsigned int i = 0; i < 4; ++i)
				mLineSegments.push_back(line_segment({mVertices[i], mVertices[i+4]}));
			for (unsigned int i = 0; i < 4; ++i)
				mLineSegments.push_back(line_segment({mVertices[i+4], mVertices[((i+1)%4)+4]}));
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "When initializing a quadrilateral faced hexahedron.\n"
									 << "Could not initialize the line segments that define the edges\n"
									 << "(bso/utilities/geoemtry/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}

		// now initialize all the quadrilaterals of the quadrilateral faced hexahedron
		try 
		{
			mPolygons.clear();
			mPolygons.reserve(6);
			mSizePolygons = 6;
			for (unsigned int i = 0; i < 4; ++i)
				mPolygons.push_back(new quadrilateral({mVertices[i],mVertices[(i+1)%4],mVertices[i+4],mVertices[((i+1)%4)+4]},tol));
			for (unsigned int i = 0; i < 2; ++i)
				mPolygons.push_back(new quadrilateral({mVertices[0+i*4],mVertices[1+i*4],mVertices[2+i*4],mVertices[3+i*4]},tol));
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "When initializing a quadrilateral faced hexahedron.\n"
									 << "Could not initialize the quadrilaterals that define the faces\n"
									 << "Received the following error:\n" << e.what() << "\n"
									 << "(bso/utilities/geoemtry/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}

		// finally decompose the quad faced hexahedron in tetrahedrons, for convenience
		mTetrahedrons.clear();
		try 
		{
			mTetrahedrons.push_back(tetrahedron({mVertices[0],mVertices[1],mVertices[3],mVertices[4]},tol));
			mTetrahedrons.push_back(tetrahedron({mVertices[1],mVertices[2],mVertices[3],mVertices[6]},tol));
			mTetrahedrons.push_back(tetrahedron({mVertices[4],mVertices[6],mVertices[5],mVertices[1]},tol));
			mTetrahedrons.push_back(tetrahedron({mVertices[4],mVertices[6],mVertices[7],mVertices[3]},tol));
			mTetrahedrons.push_back(tetrahedron({mVertices[4],mVertices[6],mVertices[3],mVertices[1]},tol));
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "When initializing a quadrilateral faced hexahedron.\n"
									 << "Could not decompose it into tetrahedrons\n"
									 << "Received the following error:\n" << e.what() << "\n"
									 << "(bso/utilities/geoemtry/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		// done!
	} // sortPoints()

	quad_hexahedron::quad_hexahedron() : polyhedron()
	{ // 
		
	} // empty ctor

	template <typename CONTAINER>
	quad_hexahedron::quad_hexahedron(const CONTAINER& l, const double& tol /*= 1e-3*/) : polyhedron(l, tol)
	{ // 
		try 
		{
			this->sortPoints(tol);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Could not sort points when initializing quadrilateral hexahedron from container.\n"
									 << "Received the following error: \n"
									 << e.what()
									 << "\n(bso/utilities/geometry/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} //

	quad_hexahedron::quad_hexahedron(const std::initializer_list<vertex>&& l, const double& tol /*= 1e-3*/) : polyhedron(std::move(l),tol)
	{ // 
		try 
		{
			this->sortPoints(tol);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Could not sort points when initializing quadrilateral hexahedron from initializer list.\n"
									 << "Received the following error: \n"
									 << e.what()
									 << "\n(bso/utilities/geometry/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} //

	quad_hexahedron::quad_hexahedron(const quad_hexahedron& rhs, const double& tol /*= 1e-3*/) : polyhedron(rhs,tol)
	{ // copy ctor, need this for mPolygons, to be copied properly
		this->sortPoints(); // without try-catch construction, since it is initailized from a valid quadrilateral faced hexahedron
	} //
	
	polyhedron* quad_hexahedron::clone()
	{
		return new quad_hexahedron(*this);
	} // virtual copy constructor
	
	double quad_hexahedron::getVolume() const
	{ // 
		double volume = 0;	
		for (const auto& i : mTetrahedrons) volume += i.getVolume();
		return volume;
	} //

	bool quad_hexahedron::isInside(const vertex& p1, const double& tol /*= 1e-3*/) const
	{ // 
		// check if it is inside a tetrahedron
		for (const auto& i : mTetrahedrons) if (i.isInside(p1, tol)) return true;
		
		// check if it is on the surface on a tetrahedron
		bool onTetSurface = false;
		for (const auto& i : mTetrahedrons) if (i.isInsideOrOn(p1, tol)) 
		{
			onTetSurface = true;
			break;
		}
		
		if (onTetSurface)
		{
			// check if it is on the surface of one of the quadrilaterals
			bool onQuadSurface = false;
			for (const auto& i : mPolygons)
			{
				if (i->isInsideOrOn(p1,tol))
				{
					onQuadSurface = true;
					break;
				}
			}
			
			if (onQuadSurface) return false;
			else return true;
		}
		else return false;
	} //

	bool quad_hexahedron::isInsideOrOn(const vertex& p1, const double& tol /*= 1e-3*/) const
	{ // 
		// check if it is inside a tetrahedron
		for (const auto& i : mTetrahedrons) if (i.isInsideOrOn(p1, tol)) return true;
		return false;
	} //

} // namespace geometry
} // namespace utilities
} // namespace bso

#endif // QUAD_HEXAHEDRON_CPP