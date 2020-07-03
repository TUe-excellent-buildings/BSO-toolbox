#ifndef SD_FEA_HPP
#define SD_FEA_HPP

#include <bso/structural_design/element/elements.hpp>
#include <Eigen/Sparse>
#include <Eigen/Dense>

namespace bso { namespace structural_design {
	
	class fea
	{
	private:
		std::vector<element::node*> mNodes;
		std::vector<element::element*> mElements;
		
		unsigned long mDOFCount = 0;
		std::vector<element::load_case> mLoadCases;
		std::map<element::load_case,Eigen::VectorXd> mLoads;
		std::map<element::load_case,Eigen::VectorXd> mDisplacements;
		
		Eigen::SparseMatrix<double> mGSM;
		bool mSystemInitialized = false;
		
		std::string msolver;
		Eigen::SimplicialLLT<Eigen::SparseMatrix<double> > mLLTSolver;
		Eigen::SimplicialLDLT<Eigen::SparseMatrix<double> > mLDLTSolver;

		// solvers
		void simplicialLLT();
		void simplicialLDLT();
		void BiCGSTAB();
		void scaledBiCGSTAB();
	public:
		fea();
		~fea();
		
		element::node* addNode(const bso::utilities::geometry::vertex& point);
		void addElement(element::element* ele);
		
		void generateGSM();
		void clearResponse();
		
		void solve(std::string solver = "SimplicialLDLT");
		Eigen::MatrixXd solveAdjoint(Eigen::MatrixXd& ae);
		bool isSingular();
		
		Eigen::VectorXd getDisplacements(element::load_case lc) const;
		const std::vector<element::node*>& getNodes() const {return mNodes;}
		std::vector<element::node*>& getNodes() {return mNodes;}
		const std::vector<element::element*>& getElements() const {return mElements;}
		std::vector<element::element*>& getElements() {return mElements;}
		const unsigned long& getDOFCount() const {return mDOFCount;}
	};
	
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/fea.cpp>

#endif // SD_FEA_HPP