#ifndef SD_CONSTRAINT_COMPONENT_CPP
#define SD_CONSTRAINT_COMPONENT_CPP

namespace bso { namespace structural_design { namespace component {
	
	constraint::constraint(const unsigned int& DOF)
	{
		mDOF = DOF;
		
		if (mDOF > 5)
		{
			std::stringstream errorMessage;
			errorMessage << "\nAssigning constraint to DOF: " << mDOF << "\n"
									 << "Which is an invalid value for a DOF.\n"
									 << "(bso/structural_design/component/constraint.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // ctor
	
	constraint::~constraint()
	{
		
	} // dtor
	
} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_CONSTRAINT_COMPONENT_CPP