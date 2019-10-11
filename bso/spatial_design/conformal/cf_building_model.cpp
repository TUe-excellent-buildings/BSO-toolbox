#ifndef CF_BUILDING_MODEL_CPP
#define CF_BUILDING_MODEL_CPP

#include <bitset>

namespace bso { namespace spatial_design { namespace conformal {
	
	void cf_building_model::addSpace(const ms_space& msSpace)
	{ // 
		mCFSpaces.push_back(new cf_space(msSpace.getGeometry(), this));
		mCFSpaces.back()->setSpaceID(msSpace.getID());
		std::string possibleSpaceType;
		if (msSpace.getSpaceType(possibleSpaceType))
		{
			mCFSpaces.back()->setSpaceType(possibleSpaceType);
		}

		auto spPtr = mCFSpaces.back();
		for (const auto& i : *spPtr)
		{
			mCFPoints.push_back(new cf_point(i, this));
			mCFPoints.back()->addSpace(spPtr);
			spPtr->addPoint(mCFPoints.back());
		}
		
		for (const auto& i : spPtr->getLines())
		{
			mCFEdges.push_back(new cf_edge(i, this));
			mCFEdges.back()->addSpace(spPtr);
			spPtr->addEdge(mCFEdges.back());
		}
		
		std::vector<std::string> possibleSurfaceTypes;
		bool surfaceTypesAvailable = msSpace.getSurfaceTypes(possibleSurfaceTypes);
		if (surfaceTypesAvailable)
		{
			std::swap(possibleSurfaceTypes[0],possibleSurfaceTypes[2]);
			std::swap(possibleSurfaceTypes[4],possibleSurfaceTypes[5]);
		}
		auto typeIte = possibleSurfaceTypes.begin();
		for (const auto& i : spPtr->getPolygons())
		{
			mCFSurfaces.push_back(new cf_surface(*i, this));
			mCFSurfaces.back()->addSpace(spPtr);
			spPtr->addSurface(mCFSurfaces.back());
			if (surfaceTypesAvailable)
			{
				mCFSurfaces.back()->setSurfaceType(*typeIte);
				++typeIte;
			}
		}
	} // 
	
	void cf_building_model::makeConformal()
	{ // 
		// check for line line intersections, and add the found vertex to the geometry model
		utilities::geometry::vertex pIntersection;
		for (unsigned int i = 0; i < mCFLines.size(); ++i)
		{
			for (unsigned int j = i+1; j < mCFLines.size(); ++j)
			{
				if (mCFLines[i]->intersectsWith(*(mCFLines[j]), pIntersection, mTol))
				{
					this->addVertex(pIntersection);
				}
			}
		}

		// check for line - rectangle intersections, and add the found vertex to the geometry model
		for (unsigned int i = 0; i < mCFRectangles.size(); ++i)
		{
			for (unsigned int j = 0; j < mCFLines.size(); ++j)
			{
				if (mCFRectangles[i]->intersectsWith(*(mCFLines[j]), pIntersection, mTol))
				{
					this->addVertex(pIntersection);
				}
			}
		}

		// check each vertex if it intersects with any space
		unsigned int i = 0;
		while (i < mCFVertices.size())
		{
			for (auto& j : mCFSpaces)
			{
				j->checkVertex(mCFVertices[i]);
			}
			++i;
		}

		// delete the lines, rectangles, and cuboids that were tagged for deletion
		mCFLines.erase(std::remove_if(mCFLines.begin(), mCFLines.end(), [](const auto& i){ return i->deletion(); }), mCFLines.end());
		mCFRectangles.erase(std::remove_if(mCFRectangles.begin(), mCFRectangles.end(), [](const auto& i){ return i->deletion(); }), mCFRectangles.end());
		mCFCuboids.erase(std::remove_if(mCFCuboids.begin(), mCFCuboids.end(), [](const auto& i){ return i->deletion(); }), mCFCuboids.end());
			
	} //  makeConformal()
	
	cf_building_model::cf_building_model(const cf_building_model& rhs)
	{
		auto newPtr = new cf_building_model(rhs.mMSModel, rhs.mTol);
		*this = *newPtr;
	} // copy ctor()

	cf_building_model::cf_building_model(const ms_building& msModel, const double& tol /*= 1e-3*/)
	:	cf_geometry_model(tol), mMSModel(msModel), mTol(tol)
	{ // 
		for (const auto& i : mMSModel)
		{
			addSpace(*i);
		}
		this->makeConformal();
	} // 
	
	cf_building_model::~cf_building_model()
	{ // 
		for (auto& i : mCFSpaces) delete i;
		for (auto& i : mCFSurfaces) delete i;
		for (auto& i : mCFEdges) delete i;
		for (auto& i : mCFPoints) delete i;
	} // 
	
} // conformal
} // spatial_design
} // bso

#endif // CF_BUILDING_MODEL_CPP