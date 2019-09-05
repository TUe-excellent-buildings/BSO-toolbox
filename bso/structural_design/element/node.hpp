#ifndef SD_NODE_HPP
#define SD_NODE_HPP

#include <bso/structural_design/component/load.hpp>
#include <bso/utilities/geometry.hpp>

#include <Eigen/Dense>

#include <map>
#include <stdexcept>

namespace Eigen {typedef Matrix<int, 6, 1> Vector6i;}
namespace Eigen {typedef Matrix<double, 6, 1> Vector6d;}

namespace bso { namespace structural_design { namespace element {

	class node : public bso::utilities::geometry::vertex
	{
	private:
		unsigned long mID;
		std::map<unsigned int, unsigned long> mNFT; // nodal freedom table, contains the global indices of the node's DOFs
		Eigen::Vector6i mNFS; // nodal freedom signature, for each local DOF index, contains info if it is active or not
		Eigen::Vector6i mConstraints; // constraints, for each local DOF index, contains if it is constrained or not
		std::map<component::load_case, Eigen::Vector6d> mLoads; // indexe dby load case, contains for each local DOF index, the magnitude of the load
		std::map<component::load_case, Eigen::Vector6d> mDisplacements; // indexed by load case, contains for each local DOF index, the magnitude of the displacement
		
		void initializeVariables();
	public:
		node(const std::initializer_list<double>&& l, const unsigned long& ID); // initialize by initializer list
		template<class T> node(const Eigen::MatrixBase<T>& rhs, const unsigned long& ID); // initialize by Eigen vector
		~node(); // dtor
		
		void updateNFS(const Eigen::Vector6i& EFS); // updates the nodal freedom signature with that of an element
		void addConstraint(const unsigned int& localDOF); // adds a constraint to the local DOF
		void addLoad(const load& l);
		void addDisplacements(const std::map<component::load_case, Eigen::VectorXd>& displacements);
		void addLoadCase(load_case lc);
		void clearDisplacements();

		Eigen::Vector6d getDisplacements(component::load_case lc) const;
		Eigen::Vector6d getLoads(component::load_case lc) const;
		const int& getConstraint(const unsigned int& n) const;
		const int& getNFS(const unsigned int& n) const;
		const unsigned long& ID() const {return mID;}
		std::vector<component::load_case> getLoadCases() const;
		
		void generateNFT(unsigned long& NFM); // this will map the local DOFs to the global DOFs of this node
		unsigned long getGlobalDOF(const unsigned int& localDOF) const;
		
		bool checkLoad(component::load_case lc, const unsigned int& localDOF, double& load) const;
	};

} // namespace element
} // namespace structural_design
} // namespace bso

#include <bso/structural_design/element/node.cpp>

#endif // SD_NODE_HPP