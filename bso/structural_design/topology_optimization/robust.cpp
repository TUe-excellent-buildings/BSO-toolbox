#ifndef SD_TOPOPT_ROBUST_CPP
#define SD_TOPOPT_ROBUST_CPP

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <cmath>

namespace bso { namespace structural_design { namespace topology_optimization {

class ROBUST;

} // namespace topology_optimization

double densityProjection(double beta, double eta, double x)
{
	return (tanh(beta * eta) + tanh(beta * (x - eta))) /
				 (tanh(beta * eta) + tanh(beta * (1 - eta)));
}

template <>
void sd_model::topologyOptimization<topology_optimization::ROBUST>(const double& f, 
						const double& rMin, const double& penal, const double& xMove,
						const double& tolerance)
{
	std::ostream out(mTopOptStreamBuffer);
	unsigned int numEle = mFEA->getElements().size();
	double Mnd;
	double beta = 1.0;
	double etae = 0.8;
	double etan = 0.5;
	double totVolume = 0; // initialised at 0, before each element volumes are added
	double c; // sum of all the elements compliances (objective value)

	Eigen::VectorXd xe(numEle), xn(numEle), x(numEle), xTilde(numEle), xNew(numEle),
									xChange(numEle), volume(numEle), dc(numEle), dv(numEle); // initialise containers for element values

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
		x(eleIndexI) = f;
		i->updateDensity(f,penal);
		
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
	xTilde = x;
	
	eleIndexI = 0;
	for (auto& i : mFEA->getElements())
	{ // for each element i
		xn(eleIndexI) = densityProjection(beta,etan,xTilde(eleIndexI));
		xe(eleIndexI) = densityProjection(beta,etae,xTilde(eleIndexI));
		++eleIndexI;
	}
	
	out << "Total Volume: " << totVolume << std::endl;

	// initialise iteration
	double change = 1;
	double xMoveBeta = (xMove*tanh(0.5*beta))/(0.5*beta);
	int loop = 0;
	int loopBeta = 0;

	double loopStart = clock(), iterationStart, timeEnd = 0.0;
	out << std::endl
			<< std::setw(5)  << std::left << "loop"
			<< std::setw(10) << std::left << "loop_beta"
			<< std::setw(15) << std::left << "Objective"
			<< std::setw(15) << std::left << "Volume"
			<< std::setw(10)  << std::left << "Change"
			<< std::setw(10)  << std::left << "Mnd"
			<< std::setw(10)  << std::left << "Time" << std::endl;

	// start iteration
	while (change > tolerance)
	{
			iterationStart = clock();

			++loop;
			++loopBeta;
			c = 0;

			// FEA
			mFEA->generateGSM();
			mFEA->solve("SimplicialLDLT");

			// objective function and sensitivity analysis (retrieve data from FEA)
			eleIndexI = 0;
			for (auto& i : mFEA->getElements())
			{
				c 						+= i->getTotalEnergy();
				dc(eleIndexI) =  i->getEnergySensitivity(penal); 
				dv(eleIndexI) =  i->getVolume();
				++eleIndexI;
			}
			
			eleIndexI = 0;
			for (auto& i : mFEA->getElements())
			{
				dc(eleIndexI) *= ((beta*pow(1/cosh(beta*(xTilde(eleIndexI)-etae)),2))/(tanh(beta*etae)+tanh(beta*(1-etae)))) / Hs(eleIndexI);
        dv(eleIndexI) *= ((beta*pow(1/cosh(beta*(xTilde(eleIndexI)-etan)),2))/(tanh(beta*etan)+tanh(beta*(1-etan)))) / Hs(eleIndexI);
				++eleIndexI;
			}

			dc = H * dc;
			dv = H * dv;
			
			// optimality criteria update of design variables and physical densities
			double l1 = 1e-50, l2 = 1e50, lmid, upper, lower;
			while (((l2-l1)/(l1+l2))>1e-3)
			{
				lmid = (l1+l2)/2.0;

				for (unsigned int i = 0; i < numEle; i++)
				{
					xNew(i) = x(i) * (std::sqrt(-dc(i)/(lmid*dv(i))));
					upper = std::min(1.0, (x(i) + xMoveBeta));
					lower = std::max(0.0, (x(i) - xMoveBeta));

					if (xNew(i) > upper)
					{
						xNew(i) = upper;
					}
					else if (xNew(i) < lower)
					{
						xNew(i) = lower;
					}
				}
				
				// filter the new densities
				xTilde = H * xNew;

				eleIndexI = 0;
				for (auto& i : mFEA->getElements())
				{// for each element
					// finish filtering
					xTilde(eleIndexI) /= Hs(eleIndexI);
					// calculate nominal and erode densities
					xn(eleIndexI) = densityProjection(beta, etan, xTilde(eleIndexI));
					xe(eleIndexI) = densityProjection(beta, etae, xTilde(eleIndexI));
					++eleIndexI;
				}
				((volume * xn.transpose()).trace() > f * totVolume) ? l1 = lmid : l2 = lmid;
			}
		
			eleIndexI = 0;
			for (auto& i : mFEA->getElements())
			{
				i->updateDensity(xe(eleIndexI), penal);
				++eleIndexI;
			}
			
			// update change
			xChange = xNew - x;
			change = xChange.cwiseAbs().maxCoeff();
			
			x = xNew;

			Mnd = 0.0;
			eleIndexI = 0;
			for (auto& i : mFEA->getElements())
			{
				Mnd += xn(eleIndexI)*(1.0-xn(eleIndexI));
				++eleIndexI;
			}
			Mnd *= ((1.0/(f*(1.0-f)))*(100.0/numEle));

			timeEnd = clock();
			out << std::setw(5)  << std::left << loop
					<< std::setw(10) << std::left << loopBeta
					<< std::setw(15) << std::left << c
					<< std::setw(15) << std::left << (volume * xNew.transpose()).trace()
					<< std::setw(10) << std::left << change
					<< std::setw(10) << std::left << Mnd
					<< std::setw(10) << std::left << (timeEnd - iterationStart)/CLOCKS_PER_SEC << std::endl;
			// update beta
			if ((beta < 256) && ((loopBeta >= 50) || (beta < 256 && change <= tolerance)))
			{
				beta *= 2;
				loopBeta = 0;
				change = 1;
				xMoveBeta = (xMove*tanh(0.5*beta))/(0.5 * beta);
				out << "beta increased to: " << beta << std::endl << std::endl;
				out << std::setw(5)  << std::left << "loop"
						<< std::setw(10) << std::left << "loop_beta"
						<< std::setw(15) << std::left << "Objective"
						<< std::setw(15) << std::left << "Volume"
						<< std::setw(10) << std::left << "Change"
						<< std::setw(10) << std::left << "Mnd"
						<< std::setw(10) << std::left << "Time" << std::endl;
			}
	} // end of iteration
	out << "Topology optimisation successfully finished after: "
			<< (timeEnd - loopStart)/CLOCKS_PER_SEC << " seconds."
			<< std::endl << std::endl;
	
	eleIndexI = 0;
	for (auto& i : mFEA->getElements())
	{
		i->updateDensity(xn(eleIndexI), penal);
		++eleIndexI;
	}
}
	
} // namespace structural_design
} // bso

#endif // SD_TOPOPT_ROBUST_CPP