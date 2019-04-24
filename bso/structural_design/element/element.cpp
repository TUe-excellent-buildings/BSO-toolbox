#ifndef SD_ELEMENT_CPP
#define SD_ELEMENT_CPP

namespace bso { namespace structural_design { namespace element {

	element::element(const unsigned long& ID, const double& E, const double& ERelativeLowerBound /*=1e-6*/)
	{ //
		mID = ID;
		mE0 = E;
		mE = E;
		mEmin = E * ERelativeLowerBound;
		
	} // ctor

	element::~element()
	{ //
		
	} // dtor

	void element::generateEFT()
	{ //
		mEFT.clear();
		mConstraints.clear();
		for (const auto& i : mNodes)
		{
			for (unsigned int j = 0; j < 6; ++j)
			{
				if (i->getNFS(j) == 0 && mEFS(j) == 1)
				{
					std::stringstream errorMessage;
					errorMessage << "Error, nodal DOF is not active, while it should be.\n"
											 << "Encoutered error while generating element freedom table.\n"
											 << "(bso/structural_design/element.cpp)" << std::endl;
					throw std::runtime_error(errorMessage.str());
				}
				if (mEFS(j) == 1)
				{
					try
					{
						mConstraints.push_back(i->getConstraint(j));
						if (mConstraints.back() == 0) mEFT.push_back(i->getGlobalDOF(j));
					}
					catch (std::exception& e)
					{
						std::stringstream errorMessage;
						errorMessage << "\nError while generating element freedom table.\n"
												 << "Received the following error:\n" << e.what()
												 << "\n(bso/structural_design/element.cpp)" << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
				}
			}
		}
	} //

	std::vector<triplet> element::getSMTriplets() const
	{ //
		unsigned int numConstraints = 0;
		for (const auto i : mConstraints) numConstraints += i;

		if (mEFT.size() != (unsigned int)mSM.cols()-numConstraints)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when retrieving the stiffness terms of an element\n"
									 << "for global assembly. Expected an element freedom table of\n"
									 << "size: " << (unsigned int)mSM.cols()-numConstraints << ", while it is: " 
									 << mEFT.size() << ".\n(bso/structural_design/element.cpp)" 
									 << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
		
		std::vector<triplet> tripletList;
		for (unsigned int m = 0; m < mSM.rows(); ++m)
		{
			for (unsigned int n = 0; n < mSM.cols(); ++n)
			{
				if ((mSM(m,n) != 0) && (mConstraints[m] == 0) && (mConstraints[n] == 0))
				{
					tripletList.push_back(triplet(mEFT[m],mEFT[n],mSM(m,n)));
				}
			}
		}
		return tripletList;
	} //

	void element::computeResponse(load_case* lc)
	{ //
		unsigned int rowSize = mSM.rows();
		Eigen::VectorXd elementDisplacements(mSM.rows());
		elementDisplacements.setZero();
		auto dispIte = elementDisplacements.data();
		for (const auto& i : mNodes)
		{
			for (unsigned int j = 0; j < 6; ++j)
			{
				if (mEFS(j) == 1)
				{
					*dispIte = i->getDisplacements(lc)(j);
				}
			}
		}
		mDisplacements[lc] = elementDisplacements;
		mEnergies[lc] = 0.5 * elementDisplacements.transpose() * mSM * elementDisplacements;
	} //
	
	void element::clearResponse()
	{ // 
		mDisplacements.clear();
		mEnergies.clear();
	} // clearResponse()

	const double& element::getEnergy(load_case* lc, const std::string& /*= ""*/) const
	{ //
		if (mEnergies.find(lc) != mEnergies.end())
		{
			return mEnergies.find(lc)->second;
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when retrieving energies from an element.\n"
									 << "Could not find it for load case: " << *lc << "\n"
									 << "(bso/structural_design/element.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	} //

	const Eigen::VectorXd& element::getDisplacements(load_case* lc) const
	{ //
		if (mDisplacements.find(lc) != mDisplacements.end())
		{
			return mDisplacements.find(lc)->second;
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when retrieving displacement from an element.\n"
									 << "Could not find it for load case: " << *lc << "\n"
									 << "(bso/structural_design/element.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	} //

} // namespace element
} // namespace structural_design
} // namespace bso

#endif // SD_ELEMENT_CPP