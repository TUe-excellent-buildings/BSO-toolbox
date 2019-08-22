#ifndef CF_BUILDING_MODEL_CPP
#define CF_BUILDING_MODEL_CPP

#include <bitset>

namespace bso { namespace spatial_design { namespace conformal {
	
	void cf_building_model::addSpace(const ms_space& msSpace)
	{ // 
		std::vector<utilities::geometry::vertex> cornerPoints;
		cornerPoints.reserve(8);
		auto coords = msSpace.getCoordinates();
		auto dims   = msSpace.getDimensions();
		
		for (unsigned int i = 0; i < 8; ++i)
		{
			auto tempPoint = coords;
			for (unsigned int j = 0; j < 3; j++)
			{
				if (std::bitset<3>(i)[j]) tempPoint[j] += dims[j];
			}
			cornerPoints.push_back(tempPoint);
		}
		
		mCFSpaces.push_back(new cf_space(cornerPoints, this));
		mCFSpaces.back()->setSpaceID(msSpace.getID());
		std::string possibleSpaceType;
		if (msSpace.getSpaceType(possibleSpaceType))
		{
			mCFSpaces.back()->setSpaceType(possibleSpaceType);
		}

		auto spPtr = mCFSpaces.back();
		for (auto i = spPtr->begin(); i != spPtr->end(); ++i)
		{
			mCFPoints.push_back(new cf_point(*i, this));
			mCFPoints.back()->addSpace(spPtr);
			spPtr->addPoint(mCFPoints.back());
		}
		
		for (auto i = spPtr->lineBegin(); i != spPtr->lineEnd(); ++i)
		{
			mCFEdges.push_back(new cf_edge(*i, this));
			mCFEdges.back()->addSpace(spPtr);
			spPtr->addEdge(mCFEdges.back());
		}
		
		std::vector<std::string> possibleSurfaceTypes;
		bool surfaceTypesAvailable = msSpace.getSurfaceTypes(possibleSurfaceTypes);
		if (surfaceTypesAvailable)
		{
			std::swap(possibleSurfaceTypes[0],possibleSurfaceTypes[2]);
			std::swap(possibleSurfaceTypes[1],possibleSurfaceTypes[3]);
			std::swap(possibleSurfaceTypes[4],possibleSurfaceTypes[5]);
		}
		auto typeIte = possibleSurfaceTypes.begin();
		for (auto i = spPtr->polygonBegin(); i != spPtr->polygonEnd(); ++i)
		{
			mCFSurfaces.push_back(new cf_surface(**i, this));
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

	cf_building_model::cf_building_model(const ms_building& msModel, const double& tol /*= 1e-3*/)
	{ // 
		mTol = tol;
		for (const auto& i : msModel)
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