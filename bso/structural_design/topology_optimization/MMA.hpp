#ifndef MMA_HPP
#define MMA_HPP

/*
MMA OPTIMIZATION ALGORITHM (header file)

I. Bouw
version 07-07-2020

C++ code of the Method of Moving Asymptotes, translated from the MMA-code written in MATLAB by Krister Svanberg (Svanberg, 1987).
The original work can be downloaded from http://www.smoptit.se/ under de GNU General Public License.
The user should refer to the academic work of Krister Svanberg when work will be published in which this code is used.
For a description of the original work see:
Svanberg, K. (1987). The method of moving asymptotes - A new method for structural optimization. International Journal for Numerical Methods in Engineering, 24(2), 359-373. doi:10.1002/nme.1620240207.

This is the C++ version (header file) of the MMA code of Krister Svanberg, written by Irma Bouw, for use in the BSO Toolbox (github.com/TUe-excellent-buildings/BSO-toolbox).
*/

namespace bso { namespace structural_design { namespace topology_optimization {

	class MMA
	{
		private:
			Eigen::VectorXd mLow; // vector with lower bounds (after updating)
			Eigen::VectorXd mUpp; // vector with upper bounds (after updating)
			Eigen::VectorXd mxmma;
			Eigen::VectorXd mymma;
			double mzmma;
			Eigen::VectorXd mlamma;
			Eigen::VectorXd mxsimma;
			Eigen::VectorXd metamma;
			Eigen::VectorXd mmumma;
			double mzetmma;
			Eigen::VectorXd msmma;
		public:
			void MMAsub(const int& m, const int& n, const int& iter, Eigen::VectorXd& xval,
						Eigen::VectorXd& xmin, Eigen::VectorXd& xmax, Eigen::VectorXd& xold1,
						Eigen::VectorXd& xold2, const double& f0val, Eigen::VectorXd& df0dx,
						Eigen::VectorXd& fval, Eigen::MatrixXd& dfdx, Eigen::VectorXd& low,
						Eigen::VectorXd& upp, const double& a0, Eigen::VectorXd& amma,
						Eigen::VectorXd& cmma, Eigen::VectorXd& dmma, const double& move = 1.0);

			void subsolv(const int& m, const int& n, const double& epsimin, Eigen::VectorXd& low,
						Eigen::VectorXd& upp, Eigen::VectorXd& alfa, Eigen::VectorXd& beta,
						Eigen::VectorXd& p0, Eigen::VectorXd& q0, Eigen::MatrixXd& P,
						Eigen::MatrixXd& Q, const double& a0, Eigen::VectorXd& amma,
						Eigen::VectorXd& bmma, Eigen::VectorXd& cmma, Eigen::VectorXd& dmma);

			// get output
			Eigen::VectorXd getLow() {return mLow;};
			Eigen::VectorXd getUpp() {return mUpp;};
			Eigen::VectorXd getxNew() {return mxmma;};
			Eigen::VectorXd getyNew() {return mymma;};
			double getzNew() {return mzmma;};
	};

} // namespace topology_optimization
} // namespace structural_design
} // bso

#include <bso/structural_design/topology_optimization/MMA.cpp>

#endif // MMA_HPP