#ifndef SD_QUADQUADRILATERAL_CPP
#define SD_QUADQUADRILATERAL_CPP

#include <bso/structural_design/component/derived_ptr_to_vertex.hpp>

#include <sstream>
#include <stdexcept>

namespace bso { namespace structural_design { namespace component {

	template <typename QUADRILATERAL_INITIALIZER>
	quadrilateral::quadrilateral(const QUADRILATERAL_INITIALIZER& l)
	:	bso::utilities::geometry::quadrilateral(l),
		geometry()
	{
		mIsQuadrilateral = true;
	} // ctor 
	
	quadrilateral::quadrilateral(std::initializer_list<bso::utilities::geometry::vertex>&& l)
	:	bso::utilities::geometry::quadrilateral(std::move(l)),
		geometry()
	{
		mIsQuadrilateral = true;
	} // ctor from initializer list

	quadrilateral::~quadrilateral()
	{
		
	} // dtor

	void quadrilateral::addStructure(const structure& s)
	{
		if (s.type() == "flat_shell")
		{
			geometry::addStructure(s);
		}
		else 
		{
			std::stringstream errorMessage;
			errorMessage << "\nError when assigning structure to quadrilateral geometry.\n"
									 << "Cannot assign structure with type: \"" << s.type() << "\"" 
									 << "to a quadrilateral geometry\n"
									 << "(bso/structural_design/component/quadrilateral.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // addStructure()

	void quadrilateral::mesh(const unsigned int& n, std::vector<point*>& pointStore)
	{
		mMeshedPoints.clear();
		mMeshedPoints.resize((n+1)*(n+1));

		namespace geom = bso::utilities::geometry;
		geom::vector v01 = mVertices[1] - mVertices[0];
		geom::vector v32 = mVertices[2] - mVertices[3];
		std::vector<geom::vertex> meshPointsV01(n+1);
		std::vector<geom::vertex> meshPointsV32(n+1);
		
		for (unsigned int i = 0; i < (n + 1); ++i)
		{
			meshPointsV01[i] = mVertices[0] + (v01 * ((double)i/((double)n)));
			meshPointsV32[i] = mVertices[3] + (v32 * ((double)i/((double)n)));
		}
		
		geom::vertex meshPoint;
		geom::vector dirVector;
		unsigned long highestID = 0;
		bool pointFound;
		for (unsigned int i = 0; i < (n+1); ++i)
		{
			dirVector = meshPointsV32[i] - meshPointsV01[i];
			for (unsigned int j = 0; j < (n+1); ++j)
			{
				meshPoint = meshPointsV01[i] + (dirVector * ((double)j/((double)n)));
				pointFound = false;
				for (const auto& k : pointStore)
				{
					if (k->isSameAs(meshPoint))
					{
						mMeshedPoints[i + (n+1)*j] = k;
						pointFound = true;
						break;
					}
					if (highestID < k->getID()) highestID = k->getID();
				}
				if (!pointFound)
				{
					pointStore.push_back(new point(highestID++,meshPoint));
					mMeshedPoints[i + (n+1)*j] = pointStore.back();
				}
			}
		}

		// pair the points that define an element together
		mElementPoints.clear();
		mElementPoints.resize(n*n);
		for (unsigned int i = 0; i < n; ++i)
		{
			for (unsigned int j = 0; j < n; ++j)
			{
				mElementPoints[i + n*j] = {
					mMeshedPoints[i + ((n+1)*j)], 		mMeshedPoints[(i+1) + ((n+1)*j)],
					mMeshedPoints[i + ((n+1)*(j+1))], mMeshedPoints[(i+1) + ((n+1)*(j+1))]
				};
			}
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
		for (auto& i : mElementPoints)
		{ // for each element
			geom::quadrilateral elementGeometry = derived_ptr_to_vertex(i);
			geom::vertex elementCenter = elementGeometry.getCenter();

			for (unsigned int j = 0; j < 4; ++j)
			{
				geom::line_segment lTemp1 = {elementGeometry[j], elementGeometry[(j+1)%4]};
				geom::line_segment lTemp2 = {elementGeometry[j], elementGeometry[(j-1)%4]};
				geom::quadrilateral partitionGeometry = {
					elementGeometry[j], elementCenter, lTemp1.getCenter(), lTemp2.getCenter()};
				double partitionArea = partitionGeometry.getArea();
				
				// find the point that corresponds to vertex j
				for (auto& k : i)
				{ // for each point k of the element
					if (k->isSameAs(elementGeometry[j]))
					{ // if it is the same as vertex i of the element geometry
						for (auto & l : mLoads)
						{ // the for each load l, add the nodal element load to point k
							k->addLoad(l * partitionArea);
						}
					}
				}
			}
		}
	} // mesh()

	
} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_QUADQUADRILATERAL_CPP