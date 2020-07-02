#ifndef SD_TOPOPT_ELEMENT_TYPE_SIMP_CPP
#define SD_TOPOPT_ELEMENT_TYPE_SIMP_CPP

namespace bso { namespace structural_design { namespace topology_optimization {

class ELE_SIMP;

namespace ele_simp {
	
void HInit(Eigen::SparseMatrix<double>& H, Eigen::VectorXd& Hs,
		 const std::vector<element::element*>& elements, Eigen::VectorXd& volume,
		 Eigen::VectorXd& x, const double& f, const double& penal, 
		 const double& rMin);
void ObjectiveAndSensitivity(const std::vector<element::element*>& elements,
		 double& c, const Eigen::VectorXd& x, Eigen::VectorXd& dc, Eigen::VectorXd& dv,
		 const Eigen::SparseMatrix<double>& H, const Eigen::VectorXd& Hs,
		 const double& penal);
void OptimalityCritUpdate(double l1, double l2, const double& xMove, 
		 const double& totalVolume, const double& f, Eigen::VectorXd& volume,
		 Eigen::VectorXd& x, Eigen::VectorXd& xNew, const Eigen::VectorXd& dv,
		 const Eigen::VectorXd& dc);

} // namespace ele_simp
} // namespace topology_optimization

template <>
void sd_model::topologyOptimization<topology_optimization::ELE_SIMP>(const double& f, 
						const double& rMin, const double& penal, const double& xMove,
						const double& tolerance)
{
	using namespace topology_optimization::ele_simp;
	std::ostream out(mTopOptStreamBuffer);
	unsigned int numEle = mFEA->getElements().size();
	std::vector<element::element*> fEle, bEle, tEle;
	double fVolume = 0, bVolume= 0, tVolume = 0, totVolume = 0; // initialised at 0, before each element volumes are added
	double cF, cB, cT; // sum of all the elements compliances (objective value)
	
	for (auto& i : mFEA->getElements())
	{
		if (!i->isActiveInCompliance()) continue;
		i->updateDensity(f,penal);
		if (i->isFlatShell())
		{
			fEle.push_back(i);
			fVolume += i->getVolume();
		}
		else if (i->isBeam())
		{
			bEle.push_back(i);
			bVolume += i->getVolume();
		}
		else if (i->isTruss())
		{
			tEle.push_back(i);
			tVolume += i->getVolume();
		}
	}
	unsigned int numFEle = fEle.size(), numBEle = bEle.size(), numTEle = tEle.size();
	
	// initialise containers for element values
	Eigen::VectorXd xF(numFEle),       xB(numBEle),       xT(numTEle),
									xNewF(numFEle),    xNewB(numBEle),    xNewT(numTEle),
									xChangeF(numFEle), xChangeB(numBEle), xChangeT(numTEle),
									volumeF(numFEle),  volumeB(numBEle),  volumeT(numTEle),
									dcF(numFEle),      dcB(numBEle),      dcT(numTEle),
									dvF(numFEle),      dvB(numBEle),      dvT(numTEle);
	xChangeF.setZero(); xChangeB.setZero(); xChangeT.setZero();
	
	// prepare filter
	Eigen::SparseMatrix<double> HF(numFEle, numFEle), HB(numBEle, numBEle), HT(numTEle, numTEle); // contains filter vectors for each element
	Eigen::VectorXd HsF(numFEle), HsB(numBEle), HsT(numTEle);
	HsF.setZero(); HsB.setZero(); HsT.setZero();
	
	HInit(HF,HsF,fEle,volumeF,xF,f,penal,rMin);
	HInit(HB,HsB,bEle,volumeB,xB,f,penal,rMin);
	HInit(HT,HsT,tEle,volumeT,xT,f,penal,rMin);
	
	totVolume = fVolume+bVolume+tVolume;
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
			cF = cB = cT = 0;

			// FEA
			mFEA->generateGSM();
			mFEA->solve("SimplicialLDLT");

			double volume = 0;
			if (fEle.size() > 0)
			{
				ObjectiveAndSensitivity(
					fEle,cF,xF,dcF,dvF,HF,HsF,penal);
				OptimalityCritUpdate(
					0,1e9,xMove,fVolume,f,volumeF,xF,xNewF,dvF,dcF);
				
				unsigned int eleIndexI = 0;
				for (auto& i : fEle)
				{
					i->updateDensity(xNewF(eleIndexI), penal);
					++eleIndexI;
				}
				xChangeF = xNewF - xF;
				volume += (volumeF * xNewF.transpose()).trace();
			}
			if (bEle.size() > 0)
			{
				ObjectiveAndSensitivity(
					bEle,cB,xB,dcB,dvB,HB,HsB,penal);
				OptimalityCritUpdate(
					0,1e9,xMove,bVolume,f,volumeB,xB,xNewB,dvB,dcB);
				
				unsigned int eleIndexI = 0;
				for (auto& i : bEle)
				{
					i->updateDensity(xNewB(eleIndexI), penal);
					++eleIndexI;
				}
				xChangeB = xNewB - xB;
				volume += (volumeB * xNewB.transpose()).trace();
			}
			if (tEle.size() > 0)
			{
				ObjectiveAndSensitivity(
					tEle,cT,xT,dcT,dvT,HT,HsT,penal);
				OptimalityCritUpdate(
					0,1e9,xMove,tVolume,f,volumeT,xT,xNewT,dvT,dcT);
				
				unsigned int eleIndexI = 0;
				for (auto& i : tEle)
				{
					i->updateDensity(xNewT(eleIndexI), penal);
					++eleIndexI;
				}
				xChangeT = xNewT - xT;
				volume += (volumeT * xNewT.transpose()).trace();
			}

			// update change
			change = 0;
			if (fEle.size() > 0)
			{
				change = std::max(change, xChangeF.cwiseAbs().maxCoeff());
			}
			if (bEle.size() > 0)
			{
				change = std::max(change, xChangeB.cwiseAbs().maxCoeff());
			}
			if (tEle.size() > 0)
			{
				change = std::max(change, xChangeT.cwiseAbs().maxCoeff());
			}

			timeEnd = clock();
			out << std::setw(5)  << std::left << loop
					<< std::setw(15) << std::left << cF + cB + cT
					<< std::setw(15) << std::left << volume
					<< std::setw(15) << std::left << change
					<< std::setw(10) << std::left << (timeEnd - iterationStart)/CLOCKS_PER_SEC << std::endl;

			xF = xNewF;
			xB = xNewB;
			xT = xNewT;
	} // end of iteration
	out << "Topology optimisation successfully finished after: "
			<< (timeEnd - loopStart)/CLOCKS_PER_SEC << " seconds."
			<< std::endl << std::endl;
}

namespace topology_optimization { namespace ele_simp {
	
	
void HInit(Eigen::SparseMatrix<double>& H, Eigen::VectorXd& Hs,
		 const std::vector<element::element*>& elements, Eigen::VectorXd& volume,
		 Eigen::VectorXd& x, const double& f, const double& penal, 
		 const double& rMin)
{
	typedef Eigen::Triplet<double> T;
	std::vector<T> tripletList;

	unsigned int eleIndexI = 0;
	for (auto& i : elements)
	{ // for each element i
		volume(eleIndexI) = i->getVolume();
		x(eleIndexI) = f;
		i->updateDensity(f,penal);
		
		unsigned int eleIndexJ = 0;
		for (auto& j : elements)
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
	
	tripletList.clear();
}

void ObjectiveAndSensitivity(const std::vector<element::element*>& elements,
		 double& c, const Eigen::VectorXd& x, Eigen::VectorXd& dc, Eigen::VectorXd& dv,
		 const Eigen::SparseMatrix<double>& H, const Eigen::VectorXd& Hs,
		 const double& penal)
{
	// objective function and sensitivity analysis (retrieve data from FEA)
	unsigned int eleIndexI = 0;
	for (const auto& i : elements)
	{
		c 						+= i->getTotalEnergy();
		dc(eleIndexI) =  i->getEnergySensitivity(penal); 
		dv(eleIndexI) =  i->getVolume();
		++eleIndexI;
	}

	dc = (dc * x.transpose()).diagonal();
	dc = H * dc;

	for (unsigned int i = 0; i < x.size(); i++)
	{
		dc(i) /= (Hs(i) * std::max(1e-3, x(i)));
	}
}

void OptimalityCritUpdate(double l1, double l2, const double& xMove, 
		 const double& totalVolume, const double& f, Eigen::VectorXd& volume,
		 Eigen::VectorXd& x, Eigen::VectorXd& xNew, const Eigen::VectorXd& dv,
		 const Eigen::VectorXd& dc)
{
	// optimality criteria update of design variables and physical densities
	double lmid, upper, lower;
	while (((l2-l1)/(l1+l2))>1e-3)
	{
		lmid = (l1+l2)/2.0;

		for (unsigned int i = 0; i < x.size(); i++)
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

		((volume * xNew.transpose()).trace() > f * totalVolume) ? l1 = lmid : l2 = lmid;
	}
}

} // namespace ele_simp
} // namespace topology_optimization
} // namespace structural_design
} // bso

#endif // SD_TOPOPT_ELEMENT_TYPE_SIMP_CPP