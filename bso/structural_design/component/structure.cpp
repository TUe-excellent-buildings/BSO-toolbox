#ifndef SD_STRUCTURE_COMPONENT_CPP
#define SD_STRUCTURE_COMPONENT_CPP

#include <sstream>
#include <stdexcept>

namespace bso { namespace structural_design { namespace component {
	
	template <class CONTAINER>
	void structure::initFromContainer(const CONTAINER& l)
	{
		for (const auto& i : l)
		{
			std::string variable = i.first;
			double value = i.second;
			bool doubleAssignment = false;

			if (variable == "E")
			{
				mE = value;
				if (mEAssigned) doubleAssignment = true;
				else mEAssigned = true;
			}
			else if (variable == "ERelativeLowerBound")
			{
				mERelativeLowerBound = value;
				if (mERelativeLowerBoundAssigned) doubleAssignment = true;
				else mERelativeLowerBoundAssigned = true;
			}
			else if (variable == "poisson")
			{
				mPoisson = value;
				if (mPoissonAssigned) doubleAssignment = true;
				else mPoissonAssigned = true;
			}
			else if (variable == "A")
			{
				mA = value;
				if (mAAssigned) doubleAssignment = true;
				else mAAssigned = true;
			}
			else if (variable == "width")
			{
				mWidth = value;
				if (mWidthAssigned) doubleAssignment = true;
				else mWidthAssigned = true;
			}
			else if (variable == "height")
			{
				mHeight = value;
				if (mHeightAssigned) doubleAssignment = true;
				else mHeightAssigned = true;
			}
			else if (variable == "thickness")
			{
				mThickness = value;
				if (mThicknessAssigned) doubleAssignment = true;
				else mThicknessAssigned = true;
			}
			else
			{
				std::stringstream errorMessage;
				errorMessage << "\nWhen trying to initialize component with type: " << mType << "\n"
										 << "An unknown variable was assigned: \"" << variable << "\"\n"
										 << "With value: " << value << "\n"
										 << "(bso/structural_design/component/structure.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			
			if (doubleAssignment)
			{
				std::stringstream errorMessage;
				errorMessage << "\nWhen trying to initialize component with type: " << mType << "\n"
										 << "The following variable was assigned more than once: " << variable << "\n"
										 << "(bso/structural_design/component/structure.cpp)" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
		}
		
		std::vector<std::string> missingVariables;
		std::vector<std::string> additionalVariables;
		if (mType == "truss")
		{
			if (!mEAssigned) missingVariables.push_back("E");
			if (!mAAssigned) missingVariables.push_back("A");
			if (mPoissonAssigned) additionalVariables.push_back("poisson");
			if (mWidthAssigned) additionalVariables.push_back("width");
			if (mHeightAssigned) additionalVariables.push_back("height");
			if (mThicknessAssigned) additionalVariables.push_back("thickness");
		}
		else if (mType == "beam")
		{
			if (!mEAssigned) missingVariables.push_back("E");
			if (!mPoissonAssigned) missingVariables.push_back("poisson");
			if (!mWidthAssigned) missingVariables.push_back("width");
			if (!mHeightAssigned) missingVariables.push_back("height");
			if (mAAssigned) additionalVariables.push_back("A");
			if (mThicknessAssigned) additionalVariables.push_back("thickness");
		}
		else if (mType == "flat_shell")
		{
			if (!mEAssigned) missingVariables.push_back("E");
			if (!mPoissonAssigned) missingVariables.push_back("poisson");
			if (!mThicknessAssigned) missingVariables.push_back("thickness");
			if (mAAssigned) additionalVariables.push_back("A");
			if (mWidthAssigned) additionalVariables.push_back("width");
			if (mHeightAssigned) additionalVariables.push_back("height");
		}
		else if (mType == "quad_hexahedron")
		{
			if (!mEAssigned) missingVariables.push_back("E");
			if (!mPoissonAssigned) missingVariables.push_back("poisson");
			if (mAAssigned) additionalVariables.push_back("A");
			if (mWidthAssigned) additionalVariables.push_back("width");
			if (mHeightAssigned) additionalVariables.push_back("height");
			if (mThicknessAssigned) additionalVariables.push_back("thickness");
		}
		else if (mType == "none")
		{
			// do nothing
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nTrying to initialize component with unknown type: \n"
									 << "\"" << mType << "\"\n"
									 << "(bso/structural_design/component/structure.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		if (!missingVariables.empty())
		{
			std::stringstream errorMessage;
			errorMessage << "\nWhen initializing a component of type: " << mType << "\n"
									 << "The following variables were not provided: \n";
			for (const auto& i : missingVariables)
			{
				errorMessage << "- " << i << "\n";
			}
			errorMessage << "(bso/structural_design/component/structure.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		if (!additionalVariables.empty())
		{
			std::stringstream errorMessage;
			errorMessage << "\nWhen initializing a component of type: " << mType << "\n"
									 << "The following invalid variables were provided: \n";
			for (const auto& i : additionalVariables)
			{
				errorMessage << "- " << i << "\n";
			}
			errorMessage << "(bso/structural_design/component/structure.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
	} // ctor
	
	bool structure::checkBadRequest(std::string variable) const
	{
		bool check = (variable == "E" 				&& !mEAssigned 			 ||
				variable == "poisson" 	&& !mPoissonAssigned ||
				variable == "A" 				&& !mAAssigned			 ||
				variable == "width" 		&& !mWidthAssigned	 ||
				variable == "height" 		&& !mHeightAssigned	 ||
				variable == "thickness" && !mThicknessAssigned);
		if (check)		
		{
			std::stringstream errorMessage;
			errorMessage << "\nBad request for variable: " << variable << "\n"
									 << "From component with type: " << mType << "\n"
									 << "Was the request valid? Was it assigned to the component?\n"
									 << "(bso/structural_design/component/structure.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	return check;
	} // badRequest()
	
	structure::structure()
	{
		mType = "none";
	}
	
	template <class CONTAINER>
	structure::structure(const std::string& type, const CONTAINER& l)
	{
		mType = type;
		initFromContainer(l);
	} // ctor()
	
	structure::structure(const std::string& type, const std::initializer_list<std::pair<std::string,double>>& l)
	{
		mType = type;
		initFromContainer(l);
	} // ctor()
	
	structure::~structure()
	{
		
	} // dtor()

	const std::string& structure::type() const
	{
		return mType;
	} // type()
	
	const double& structure::E() const
	{
		checkBadRequest("E");
		return mE;
	} // E()
	
	const double& structure::ERelativeLowerBound() const
	{
		return mERelativeLowerBound;
	} // ERelativeLowerBound()
	
	const double& structure::poisson() const
	{
		checkBadRequest("poisson");
		return mPoisson;
	} // poisson()
	
	const double& structure::A() const
	{
		checkBadRequest("A");
		return mA;
	} // A()
	
	const double& structure::width() const
	{
		checkBadRequest("width");
		return mWidth;
	} // width()
	
	const double& structure::height() const
	{
		checkBadRequest("height");
		return mHeight;
	} // height()
	
	const double& structure::thickness() const
	{
		checkBadRequest("thickness");
		return mThickness;
	} // thickness()
	
	void structure::rescaleStructuralVolume(const double& scaleFactor)
	{
		double sqrtFactor = std::pow(scaleFactor,0.5);
		mA 					*= scaleFactor; 
		mWidth 			*= sqrtFactor; 
		mHeight 		*= sqrtFactor;
		mThickness 	*= scaleFactor; 
	}
		
} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_STRUCTURE_COMPONENT_CPP
