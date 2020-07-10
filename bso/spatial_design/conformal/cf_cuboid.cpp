#ifndef CF_CUBOID_CPP
#define CF_CUBOID_CPP

namespace bso { namespace spatial_design { namespace conformal {
	
	cf_cuboid::cf_cuboid(const utilities::geometry::quad_hexahedron& rhs, cf_geometry_model* geometryModel)
	: utilities::geometry::quad_hexahedron(rhs, geometryModel->tolerance())
	{
		mGeometryModel = geometryModel;
		for (const auto& i : mVertices)
		{
			mCFVertices.push_back(mGeometryModel->addVertex(i));
			mCFVertices.back()->addCuboid(this);
		}
		for (const auto& i : mLineSegments)
		{
			mCFLines.push_back(mGeometryModel->addLine(i));
			mCFLines.back()->addCuboid(this);
		}
		for (const auto& i : mPolygons)
		{
			mCFRectangles.push_back(mGeometryModel->addRectangle(*i));
			mCFRectangles.back()->addCuboid(this);
		}
	} // ctor
	
	cf_cuboid::~cf_cuboid()
	{
		for (const auto& i : mCFVertices) i->removeCuboid(this);
		for (const auto& i : mCFLines) i->removeCuboid(this);
		for (const auto& i : mCFRectangles) i->removeCuboid(this);
	} // dtor
	
	void cf_cuboid::split(cf_vertex* pPtr)
	{ // 
		for (auto& i : mCFVertices)
		{
			if (pPtr == i) return;
		}

		std::vector<cf_vertex*> newVertices;
		std::vector<cf_cuboid*> newCuboids;
		bool split = false;
		
		std::vector<utilities::geometry::vector> normals; // the three normals of the cuboid
					
		for (auto& i : mPolygons)
		{
			bool normalFound = false;
			for (const auto& j : normals)
			{
				if (i->normal().isParallel(j, mGeometryModel->tolerance())) normalFound = true;
			}
			if (!normalFound) normals.push_back(i->normal().normalized());
			if (normals.size() == 3) break;
		}
		if (normals.size() != 3) 
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, expected to find 3 normals of a cuboid\n"
									 << "found: " << normals.size() << ".\n"
									 << "(bso/spatial_design/conformal/cf_cuboid.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		
		if (this->isInside(*pPtr, mGeometryModel->tolerance()))
		{
			split = true;
			newVertices.push_back(pPtr);
			
			for (const auto& i : mPolygons)
			{ // find the point closest on each surface
				newVertices.push_back(mGeometryModel->addVertex(i->getPointClosestTo(*pPtr)));
				if (!(i->isInside(*(newVertices.back()), mGeometryModel->tolerance())))
				{
					std::stringstream errorMessage;
					errorMessage << "\nError, found a point closest to a rectangle,\n"
											 << "but that point is not inside the rectangle.\n"
											 << "While splitting a cuboid at a point inside that cuboid.\n"
											 << "Rectangle: " << *i << "\n"
											 << "Split point: " << pPtr->transpose() << "\n"
											 << "Found point: " << newVertices.back()->transpose() << "\n"
											 << "(bso/spatial_design/conformal/cf_cuboid.cpp)" << std::endl;
					throw std::runtime_error(errorMessage.str());
				}
			}
			
			for (const auto& i : mLineSegments)
			{ // ifnd the point closest on each line
				newVertices.push_back(mGeometryModel->addVertex(i.getPointClosestTo(*pPtr)));
				if (!(i.isOnLine(*(newVertices.back()), mGeometryModel->tolerance())))
				{
					std::stringstream errorMessage;
					errorMessage << "\nError, found a point closest to a line segment,\n"
											 << "but that point is not on the line segment.\n"
											 << "While splitting a cuboid at a point inside that cuboid.\n"
											 << "Line: " << i  << "\n"
											 << "Split point: " << pPtr->transpose() << "\n"
											 << "Found point: " << newVertices.back()->transpose() << "\n"
											 << "(bso/spatial_design/conformal/cf_cuboid.cpp)" << std::endl;
					throw std::runtime_error(errorMessage.str());
				}
			}
			
			for (const auto& i: mVertices)
			{
				std::vector<utilities::geometry::vertex> cornerPoints;
				cornerPoints.push_back(*pPtr);
				cornerPoints.push_back(i);
				
				for (const auto& j : normals)
				{
					double projectedDistance = j.dot(cornerPoints[1] - cornerPoints[0]);
					cornerPoints.push_back(cornerPoints[0] + projectedDistance * j);
					cornerPoints.push_back(cornerPoints[1] - projectedDistance * j);
				}
					
				newCuboids.push_back(mGeometryModel->addCuboid(
					utilities::geometry::quad_hexahedron(cornerPoints, mGeometryModel->tolerance())));
			}
		}
		
		if (!split)
		{
			for (unsigned int i = 0; i < 6; ++i)
			{ // check if the point is on any of the rectangles of this cuboid
				
				if (mPolygons[i]->isInside(*pPtr, mGeometryModel->tolerance()))
				{
				 // if it is, lets split the cuboid into four new ones
					split = true;
					newVertices.push_back(pPtr);
					unsigned int opposite = 6;
					// first find the rectangle opposite to rectangle i
					for (unsigned int j = 0; j < 6; ++j)
					{
						if (j != i && mPolygons[i]->normal().isParallel(mPolygons[j]->normal(), mGeometryModel->tolerance()))
						{
							opposite = j;
							break;
						}
					}
					if (opposite == 6)
					{
						std::stringstream errorMessage;
						errorMessage << "\nError, could not find opposite surface when\n"
												 << "splitting a cuboid from a point on a surface\n"
												 << "(bso/spatial_design/conformal/cf_cuboid)." << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					newVertices.push_back(mGeometryModel->addVertex(mPolygons[opposite]->getPointClosestTo(*pPtr)));
					if (!(mPolygons[opposite]->isInside(*(newVertices.back()))))
					{
						std::stringstream errorMessage;
						errorMessage << "\nError, found a point closest to a surface,\n"
												 << "but that point is not inside the surface.\n"
												 << "When splitting cuboid at surface.\n"
												 << "Surface:\n" << *mPolygons[opposite] << "\n"
												 << "Split point: " << pPtr->transpose() << "\n"
												 << "Found point: " << (newVertices.back())->transpose() << "\n"
												 << "(bso/spatial_design/conformal/cf_cuboid)." << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					
					unsigned int splitSurfaces[2] = {i,opposite};
					
					// next find the vertices on each of the lines of the surface
					for (unsigned int j = 0; j < 2; ++j)
					{
						for (const auto& k : mPolygons[splitSurfaces[j]]->getLines())
						{
							newVertices.push_back(mGeometryModel->addVertex(k.getPointClosestTo(*(newVertices[j]))));
							if (!k.isOnLine(*(newVertices.back()), mGeometryModel->tolerance()))
							{
								std::stringstream errorMessage;
								errorMessage << "\nError, found a point closest to a line segment,\n"
														 << "but that point is not on the line segment.\n"
														 << "When splitting cuboid at surface.\n"
														 << "Surface:\n" << *mPolygons[splitSurfaces[j]] << "\n"
														 << "Line:\n" << k << "\n"
														 << "Split point: " << pPtr->transpose() << "\n"
														 << "Found point: " << newVertices.back()->transpose() << "\n"
														 << "(bso/spatial_design/conformal/cf_cuboid)." << std::endl;
							throw std::runtime_error(errorMessage.str());
							}
						}
					}
					
					for (const auto j : *(mPolygons[opposite]))
					{
						std::vector<utilities::geometry::vertex> cornerPoints;
						cornerPoints.push_back(*pPtr);
						cornerPoints.push_back(j);
						for (const auto& k : normals)
						{
							double projectedDistance = k.dot(cornerPoints[1] - cornerPoints[0]);
							cornerPoints.push_back(cornerPoints[0] + projectedDistance * k);
							cornerPoints.push_back(cornerPoints[1] - projectedDistance * k);
						}
						newCuboids.push_back(mGeometryModel->addCuboid(
							utilities::geometry::quad_hexahedron(cornerPoints, mGeometryModel->tolerance())));
					}
					break;
				}
			}
		}
		
		if (!split)
		{
			for (unsigned int i = 0; i < 12; ++i)
			{
				if (mLineSegments[i].isOnLine(*pPtr,mGeometryModel->tolerance()))
				{
					split = true;
					newVertices.push_back(pPtr);
					utilities::geometry::vector v1 = mLineSegments[i].getVector();
					
					for (unsigned int j = 0; j < 12; ++j)
					{
						if (j == i) continue;
						
						if (mLineSegments[j].getVector().isParallel(v1, mGeometryModel->tolerance()))
						{
							newVertices.push_back(mGeometryModel->addVertex(mLineSegments[j].getPointClosestTo(*pPtr)));
							if (!mLineSegments[j].isOnLine(*(newVertices.back()), mGeometryModel->tolerance()))
							{
								std::stringstream errorMessage;
								errorMessage << "\nError, found a point closest to a line segment,\n"
														 << "but that point is not on the line segment.\n"
														 << "When splitting cuboid at a line segment.\n"
														 << "Line segment: " << mLineSegments[j] << "\n"
														 << "Split point: " << pPtr->transpose() << "\n"
														 << "Found point: " << newVertices.back()->transpose() << "\n"
														 << "(bso/spatial_design/conformal/cf_cuboid)." << std::endl;
								throw std::runtime_error(errorMessage.str());
							}
						}
					}
					
					if (newVertices.size() != 4)
					{
						std::stringstream errorMessage;
						errorMessage << "\nError, expected to find 4 new vertices,\n"
												 << "when dividing a cuboid at a line segment.\n"
												 << "Found: " << newVertices.size() << "\n"
												 << "(bso/spatial_design/conformal/cf_cuboid)." << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					
					for (const auto& j : mPolygons)
					{
						if (v1.isParallel(j->normal(), mGeometryModel->tolerance()))
						{
							std::vector<utilities::geometry::vertex> cornerPoints;
							for (unsigned int k = 0; k < 4; ++k)
							{
								cornerPoints.push_back(*newVertices[k]);
							}
							cornerPoints.insert(cornerPoints.end(),j->begin(), j->end());
							
							newCuboids.push_back(mGeometryModel->addCuboid(
								utilities::geometry::quad_hexahedron(cornerPoints, mGeometryModel->tolerance())));
						}
					}
					if (newCuboids.size() != 2)
					{
						std::stringstream errorMessage;
						errorMessage << "\nError, expected to find 2 new cuboids,\n"
												 << "when dividing a cuboid at a line segment.\n"
												 << "Found: " << newCuboids.size() << "\n"
												 << "(bso/spatial_design/conformal/cf_cuboid)." << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					break;
				}
			}
		}

		if (split)
		{
			mDeletion = true;
			for (auto& i : mCFSpaces)
			{
				i->removeCuboid(this);
				for (auto& j : newCuboids)
				{
					i->addCuboid(j);
					j->addSpace(i);
				}
			}
			for (auto& i : mCFVertices) i->removeCuboid(this);
			for (auto& i : mCFLines) i->removeCuboid(this);
			for (auto& i : mCFRectangles) i->removeCuboid(this);
			
			for (const auto& i : newVertices) this->checkAssociated(i);
			mCFSpaces.clear();
		}
	} // 

	void cf_cuboid::checkAssociated(cf_vertex* pPtr)
	{ // 
		for (auto& i : mCFSpaces)
		{
			i->checkVertex(pPtr);
			for (auto& j : i->cfEdges())
			{
				j->checkVertex(pPtr);
			}
			for (auto& j : i->cfSurfaces())
			{
				j->checkVertex(pPtr);
			}
		}
	} // 

} // conformal
} // spatial_design
} // bso

#endif // CF_CUBOID_CPP