#ifndef SD_QUAD_HEXAHEDRON_ELEMENT_HPP
#define SD_QUAD_HEXAHEDRON_ELEMENT_HPP

namespace bso { namespace structural_design { namespace element {
	
	class quad_hexahedron : public bso::utilities::geometry::quad_hexahedron,
													public element
	{
	private:
		double mPoisson;
		
		Eigen::MatrixXd mT;

		template<class CONTAINER>
		void deriveStiffnessMatrix(CONTAINER& l);
	public:
		template<class CONTAINER>
		quad_hexahedron(const unsigned long& ID, const double& E, const double& poisson,
										CONTAINER& l, const double ERelativeLowerBound = 1e-6,
										const double geomTol = 1e-3);
		quad_hexahedron(const unsigned long& ID, const double& E, const double& poisson,
										std::initializer_list<node*>&& l, const double ERelativeLowerBound = 1e-6,
										const double geomTol = 1e-3);
		~quad_hexahedron();
			
		double getProperty(std::string var) const;
		double getVolume() const;
		bso::utilities::geometry::vertex getCenter() const;
	};
	
} // namespace element
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/element/quad_hexahedron.cpp>

#endif // SD_QUAD_HEXAHEDRON_ELEMENT_HPP