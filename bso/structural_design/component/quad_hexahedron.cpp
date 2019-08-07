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
		mMeshedPoints.clear();
		mMeshedPoints.resize(pow((n+1),3));

		namespace geom = bso::utilities::geometry;
		geom::vector v04 = mVertices[4] - mVertices[0];
		geom::vector v15 = mVertices[5] - mVertices[1];
		geom::vector v26 = mVertices[6] - mVertices[2];
		geom::vector v37 = mVertices[7] - mVertices[3];
		std::vector<geom::vertex> meshPointsLine04(n+1);
		std::vector<geom::vertex> meshPointsLine15(n+1);
		std::vector<geom::vertex> meshPointsLine26(n+1);
		std::vector<geom::vertex> meshPointsLine37(n+1);
		
		for (unsigned int i = 0; i < (n + 1); ++i)
		{
			meshPointsLine04[i] = mVertices[0] + (v04 * ((double)i/((double)n)));
			meshPointsLine15[i] = mVertices[1] + (v15 * ((double)i/((double)n)));
			meshPointsLine26[i] = mVertices[2] + (v26 * ((double)i/((double)n)));
			meshPointsLine37[i] = mVertices[3] + (v37 * ((double)i/((double)n)));
		}
		
		std::vector<geom::vertex> meshPointsQuad0154(pow((n+1),2));
		std::vector<geom::vertex> meshPointsQuad3267(pow((n+1),2));
		
		for (unsigned int i = 0; i < (n + 1); ++i)
		{
			geom::vector dirVector01 = meshPointsLine15[i] - meshPointsLine04[i];
			geom::vector dirVector32 = meshPointsLine26[i] - meshPointsLine37[i];
			for (unsigned int j = 0; j < (n + 1); ++j)
			{
				meshPointsQuad0154[i + ((n+1)*j)] = meshPointsLine04[i] + (dirVector01 * ((double)j/((double)n)));
				meshPointsQuad3267[i + ((n+1)*j)] = meshPointsLine37[i] + (dirVector32 * ((double)j/((double)n)));
			}
		}
		
		geom::vertex meshPoint;
		geom::vector dirVector;
		unsigned long highestID = 0;
		bool pointFound;
		for (unsigned int i = 0; i < (n + 1); ++i)
		{
			for (unsigned int j = 0; j < (n+1); ++j)
			{
				dirVector = meshPointsQuad3267[i + ((n+1)*j)] - meshPointsQuad0154[i + ((n+1)*j)];
				for (unsigned int k = 0; k < (n+1); ++k)
				{
					meshPoint = meshPointsQuad0154[i + ((n+1)*j)] + (dirVector * ((double)k/((double)n)));
					pointFound = false;
					for (const auto& l : pointStore)
					{
						if (l->isSameAs(meshPoint))
						{
							mMeshedPoints[i + ((n+1)*j) + (pow(n+1,2)*k)] = l;
							pointFound = true;
							break;
						}
						if (highestID < l->getID()) highestID = l->getID();
					}
					if (!pointFound)
					{
						pointStore.push_back(new point(highestID++,meshPoint));
						mMeshedPoints[i + ((n+1)*j) + (pow(n+1,2)*k)] = pointStore.back();
					}
				}
			}
		}			

		// pair the points that define an element together
		mElementPoints.clear();
		mElementPoints.resize(pow(n,3));
		for (unsigned int i = 0; i < n; ++i)
		{
			for (unsigned int j = 0; j < n; ++j)
			{
				for (unsigned int k = 0; k < n; ++k)
				{
					mElementPoints[i + (n*j) + (pow(n,2)*k)] = {
						mMeshedPoints[i 		+ ((n+1)*j) 		+ (pow(n+1,2)*k)],
						mMeshedPoints[i 		+ ((n+1)*j) 		+ (pow(n+1,2)*(k+1))],
						mMeshedPoints[i 		+ ((n+1)*(j+1)) + (pow(n+1,2)*k)],
						mMeshedPoints[i 		+ ((n+1)*(j+1)) + (pow(n+1,2)*(k+1))],
						mMeshedPoints[(i+1) + ((n+1)*j) 		+ (pow(n+1,2)*k)],
						mMeshedPoints[(i+1) + ((n+1)*j) 		+ (pow(n+1,2)*(k+1))],
						mMeshedPoints[(i+1) + ((n+1)*(j+1)) + (pow(n+1,2)*k)],
						mMeshedPoints[(i+1) + ((n+1)*(j+1)) + (pow(n+1,2)*(k+1))]
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
				for (auto k = elementGeometry.lineBegin(); k!= elementGeometry.lineEnd(); ++k)
				{ // each line segment k in the element's geometry
					for (const auto& l : *k)
					{ // for each vertex l in line segment k
						if (elementGeometry[j].isSameAs(l))
						{ // if vertex j is in line segment l
							// add the center of line segment l to the partition
							partitionVertices.push_back(k->getCenter());
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
				for (auto k = elementGeometry.polygonBegin(); k != elementGeometry.polygonEnd(); ++k)
				{ // for each polygon k in the element's geometry
					for (auto l = (*k)->begin(); l != (*k)->end(); ++l)
					{ // for each vertex l in polygon k
						if (elementGeometry[j].isSameAs(*l))
						{
							// add the center of line segment l to the partition
							partitionVertices.push_back((*k)->getCenter());
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
	} // 

	
} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_QUAD_HEXAHEDRON_CPP