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
		std::vector<element::load_case*> mLoadCases;
		std::map<element::load_case*,Eigen::VectorXd> mLoads;
		std::map<element::load_case*,Eigen::VectorXd> mDisplacements;
		
		Eigen::SparseMatrix<double> mGSM;
		bool mSystemInitialized = false;
		
		// solvers
		void simplicialLLT();
		void BiCGSTAB();
		void scaledBiCGSTAB();
	public:
		fea();
		~fea();
		
		element::node* addNode(const bso::utilities::geometry::vertex& point);
		void addElement(element::element* ele);
		
		void generateGSM();
		void clearResponse();
		
		void solve(std::string solver = "SimplicialLLT");
		
		auto nodesBegin()    const {return mNodes.begin();}
		auto nodesEnd()      const {return mNodes.end();}
		auto elementsBegin() const {return mElements.begin();}
		auto elementsEnd()   const {return mElements.end();}
	};
	
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/fea.cpp>

#endif // SD_FEA_HPP