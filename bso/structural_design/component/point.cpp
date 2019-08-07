#ifndef SD_POINT_CPP
#define SD_POINT_CPP

namespace bso { namespace structural_design { namespace component {
	
	template<class T>
	point::point(const unsigned long& ID, const Eigen::MatrixBase<T>& rhs) :
		bso::utilities::geometry::vertex(rhs)
	{
		mID = ID;
	} // ctor
	
	point::point(const unsigned long& ID, const std::initializer_list<double>&& l) :
		bso::utilities::geometry::vertex(std::move(l))
	{
		mID = ID;
	}
	
	point::~point()
	{
		
	} // dtor

	void point::addLoad(const load& l)
	{
		mLoads.push_back(l);
	} // addLoad

	void point::addConstraint(const constraint& c)
	{
		mConstraints.push_back(c);
	} // addConstraint

} // namespace component
} // namespace structural_design
} // namespace bso

#endif // SD_POINT_CPP