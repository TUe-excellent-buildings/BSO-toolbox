#ifndef SD_QUAD_HEXAHEDRON_ELEMENT_HPP
#define SD_QUAD_HEXAHEDRON_ELEMENT_HPP

namespace bso { namespace structural_design { namespace element {
	
	class quad_hexahedron : public bso::utilities::geometry::quad_hexahedron,
													public element
	{
	private:
		double mPoisson;
		
		Eigen::MatrixXd mT;
		Eigen::MatrixXd mETermSolid; // 6x6 matrix with normal- and shear elasticity terms
		Eigen::MatrixXd mBSum, mBAv; // sum and average of strain-displacement matrices in each integration point
		Eigen::VectorXd mDispLoc;
		Eigen::Vector6d mStress;

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

		void computeResponse(load_case lc);

		double getProperty(std::string var) const;
		double getVolume() const;
		bso::utilities::geometry::vertex getCenter() const;
		double getStressAtCenter (const double& alpha = 0, const double& beta = 1.0 / sqrt(3)) const;
		Eigen::VectorXd getStressSensitivityTermAE(const unsigned long freeDOFs, const double& alpha = 0) const;
		Eigen::VectorXd getStressSensitivity(Eigen::MatrixXd& Lamda, const double& penal = 1, const double& beta = 1.0 / sqrt(3)) const;
		Eigen::Vector6d getStress() {return mStress;} // for unit test
	};
	
} // namespace element
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/element/quad_hexahedron.cpp>

#endif // SD_QUAD_HEXAHEDRON_ELEMENT_HPP