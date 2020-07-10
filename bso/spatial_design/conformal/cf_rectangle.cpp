#ifndef CF_RECTANGLE_CPP
#define CF_RECTANGLE_CPP

namespace bso { namespace spatial_design { namespace conformal {
	
	cf_rectangle::cf_rectangle(const utilities::geometry::quadrilateral& rhs, cf_geometry_model* geometryModel)
	: utilities::geometry::quadrilateral(rhs, geometryModel->tolerance())
	{
		mGeometryModel = geometryModel;
		for (const auto& i : mVertices)
		{
			mCFVertices.push_back(mGeometryModel->addVertex(i));
			mCFVertices.back()->addRectangle(this);
		}
		for (const auto& i : mLineSegments)
		{
			mCFLines.push_back(mGeometryModel->addLine(i));
			mCFLines.back()->addRectangle(this);
		}
	} // ctor
	
	cf_rectangle::~cf_rectangle()
	{
		for (const auto& i : mCFVertices) i->removeRectangle(this);
		for (const auto& i : mCFLines) i->removeRectangle(this);
	} // dtor
	
	void cf_rectangle::split(cf_vertex* pPtr)
	{ //
		for (auto& i : mCFVertices)
		{
			if (pPtr == i) return;
		}
		
		std::vector<cf_vertex*> newVertices;
		std::vector<cf_rectangle*> newRectangles;
		bool split = false;
		// check if the point is inside the rectangle
		if (this->isInside(*pPtr, mGeometryModel->tolerance()))
		{
			split = true;
			newVertices.push_back(pPtr);
			// get the four new vertices on each of the rectangles line segments
			for (const auto& i : mLineSegments)
			{ // for each 
				newVertices.push_back(mGeometryModel->addVertex(i.getPointClosestTo(*pPtr)));
				if (!(i.isOnLine(*(newVertices.back()), mGeometryModel->tolerance())))
				{
					std::stringstream errorMessage;
					errorMessage << "Error. When finding a point on a rectangle's\n"
											 << "line closest to a point inside the rectangle.\n"
											 << "Found a point that is not on that line.\n"
											 << "rectangle: " << *this << "\n"
											 << "point: " << *pPtr << "\n"
											 << "(bso/spatial_design/conformal/cf_rectangle.cpp)" << std::endl;
					throw std::runtime_error(errorMessage.str());
				}
			}
			
			for (unsigned int i = 0; i < 4; ++i)
			{ // create four new rectangles
				std::vector<utilities::geometry::vertex> cornerVertices;
				cornerVertices.push_back(*newVertices[0]);
				cornerVertices.push_back(*newVertices[(i)+1]);
				cornerVertices.push_back(*newVertices[(i+1)%4+1]);
				// find the vertex that both line segments have in common
				if (mLineSegments[i][0].isSameAs(mLineSegments[(i+1)%4][0], mGeometryModel->tolerance()) ||
						mLineSegments[i][0].isSameAs(mLineSegments[(i+1)%4][1], mGeometryModel->tolerance()))
				{
					cornerVertices.push_back(mLineSegments[i][0]);
				}
				else
				{
					cornerVertices.push_back(mLineSegments[i][1]);
				}
				newRectangles.push_back(mGeometryModel->addRectangle(
					utilities::geometry::quadrilateral(cornerVertices, mGeometryModel->tolerance())));				
			}
			
		}
		
		if (!split)
		{ // check if the point is on any of the rectangle's lines
	
			for (unsigned int i = 0; i < 4; ++i)
			{
				if (mLineSegments[i].isOnLine(*pPtr, mGeometryModel->tolerance()))
				{
					split = true;
					newVertices.push_back(pPtr);
					newVertices.push_back(mGeometryModel->addVertex(mLineSegments[(i+2)%4].getPointClosestTo(*pPtr)));
					if (!mLineSegments[(i+2)%4].isOnLine(*(newVertices.back()), mGeometryModel->tolerance()))
					{
						std::stringstream errorMessage;
						errorMessage << "\nError. When finding a point on a rectangle's\n"
												 << "opposite to a point intersecting with one of the rectangle's lines.\n"
												 << "Found a point that is not on that line.\n"
												 << "Rectangle:\n" << *this << "\n"
												 << "Line: " << mLineSegments[(i+2)%4] << "\n"
												 << "Split point: " << pPtr->transpose() << "\n"
												 << "Found point: " << newVertices.back()->transpose() << "\n"
												 << "(bso/spatial_design/conformal/cf_rectangle.cpp)" << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					newRectangles.push_back(mGeometryModel->addRectangle(utilities::geometry::quadrilateral({
						*newVertices[0],
						*newVertices[1],
						mLineSegments[(i+1)%4][0],
						mLineSegments[(i+1)%4][1]
					}, mGeometryModel->tolerance())));
					newRectangles.push_back(mGeometryModel->addRectangle(utilities::geometry::quadrilateral({
						*newVertices[0],
						*newVertices[1],
						mLineSegments[(i-1)%4][0],
						mLineSegments[(i-1)%4][1]
					}, mGeometryModel->tolerance())));
					break;
				}
			}
		}

		if (split)
		{
			mDeletion = true;
			for (auto& i : mCFSurfaces)
			{
				i->removeRectangle(this);
				for (auto& j : newRectangles)
				{
					i->addRectangle(j);
					j->addSurface(i);
				}
			}
			for (auto& i : mCFVertices) i->removeRectangle(this);
			for (auto& i : mCFLines) i->removeRectangle(this);
			
			for (const auto& i : newVertices) this->checkAssociated(i);	
			mCFSurfaces.clear();
		}
	} //
	
	void cf_rectangle::checkAssociated(cf_vertex* pPtr)
	{ //
		for (auto& i : mCFSurfaces)
		{
			i->checkVertex(pPtr);
			for (auto& j : i->cfEdges())
			{
				j->checkVertex(pPtr);
			}
			for (auto& j : i->cfSpaces())
			{
				j->checkVertex(pPtr);
			}
		}
	} //
	
} // conformal
} // spatial_design
} // bso

#endif // CF_RECTANGLE_CPP