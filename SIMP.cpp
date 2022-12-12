#ifndef SD_TOPOPT_SIMP_CPP
#define SD_TOPOPT_SIMP_CPP

#include <Eigen/Dense>
#include <Eigen/Sparse>

#include <vector>
#include <map>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>

namespace bso { namespace structural_design { namespace topology_optimization {

class SIMP;

} // namespace topology_optimization

template <>
void sd_model::topologyOptimization<topology_optimization::SIMP>(const double& f, 
					const double& rMin, const double& penal, const double& xMove,
					const double& tolerance)
{
	std::ostream out(mTopOptStreamBuffer);
	unsigned int numEle = mFEA->getElements().size();
	double totVolume = 0; // initialised at 0, before each element volumes are added
	double c; // sum of all the elements compliances (objective value)

	Eigen::VectorXd x(numEle), xNew(numEle), xChange(numEle),
									volume(numEle), dc(numEle), dv(numEle); // initialise containers for element values

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
	out << "Total Volume: " << totVolume << std::endl;

	// initialise iteration
	double change = 1;
	int loop = 0;

	double loopStart = clock(), iterationStart, timeEnd = 0.0;

	// start iteration
	while (change > tolerance)
	{
			iterationStart = clock();
			if (loop%20 == 0)
			{
					out << std::endl
							<< std::setw(5)  << std::left << "loop"
							<< std::setw(15) << std::left << "Objective"
							<< std::setw(15) << std::left << "Volume"
							<< std::setw(15) << std::left << "Change"
							<< std::setw(10) << std::left << "Time" << std::endl;
			}

			++loop;
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

			dc = (dc * x.transpose()).diagonal();
			dc = H * dc;

			for (unsigned int i = 0; i < numEle; i++)
			{
				dc(i) /= (Hs(i) * std::max(1e-3, x(i)));
			}

			// optimality criteria update of design variables and physical densities
			double l1 = 0, l2 = 1e9, lmid, upper, lower;
			while (((l2-l1)/(l1+l2))>1e-3)
			{
				lmid = (l1+l2)/2.0;

				for (unsigned int i = 0; i < numEle; i++)
				{
					xNew(i) = x(i) * (std::sqrt(-dc(i)/(lmid*dv(i))));
					upper = std::min(1.0, (x(i) + xMove));
					lower = std::max(0.0, (x(i) - xMove));

					if (xNew(i) > upper)
					{
						xNew(i) = upper;
					}
					else if (xNew(i) < lower)
					{
						xNew(i) = lower;
					}
				}

				((volume * xNew.transpose()).trace() > f * totVolume) ? l1 = lmid : l2 = lmid;
			}
		
			eleIndexI = 0;
			for (auto& i : mFEA->getElements())
			{
				i->updateDensity(xNew(eleIndexI), penal);
				++eleIndexI;
			}

			// update change
			xChange = xNew - x;
			change = xChange.cwiseAbs().maxCoeff();

			timeEnd = clock();
			out << std::setw(5)  << std::left << loop
					<< std::setw(15) << std::left << c
					<< std::setw(15) << std::left << (volume * xNew.transpose()).trace()
					<< std::setw(15) << std::left << change
					<< std::setw(10) << std::left << (timeEnd - iterationStart)/CLOCKS_PER_SEC << std::endl;

			x = xNew;
	} // end of iteration
	out << "Topology optimisation successfully finished after: "
			<< (timeEnd - loopStart)/CLOCKS_PER_SEC << " seconds."
			<< std::endl << std::endl;
}
	
} // namespace structural_design
} // bso

#endif // SD_TOPOPT_SIMP_CPP
