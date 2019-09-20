#ifndef SD_QUAD_HEXAHEDRON_CPP
#define SD_QUAD_HEXAHEDRON_CPP

#include <bso/structural_design/component/derived_ptr_to_vertex.hpp>

#include <sstream>
#include <stdexcept>


namespace bso { namespace structural_design { namespace component {
	
	template <typename QUAD_HEXAHEDRON_INITIALIZER>
	quad_hexahedron::quad_hexahedron(const QUAD_HEXAHEDRON_INITIALIZER& l)
	:	bso::utilities::geometry::quad_hexahedron(l),
		geometry()
	{
		mIsQuadHexahedron = true;
	} // 
	
	quad_hexahedron::quad_hexahedron(std::initializer_list<bso::utilities::geometry::vertex>&& l)
	:	bso::utilities::geometry::quad_hexahedron(std::move(l)),
		geometry()
	{
		mIsQuadHexahedron = true;
	} // 

	quad_hexahedron::~quad_hexahedron()
	{
		
	} // 

	void quad_hexahedron::addStructure(const structure& s)
	{
		if (s.type() == "quad_hexahedron")
		{
			geometry::addStructure(s);
		}
		else 
		{
			std::stringstream errorMessage;
			errorMessage << "\nError when assigning structure to quadrilateral hexahedron geomtry.\n"
									 << "Cannot assign structure with type: \"" << s.type() << "\"" 
									 << "to a quadrilateral hexahedron geometry\n"
									 << "(bso/structural_design/component/quad_hexahedron.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // 

	void quad_hexahedron::mesh(const unsigned int& n, std::vector<point*>& pointStore)
	{
		this->mesh(0,1,2,n,n,n,pointStore);
	} // 
	
	void quad_hexahedron:: mesh(const unsigned int& v0Index,
				const unsigned int& v1Index, const unsigned int& v2Index, 
				const unsigned int& n1, const unsigned int& n2, const unsigned int& n3,
				std::vector<point*>& pointStore)
	{
		mMeshedPoints.clear();
		mMeshedPoints.resize((n1+1)*(n2+1)*(n3+1));
		namespace geom = bso::utilities::geometry;
		std::vector<unsigned int> indices = {v0Index, v1Index, v2Index};

		// find index to v4
		for (unsigned int i = 0; i < 8; ++i)
		{
			if (std::find(indices.begin(), indices.end(), i) != indices.end()) continue;
			try
			{ // see if a quadrilateral can be formed using the vertex indexed at i
				geom::quadrilateral qTest = {mVertices[indices[0]], mVertices[indices[1]], 
																		 mVertices[indices[2]], mVertices[i]};
			}
			catch (std::exception& e)
			{ // if not check next index
				continue;
			} // else add this index as the fourth vertex and break from the loop
			indices.push_back(i);
			break;
		}

		// find the compliment of indices
		std::vector<unsigned int> otherIndices;
		for (unsigned int i = 0; i < 8; ++i)
		{
			if (std::find(indices.begin(),indices.end(),i) == indices.end())
			{
				otherIndices.push_back(i);
			}
		}

		// add vertices 4,5,6,7 to indices by checking vertices in otherIndices if
		// they make a line in the quad_hexahedron using vertices 0,1,2,3 respectively
		for (unsigned int i = 0; i < 4; ++i)
		{
			for (unsigned int j = 0; j < otherIndices.size(); ++j)
			{
				geom::line_segment checkline = {mVertices[indices[i]], mVertices[otherIndices[j]]};
				for (const auto k : mLineSegments)
				{
					if (k.isSameAs(checkline))
					{
						indices.push_back(otherIndices[j]);
						otherIndices.erase(otherIndices.begin() + j);
						break;
					}
				}
			}
		}

		if (indices.size() != 8)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError when meshing a quadrilateral faced hexahedron.\n"
									 << "Could not sort all the vertices.\n"
									 << "(bso/structural_design/component/quad_hexahedron.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}

		geom::vector v04 = mVertices[indices[4]] - mVertices[indices[0]];
		geom::vector v15 = mVertices[indices[5]] - mVertices[indices[1]];
		geom::vector v26 = mVertices[indices[6]] - mVertices[indices[2]];
		geom::vector v37 = mVertices[indices[7]] - mVertices[indices[3]];
		std::vector<geom::vertex> meshPointsLine04(n1+1);
		std::vector<geom::vertex> meshPointsLine15(n1+1);
		std::vector<geom::vertex> meshPointsLine26(n1+1);
		std::vector<geom::vertex> meshPointsLine37(n1+1);

		for (unsigned int i = 0; i < (n1 + 1); ++i)
		{
			meshPointsLine04[i] = mVertices[indices[0]] + (v04 * ((double)i/((double)n1)));
			meshPointsLine15[i] = mVertices[indices[1]] + (v15 * ((double)i/((double)n1)));
			meshPointsLine26[i] = mVertices[indices[2]] + (v26 * ((double)i/((double)n1)));
			meshPointsLine37[i] = mVertices[indices[3]] + (v37 * ((double)i/((double)n1)));
		}

		std::vector<geom::vertex> meshPointsQuad0154((n1+1)*(n2+1));
		std::vector<geom::vertex> meshPointsQuad3267((n1+1)*(n2+1));

		for (unsigned int i = 0; i < (n1 + 1); ++i)
		{
			geom::vector dirVector01 = meshPointsLine15[i] - meshPointsLine04[i];
			geom::vector dirVector32 = meshPointsLine26[i] - meshPointsLine37[i];
			for (unsigned int j = 0; j < (n2 + 1); ++j)
			{
				meshPointsQuad0154[i + ((n1+1)*j)] = meshPointsLine04[i] + (dirVector01 * ((double)j/((double)n2)));
				meshPointsQuad3267[i + ((n1+1)*j)] = meshPointsLine37[i] + (dirVector32 * ((double)j/((double)n2)));
			}
		}

		geom::vertex meshPoint;
		geom::vector dirVector;
		unsigned long highestID = 0;
		bool pointFound;
		for (unsigned int i = 0; i < (n1 + 1); ++i)
		{
			for (unsigned int j = 0; j < (n2 + 1); ++j)
			{
				dirVector = meshPointsQuad3267[i + ((n1+1)*j)] - meshPointsQuad0154[i + ((n1+1)*j)];

				for (unsigned int k = 0; k < (n3 + 1); ++k)
				{
					meshPoint = meshPointsQuad0154[i + ((n1+1)*j)] + (dirVector * ((double)k/((double)n3)));
					pointFound = false;
					for (const auto& l : pointStore)
					{
						if (l->isSameAs(meshPoint))
						{
							mMeshedPoints[i + ((n1+1)*j) + (((n1+1)*(n2+1))*k)] = l;
							pointFound = true;
							break;
						}
						if (highestID < l->getID()) highestID = l->getID();
					}
					if (!pointFound)
					{
						pointStore.push_back(new point(highestID++,meshPoint));
						mMeshedPoints[i + ((n1+1)*j) + (((n1+1)*(n2+1))*k)] = pointStore.back();
					}
				}
			}
		}		

		// pair the points that define an element together
		mElementPoints.clear();
		mElementPoints.resize(n1*n2*n3);
		for (unsigned int i = 0; i < n1; ++i)
		{
			for (unsigned int j = 0; j < n2; ++j)
			{
				for (unsigned int k = 0; k < n3; ++k)
				{
					mElementPoints[i + (n1*j) + ((n1*n2)*k)] = {
						mMeshedPoints[i 		+ ((n1+1)*j) 		 + (((n1+1)*(n2+1))*k)],
						mMeshedPoints[i 		+ ((n1+1)*j) 		 + (((n1+1)*(n2+1))*(k+1))],
						mMeshedPoints[i 		+ ((n1+1)*(j+1)) + (((n1+1)*(n2+1))*k)],
						mMeshedPoints[i 		+ ((n1+1)*(j+1)) + (((n1+1)*(n2+1))*(k+1))],
						mMeshedPoints[(i+1) + ((n1+1)*j) 		 + (((n1+1)*(n2+1))*k)],
						mMeshedPoints[(i+1) + ((n1+1)*j) 		 + (((n1+1)*(n2+1))*(k+1))],
						mMeshedPoints[(i+1) + ((n1+1)*(j+1)) + (((n1+1)*(n2+1))*k)],
						mMeshedPoints[(i+1) + ((n1+1)*(j+1)) + (((n1+1)*(n2+1))*(k+1))]
					};
				}
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

			geom::quad_hexahedron elementGeometry = derived_ptr_to_vertex(i);
			geom::vertex elementCenter = elementGeometry.getCenter();

			for (unsigned int j = 0; j < 8; ++j)
			{ // for each of the element's vertices j, find the (partition) volume that corresponds to how much of the load is carried to that vertex
				std::vector<geom::vertex> partitionVertices;
				for (const auto& k : elementGeometry.getLines())
				{ // each line segment k in the element's geometry
					for (const auto& l : k)
					{ // for each vertex l in line segment k
						if (elementGeometry[j].isSameAs(l))
						{ // if vertex j is in line segment l
							// add the center of line segment l to the partition
							partitionVertices.push_back(k.getCenter());
							break;
						}
					}
				}
				if (partitionVertices.size() != 3)
				{
					std::stringstream errorMessage;
					errorMessage << "\nFound " << partitionVertices.size() << "vertices" 
											 << "from line segments on an element's geometry\n"
											 << "when computing the load on an element's point.\n"
											 << "Expected 3.\n"
											 << "(bso/structural_design/component/quad_hexahedron.cpp)" << std::endl;
					throw std::runtime_error(errorMessage.str());
				}
				for (const auto& k : elementGeometry.getPolygons())
				{ // for each polygon k in the element's geometry
					for (const auto l : *k)
					{ // for each vertex l in polygon k
						if (elementGeometry[j].isSameAs(l))
						{
							// add the center of line segment l to the partition
							partitionVertices.push_back(k->getCenter());
							break;
						}
					}
				}
				if (partitionVertices.size() != 6)
				{
					std::stringstream errorMessage;
					errorMessage << "\nFound " << partitionVertices.size() - 3 << "vertices" 
											 << "from polyhedrons on an element's geometry\n"
											 << "when computing the load on an element's point.\n"
											 << "Expected 3.\n"
											 << "(bso/structural_design/component/quad_hexahedron.cpp)" << std::endl;
					throw std::runtime_error(errorMessage.str());
				}
				partitionVertices.push_back(elementGeometry[j]);
				partitionVertices.push_back(elementCenter);
				geom::quad_hexahedron partitionGeometry(partitionVertices);
				double partitionVolume = partitionGeometry.getVolume();
				
				// find the point that corresponds to vertex j
				for (auto& k : i)
				{ // for each point k of the element
					if (k->isSameAs(elementGeometry[j]))
					{ // if it is the same as vertex i of the element geometry
						for (auto & l : mLoads)
						{ // the for each load l, add the nodal element load to point k
							k->addLoad(l * partitionVolume);
						}
					}
				}
			}
		}
	} // mesh()

	
} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_QUAD_HEXAHEDRON_CPP