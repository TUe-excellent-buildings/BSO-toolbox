#ifndef SD_LINE_SEGMENT_CPP
#define SD_LINE_SEGMENT_CPP

#include <bso/structural_design/component/derived_ptr_to_vertex.hpp>

#include <sstream>
#include <stdexcept>

namespace bso { namespace structural_design { namespace component {
	
	template <typename LINE_SEGMENT_INITIALIZER>
	line_segment::line_segment(const LINE_SEGMENT_INITIALIZER& l)
	:	bso::utilities::geometry::line_segment(l),
		geometry()
	{
		mIsLineSegment = true;
	} // 
	
	line_segment::line_segment(std::initializer_list<bso::utilities::geometry::vertex>&& l)
	:	bso::utilities::geometry::line_segment(std::move(l)),
		geometry()
	{
		mIsLineSegment = true;
	} // 

	line_segment::~line_segment()
	{
		
	} // 

	void line_segment::addStructure(const structure& s)
	{
		if (s.type() == "beam" || s.type() == "truss")
		{
			geometry::addStructure(s);
		}
		else 
		{
			std::stringstream errorMessage;
			errorMessage << "\nError when assigning structure to line segment geometry.\n"
									 << "Cannot assign structure with type: \"" << s.type() << "\"" 
									 << "to a line segment geometry\n"
									 << "(bso/structural_design/line_segment.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // 

	void line_segment::mesh(const unsigned int& n, std::vector<point*>& pointStore)
	{
		bso::utilities::geometry::vector dirVector = mVertices[1] - mVertices[0];
		
		mMeshedPoints.clear();
		mMeshedPoints.resize(n+1);

		bso::utilities::geometry::vertex meshPoint;
		unsigned long highestID = 0;
		bool pointFound;
		for (unsigned int i = 0; i < (n + 1); ++i)
		{
			meshPoint = mVertices[0] + (dirVector * ((double)i/((double)n)));
			pointFound = false;
			for (const auto& j : pointStore)
			{
				if (j->isSameAs(meshPoint))
				{
					mMeshedPoints[i] = j;
					pointFound = true;
					break;
				}
				if (highestID < j->getID()) highestID = j->getID();
			}
			if (!pointFound)
			{
				pointStore.push_back(new point(highestID++,meshPoint));
				mMeshedPoints[i] = pointStore.back();
			}
		}

		// pair the points that define an element together
		mElementPoints.clear();
		mElementPoints.resize(n);
		for (unsigned int i = 0; i < n; ++i)
		{
			mElementPoints[i] = {
				mMeshedPoints[i],
				mMeshedPoints[i + 1]
			};

		}

		// assign the constraints to all the points
		for (auto& i : mConstraints)
		{
			for (auto& j : mMeshedPoints)
			{
				j->addConstraint(i);
			}
		}

		// assign the loads to all the points
		double elementLength = this->getLength() / (double)n;
		for (auto& i : mLoads)
		{
			load nodalElementLoad = i * (elementLength / 2.0);
			for (auto& j : mElementPoints)
			{
				for (auto& k : j)
				{
					k->addLoad(nodalElementLoad);
				}
			}
		}

	} // 

	
} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_LINE_SEGMENT_CPP