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
		
		std::map<load_case, std::map<std::string, double>> mSeparatedEnergies;
		
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
		bso::utilities::geometry::vertex getCenter() const;
	};
	
} // namespace element
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/element/flat_shell.cpp>

#endif // SD_FLAT_SHELL_ELEMENT_HPP