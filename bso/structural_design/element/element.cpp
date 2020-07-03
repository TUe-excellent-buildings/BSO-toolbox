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
		mEFT.clear();unsigned int count = 0;
		unsigned int localDOF = 0;
		for (const auto& i : mNodes)
		{
			for (unsigned int j = 0; j < 6; ++j)
			{
				if (i->getNFS(j) == 0 && mEFS(j) == 1)
				{
					std::stringstream errorMessage;
					errorMessage << "Error, nodal DOF is not active, while it should be.\n"
											 << "Encountered error while generating element freedom table.\n"
											 << "(bso/structural_design/element.cpp)" << std::endl;
					throw std::runtime_error(errorMessage.str());
				}
				if (mEFS(j) == 1)
				{
					try
					{
						if (i->getConstraint(j) == 0) mEFT[localDOF] = i->getGlobalDOF(j);
					}
					catch (std::exception& e)
					{
						std::stringstream errorMessage;
						errorMessage << "\nError while generating element freedom table.\n"
												 << "Received the following error:\n" << e.what()
												 << "\n(bso/structural_design/element.cpp)" << std::endl;
						throw std::runtime_error(errorMessage.str());
					}
					++localDOF;
				}
			}
		}
	} //

	std::vector<triplet> element::getSMTriplets() const
	{ //
		std::vector<triplet> tripletList;
		for (unsigned int m = 0; m < mSM.rows(); ++m)
		{
			for (unsigned int n = 0; n < mSM.cols(); ++n)
			{
				if ((mSM(m,n) != 0) && (mEFT.find(m) != mEFT.end()) && (mEFT.find(n) != mEFT.end()))
				{
					tripletList.push_back(triplet(mEFT.at(m),mEFT.at(n),mSM(m,n))); // have to use map::at() because triplet initializer takes non const argument by reference
				}
			}
		}
		return tripletList;
	} //

	void element::computeResponse(load_case lc)
	{ //
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
					++dispIte;
				}
			}
		}
		mDisplacements[lc] = elementDisplacements;
		mEnergies[lc] = 0.5 * elementDisplacements.transpose() * mSM * elementDisplacements;
		mTotalEnergy += mEnergies[lc];
	} //
	
	void element::clearResponse()
	{ // 
		mDisplacements.clear();
		mEnergies.clear();
		mTotalEnergy = 0;
	} // clearResponse()
	
	void element::updateDensity(const double& x, const double& penal /*= 1*/, std::string type /*= "modifiedSIMP"*/)
	{
		if (type == "modifiedSIMP")
		{
			mDensity = x;
			mE = mEmin + std::pow(mDensity,penal)*(mE0 - mEmin);
			mSM = (mE/mE0) * mOriginalSM;
		}
		else if (type == "regularSIMP")
		{
			mDensity = x;
			mE = std::pow(mDensity,penal)*mE0;
			mSM = (mE/mE0) * mOriginalSM;
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nTrying to update density with unknown update type:\n"
									 << type << "\n"
									 << "(bso/structural_design/element/element.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // updateDensity()
	
	double element::getTotalEnergy(const std::string& type /*= ""*/) const
	{
		if (type == "") return mTotalEnergy;
		else return 0.0;
	} // getTotalEnergy()
	
	double element::getEnergySensitivity(const double& penal /* 1*/) const
	{
		return mTotalEnergy * (-(penal*pow(mDensity,penal - 1)*(mE0 - mEmin))/(mEmin + pow(mDensity,penal)*(mE0 - mEmin)));
	} // getEnergySensitivity()
	
	double element::getVolumeSensitivity() const
	{
		return this->getVolume();
	} // getVolumeSensitivity()

	double element::getStressAtCenter(const double& alpha /* 0*/, const double& beta /* 1.0 / sqrt(3)*/) const
	{
		std::stringstream errorMessage;
		errorMessage << "\nCannot call getStressAtCenter() for this element type\n"
							<< "(bso/structural_design/element/element.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	} // getStressCenter() gives error (standard) except for elements in which the function is overridden

	Eigen::VectorXd element::getStressSensitivityTermAE(const unsigned long freeDOFs, const double& alpha /* 0*/) const
	{
		std::stringstream errorMessage;
		errorMessage << "\nCannot call getStressSensitivityTermAE() for this element type\n"
							<< "(bso/structural_design/element/element.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	} // getStressSensitivityTermAE() gives error (standard) except for elements in which the function is over-written

	Eigen::VectorXd element::getStressSensitivity(Eigen::MatrixXd& Lamda, const double& penal /* 1*/, const double& beta /* 1.0 / sqrt(3)*/) const
	{
		std::stringstream errorMessage;
		errorMessage << "\nCannot call getStressSensitivity() for this element type\n"
							<< "(bso/structural_design/element/element.cpp)" << std::endl;
		throw std::runtime_error(errorMessage.str());
	} // getStressSensitivity() gives error (standard) except for elements in which the function is over-written

	const double& element::getEnergy(load_case lc, const std::string& type/*= ""*/) const
	{ //
		if (mEnergies.find(lc) != mEnergies.end())
		{
			return mEnergies.find(lc)->second;
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when retrieving energies from an element.\n"
									 << "Could not find it for load case: " << lc << "\n"
									 << "(bso/structural_design/element.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	} //

	const Eigen::VectorXd& element::getDisplacements(load_case lc) const
	{ //
		if (mDisplacements.find(lc) != mDisplacements.end())
		{
			return mDisplacements.find(lc)->second;
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when retrieving displacement from an element.\n"
									 << "Could not find it for load case: " << lc << "\n"
									 << "(bso/structural_design/element.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	} //

} // namespace element
} // namespace structural_design
} // namespace bso

#endif // SD_ELEMENT_CPP