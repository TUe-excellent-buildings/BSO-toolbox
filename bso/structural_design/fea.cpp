#ifndef SD_FEA_CPP
#define SD_FEA_CPP

#include <sstream>
#include <stdexcept>

namespace bso { namespace structural_design {
	
	void fea::simplicialLLT()
	{
		Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > solver;
		solver.compute(mGSM);
		if (solver.info() != Eigen::Success)
		{
			std::stringstream errorMessage;
			errorMessage << "\nWhen solving an FEA system with SimplicialLLT,\n"
									 << "Could not decompose the GSM\n"
									 << "(bso/structural_design/fea.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		
		for (auto& lc : mLoadCases)
		{
			try
			{
				mDisplacements[lc] = solver.solve(mLoads[lc]);
				if (solver.info() != Eigen::Success)
				{
					throw std::runtime_error("Solver failed");
				}
			}
			catch (std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "\nWhen solving FEA system with SimplicialLLT for load case: " << *lc << "\n"
										 << "received the following error:\n" << e.what() << "\n"
										 << "(bso/structural_design/fea.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
		}
	} // simplicialLLT()
	
	void fea::simplicialLDLT()
	{
		Eigen::SimplicialLDLT<Eigen::SparseMatrix<double> > solver;
		solver.compute(mGSM);
		if (solver.info() != Eigen::Success)
		{
			std::stringstream errorMessage;
			errorMessage << "\nWhen solving an FEA system with SimplicialLDLT,\n"
									 << "Could not decompose the GSM\n"
									 << "(bso/structural_design/fea.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		
		for (auto& lc : mLoadCases)
		{
			try
			{
				mDisplacements[lc] = solver.solve(mLoads[lc]);
				if (solver.info() != Eigen::Success)
				{
					throw std::runtime_error("Solver failed");
				}
			}
			catch (std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "\nWhen solving FEA system with SimplicialLDLT for load case: " << *lc << "\n"
										 << "received the following error:\n" << e.what() << "\n"
										 << "(bso/structural_design/fea.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
		}
	} // simplicialLDLT()
	
	void fea::BiCGSTAB()
	{
		Eigen::BiCGSTAB<Eigen::SparseMatrix<double>, Eigen::DiagonalPreconditioner<double>> solver;
		
		for (auto& lc : mLoadCases)
		{
			try
			{
				solver.compute(mGSM);
				solver.setTolerance(1e-3);
				
				if (solver.info() != Eigen::Success)
				{
					throw std::runtime_error("Solver failed decompose matrix GSM");
				}
				
				mDisplacements[lc] = solver.solve(mLoads[lc]);
				if (solver.info() != Eigen::Success)
				{
					throw std::runtime_error("Solver failed to solve GSM for loads");
				}
			}
			catch (std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "\nWhen solving FEA system with BiCGSTAB for load case: " << *lc << "\n"
										 << "received the following error:\n" << e.what() << "\n"
										 << "(bso/structural_design/fea.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
		}
	} // BiCGSTAB()
	
	void fea::scaledBiCGSTAB()
	{
		Eigen::BiCGSTAB<Eigen::SparseMatrix<double>, Eigen::DiagonalPreconditioner<double>> solver;
		
		for (auto& lc : mLoadCases)
		{
			try
			{
				solver.compute(mGSM);
				if (solver.info() != Eigen::Success)
				{
					throw std::runtime_error("solver could not decompose matrix GSM");
				}
				// get a rough solution
				solver.setMaxIterations(3);
				mDisplacements[lc] = solver.solve(mLoads[lc]);
				
				if (solver.info() != Eigen::Success)
				{
					throw std::runtime_error("Solver failed to roughly solve GSM for loads");
				}
				
				// scale the GSM into a temporary GSM matrix: Could
				Eigen::VectorXd wInverse = (mDisplacements[lc].array().abs()+1e-6).inverse();
				Eigen::SparseMatrix<double> C;
				C.resize(mDOFCount,mDOFCount);
				C = mGSM * wInverse.asDiagonal();
				
				solver.compute(C);
				solver.setMaxIterations(mDOFCount*10);
				
				if (solver.info() != Eigen::Success)
				{
					throw std::runtime_error("Solver could not decompose matrix C");
				}
				
				Eigen::VectorXd y(mDOFCount);
				y = solver.solve(mLoads[lc]);
				mDisplacements[lc] = wInverse.asDiagonal() * y;
				if (solver.info() != Eigen::Success)
				{
					throw std::runtime_error("Solver failed to solver for y");
				}
			}
			catch (std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "\nWhen solving FEA system with scaled BiCGSTAB for load case: " << *lc << "\n"
										 << "received the following error:\n" << e.what() << "\n"
										 << "(bso/structural_design/fea.cpp)" << std::endl;
				throw std::runtime_error(errorMessage.str());
			}
		}
	} // scaledBiCGSTAB()
	
	fea::fea()
	{
		
	} // ctor
	
	fea::~fea()
	{
		for (auto& i : mNodes) delete i;
		for (auto& i : mElements) delete i;
	} // dtor
	
	element::node* fea::addNode(const bso::utilities::geometry::vertex& point)
	{
		for (auto i : mNodes)
		{
			if (i->isSameAs(point)) 
			{
				return i;
			}
		}
		unsigned int nodeID = mNodes.size()+1;
		mNodes.push_back(new element::node(point,nodeID));
		return mNodes.back();
	} // addNode()
	
	void fea::addElement(element::element* ele)
	{
		mElements.push_back(ele);
	} // addElement()
	
	void fea::generateGSM()
	{
		if (!mSystemInitialized)
		{
			mDOFCount = 0;
			for (auto& i : mNodes) i->generateNFT(mDOFCount);
			for (auto& i : mElements) i->generateEFT();
			
			// get all the load cases
			for (auto& i : mNodes)
			{
				auto nodalLC = i->getLoadCases();
				for (auto& j : nodalLC)
				{
					if (std::find(mLoadCases.begin(), mLoadCases.end(),j) == mLoadCases.end())
					{
						mLoadCases.push_back(j);
					}
				}
			}
			
			// create and fill the load vectors
			for (auto& i : mLoadCases)
			{
				mLoads[i] = Eigen::VectorXd::Zero(mDOFCount);
				for (auto & j : mNodes)
				{
					Eigen::Vector6d nodalLoads;
					try
					{
						Eigen::Vector6d nodalLoads = j->getLoads(i);
					}
					catch (std::exception& e)
					{
						// this node does not have a load with this load case
						j->addLoadCase(i);
						Eigen::Vector6d nodalLoads = j->getLoads(i);
					}
					for (unsigned int k = 0; k < 6; ++k)
					{
						if (j->getNFS(k) == 0 || j->getConstraint(k) == 1) continue;
						unsigned int DOF = j->getGlobalDOF(k);
						mLoads[i](DOF) = nodalLoads(k);
					}
				}
			}
			
			// create the displacement vectors
			for (auto& i : mLoadCases) mDisplacements[i] = Eigen::VectorXd::Zero(mDOFCount);
			
			mSystemInitialized = true;
		}
	
		mGSM.resize(0,0); // clear it in case there are still any components left
		mGSM.resize(mDOFCount,mDOFCount); // size it to the numbe rof DOF''s in the system
		
		std::vector<element::triplet > triplets;
		for (const auto& i : mElements)
		{
			auto trips = i->getSMTriplets();
			triplets.insert(triplets.end(), trips.begin(), trips.end());
		}
		
		mGSM.setFromTriplets(triplets.begin(), triplets.end());
	} // generateGSM()
	
	void fea::clearResponse()
	{
		for (auto& i : mElements) i->clearResponse();
		for (auto& i : mNodes) i->clearDisplacements();
		for (auto& i : mDisplacements) i.second.setZero();
	} // clearResponse()
	
	void fea::solve(std::string solver /*= "SimplicialLLT"*/)
	{
		// solve the system with the specified solver
		this->clearResponse();
		if (solver == "SimplicialLLT") this->simplicialLLT();
		else if (solver == "SimplicialLDLT") this->simplicialLDLT();
		else if (solver == "BiCGSTAB") this->BiCGSTAB();
		else if (solver == "scaledBiCGSTAB") this->scaledBiCGSTAB();
		else 
		{
			std::stringstream errorMessage;
			errorMessage << "\nTrying to solve FEA system with unknow solver:\n"
									 << solver << "\n"
									 << "(bso/structural_design/fea.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}

		// add the displacements to the nodes
		for (auto& i : mNodes) i->addDisplacements(mDisplacements);
		
		// compute the responses for elements for every load case
		for (auto& i : mElements) 
		{
			for (auto& j : mLoadCases) 
			{
				i->computeResponse(j);
			}
		}		
	} // solve()
	
	Eigen::VectorXd fea::getDisplacements(element::load_case* lc) const
	{
		auto dispSearch = mDisplacements.find(lc);
		if (dispSearch == mDisplacements.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nRequested displacements for unknown load case:\n"
									 << *lc << "\n"
									 << "(bso/structural_design/fea.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		return dispSearch->second;
	}
	
} // namespace structural_design
} // namespace bso

#endif // SD_FEA_CPP