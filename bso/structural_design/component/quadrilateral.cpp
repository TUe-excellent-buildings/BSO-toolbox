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
		this->mesh(0,1,n,n,pointStore);
	} // mesh()
	
	void quadrilateral::mesh(const unsigned int& v0Index, const unsigned int& v1Index,
				const unsigned int& n1, const unsigned int& n2, std::vector<point*>& pointStore)
	{
		mMeshedPoints.clear();
		mMeshedPoints.resize((n1+1)*(n2+1));
		namespace geom = bso::utilities::geometry;
		std::vector<unsigned int> indices = {v0Index, v1Index};

		for (unsigned int i = 0; i < 4; ++i)
		{
			bool indexFound = false;
			for (const auto& j : indices)
			{
				if (i == j)
				{
					indexFound = true;
					break;
				}
			}
			if (!indexFound) indices.push_back(i);
		}
		if (!indices.size() == 4)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError when meshing, could not find all vertex indices.\n"
									 << "(bso/structural_design/component/quadrilateral.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}

		geom::line_segment l03 = {mVertices[indices[0]], mVertices[indices[3]]},
											 l12 = {mVertices[indices[1]], mVertices[indices[2]]};
		if (l03.intersectsWith(l12)) std::swap(indices[2], indices[3]);
		
		geom::vector v01 = mVertices[indices[1]] - mVertices[indices[0]];
		geom::vector v32 = mVertices[indices[2]] - mVertices[indices[3]];
		std::vector<geom::vertex> meshPointsV01(n1+1);
		std::vector<geom::vertex> meshPointsV32(n1+1);
		
		for (unsigned int i = 0; i < (n1 + 1); ++i)
		{
			meshPointsV01[i] = mVertices[indices[0]] + (v01 * ((double)i/((double)n1)));
			meshPointsV32[i] = mVertices[indices[3]] + (v32 * ((double)i/((double)n1)));
		}
		
		geom::vertex meshPoint;
		geom::vector dirVector;
		unsigned long highestID = 0;
		bool pointFound;
		for (unsigned int i = 0; i < (n1+1); ++i)
		{
			dirVector = meshPointsV32[i] - meshPointsV01[i];
			for (unsigned int j = 0; j < (n2+1); ++j)
			{
				meshPoint = meshPointsV01[i] + (dirVector * ((double)j/((double)n2)));
				pointFound = false;
				for (const auto& k : pointStore)
				{
					if (k->isSameAs(meshPoint))
					{
						mMeshedPoints[i + (n2+1)*j] = k;
						pointFound = true;
						break;
					}
					if (highestID < k->getID()) highestID = k->getID();
				}
				if (!pointFound)
				{
					pointStore.push_back(new point(highestID++,meshPoint));
					mMeshedPoints[i + (n2+1)*j] = pointStore.back();
				}
			}
		}

		// pair the points that define an element together
		mElementPoints.clear();
		mElementPoints.resize(n1*n2);
		for (unsigned int i = 0; i < n1; ++i)
		{
			for (unsigned int j = 0; j < n2; ++j)
			{
				mElementPoints[i + n1*j] = {
					mMeshedPoints[i + ((n1+1)*j)], 		mMeshedPoints[(i+1) + ((n1+1)*j)],
					mMeshedPoints[i + ((n1+1)*(j+1))], mMeshedPoints[(i+1) + ((n1+1)*(j+1))]
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
	}

	
} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_QUADQUADRILATERAL_CPP