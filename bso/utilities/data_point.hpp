#ifndef BSO_DATA_POINT_HPP
#define BSO_DATA_POINT_HPP

#include <vector>
#include <Eigen/Dense>
#include <initializer_list>

namespace bso { namespace utilities {
	
class data_point
{
private:
	Eigen::VectorXd mData;
	std::vector<std::string> mLabels;
	
	void mCheckValidDimensions(const data_point& pCheck) const;
	template <class CONTAINER>
	void mInitFromContainer(const CONTAINER& l);
public:
	data_point();
	data_point(const unsigned int& size);
	data_point(const Eigen::VectorXd& data);
	data_point(const std::vector<double>& l);
	data_point(const std::initializer_list<double>&& l);
	~data_point();
	
	double calcDistanceTo(const data_point& p2) const;
	double calcSquaredDistanceTo(const data_point& p2) const;
	void normalize(const data_point& pStart, const data_point& pEnd,
		const std::vector<unsigned int>& logScales = {});
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
	
	double aggregateToSum(data_point weights = data_point(0)) const;
	double aggregateToProduct(data_point weights = data_point(0)) const;
	double aggregateToDistance(const data_point& measurePoint,
		data_point weights = data_point(0)) const;
	
	auto begin() {return mData.data();}
	auto end() {return mData.data() + mData.size();}
	void addLabel(const std::string& label) {mLabels.push_back(label);}
	const std::vector<std::string>& getLabels() const {return mLabels;}
	unsigned int size() const {return mData.size();}
	
	const double& operator()(const unsigned int& n) const {return mData(n);}
	double& operator()(const unsigned int& n) {return mData(n);}
	
	data_point operator + (const data_point& rhs) const;
	data_point operator - (const data_point& rhs) const;
	data_point& operator += (const data_point& rhs);
	data_point& operator -= (const data_point& rhs);
	
	data_point operator * (const double& rhs) const;
	data_point operator / (const double& rhs) const;
	data_point& operator *= (const double& rhs);
	data_point& operator /= (const double& rhs);
	
	bool operator <= (const data_point& rhs) const;
	bool operator >= (const data_point& rhs) const;
	bool operator < (const data_point& rhs) const;
	bool operator > (const data_point& rhs) const;
	bool operator == (const data_point& rhs) const;
	bool operator != (const data_point& rhs) const;
	
	bool isApproximately(const data_point& rhs, const double tol = 1e-3) const;
	bool dominates(const data_point& rhs) const;
	bool isDominatedBy(const data_point& rhs) const;
	
	friend std::ostream& operator << (std::ostream& stream, const data_point& p);
};
	
} // namespace utilities
} // namespace bso

#include <bso/utilities/data_point.cpp>

#endif // BSO_DATA_POINT_HPP