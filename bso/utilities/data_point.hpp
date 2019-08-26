#ifndef BSO_DATA_POINT_HPP
#define BSO_DATA_POINT_HPP

#include <Eigen/Dense>
#include <initializer_list>

namespace bso { namespace utilities {
	
class data_point
{
private:
	Eigen::VectorXd mData;
	
	void mCheckValidDimensions(const data_point& pCheck) const;
	template <class CONTAINER>
	void mInitFromContainer(const CONTAINER& l);
public:
	data_point();
	data_point(const Eigen::VectorXd& data);
	data_point(const std::vector<double>& l);
	data_point(const std::initializer_list<double>&& l);
	~data_point();
	
	double calcDistanceTo(const data_point& p2) const;
	void normalize(const data_point& pStart, const data_point& pEnd);
	data_point normalized(const data_point& pStart, const data_point& pEnd) const;
	template<class CONTAINER>
	void setToUtopia(const CONTAINER& dataCollection);
	template<class CONTAINER>
	void setToDistopia(const CONTAINER& dataCollection);
	template<class CONTAINER>
	std::vector<data_point> findClosestIn(const CONTAINER& dataCollection,
		const double& tol = 1e-3) const;
	template<class CONTAINER>
	std::vector<data_point> findFurthestIn(const CONTAINER& dataCollection,
		const double& tol = 1e-3) const;
	
	auto begin() {return mData.data();}
	auto end() {return mData.data() + mData.size();}
	unsigned int size() const {return mData.size();}
	
	const double& operator()(const unsigned int& n) const {return mData(n);}
	double& operator()(const unsigned int& n) {return mData(n);}
	
	friend std::ostream& operator << (std::ostream& stream, const data_point& p);
};
	
} // namespace utilities
} // namespace bso

#include <bso/utilities/data_point.cpp>

#endif // BSO_DATA_POINT_HPP