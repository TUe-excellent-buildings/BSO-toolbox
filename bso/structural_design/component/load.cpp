#ifndef SD_LOAD_COMPONENT_CPP
#define SD_LOAD_COMPONENT_CPP

#include <sstream>
#include <stdexcept>

namespace bso { namespace structural_design { namespace component {
	
	load_case::load_case(std::string description /*= ""*/)
	{
		mDescription = description;
	}
	
	load_case::~load_case()
	{
		
	} // dtor
	
	const std::string& load_case::description() const
	{
		return mDescription;
	}
	
	bool load_case::operator == (const load_case& rhs) const
	{
		return (mDescription == rhs.mDescription);
	} // operator ==
	
	bool load_case::operator > (const load_case& rhs) const
	{
		return (mDescription > rhs.mDescription);
	} // operator >
	
	bool load_case::operator >= (const load_case& rhs) const
	{
		return (mDescription >= rhs.mDescription);
	} // operator >=
	
	bool load_case::operator < (const load_case& rhs) const
	{
		return (mDescription < rhs.mDescription);
	} // operator <
	
	bool load_case::operator <= (const load_case& rhs) const
	{
		return (mDescription <= rhs.mDescription);
	} // operator <=
	
	
	std::ostream& operator << (std::ostream& stream, const load_case& lc)
	{
		stream << lc.mDescription;
		return stream;
	}
	
	load::load(load_case lc, const double& magnitude, const unsigned int& DOF)
	{
		mLoadCase = lc;
		mMagnitude = magnitude;
		mDOF = DOF;
		
		if (mDOF > 5)
		{
			std::stringstream errorMessage;
			errorMessage << "\nAssigning load to DOF: " << mDOF << "\n"
									 << "Which is an invalid value for a DOF.\n"
									 << "(bso/structural_design/component/load.cpp)" << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	} // ctor
	
	load::~load()
	{
		
	} // dtor
	
	load& load::operator += (const double& rhs)
	{
		mMagnitude += rhs;
		return *this;
	} // operator +=
	
	load& load::operator -= (const double& rhs)
	{
		mMagnitude -= rhs;
		return *this;
	} // operator -=
	
	load& load::operator *= (const double& rhs)
	{
		mMagnitude *= rhs;
		return *this;
	} // operator *=
	
	load& load::operator /= (const double& rhs)
	{
		mMagnitude /= rhs;
		return *this;
	} // operator /=
	
	load operator + (load lhs, const double& rhs)
	{
		lhs += rhs;
		return lhs;
	} // operator +
	
	load operator - (load lhs, const double& rhs)
	{
		lhs -= rhs;
		return lhs;
	} // operator -
	
	load operator * (load lhs, const double& rhs)
	{
		lhs *= rhs;
		return lhs;
	} // operator *
	
	load operator / (load lhs, const double& rhs)
	{
		lhs /= rhs;
		return lhs;
	} // operator /
	
	
	
} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_LOAD_COMPONENT_CPP