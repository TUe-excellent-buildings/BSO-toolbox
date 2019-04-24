#ifndef SD_TRUSS_ELEMENT_HPP
#define SD_TRUSS_ELEMENT_HPP

#include <bso/utilities/geometry/line_segment.hpp>
#include <bso/structural_design/element/element.hpp>

namespace bso { namespace structural_design { namespace element {
	
	class truss : public bso::utilities::geometry::line_segment,
							 public element
	{
	private:
		double mA; // surface area [mm³]
		void deriveStiffnessMatrix();
	public:
		truss(const unsigned long& ID, const double& E, const double& A,
					std::initializer_list<node*>&& l, const double ERelativeLowerBound = 1e-6);
		~truss();
		
		double getProperty(std::string var) const;
	};
	
} // namespace element
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/element/truss.cpp>

#endif // SD_TRUSS_ELEMENT_HPP