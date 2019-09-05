#ifndef SD_NODE_CPP
#define SD_NODE_CPP

namespace bso { namespace structural_design { namespace element {

	void node::initializeVariables()
	{
		mConstraints.setZero();
		mNFS.setZero();
	} // initializeVariables

	node::node(const std::initializer_list<double>&& l, const unsigned long& ID) :
		bso::utilities::geometry::vertex(std::move(l))
	{  // initialize by initializer list
		mID = ID;
		this->initializeVariables();
	} //
	
	template<class T> node::node(const Eigen::MatrixBase<T>& rhs, const unsigned long& ID) :
		bso::utilities::geometry::vertex(rhs)
	{  // initialize by Eigen vector
		mID = ID;
		this->initializeVariables();
	} //
	
	node::~node()
	{  // dtor
		
	} //
	
	void node::updateNFS(const Eigen::Vector6i& EFS)
	{  // updates the nodal freedom signature with that of an element's node
		for (unsigned int i = 0; i < 6; ++i)
		{
			if (EFS[i] == 1)
			{
				mNFS[i] = 1;
			}
		}
	} // updateNFS()
	
	void node::addConstraint(const unsigned int& localDOF)
	{  // adds a constraint to the local DOF
		if (localDOF > 5)
		{
			std::stringstream errorMessage;
			errorMessage << "\nRequested a nodal constraint from DOF: " << localDOF << "\n"
									 << "which is out of range: [0-5] (x,y,z,rx,ry,rz)\n"
									 << "(bso/structural_design/element/node.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		mConstraints(localDOF) = 1;
	} //
	
	void node::addLoad(const load& l)
	{ 
		if (mLoads.find(l.loadCase()) == mLoads.end())
		{
			this->addLoadCase(l.loadCase());
		}
		mLoads[l.loadCase()](l.DOF()) += l.magnitude();

	} //
	
	void node::addDisplacements(const std::map<component::load_case, Eigen::VectorXd>& displacements)
	{
		mDisplacements.clear();
		for (const auto& i : displacements)
		{
			Eigen::Vector6d tempDisplacements;
			tempDisplacements.setZero();
			for (unsigned int j = 0; j < 6; ++j)
			{
				if (mNFS(j) == 1 && mConstraints(j) == 0)
				{
					tempDisplacements(j) = i.second[mNFT[j]];
				}
			}
			mDisplacements[i.first] = tempDisplacements;
		}
	}
	
	void node::addLoadCase(load_case lc)
	{
		mLoads[lc] = Eigen::Vector6d::Zero();
	} // addLoadCase()
	
	void node::clearDisplacements()
	{ // 
		mDisplacements.clear();
	} // clearDisplacements()
	
	Eigen::Vector6d node::getDisplacements(component::load_case lc) const
	{
		auto lcSearch = mDisplacements.find(lc);
		if (lcSearch == mDisplacements.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not access displacements for load case:\n"
									 << lc << ".\n"
									 << "In node: " << *this << ".\n"
									 << "(bso/structural_design/element/node.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		return lcSearch->second; 
	}
	
	Eigen::Vector6d node::getLoads(component::load_case lc) const
	{
		auto lcSearch = mLoads.find(lc);
		if (lcSearch == mLoads.end())
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not access loads for load case:\n"
									 << lc << ".\n"
									 << "In node: " << *this << ".\n"
									 << "(bso/structural_design/element/node.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		return lcSearch->second;
	}
	
	const int& node::getConstraint(const unsigned int& n) const
	{
		if (n > 5)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not access nodal constraint at DOF:\n"
									 << n << ", which is out of range: [0-5] (x,y,z,rx,ry,rz).\n"
									 << "In node: " << *this << ".\n"
									 << "(bso/structural_design/element/node.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		return mConstraints[n];
	}
	
	const int& node::getNFS(const unsigned int& n) const
	{
		if (n > 5)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not access nodal freedom signature at DOF:\n"
									 << n << ", which is out of range: [0-5] (x,y,z,rx,ry,rz).\n"
									 << "In node: " << *this << ".\n"
									 << "(bso/structural_design/element/node.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		return mNFS[n];
	}
	
	std::vector<component::load_case> node::getLoadCases() const
	{
		std::vector<component::load_case> temp;
		for (auto& i : mLoads) temp.push_back(i.first);
		return temp;
	} // get LoadCases()
	
	void node::generateNFT(unsigned long& NFM)
	{ // 
		for (unsigned int i = 0; i < 6; ++i)
		{
			if (mNFS(i) == 1 && mConstraints(i) == 0)
			{
				mNFT[i] = NFM++;
			}
		}
	} // generateNFT()
	
	unsigned long node::getGlobalDOF(const unsigned int& localDOF) const
	{ 
		if (localDOF > 5)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, could not access nodal global DOF at local DOF:\n"
									 << localDOF << ", which is out of range: [0-5] (x,y,z,rx,ry,rz).\n"
									 << "In node: " << *this << ".\n"
									 << "(bso/structural_design/element/node.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		if (mNFS(localDOF) == 0 || mConstraints(localDOF) == 1)
		{
			std::stringstream errorMessage;
			errorMessage << "Error, requesting a non-existing global DOF from a node: "
									 << *this << " at local DOF: " << localDOF << ".\n"
									 << "(bso/structural_design/element/node.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		if (mNFT.find(localDOF) == mNFT.end())
		{
			std::stringstream errorMessage;
			errorMessage << "Error, could not find the global DOF from a node.\n"
									 << "(bso/structural_design/element/node.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		return mNFT.find(localDOF)->second;
	} //
	
	bool node::checkLoad(component::load_case lc, const unsigned int& localDOF, double& load) const
	{ 
		if (mNFS(localDOF) == 1 && mConstraints(localDOF) == 0)
		{
			if (mLoads.find(lc) != mLoads.end())
			{
				load = mLoads.find(lc)->second(localDOF);
				if (load != 0) return true;
				else return false;
			}
			else return false;
		}
		else return false;
	} //
	
} // namespace element
} // namespace structural_design
} // namespace bso

#endif // SD_NODE_CPP