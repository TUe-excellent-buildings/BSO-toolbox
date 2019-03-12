#ifndef CF_EDGE_CPP
#define CF_EDGE_CPP

namespace bso { namespace spatial_design { namespace conformal {
	
	cf_edge::cf_edge(const utilities::geometry::line_segment& rhs, cf_building_model* buildingModel)
	: utilities::geometry::line_segment(rhs)
	{ // 
		mBuildingModel = buildingModel;
		mCFLines.push_back(mBuildingModel->addLine(*this));
		mCFLines.back()->addEdge(this);
	} // 

	void cf_edge::checkVertex(cf_vertex* pPtr)
	{ // 
		// check if the vertex is in or on the edge's geometry
		if (!(this->isOnLine(*pPtr, mBuildingModel->tolerance()))) return; // if it isn't return the function immediately
	
		// check each line if it can be split by the vertex
		for (unsigned int i = 0; i < mCFLines.size(); ++i)
		{
			mCFLines[i]->split(pPtr);
		}
	} // 
	
} // conformal
} // spatial_design
} // bso

#endif // CF_EDGE_CPP