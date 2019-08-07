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
		
	} // ctor()

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

	void sd_model::mesh(const unsigned int& n)
	{
		// intiialize a new FEA system
		this->clearMesh();

		// mesh the points
		for (auto& i : mPoints)
		{
			unsigned long pointID = mMeshedPoints.size();
			mMeshedPoints.push_back(new component::point(pointID, *i));
			auto meshedPoint = mMeshedPoints.back();
			for (auto j = i->loadBegin(); j != i->loadEnd(); ++j)
			{
				meshedPoint->addLoad(*j);
			}
			for (auto j = i->constraintBegin(); j != i->constraintEnd(); ++j)
			{
				meshedPoint->addConstraint(*j);
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
			for (auto k = i->loadBegin(); k != i->loadEnd(); ++k)
			{
				nodePtr->addLoad(*k);
			}
			
			// add constraints
			for (auto k = i->constraintBegin(); k != i->constraintEnd(); ++k)
			{
				nodePtr->addConstraint(k->DOF());
			}
		}

		// create the elements
		unsigned long elementID = 0;
		element::element* elePtr;
		for (auto& i : mGeometries)
		{
			if (i->hasTruss())
			{
				for (auto j = i->structureBegin(); j != i->structureEnd(); ++j)
				{
					double ERelativeLowerBound = 1e-6;
					if (j->hasERelativeLowerBoundAssigned()) ERelativeLowerBound = j->ERelativeLowerBound();
					
					if (j->type() == "truss")
					{
						auto firstPoint  = *(i->meshedPointsBegin());
						auto secondPoint = *(std::prev(i->meshedPointsEnd(),1));

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

						elePtr = new element::truss(elementID++,j->E(), j->A(),
													{firstNodeSearch->second,secondNodeSearch->second},
													ERelativeLowerBound);
						mFEA->addElement(elePtr);
					}
				}
			}
			for (auto j = i->elementPointsBegin(); j != i->elementPointsEnd(); ++j)
			{
				if (std::distance(i->structureBegin(),i->structureEnd()) == 0) continue;
				std::vector<element::node*> elementNodes;
				for (auto k : *j)
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
				for (auto k = i->structureBegin(); k != i->structureEnd(); ++k)
				{
					double ERelativeLowerBound = 1e-6;
					if (k->hasERelativeLowerBoundAssigned()) ERelativeLowerBound = k->ERelativeLowerBound();
					if (k->type() == "truss"){ }// do nothing, these are meshed by one element already
					else if (k->type() == "beam")
					{
						elePtr = new element::beam(elementID++,
													k->E(), k->width(), k->height(), 
													k->poisson(), elementNodes, ERelativeLowerBound);
						mFEA->addElement(elePtr);
					}
					else if (k->type() == "flat_shell")
					{
						elePtr = new element::flat_shell(elementID++,
													k->E(), k->thickness(), k->poisson(), 
													elementNodes, ERelativeLowerBound);
						mFEA->addElement(elePtr);
					}
					else if (k->type() == "quad_hexahedron")
					{
						elePtr = new element::quad_hexahedron(elementID++,
													k->E(), k->poisson(), elementNodes, ERelativeLowerBound);
						mFEA->addElement(elePtr);
					}
					else
					{
						std::stringstream errorMessage;
						errorMessage << "\nWhen meshing a geometry of a structural model,\n"
												 <<	"Encountered an unknown element type to be meshed:\n"
												 << k->type() << "\n"
												 << "(bso/structural_design/sd_model.cpp)" << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					if (k->isGhostComponent() == true)
					{
						elePtr->isActiveInCompliance() = false;
						elePtr->visualize() = false;
					}
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
	
	void sd_model::topologyOptimization(
				const std::string& algorithm, const double& f, 
				const double& rMin, const double& penal, const double& xMove,
				const double& tolerance, std::ostream& out)
	{
		if (algorithm == "SIMP")
		{
			topology_optimization::SIMP(mFEA,f,rMin,penal,xMove,tolerance,out);
		}
		else if (algorithm == "robust")
		{
			topology_optimization::robust(mFEA,f,rMin,penal,xMove,tolerance,out);
		}
		else if (algorithm == "elementTypeSIMP")
		{
			topology_optimization::elementTypeSIMP(mFEA,f,rMin,penal,xMove,tolerance,out);
		}
		else if (algorithm == "componentWiseSIMP")
		{
			topology_optimization::componentWiseSIMP(mFEA,f,rMin,penal,xMove,tolerance,out);
		}
		else 
		{
			std::stringstream errorMessage;
			errorMessage << "\nRequesting invalid topology optimization algorithm:\n"
									 << algorithm << "\nWhich could not be identified as an algorithm\n"
									 << "(bso/structural_design/sd_model.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	}
	
	void sd_model::topologyOptimization(
				const std::string& algorithm, const double& f, 
				const double& rMin, const double& penal, const double& xMove,
				const double& tolerance)
	{
		std::ostream dummyStream(0);
		this->topologyOptimization(algorithm,f,rMin,penal,xMove,tolerance,dummyStream);
	}

	void sd_model::getResult()
	{
		
	} // getResult()

	
} // namespace structural_design
} // namespace bso

#endif // SD_ANALYSIS_CPP