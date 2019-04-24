#ifndef SD_LOAD_CASE_HPP
#define SD_LOAD_CASE_HPP

namespace bso { namespace structural_design { namespace component {

	class load_case
	{
	private:
		std::string mDescription = "";
	public:
		load_case(std::string description = "");
		~load_case();
		
		const std::string& description() const;
		
		friend std::ostream& operator << (std::ostream& stream, const load_case& lc);
	};
	
} // namespace component
namespace element {
	typedef component::load_case load_case;
}
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/component/load_case.cpp>

#endif // SD_ELEMENT_HPP