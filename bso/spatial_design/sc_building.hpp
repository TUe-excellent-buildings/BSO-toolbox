#ifndef SC_BUILDING_HPP
#define SC_BUILDING_HPP

#include <sstream>
#include <vector>

namespace bso { namespace spatial_design {

class sc_building
{
private:
	std::vector<double> mWValues;
	std::vector<double> mDValues;
	std::vector<double> mHValues;
	std::vector<std::vector<int> > mBValues;

	void checkValidity() const;
	
	friend class ms_building;
public:
	sc_building();
	sc_building(std::string input, std::string separators = "\t,; ");
	sc_building(const sc_building& rhs);
	~sc_building();
	
	void writeToFile(const std::string& fileName) const;
	std::string writeToString(char separator = ',') const;
	
	bool isEmpty() const; // returns true if any of the W, D, H, or B vectors are empty
	
	unsigned int getWIndex(const unsigned int& cellIndex) const; // returns the W index of the specified cell index
	unsigned int getDIndex(const unsigned int& cellIndex) const; // returns the D index of the specified cell index
	unsigned int getHIndex(const unsigned int& cellIndex) const; // returns the H index of the specified cell index
	unsigned int getCellIndex(const unsigned int& wIndex, const unsigned int& dIndex, const unsigned int& hIndex, const unsigned int& spaceIndex = 0) const; // returns the cell index of the specified indices
	
	unsigned int getWSize() const; // get the size of the W vector
	unsigned int getDSize() const; // get the size of the W vector
	unsigned int getHSize() const; // get the size of the W vector
	unsigned int getBSize() const; // get the size of the W vector
	unsigned int getBRowSize() const; // get the size of the B vector
	bool getSpaceIndex(const unsigned int& cellIndex, unsigned int& spaceIndex) const;  // returns true if the cell is active, and if so it returns the first space index for which this is the case
	
	double getWValue(const unsigned int& index) const; // returns the W value at the specified index
	double getDValue(const unsigned int& index) const; // returns the D value at the specified index
	double getHValue(const unsigned int& index) const; // returns the H value at the specified index
	int getBValue(const unsigned int& spaceIndex, const unsigned int& cellIndex) const;  // returns the B value at the specified indices
	
	bool operator == (const sc_building& rhs); // checks if two supercubes are equal
	bool operator != (const sc_building& rhs); // checks if two supercubes differ
	
	friend std::ostream& operator<< (std::ostream& stream, const sc_building& building);
};

} // namespace spatial_design
} // namespace bso

#include <bso/spatial_design/sc_building.cpp>

#endif // SC_BUILDING_HPP