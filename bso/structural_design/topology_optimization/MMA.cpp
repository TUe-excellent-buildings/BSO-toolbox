#ifndef MMA_CPP
#define MMA_CPP

/*
MMA OPTIMIZATION ALGORITHM (C++ file)

I. Bouw
version 07-07-2020

C++ code of the Method of Moving Asymptotes, translated from the MMA-code written in MATLAB by Krister Svanberg (Svanberg, 1987).
The original work can be downloaded from http://www.smoptit.se/ under de GNU General Public License.
The user should refer to the academic work of Krister Svanberg when work will be published in which this code is used.
For a description of the original work see:
Svanberg, K. (1987). The method of moving asymptotes - A new method for structural optimization. International Journal for Numerical Methods in Engineering, 24(2), 359-373. doi:10.1002/nme.1620240207.

This is the C++ version (C++ file) of the MMA code of Krister Svanberg, written by Irma Bouw, for use in the BSO Toolbox (github.com/TUe-excellent-buildings/BSO-toolbox).
*/

#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <iostream>

namespace bso { namespace structural_design { namespace topology_optimization {

	void MMA::MMAsub(const int& m, const int& n, const int& iter, Eigen::VectorXd& xval,
						Eigen::VectorXd& xmin, Eigen::VectorXd& xmax, Eigen::VectorXd& xold1,
						Eigen::VectorXd& xold2, const double& f0val, Eigen::VectorXd& df0dx,
						Eigen::VectorXd& fval, Eigen::MatrixXd& dfdx, Eigen::VectorXd& low,
						Eigen::VectorXd& upp, const double& a0, Eigen::VectorXd& amma,
						Eigen::VectorXd& cmma, Eigen::VectorXd& dmma, const double& move /*= 1.0*/)
	// generate the sub-problem
	{
		// INPUT:
		// m	= nr of constraints
		// n	= nr of variables x_j
		// iter	= current iteration number (= 1 the first time mmasub is called)
		// xval	= column vector with the current values of the variables x_j
		// xmin	= column vector with the lower bounds for the variables x_j
		// xmax	= column vector with the upper bounds for the variables x_j
		// xold1	= xval, one iteration ago (provided that iter > 1)
		// xold2	= xval, two iterations ago (provided that iter > 2)
		// f0val	= the value of the objective function f_0 at xval
		// df0dx	= column vector with the derivatives of the objective function f_0 with respect to the variables x_j, calculated at xval
		// fval	= column vector with values of the constraint functions f_i at xval
		// dfdx	= (m,n)-matrix with the derivatives of the constraint functions f_i with respect to the variables x_j, calculated at xval
		// low	= column vector with the lower asymptotes from the previous iteration (provided that iter > 1)
		// upp	= column vector with the upper asymptotes from the previous iteration (provided that iter > 1)
		// a0	= the constants a_0 in the term a_0 * z
		// a 	= column vector with the constants a_i in the terms a_i * z
		// c 	= column vector with the constants c_i in the terms c_i * y_i
		// d	= column vector with the constants d_i in the terms 0.5*d_i * (y_i)^2

		// define values
		const double epsimin = 1e-7;
		const double raa0 = 1e-5;
		const double albefa = 0.1;		// to update bounds alfa and beta
		const double asyinit = 0.5;		// to update low & upp for iter < 2.5
		const double asyincr = 1.2;		// to update low & upp for iter > 2.5
		const double asydecr = 0.7;		// to update low & upp for iter > 2.5
		Eigen::VectorXd eeen, eeem;
		eeen.setOnes(n);
		eeem.setOnes(m);

		// calculation of asymptotes low and upp
		if (iter < 2.5)
		{
			low = xval - asyinit * (xmax - xmin);
			upp = xval + asyinit * (xmax - xmin);
		}
		else
		{
			Eigen::VectorXd zzz = ((xval - xold1)).cwiseProduct((xold1 - xold2));
			Eigen::VectorXd factor(zzz.size());
			for (int i = 0; i < n; ++i)
			{
				if (zzz(i) > 0)
				{
					factor(i) = asyincr;
				}
				else if (zzz(i) < 0)
				{
					factor(i) = asydecr;
				}
				else
				{
					factor(i) = 1.0;
				}
			}
			low = xval - factor.cwiseProduct((xold1 - low));
			upp = xval + factor.cwiseProduct((upp - xold1));
			low = low.cwiseMax((xval - 10.0*(xmax - xmin)));
			low = low.cwiseMin((xval - 0.01*(xmax - xmin)));
			upp = upp.cwiseMin((xval + 10.0*(xmax - xmin)));
			upp = upp.cwiseMax((xval + 0.01*(xmax - xmin)));
		}

		// calculation of bounds alfa and beta
		Eigen::VectorXd alfa(n), beta(n);
		Eigen::VectorXd zzz1 = low + albefa * (xval - low);
		Eigen::VectorXd zzz2 = xval - move * (xmax - xmin);
		Eigen::VectorXd zzz3 = upp - albefa * (upp - xval);
		Eigen::VectorXd zzz4 = xval + move * (xmax - xmin);
		alfa = xmin.cwiseMax((zzz1.cwiseMax(zzz2)));
		beta = xmax.cwiseMin((zzz3.cwiseMin(zzz4)));

		// calculation of p0, q0 (objective), P, Q (constraints), and b
		Eigen::VectorXd ux1 = upp - xval;
		Eigen::VectorXd xl1 = xval - low;
		Eigen::VectorXd xmamiinv(n), ux2(n), xl2(n), uxinv(n), xlinv(n), p0(n), q0(n), pq0(n);
		Eigen::MatrixXd P(m,n), Q(m,n), PQ(m,n);
		xmamiinv = eeen.cwiseQuotient(((xmax - xmin).cwiseMax(0.00001)));
		ux2 = ux1.cwiseProduct(ux1);	// (U - x)^2
		xl2 = xl1.cwiseProduct(xl1);	// (x - L)^2
		uxinv = eeen.cwiseQuotient(ux1);
		xlinv = eeen.cwiseQuotient(xl1);
		p0 = df0dx.cwiseMax(0.0);
		q0 = (-1.0*df0dx).cwiseMax(0.0);
		pq0 = 0.001*(p0 + q0) + raa0*xmamiinv;
		p0 = (p0 + pq0).cwiseProduct(ux2);
		q0 = (q0 + pq0).cwiseProduct(xl2);
		P = dfdx.cwiseMax(0.0);
		Q = (-1.0*dfdx).cwiseMax(0.0);
		PQ = 0.001*(P + Q) + raa0*eeem*(xmamiinv.transpose());
		P = (P + PQ) * ux2.asDiagonal();
		Q = (Q + PQ) * xl2.asDiagonal();
		Eigen::VectorXd bmma = P * uxinv + Q * xlinv - fval;
		mLow = low;
		mUpp = upp;
		subsolv(m,n,epsimin,low,upp,alfa,beta,p0,q0,P,Q,a0,amma,bmma,cmma,dmma);
	} // MMAsub()

	void MMA::subsolv(const int& m, const int& n, const double& epsimin, Eigen::VectorXd& low,
						Eigen::VectorXd& upp, Eigen::VectorXd& alfa, Eigen::VectorXd& beta,
						Eigen::VectorXd& p0, Eigen::VectorXd& q0, Eigen::MatrixXd& P,
						Eigen::MatrixXd& Q, const double& a0, Eigen::VectorXd& amma,
						Eigen::VectorXd& bmma, Eigen::VectorXd& cmma, Eigen::VectorXd& dmma)
	// solve the sub-problem
	{
		// define values
		int itttmax = 100;
		Eigen::VectorXd eeen, eeem;
		eeen.setOnes(n);
		eeem.setOnes(m);
		Eigen::MatrixXd eemmat, eenmat;
		eemmat.setIdentity(m,m);	// required at line 256 to enable summation
		eenmat.setIdentity(n,n);	// required at line 273 to enable summation
		Eigen::VectorXd x = 0.5 * (alfa + beta);
		Eigen::VectorXd y, lam, s;
		y.setOnes(m);
		double z = 1.0;
		lam.setOnes(m);
		s = lam;
		Eigen::VectorXd xsi(n), eta(n), mu(m);
		xsi = (eeen.cwiseQuotient((x - alfa))).cwiseMax(1.0);
		eta = (eeen.cwiseQuotient((beta - x))).cwiseMax(1.0);
		mu = (eeem.cwiseQuotient((0.5*cmma))).cwiseMax(1.0);
		double zet = 1.0;
		double epsi = 1.0;
		int itera = 0;

		// initialize containers line 189 - 218 (first while-statement)
		Eigen::VectorXd ux11(n), xl11(n), ux22(n), xl22(n), uxinv1(n), xlinv1(n);
		Eigen::VectorXd plam(n), qlam(n), gvec(m), dpsidx(n), epsvecn(n), epsvecm(m);
		Eigen::VectorXd rex(n), rey(m), relam(m), rexsi(n), reeta(n), remu(m), res(m);
		double rez, rezet;
		Eigen::VectorXd residu1(rex.size() + rey.size() + 1);
		Eigen::VectorXd residu2(relam.size() + rexsi.size() + reeta.size() + remu.size() + 1 + res.size());
		Eigen::VectorXd residu(residu1.size() + residu2.size());
		// initialize containers line 219 - 249 (second while-statement)
		Eigen::VectorXd ux111(n), xl111(n), ux222(n), xl222(n), ux333(n), xl333(n);
		Eigen::VectorXd uxinv11(n), xlinv11(n), uxinv22(n), xlinv22(n);
		Eigen::VectorXd plam1(n), qlam1(n), gvec1(m), dpsidx1(n);
		Eigen::VectorXd delx(n), dely(m), dellam(m), diagx(n), diagxinv(n), diagy(m);
		Eigen::VectorXd diagyinv(m), diaglam(m), diaglamyi(m);
		double delz;
		Eigen::MatrixXd GG(m,n);
		// initialize containers line 250 - 289 (if-statement)
		Eigen::VectorXd blam1(m), blam2(n), blam(m);
		Eigen::VectorXd diaglamyiinv(m), dellamyi(m), axz(n), bx(n);
		Eigen::MatrixXd Alam(m,m), AA(m + 1, m + 1), Axx(n,n), AxA(n + 1, n + 1);
		Eigen::VectorXd bb(m + 1), solut(m + 1), bxb(n + 1), solution(n + 1);
		Eigen::VectorXd dlam1(m), dlam(m), dx1(n), dx(n);
		double dz, azz, bz;
		// initialize containers line 290 - 322
		Eigen::VectorXd dy(m), dxsi(n), deta(n), dmu(m), ds(m);
		double dzet, stmxx, stmalfa, stmbeta, stmalbe, stmalbexx, stminv, steg;
		Eigen::VectorXd xx(y.size() + 1 + lam.size() + xsi.size() + eta.size() + mu.size() + 1 + s.size());
		Eigen::VectorXd dxx(xx.size()), stepxx(xx.size()), stepalfa(n), stepbeta(n);
		Eigen::VectorXd xold(n), yold(m), lamold(m), xsiold(n), etaold(n), muold(m), sold(m);
		double zold, zetold;

		while (epsi > epsimin)
		{
			epsvecn.setConstant(epsi);
			epsvecm.setConstant(epsi);
			ux11 = upp - x;
			xl11 = x - low;
			plam = p0 + P.transpose() * lam;
			qlam = q0 + Q.transpose() * lam;
			ux22 = ux11.cwiseProduct(ux11);	// (U - x)^2
			xl22 = xl11.cwiseProduct(xl11);	// (x - L)^2
			uxinv1 = eeen.cwiseQuotient(ux11);
			xlinv1 = eeen.cwiseQuotient(xl11);
			dpsidx = plam.cwiseQuotient(ux22) - qlam.cwiseQuotient(xl22);
			gvec = P * uxinv1 + Q * xlinv1;
			rex = dpsidx - xsi + eta;
			rey = cmma + (dmma * y.transpose()).diagonal() - mu - lam;
			rez = a0 - zet - amma.transpose() * lam;
			relam = gvec - z * amma - y + s - bmma;
			rexsi = ((x - alfa) * xsi.transpose()).diagonal() - epsvecn;
			reeta = ((beta - x) * eta.transpose()).diagonal() - epsvecn;
			remu = (mu * y.transpose()).diagonal() - epsvecm;
			rezet = zet * z - epsi;
			res = (lam * s.transpose()).diagonal() - epsvecm;
			residu1 << rex, rey, rez;
			residu2 << relam, rexsi, reeta, remu, rezet, res;
			residu << residu1, residu2;
			double residunorm = std::sqrt(residu.transpose() * residu);
			double residumax = std::max(residu.maxCoeff(), -1.0 * residu.minCoeff());
			int ittt = 0;

			while (residumax > 0.9 * epsi && ittt < itttmax)
			{
				++ittt;
				++itera;
				ux111 = upp - x;
				xl111 = x - low;
				plam1 = p0 + P.transpose() * lam;
				qlam1 = q0 + Q.transpose() * lam;
				ux222 = ux111.cwiseProduct(ux111);	// (U - x)^2
				xl222 = xl111.cwiseProduct(xl111);	// (x - L)^2
				ux333 = ux111.cwiseProduct(ux222);
				xl333 = xl111.cwiseProduct(xl222);
				uxinv11 = eeen.cwiseQuotient(ux111);
				xlinv11 = eeen.cwiseQuotient(xl111);
				uxinv22 = eeen.cwiseQuotient(ux222);
				xlinv22 = eeen.cwiseQuotient(xl222);
				dpsidx1 = plam1.cwiseQuotient(ux222) - qlam1.cwiseQuotient(xl222);
				gvec1 = P * uxinv11 + Q * xlinv11;
				GG = P * uxinv22.asDiagonal() - Q * xlinv22.asDiagonal();
				delx = dpsidx1 - epsvecn.cwiseQuotient((x - alfa)) + epsvecn.cwiseQuotient((beta - x));
				diagx = plam1.cwiseQuotient(ux333) + qlam1.cwiseQuotient(xl333);
				diagx = 2*diagx + xsi.cwiseQuotient((x - alfa)) + eta.cwiseQuotient((beta - x));
				diagxinv = eeen.cwiseQuotient(diagx);
				dely = cmma + dmma.cwiseProduct(y) - lam - epsvecm.cwiseQuotient(y);
				diagy = dmma + mu.cwiseQuotient(y);
				diagyinv = eeem.cwiseQuotient(diagy);
				dellam = gvec1 - z*amma - y - bmma + epsvecm.cwiseQuotient(lam);
				diaglam = s.cwiseQuotient(lam);
				delz = a0 - amma.transpose() * lam - epsi / z;
				diaglamyi = diaglam + diagyinv;

				if (m < n)
				{
					blam1 = dellam + dely.cwiseQuotient(diagy);
					blam2 = delx.cwiseQuotient(diagx);
					blam = blam1 - GG * blam2;
					bb << blam, delz;
					Alam = eemmat * diaglamyi.asDiagonal() + GG * diagxinv.asDiagonal() * GG.transpose();
					AA.block(0,0,m,m) = Alam;
					AA.block(0,m,m,1) = amma;
					AA.block(m,0,1,m) = amma.transpose();
					AA(m,m) = -1.0 * zet / z;
					solut = AA.llt().solve(bb);
					dlam = solut.head(m);
					dz = solut(m);
					dx1 = (GG.transpose() * dlam);
					dx = -1.0*blam2 - dx1.cwiseQuotient(diagx);
				}
				else
				{
					diaglamyiinv = eeem.cwiseQuotient(diaglamyi);
					dellamyi = dellam + dely.cwiseQuotient(diagy);
					blam = amma.cwiseQuotient(diaglamyi);
					blam1 = dellamyi.cwiseQuotient(diaglamyi);
					Axx = eenmat * diagx.asDiagonal() + GG.transpose() * diaglamyiinv.asDiagonal() * GG;
					azz = zet / z + amma.transpose() * blam;
					axz = -1.0 * GG.transpose() * blam;
					bx = delx + GG.transpose() * blam1;
					bz = delz - amma.transpose() * blam1;
					AxA.block(0,0,n,n) = Axx;
					AxA.block(0,n,n,1) = axz;
					AxA.block(n,0,1,n) = axz.transpose();
					AxA(n,n) = azz;
					bxb << -1.0 * bx, -1.0 * bz;
					solution = AxA.llt().solve(bxb);
					dx = solution.head(n);
					dz = solution(n);
					dlam1 = GG * dx;
					dlam = dlam1.cwiseQuotient(diaglamyi) - dz * blam + dellamyi.cwiseQuotient(diaglamyi);
				}

				dy = -1.0 * dely.cwiseQuotient(diagy) + dlam.cwiseQuotient(diagy);
				dmu = -1.0 * mu + epsvecm.cwiseQuotient(y) - mu.cwiseProduct((dy.cwiseQuotient(y)));
				ds = -1.0 * s + epsvecm.cwiseQuotient(lam) - s.cwiseProduct((dlam.cwiseQuotient(lam)));
				dxsi = -1.0 * xsi + epsvecn.cwiseQuotient((x - alfa)) - xsi.cwiseProduct((dx.cwiseQuotient((x - alfa))));
				deta = -1.0 * eta + epsvecn.cwiseQuotient((beta - x)) + eta.cwiseProduct((dx.cwiseQuotient((beta - x))));
				dzet = -1.0 * zet + epsi / z - zet * dz / z;
				xx << y, z, lam, xsi, eta, mu, zet, s;
				dxx << dy, dz, dlam, dxsi, deta, dmu, dzet, ds;

				stepxx = -1.01 * dxx.cwiseQuotient(xx);
				stmxx = stepxx.maxCoeff();
				stepalfa = -1.01 * dx.cwiseQuotient((x - alfa));
				stepbeta = 1.01 * dx.cwiseQuotient((beta - x));
				stmalfa = stepalfa.maxCoeff();
				stmbeta = stepbeta.maxCoeff();
				stmalbe = std::max(stmalfa, stmbeta);
				stmalbexx = std::max(stmalbe, stmxx);
				stminv = std::max(stmalbexx, 1.0);
				steg = 1.0 / stminv;

				xold = x;
				yold = y;
				zold = z;
				lamold = lam;
				xsiold = xsi;
				etaold = eta;
				muold = mu;
				zetold = zet;
				sold = s;

				int itto = 0;
				double resinew = 2.0 * residunorm;

				while (resinew > residunorm && itto < 50)
				{
					x = xold + steg * dx;
					y = yold + steg * dy;
					z = zold + steg * dz;
					lam = lamold + steg * dlam;
					xsi = xsiold + steg * dxsi;
					eta = etaold + steg * deta;
					mu = muold + steg * dmu;
					zet = zetold + steg * dzet;
					s = sold + steg * ds;
					ux11 = upp - x;
					xl11 = x - low;
					plam = p0 + P.transpose() * lam;
					qlam = q0 + Q.transpose() * lam;
					ux22 = ux11.cwiseProduct(ux11);	// (U - x)^2
					xl22 = xl11.cwiseProduct(xl11);	// (x - L)^2
					uxinv1 = eeen.cwiseQuotient(ux11);
					xlinv1 = eeen.cwiseQuotient(xl11);
					dpsidx = plam.cwiseQuotient(ux22) - qlam.cwiseQuotient(xl22);
					gvec = P * uxinv1 + Q * xlinv1;
					rex = dpsidx - xsi + eta;
					rey = cmma + (dmma * y.transpose()).diagonal() - mu - lam;
					rez = a0 - zet - amma.transpose() * lam;
					relam = gvec - z * amma - y + s - bmma;
					rexsi = ((x - alfa) * xsi.transpose()).diagonal() - epsvecn;
					reeta = ((beta - x) * eta.transpose()).diagonal() - epsvecn;
					remu = (mu * y.transpose()).diagonal() - epsvecm;
					rezet = zet * z - epsi;
					res = (lam * s.transpose()).diagonal() - epsvecm;
					residu1 << rex, rey, rez;
					residu2 << relam, rexsi, reeta, remu, rezet, res;
					residu << residu1, residu2;
					resinew = std::sqrt(residu.transpose() * residu);
					steg = steg / 2.0;
				} // end: while (resinew > residunorm && itto < 50)

				residunorm = resinew;
				residumax = std::max(residu.maxCoeff(), -1.0 * residu.minCoeff());
				steg = 2 * steg;
				if (ittt > (itttmax - 1))
				{
					std::cout << "MMA_epsi: " << epsi << ", MMA_ittt: " << ittt << std::endl;
				}
			} // end: while (residumax > 0.9 * epsi && ittt < 200)

			epsi = 0.1 * epsi;
		} // end: while (epsi > epsimin)

		mxmma = x;
		mymma = y;
		mzmma = z;
		mlamma = lam;
		mxsimma = xsi;
		metamma = eta;
		mmumma = mu;
		mzetmma = zet;
		msmma = s;
	} // subsolv()

} // namespace topology_optimization
} // namespace structural_design
} // bso

#endif // MMA_CPP