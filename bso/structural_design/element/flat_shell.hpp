#ifndef SD_FLAT_SHELL_ELEMENT_HPP
#define SD_FLAT_SHELL_ELEMENT_HPP

namespace bso { namespace structural_design { namespace element {
	
	class flat_shell : public bso::utilities::geometry::quadrilateral,
										 public element
	{
	private:
		double mThickness;
		double mPoisson;
		
		double mShearEnergy;
		double mAxialEnergy;
		double mBendEnergy;
		
		Eigen::MatrixXd mSMNormal;
		Eigen::MatrixXd mSMShear;
		Eigen::MatrixXd mSMBending;
		
		Eigen::MatrixXd mT;
		Eigen::MatrixXd mETermSolid; // 3x3 matrix with normal- and shear terms
		Eigen::MatrixXd mB1, mB2, mB3, mB4, mBAv; // 3x8 (strain-displacement) matrices for in-plane behaviour
		
		std::map<load_case, std::map<std::string, double>> mSeparatedEnergies;
		Eigen::VectorXd melementDisp8DOF;
		Eigen::Vector3d mStress;
		Eigen::VectorXd mE0K0U;
		
		template<class CONTAINER>
		void deriveStiffnessMatrix(CONTAINER& l);
	public:
		template<class CONTAINER>
		flat_shell(const unsigned long& ID, const double& E, const double& thickness, const double& poisson,
							 CONTAINER& l, const double ERelativeLowerBound = 1e-6, const double geomTol = 1e-3);
		flat_shell(const unsigned long& ID, const double& E, const double& thickness, const double& poisson,
							 std::initializer_list<node*>&& l, const double ERelativeLowerBound = 1e-6, const double geomTol = 1e-3);
		~flat_shell();
		
		void computeResponse(load_case lc);
		void clearResponse();
		
		const double& getEnergy(load_case lc, const std::string& type = "") const;
		double getTotalEnergy(const std::string& type = "") const;
		
		double getProperty(std::string var) const;
		double getVolume() const;
		double getStressAtCenter(const double& alpha = 0, const double& beta = 1.0 / sqrt(3)) const;
		Eigen::VectorXd getStressSensitivityTermAE(const unsigned long freeDOFs, const double& alpha = 0) const;
		Eigen::VectorXd getStressSensitivity(Eigen::MatrixXd& Lamda, const double& penal = 1, const double& beta = 1.0 / sqrt(3)) const;
		bso::utilities::geometry::vertex getCenter() const;
		Eigen::VectorXd getStress() {return mStress;} // for unit test
	};
	
} // namespace element
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/element/flat_shell.cpp>

#endif // SD_FLAT_SHELL_ELEMENT_HPP