#ifndef SD_LOAD_CASE_CPP
#define SD_LOAD_CASE_CPP

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
	
	std::ostream& operator << (std::ostream& stream, const load_case& lc)
	{
		stream << lc.mDescription;
		return stream;
	}
	
} // namespace element
} // namespace structural_design
} // namespace bso

#endif // SD_LOAD_CASE_CPP