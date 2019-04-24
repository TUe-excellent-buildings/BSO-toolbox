#ifndef SD_ELEMENT_HPP
#define SD_ELEMENT_HPP

#include <bso/structural_design/component/load_case.hpp>
#include <bso/structural_design/element/node.hpp>

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <vector>
#include <map>

namespace bso { namespace structural_design { namespace element {
	
	typedef Eigen::Triplet<double> triplet;
	
	class element
	{
	private:
		unsigned long mID; // the ID of this element
	protected:
		// nodes, constraints and stiffness matrices
		std::vector<node*> mNodes; // pointers to the nodes of this element
		Eigen::Vector6i mEFS; // the freedom signature of that belongs to each node of this element

		std::vector<unsigned long> mEFT; // element freedom table, the global DOF indices of each DOF of this element's node
		std::vector<bool> mConstraints; // constraints per DOF
		
		Eigen::MatrixXd mOriginalSM; // the element stiffness matrix before applying topology densities
		Eigen::MatrixXd mSM; // the element stiffness matrix after applying topology densities
		
		std::map<load_case*, Eigen::VectorXd> mDisplacements;
		std::map<load_case*, double> mEnergies;
		
		// Variables related to the stiffness of this element, mostly related to topology optimization
		double mDensity = 1.0; // element density
		double mEmin; // the minimum youngs modulus [N/mm²]
		double mE0; // the initial youngs modulus [N/mm²]
		double mE; // the actual youngs modulus [N/mm²]
		
		// for identification
		bool mIsTruss = false;
		bool mIsBeam = false;
		bool mIsFlatShell = false;
		bool mIsQuadHexahedron = false;
		bool mVisualize = true;
		bool mActiveInCompliance = true;
		bool mVisualizeTransparant = false;
		
	public:
		element(const unsigned long& ID, const double& E, const double& ERelativeLowerBound = 1e-6);
		virtual ~element();
		
		virtual void generateEFT();
		virtual std::vector<triplet> getSMTriplets() const;
		virtual void computeResponse(load_case* lc);
		virtual void clearResponse();
		
		virtual double getProperty(std::string) const = 0;
		
		const unsigned long& ID() const {return mID;}
		virtual const bool& isTruss() const {return mIsTruss;}
		virtual const bool& isBeam() const {return mIsBeam;}
		virtual const bool& isFlatShell() const {return mIsFlatShell;}
		virtual const bool& isQuadHexahedron() const {return mIsQuadHexahedron;}
		virtual const bool& visualize() const {return mVisualize;}
		virtual bool& visualize() {return mVisualize;}
		virtual const bool& visualizeTransparant() const {return mVisualizeTransparant;}
		virtual const bool& isActiveInCompliance() const {return mActiveInCompliance;}
		virtual const double& getDensity() const {return mDensity;}
		virtual double& density() {return mDensity;}
		virtual const double& getEnergy(load_case* lc, const std::string& = "") const;
		virtual const Eigen::VectorXd& getDisplacements(load_case* lc) const;
		
	};
	
} // namespace element
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/element/element.cpp>

#endif // SD_ELEMENT_HPP