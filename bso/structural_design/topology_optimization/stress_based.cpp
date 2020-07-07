#ifndef SD_TOPOPT_STRESS_BASED_CPP
#define SD_TOPOPT_STRESS_BASED_CPP

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>

#include <bso/structural_design/topology_optimization/MMA.hpp>

namespace bso { namespace structural_design { namespace topology_optimization {

class STRESS_BASED;

} // namespace topology_optimization

template <>
void sd_model::topologyOptimization<topology_optimization::STRESS_BASED>(
					const double& volinit, const double& rMin, const double& penal,
					const double& xMin, const double& TStrength, const double& CStrength, const double& tolerance, const double& move)
{
	std::ostream out(mTopOptStreamBuffer);
	unsigned int numEle = mFEA->getElements().size();
	double totVolume = 0; // initialised at 0, before each element volumes are added
	double c; // sum of all the elements compliances
	double eps = sqrt(xMin);
	double alpha = (CStrength - TStrength) / (sqrt(3) * (CStrength + TStrength));
	double beta = (2 * CStrength * TStrength) / (sqrt(3) * (CStrength + TStrength));

	Eigen::VectorXd x(numEle), xPhys(numEle), xNew(numEle), xChange(numEle),
					volume(numEle), s(numEle), dv(numEle); // initialise containers for element values
	Eigen::MatrixXd ds(numEle,numEle); // initialise container for stress sensitivity matrix
	Eigen::VectorXd vf;
	vf.setOnes(10); // initialize vector with last 10 volume-fraction values for convergence criterion

	// prepare filter
	Eigen::SparseMatrix<double> H(numEle, numEle); // contains filter vectors for each element
	Eigen::VectorXd Hs; // contains sums of filter vectors of each element
	Hs.setZero(numEle); // initialise to size and all zeros
	typedef Eigen::Triplet<double> T;
	std::vector<T> tripletList;

	unsigned int eleIndexI = 0;
	for (auto& i : mFEA->getElements())
	{ // for each element i
		volume(eleIndexI) = i->getVolume();
		x(eleIndexI) = volinit;
		i->updateDensity(volinit,penal,"regularSIMP");

		unsigned int eleIndexJ = 0;
		for (auto& j : mFEA->getElements())
		{ // and for each element j
			// calculate distance center to center distance r_ij between element i and j
			bso::utilities::geometry::line_segment lij =
				{i->getCenter(), j->getCenter()};
			double rij = lij.getLength();

			if (rij < rMin)
			{
				tripletList.push_back(T(eleIndexI, eleIndexJ, (rMin - rij)));
				Hs(eleIndexI) += rMin - rij;
			}
			++eleIndexJ;
		}
		++eleIndexI;
	}
	H.setFromTriplets(tripletList.begin(), tripletList.end());
	totVolume = volume.sum();
	tripletList.clear();
	out << "Total Volume: " << totVolume << std::endl;

	// initialise iteration
	xPhys = x;
	const unsigned long freeDOFs = mFEA->getDOFCount();
	double change = 1.0;
	double changevol = 1.0;
	int loop = 0;
	double loopStart = clock(), iterationStart, startMMA, endMMA, timeEnd = 0.0;

	// define MMA parameters
	const int n = numEle;	// nr of variables
	const int m = numEle;	// nr of constraints
	Eigen::VectorXd xmin(n), xmax(n), xold1(n), xold2(n), low(n), upp(n);
	xmin.setConstant(xMin);
	xmax.setOnes();
	xold1 = x;
	xold2 = xold1;
	low = xmin;
	upp = xmax;
	const double a0 = 1.0;
	Eigen::VectorXd amma(m), cmma(m), dmma(m);
	amma.setZero();
	cmma.setConstant(1000);
	dmma.setZero();

	// start iteration
	while (changevol > tolerance || s.maxCoeff() > 1e-5)
	{
			iterationStart = clock();
			if (loop%20 == 0)
			{
					out << std::endl
							<< std::setw(5)  << std::left << "Loop"
							<< std::setw(13) << std::left << "Compliance"
							<< std::setw(13) << std::left << "Volfrac."
							<< std::setw(13) << std::left << "Changevol."
							<< std::setw(13) << std::left << "Stress"
							<< std::setw(10) << std::left << "TimeMMA"
							<< std::setw(10) << std::left << "Time" << std::endl;
			}

			++loop;
			c = 0;

			double volfrac = (volume * xPhys.transpose()).trace() / totVolume;
			if (loop < 11) {vf(loop-1) = volfrac;}
			else
			{
				for (int i = 0; i < 9; ++i) {vf(i) = vf(i+1);}
				vf(9) = volfrac;
			}
			if (loop > 10)
			{
				changevol = std::abs((vf.head(5).sum()-vf.tail(5).sum())/vf.tail(5).sum());
			}

			// FEA
			mFEA->generateGSM();
			mFEA->solve("SimplicialLDLT");

			Eigen::MatrixXd ae;
			ae.setZero(freeDOFs,numEle); // adjoint load vectors for stress sensitivity calculation

			// objective function and sensitivity analysis (retrieve data from FEA)
			eleIndexI = 0;
			for (auto& i : mFEA->getElements())
			{
				c 						+= i->getTotalEnergy();
				dv(eleIndexI) =  i->getVolume();
				dv(eleIndexI) /= totVolume;
				s(eleIndexI)  =  i->getStressAtCenter(alpha, beta); // gives Drucker-Prager stress for unequal strength limits, and Von Mises stress for equal strength limits
				s(eleIndexI) +=  eps - 1 - eps / xPhys(eleIndexI); // relaxed stress, should be < 0
				ae.col(eleIndexI) = i->getStressSensitivityTermAE(freeDOFs, alpha); // adjoint load vectors are required for the stress sensitivity calculation
				++eleIndexI;
			}

			Eigen::MatrixXd Lambda = mFEA->solveAdjoint(ae); // solve equilibrium equations with adjoint load vectors [K*Lambda(e) = a(e)]

			// stress sensitivity calculation
			eleIndexI = 0;
			for (auto& i : mFEA->getElements())
			{
				ds.col(eleIndexI) = i->getStressSensitivity(Lambda, penal, beta);
				ds(eleIndexI,eleIndexI) += eps / pow(xPhys(eleIndexI),2); // add relaxation term on diagonal
				++eleIndexI;
			}

			// filter stress sensitivity
			for (unsigned int i = 0; i < numEle; i++)
			{
				for (unsigned int j = 0; j < numEle; j++)
				{
					ds(i,j) /= Hs(j);
				}
				ds.row(i) = ds.row(i) * H.transpose();
			}

			// filter volume sensitivity
			for (unsigned int i = 0; i < numEle; i++)
			{
				dv(i) /= Hs(i);
			}
			dv = H * dv;

			// update of design variables and physical densities (MMA solver)
			startMMA = clock();
			topology_optimization::MMA MMA;
			MMA.MMAsub(m,n,loop,x,xmin,xmax,xold1,xold2,volfrac,dv,s,ds,low,upp,a0,amma,cmma,dmma,move);
			low = MMA.getLow();
			upp = MMA.getUpp();
			xNew = MMA.getxNew();
			xold2 = xold1;
			xold1 = x;
			endMMA = clock();

			// density filter
			xPhys = H * xNew;
			for (unsigned int i = 0; i < numEle; i++)
			{
				xPhys(i) /= Hs(i);
			}

			eleIndexI = 0;
			for (auto& i : mFEA->getElements())
			{
				i->updateDensity(xPhys(eleIndexI), penal, "regularSIMP");
				++eleIndexI;
			}

			timeEnd = clock();
			out << std::setw(5)  << std::left << loop
					<< std::setw(13) << std::left << 2*c
					<< std::setw(13) << std::left << volfrac
					<< std::setw(13) << std::left << changevol
					<< std::setw(13) << std::left << s.maxCoeff()
					<< std::setw(10) << std::left << (endMMA - startMMA)/CLOCKS_PER_SEC
					<< std::setw(10) << std::left << (timeEnd - iterationStart)/CLOCKS_PER_SEC << std::endl;

			x = xNew;

	} // end of iteration
	out << "Topology optimisation successfully finished after: "
			<< (timeEnd - loopStart)/CLOCKS_PER_SEC << " seconds."
			<< std::endl << std::endl;
} // topology_optimization::STRESS_BASED

} // namespace structural_design
} // bso

#endif // SD_TOPOPT_STRESS_BASED_CPP