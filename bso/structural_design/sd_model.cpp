#ifndef SD_ANALYSIS_CPP
#define SD_ANALYSIS_CPP

#include <bso/structural_design/topology_optimization/topology_optimization.hpp>

namespace bso { namespace structural_design {
	
	void sd_model::clearMesh()
	{
		if (mIsMeshed)
		{
			mIsMeshed = false;
			for (auto& i : mGeometries) i->clearMesh();
			for (auto& i : mMeshedPoints) delete i;
			delete mFEA;
			mFEA = new fea();
			mMeshedPoints.clear();
		}
	} // clearMesh()

	sd_model::sd_model() 
	{
		mTopOptStreamBuffer = nullptr;
	} // ctor()
	
	sd_model::sd_model(const sd_model& rhs)
	{
		for (const auto& i : rhs.mPoints)
		{
			this->addPoint(*i);
		}
		for (const auto& i : rhs.mGeometries)
		{
			// copy the geometry
			structural_design::component::geometry* newSDGeom;
			if (i->isLineSegment())
			{
				auto linePtr = dynamic_cast<utilities::geometry::line_segment*>(i);
				newSDGeom = this->addGeometry(*linePtr);
			}
			else if(i->isQuadrilateral())
			{
				auto quadPtr = dynamic_cast<utilities::geometry::quadrilateral*>(i);
				newSDGeom = this->addGeometry(*quadPtr);
			}
			else if (i->isQuadHexahedron())
			{
				auto quadHexPtr = dynamic_cast<utilities::geometry::quad_hexahedron*>(i);
				newSDGeom = this->addGeometry(*quadHexPtr);
			}
			else 
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, while copying a structural model, did not\n"
										 << "recognize one of its geometries.\n"
										 << "(bso/structural_design/sd_model.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			// add the possible structure, loads, and constraints to the geoemtry
			for (const auto& j : i->getStructures()) newSDGeom->addStructure(j);
			for (const auto& j : i->getLoads()) newSDGeom->addLoad(j);
			for (const auto& j : i->getConstraints()) newSDGeom->addConstraint(j);
		}
		mMeshSize = rhs.mMeshSize;
		mTopOptStreamBuffer = rhs.mTopOptStreamBuffer;
	}

	sd_model::~sd_model()
	{
		for (auto& i : mPoints) delete i;
		for (auto& i : mGeometries) delete i;
		for (auto& i : mMeshedPoints) delete i;
		if (mIsMeshed) delete mFEA;
	} // //dtor()

	component::point* sd_model::addPoint(bso::utilities::geometry::vertex p)
	{
		for (auto& i : mPoints)
		{
			if (i->isSameAs(p))
			{
				return i;
			}
		}
		unsigned int pointID = mPoints.size();
		mPoints.push_back(new component::point(pointID, p));
		return mPoints.back();
	} // addPoint()

	component::geometry* sd_model::addGeometry(const bso::utilities::geometry::line_segment& g)
	{
		for (auto& i : mGeometries)
		{
			if (i->isLineSegment())
			{
				component::line_segment* temp = dynamic_cast<component::line_segment*>(i);
				if (temp->isSameAs(g)) return i;
			}
		}
		mGeometries.push_back(new component::line_segment(g));
		return mGeometries.back();
	} // addGeometry(line_segment)
	
	component::geometry* sd_model::addGeometry(const bso::utilities::geometry::quadrilateral& g)
	{
		for (auto& i : mGeometries)
		{
			if (i->isQuadrilateral())
			{
				component::quadrilateral* temp = dynamic_cast<component::quadrilateral*>(i);
				if (temp->isSameAs(g)) return i;
			}
		}
		mGeometries.push_back(new component::quadrilateral(g));
		return mGeometries.back();
	} // addGeometry(line_segment)
	
	component::geometry* sd_model::addGeometry(const bso::utilities::geometry::quad_hexahedron& g)
	{
		for (auto& i : mGeometries)
		{
			if (i->isQuadHexahedron())
			{
				component::quad_hexahedron* temp = dynamic_cast<component::quad_hexahedron*>(i);
				if (temp->isSameAs(g)) return i;
			}
		}
		mGeometries.push_back(new component::quad_hexahedron(g));
		return mGeometries.back();
	} // addGeometry(line_segment)
	
	void sd_model::setMeshSize(const unsigned int& n)
	{
		if (n == 0)
		{
			std::stringstream errorMessage;
			errorMessage << "\nErrro, cannot set mesh size to zero,\n"
									 << "(bso/structural_design/sd_model.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		mMeshSize = n;
	} // setMeshSize()
	
	void sd_model::mesh()
	{
		this->mesh(mMeshSize);
	} // mesh

	void sd_model::mesh(const unsigned int& n, bool meshLoadPanels /* = true */)
	{
		// intiialize a new FEA system
		this->clearMesh();

		// mesh the points
		for (auto& i : mPoints)
		{
			unsigned long pointID = mMeshedPoints.size();
			mMeshedPoints.push_back(new component::point(pointID, *i));
			auto meshedPoint = mMeshedPoints.back();
			for (const auto& j : i->getLoads())
			{
				meshedPoint->addLoad(j);
			}
			for (const auto& j : i->getConstraints())
			{
				meshedPoint->addConstraint(j);
			}
		}

		// mesh the geometries
		for (auto& i : mGeometries)
		{
			i->mesh(n,mMeshedPoints);
		}

		// create the nodes in the fea system and add loads and constraints to them
		std::map<component::point*, element::node*> nodeMap;
		element::node* nodePtr;
		mFEA = new fea();
		for (auto& i : mMeshedPoints)
		{
			nodePtr = mFEA->addNode(*i);
			nodeMap[i] = nodePtr;
			
			// add loads
			for (const auto& j : i->getLoads())
			{
				nodePtr->addLoad(j);
			}
			
			// add constraints
			for (const auto& j : i->getConstraints())
			{
				nodePtr->addConstraint(j.DOF());
			}
		}

		// create the elements
		unsigned long elementID = 0;
		element::element* elePtr;
		for (auto& i : mGeometries)
		{
			if (i->hasTruss())
			{
				for (const auto& j : i->getStructures())
				{
					if (!meshLoadPanels && j.isGhostComponent()) continue;
					double ERelativeLowerBound = 1e-6;
					if (j.hasERelativeLowerBoundAssigned()) ERelativeLowerBound = j.ERelativeLowerBound();
					
					if (j.type() == "truss")
					{
						auto firstPoint  = i->getMeshedPoints()[0];
						auto secondPoint = i->getMeshedPoints().back();

						auto firstNodeSearch  = nodeMap.find(firstPoint);
						auto secondNodeSearch = nodeMap.find(secondPoint);

						if ((firstNodeSearch  == nodeMap.end()) ||
								(secondNodeSearch == nodeMap.end()))
						{
							std::stringstream errorMessage;
							errorMessage << "\nWhen meshing a truss,\n"
													 <<	"Could not find the node at:\n"
													 << *firstPoint << " or " << *secondPoint << "\n"
													 << "(bso/structural_design/sd_model.cpp)" << std::endl;
							throw std::runtime_error(errorMessage.str());
						}

						elePtr = new element::truss(elementID++,j.E(), j.A(),
													{firstNodeSearch->second,secondNodeSearch->second},
													ERelativeLowerBound);
						mFEA->addElement(elePtr);
						i->addElement(elePtr);
						if (j.isGhostComponent()) elePtr->isActiveInCompliance() = false;
						if (!j.isVisible()) elePtr->visualize() = false;
					}
				}
			}
			for (const auto& j : i->getElementPoints())
			{
				if (i->getElementPoints().size() == 0) continue;
				std::vector<element::node*> elementNodes;
				for (const auto& k : j)
				{
					auto nodeSearch = nodeMap.find(k);
					if (nodeSearch == nodeMap.end())
					{
						std::stringstream errorMessage;
						errorMessage << "\nWhen meshing a geometry of a structural model,\n"
												 <<	"Could not find the node at:\n"
												 << *k << "\n"
												 << "(bso/structural_design/sd_model.cpp)" << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					elementNodes.push_back(nodeSearch->second);
				}
				for (const auto& k : i->getStructures())
				{
					if (!meshLoadPanels && k.isGhostComponent()) continue;
					double ERelativeLowerBound = 1e-6;
					if (k.hasERelativeLowerBoundAssigned()) ERelativeLowerBound = k.ERelativeLowerBound();
					if (k.type() == "truss"){ continue; }// do nothing, these are meshed by one element already
					else if (k.type() == "beam")
					{
						elePtr = new element::beam(elementID++,
													k.E(), k.width(), k.height(), 
													k.poisson(), elementNodes, ERelativeLowerBound);
						mFEA->addElement(elePtr);
					}
					else if (k.type() == "flat_shell")
					{
						elePtr = new element::flat_shell(elementID++,
													k.E(), k.thickness(), k.poisson(), 
													elementNodes, ERelativeLowerBound);
						mFEA->addElement(elePtr);
					}
					else if (k.type() == "quad_hexahedron")
					{
						elePtr = new element::quad_hexahedron(elementID++,
													k.E(), k.poisson(), elementNodes, ERelativeLowerBound);
						mFEA->addElement(elePtr);
					}
					else
					{
						std::stringstream errorMessage;
						errorMessage << "\nWhen meshing a geometry of a structural model,\n"
												 <<	"Encountered an unknown element type to be meshed:\n"
												 << k.type() << "\n"
												 << "(bso/structural_design/sd_model.cpp)" << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					if (k.isGhostComponent()) elePtr->isActiveInCompliance() = false;
					if (!k.isVisible()) elePtr->visualize() = false;
					i->addElement(elePtr);
				}
			}
		}

		// generate the fea system
		mFEA->generateGSM();
		mIsMeshed = true;
	} // mesh()

	void sd_model::analyze(std::string solver /*= : SimplicialLLT*/)
	{
		if (!mIsMeshed)
		{
			try 
			{
				this->mesh();
			}
			catch(std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "\nWhen analyzing an unmeshed structural design model,\n"
										 << "failed to mesh the model, received the following exception:\n"
										 << e.what() << "\n"
										 << "(bso/structural_design/sd_model.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
		}
		try
		{
			mFEA->solve(solver);
		}
		catch (std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "\nWhen analyzing a structural design model, received the\n"
									 << "following error message: \n" << e.what() << "\n"
									 << "(bso/structural_design/sd_model.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	} // analyze()
	
	bool sd_model::isStable()
	{
		bool preMeshed = mIsMeshed;
		mesh(1,false);
		mIsMeshed = false;
		bool isStable = !mFEA->isSingular();
		if (preMeshed) this->mesh(); // mesh it back to original mesh size
		return isStable;
	}
	
	void sd_model::rescaleStructuralVolume(const double& scaleFactor)
	{
		for (auto& i : mGeometries)
		{
			i->rescaleStructuralVolume(scaleFactor);
		}
	} // rescaleStructuralVolume()
	
	void sd_model::setElementDensities(const double& volumeFraction, const double& penalty)
	{
		if (!mIsMeshed)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, called setElementDensities() on an unmeshed\n"
									 << "structural design model,\n"
									 << "(bso/structural_design/sd_model.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		for (auto& i : mFEA->getElements())
		{
			i->updateDensity(volumeFraction,penalty);
		}
		mFEA->generateGSM();
	} // setElementDensities()
	
	void sd_model::setTopOptOutputStream(std::ostream& out)
	{
		mTopOptStreamBuffer = out.rdbuf();
	}
	
	sd_results sd_model::getTotalResults()
	{
		sd_results results;
		for (const auto& i : mFEA->getElements())
		{
			if (i->isActiveInCompliance())
			{
				results.mTotalStrainEnergy += i->getTotalEnergy();
				if (i->isFlatShell())
				{
					results.mShearStrainEnergy += i->getTotalEnergy("shear");
					results.mAxialStrainEnergy += i->getTotalEnergy("axial");
					results.mBendStrainEnergy  += i->getTotalEnergy("bending");
				}
				results.mTotalStructuralVolume += i->getVolume();
			}
			else
			{
				results.mGhostStrainEnergy += i->getTotalEnergy();
				results.mGhostStructuralVolume += i->getVolume();
			}
		}
		return results;
	} // getTotalResults()
	
	sd_results sd_model::getPartialResults(bso::utilities::geometry::polygon* geom)
	{
		sd_results results;
		
		for (const auto& i : mFEA->getElements())
		{
			bool allPointsInsideOrOn = true;
			for (const auto& j : i->getNodes())
			{
				auto vertexPtr = dynamic_cast<bso::utilities::geometry::vertex*>(j);
				if (!geom->isInsideOrOn(*vertexPtr))
				{
					allPointsInsideOrOn = false;
					break;
				}
			}
			if (!allPointsInsideOrOn) continue;
			
			if (i->isActiveInCompliance())
			{
				results.mTotalStrainEnergy += i->getTotalEnergy();
				if (i->isFlatShell())
				{
					results.mShearStrainEnergy += i->getTotalEnergy("shear");
					results.mAxialStrainEnergy += i->getTotalEnergy("axial");
					results.mBendStrainEnergy  += i->getTotalEnergy("bending");
				}
				results.mTotalStructuralVolume += i->getVolume();
			}
			else
			{
				results.mGhostStrainEnergy += i->getTotalEnergy();
				results.mGhostStructuralVolume += i->getVolume();
			}
		}
		return results;
	} // getPartialResults()
	
	sd_results sd_model::getPartialResults(bso::utilities::geometry::polyhedron* geom)
	{
		sd_results results;
		
		for (const auto& i : mFEA->getElements())
		{
			bool allPointsInsideOrOn = true;
			for (const auto& j : i->getNodes())
			{
				auto vertexPtr = dynamic_cast<bso::utilities::geometry::vertex*>(j);
				if (!geom->isInsideOrOn(*vertexPtr))
				{
					allPointsInsideOrOn = false;
					break;
				}
			}
			if (!allPointsInsideOrOn) continue;

			if (i->isActiveInCompliance())
			{
				results.mTotalStrainEnergy += i->getTotalEnergy();
				results.mTotalStructuralVolume += i->getVolume();
			}
			else
			{
				results.mGhostStrainEnergy += i->getTotalEnergy();
				results.mGhostStructuralVolume += i->getVolume();
			}
		}
		return results;
	} // getPartialResults()
	

} // namespace structural_design
} // namespace bso

#endif // SD_ANALYSIS_CPP