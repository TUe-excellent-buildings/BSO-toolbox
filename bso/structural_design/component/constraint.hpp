#ifndef SD_CONSTRAINT_COMPONENT_HPP
#define SD_CONSTRAINT_COMPONENT_HPP

namespace bso { namespace structural_design { namespace component {
	
	class constraint
	{
	private:
		unsigned int mDOF;
	public:
		constraint(const unsigned int& DOF);
		~constraint();

		const unsigned int& DOF() const {return mDOF;};
	};
	
} // namespace component
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/constraint.cpp>

#endif // SD_CONSTRAINT_COMPONENT_HPP