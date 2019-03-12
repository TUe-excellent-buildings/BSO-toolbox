#ifndef CF_SURFACE_CPP
#define CF_SURFACE_CPP

namespace bso { namespace spatial_design { namespace conformal {
	
	cf_surface::cf_surface(const utilities::geometry::quadrilateral& rhs, cf_building_model* buildingModel)
	: utilities::geometry::quadrilateral(rhs, buildingModel->tolerance())
	{ // 
		mBuildingModel = buildingModel;
		mCFRectangles.push_back(mBuildingModel->addRectangle(*this));
		mCFRectangles.back()->addSurface(this);
	} // 

	void cf_surface::checkVertex(cf_vertex* pPtr)
	{ //
		// check if the vertex is on this surface's geometry
		bool onGeometry = false;
		if ((this->isInside(*pPtr, mBuildingModel->tolerance()))) onGeometry = true;
		for (const auto& i : mLineSegments)
		{
			if (onGeometry) break;
			if (i.isOnLine(*pPtr, mBuildingModel->tolerance()))
			{
				onGeometry = true;
				break;
			}
		}
		if (!onGeometry) return;  // if it isn't return the function immediately
	
		// check each rectangle if it can be split by the vertex
		for (unsigned int i = 0; i < mCFRectangles.size(); ++i)
		{
			mCFRectangles[i]->split(pPtr);
		}
	} // 
	
} // conformal
} // spatial_design
} // bso

#endif // CF_SURFACE_CPP