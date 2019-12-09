#ifndef SC_BUILDING_CPP
#define SC_BUILIDNG_CPP

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <exception>

#include <boost/tokenizer.hpp>
#include <boost/algorithm/string.hpp>

#include <bso/utilities/trim_and_cast.hpp>

namespace bso { namespace spatial_design {
	
sc_building::sc_building()
{
	
} // sc_building()

sc_building::sc_building(std::string input, std::string separators /*= ","*/)
{
	bool readFromFile = true;
	bool readFromLine = true;
	std::string fileError = "";
	std::string stringError = "";

	std::ifstream inputStream;
	try
	{
		if (!input.empty()) inputStream.open(input.c_str());
		
		if (input.empty() || !inputStream.is_open())
		{
			std::stringstream errorMessage;
			errorMessage << "Could not initialize an SC building spatial design with the following input file:" << std::endl
									 << ((input.empty())? "no input file given" : input) << std::endl
									 << "(bso/spatial_design/sc_building.cpp). " << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}

		std::string line;
		
		boost::char_separator<char> sep(separators.c_str());
		typedef boost::tokenizer< boost::char_separator<char> > t_tokenizer;
		
		while (!inputStream.eof())
		{
			getline(inputStream,line);
			if (line == "") continue;
			try
			{
				t_tokenizer tok(line, sep);
				t_tokenizer::iterator token = tok.begin();
				
				char tokenID;
				try 
				{
					tokenID = bso::utilities::trim_and_cast_char(*(token++));
				}
				catch (std::invalid_argument& e)
				{
					continue;
				}
				switch(tokenID)
				{
					case 'w':
					{
						while (token != tok.end()) mWValues.push_back(bso::utilities::trim_and_cast_double(*(token++)));
						break;
					}
					case 'd':
					{
						while (token != tok.end()) mDValues.push_back(bso::utilities::trim_and_cast_double(*(token++)));
						break;
					}
					case 'h':
					{
						while (token != tok.end()) mHValues.push_back(bso::utilities::trim_and_cast_double(*(token++)));
						break;
					}
					case 'b':
					{
						std::vector<int> row;
						while (token != tok.end()) row.push_back(bso::utilities::trim_and_cast_int(*(token++)));
						mBValues.push_back(row);
						break;
					}
					default:
					{
						continue;
					}
				}
			}
			catch(std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "Encountered an error while parsing the following line of an SC input file:" << std::endl
										 << line << std::endl
										 << "(bso/spatial_design/sc_building.cpp). Got the following error: " << std::endl
										 << e.what() << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
		}
	}
	catch(std::exception& e)
	{
		fileError = e.what();
		readFromFile = false;
	}

	if (!readFromFile)
	{
		try
		{
			boost::char_separator<char> sep(separators.c_str());
			typedef boost::tokenizer< boost::char_separator<char> > t_tokenizer;
			
			t_tokenizer tok(input, sep);
			t_tokenizer::iterator token = tok.begin();

			unsigned int w = bso::utilities::trim_and_cast_uint(*(token));
			unsigned int d = bso::utilities::trim_and_cast_uint(*(++token));
			unsigned int h = bso::utilities::trim_and_cast_uint(*(++token));
			unsigned int b = bso::utilities::trim_and_cast_uint(*(++token));
			
			mWValues = std::vector<double>(w);
			mDValues = std::vector<double>(d);
			mHValues = std::vector<double>(h);
			mBValues = std::vector<std::vector<int> >(b);
			
			for (auto&& i : mWValues) i = 1000 * bso::utilities::trim_and_cast_double(*(++token));
			for (auto&& i : mDValues) i = 1000 * bso::utilities::trim_and_cast_double(*(++token));
			for (auto&& i : mHValues) i = 1000 * bso::utilities::trim_and_cast_double(*(++token));
			unsigned int count = 1;
			for (auto&& i : mBValues)
			{
				i = std::vector<int>(w*d*h);
				unsigned int cellIndex = 0;
				for (auto&& j : i) 
				{
					++token;
					if (token == tok.end())
					{
						std::stringstream errorMessage;
						errorMessage << "Could not initialize an SC building spatial design via an input line.\n"
												 << "Reached end of line before expectation. Reached space: " << count << std::endl
												 << "Reached cell index: " << cellIndex << std::endl
												 << "(bso/spatial_design/sc_building.cpp)" << std::endl;
						throw std::invalid_argument(errorMessage.str());
					}
					try
					{
						j = bso::utilities::trim_and_cast_int(*(token));
					}
					catch(std::exception& e)
					{
						j = bso::utilities::trim_and_cast_double(*(token));
					}
					++cellIndex;
				}
				i.insert(i.begin(), count);
				++count;
			}
		}
		catch(std::exception& e)
		{
			stringError = e.what();
			readFromLine = false;
		}
	}

	if (!readFromFile && !readFromLine)
	{
		std::stringstream errorMessage;
		errorMessage << "Could not initialize an SC building spatial design with the following input line:" << std::endl
								 << input << std::endl
								 << "Got the following error messages: " << std::endl
								 << "When trying to initialize from a textfile using the input line as file name:" << std::endl
								 << fileError << std::endl << std::endl
								 << "When trying to initialize from interpreting the input line as an sc_building:" <<  std::endl
								 << stringError << std::endl
								 << "(bso/spatial_design/sc_building.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}

	try 
	{
		this->checkValidity();
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "Could not initialize an SC building spatial design with the following input line:" << std::endl
								 << input << std::endl
								 << "This input generated an invalid SC building spatial design (bso/spatial_design/sc_building.cpp)." << std::endl
								 << "Got the following error: " << std::endl
								 << e.what() << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // sc_building()

sc_building::sc_building(const sc_building& rhs)
{
	mWValues = rhs.mWValues;
	mDValues = rhs.mDValues;
	mHValues = rhs.mHValues;
	mBValues = rhs.mBValues;
	
	this->checkValidity();
} // sc_building()

sc_building::~sc_building()
{
	
} // ~sc_building()

void sc_building::checkValidity() const
{
	// check if the number of elements in each mBValues is correct
	unsigned int checkSize = getBRowSize();

	for (auto i : mBValues)
	{
		if (i.size() - 1 != checkSize)
		{
			std::stringstream errorMessage;
			errorMessage << "Size of supercube bitmask is incorrect, found a space with " 
									 << i.size() - 1 << " bits, expected " << checkSize
									 << "(bso/spatial_design/sc_building.cpp)" << std::endl;
			throw std::logic_error(errorMessage.str());
		}
	}
	for (auto i : mBValues)
	{
		for (unsigned int j = 1; j < i.size(); j++)
		{
			if (i[j] != 0 && i[j] != 1)
			{
				std::stringstream errorMessage;
				errorMessage << "Cell values of supercube are invalid, encountered a cell value of: " << i[j] << std::endl 
										 << "At index (w,d,h): " << getWIndex(j+1) << "," << getDIndex(j+1) << "," << getHIndex(j+1) << "."
										 << "Only ones and zeros are allowed. (bso/spatial_design/sc_building.cpp)" << std::endl;
				throw std::logic_error(errorMessage.str());
			}
		}
	}
	
} // checkValidity()

void sc_building::writeToFile(const std::string& fileName) const
{
	std::ofstream output;
	output.open(fileName.c_str());
	
	bool first = true;
	for (auto i : mWValues)
	{
		output << ((first)? "w," : ",") << i;
		if (first) first = false;
	}

	first = true;
	for (auto i : mDValues)
	{
		output << ((first)? "\nd," : ",") << i;
		if (first) first = false;
	}

	first = true;
	for (auto i : mHValues)
	{
		output << ((first)? "\nh," : ",") << i;
		if (first) first = false;
	}

	for (auto i : mBValues)
	{
		output << "\nb";
		first = true;
		for (auto j : i) output << "," << j;
	}
	
	output.close();
} // writeToFile()

std::string sc_building::writeToString(char separator /*= ','*/) const
{
	std::stringstream stream;
	stream << mWValues.size()
				 << separator << mDValues.size()
				 << separator << mHValues.size()
				 << separator << mBValues.size();
	
	for (const auto& i : mWValues) stream << separator << i/1000;
	for (const auto& i : mDValues) stream << separator << i/1000;
	for (const auto& i : mHValues) stream << separator << i/1000;
	
	for (const auto& i : mBValues)
	{
		bool first = true;
		for (const auto& j : i)
		{
			if (first) first = false;
			else stream << separator << j;
		}
	}
	return stream.str();
}

bool sc_building::isEmpty() const
{ // returns true if any of the W, D, H, or B vectors are empty
	return (mWValues.empty() || mDValues.empty() || mHValues.empty() || mBValues.empty());
}

unsigned int sc_building::getWIndex(const unsigned int& cellIndex) const
{ // returns the W index of the specified cell index
	if(isEmpty())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get W index at specified cell index: " << cellIndex << std::endl
								 << "Supercube is empty (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (cellIndex < 1 || cellIndex > getBRowSize())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get W index at specified cell index: " << cellIndex << std::endl
								 << "index is out of range: " << 1 << " - " << getBRowSize() << "." << std::endl
								 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	unsigned int nC = cellIndex - 1;
	unsigned int nD = mDValues.size();
	unsigned int nH = mHValues.size();
	return nC/(nD*nH);
} // getWIndex()

unsigned int sc_building::getDIndex(const unsigned int& cellIndex) const
{ // returns the D index of the specified cell index
	if(isEmpty())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get D index at specified cell index: " << cellIndex << std::endl
								 << "Supercube is empty (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (cellIndex < 1 || cellIndex > getBRowSize())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get D index at specified cell index: " << cellIndex << std::endl
								 << "index is out of range: " << 1 << " - " << getBRowSize() << "." << std::endl
								 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	unsigned int nC = cellIndex - 1;
	unsigned int nD = mDValues.size();
	unsigned int nH = mHValues.size();
	return (nC/nH)%nD;
} // getDIndex()

unsigned int sc_building::getHIndex(const unsigned int& cellIndex) const
{ // returns the H index of the specified cell index
	if(isEmpty())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get H index at specified cell index: " << cellIndex << std::endl
								 << "Supercube is empty (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (cellIndex < 1 || cellIndex > getBRowSize())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get H index at specified cell index: " << cellIndex << std::endl
								 << "index is out of range: " << 1 << " - " << getBRowSize() << "." << std::endl
								 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	unsigned int nC = cellIndex - 1;
	unsigned int nH = mHValues.size();
	return nC%nH;
} // getHIndex()

unsigned int sc_building::getCellIndex(const unsigned int& wIndex, const unsigned int& dIndex, const unsigned int& hIndex, const unsigned int& spaceIndex /*= 0*/) const
{ // returns the cell index of the specified indices
	if(isEmpty())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get cell index at specified indices: " << std::endl
								 << "Supercube is empty (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	if (wIndex >= mWValues.size() || dIndex >= mDValues.size() || hIndex >= mHValues.size() || spaceIndex >= mBValues.size())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get cell index at specified indices (w,d,h,l): " << wIndex << "," << dIndex << "," << hIndex << "," << spaceIndex << std::endl
								 << "At least one of the indices is out of range:" << std::endl
								 << "should be greater or equal to zero or smaller than (w,d,h,l): " << mWValues.size() << "," << mWValues.size() << "," << mDValues.size() << "," << mHValues.size() << "," << mBValues.size() << std::endl
								 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	return (wIndex * (mDValues.size() * mHValues.size())) + (dIndex * mHValues.size()) + hIndex + 1;
} // getCellIndex

unsigned int sc_building::getWSize() const
{ // get the size of the W vector
	return mWValues.size();
} // getWSize()

unsigned int sc_building::getDSize() const
{ // get the size of the W vector
	return mDValues.size();
} // getDSize()

unsigned int sc_building::getHSize() const
{ // get the size of the W vector
	return mHValues.size();
} // getHSize()

unsigned int sc_building::getBSize() const
{ // get the size of the W vector
	return mBValues.size();
} // getBSize()

unsigned int sc_building::getBRowSize() const
{ // get the size of the B vector
	return (mWValues.size() *  mDValues.size() * mHValues.size());
} // getBRowSize()

bool sc_building::getSpaceIndex(const unsigned int& cellIndex, unsigned int& spaceIndex) const
{  // returns true if the cell is active, and if so it returns the first space index for which this is the case
	if (isEmpty())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get space index at specified cell index: " << cellIndex << std::endl
								 << "Supercube is empty (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	else if (cellIndex > getBRowSize())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot check cell value at specified cell index: " << cellIndex << std::endl
								 << "Index exceeds vector bounds (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	for (unsigned int i = 0; i < mBValues.size(); i++)
	{
		if (mBValues[i][cellIndex] == 1)
		{
			spaceIndex = i;
			return true;
		}
	}
	return false;
} // getSpaceIndex()

double sc_building::getWValue(const unsigned int& index) const
{ // returns the W value at the specified index
	if (index >= mWValues.size())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get W-value at specified index: " << index << std::endl
								 << "Index exceeds vector bounds (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	else return mWValues[index];
} // getWValue()

double sc_building::getDValue(const unsigned int& index) const
{ // returns the D value at the specified index
	if (index >= mDValues.size())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get D-value at specified index: " << index << std::endl
								 << "Index exceeds vector bounds (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	else return mDValues[index];
} // getDValue()

double sc_building::getHValue(const unsigned int& index) const
{ // returns the H value at the specified index
	if (index >= mHValues.size())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get H-value at specified index: " << index << std::endl
								 << "Index exceeds vector bounds (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	else return mHValues[index];
} // getHValue()

int sc_building::getBValue(const unsigned int& spaceIndex, const unsigned int& cellIndex) const
{  // returns the B value at the specified indices
	if (spaceIndex >= mBValues.size())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get B value at specified space index: " << spaceIndex << std::endl
								 << "Index exceeds vector bounds (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	else if (cellIndex >= mBValues[spaceIndex].size())
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot get cell value at specified cell index: " << cellIndex << std::endl
								 << "Index exceeds vector bounds (bso/spatial_design/ms_building.cpp)." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	else return mBValues[spaceIndex][cellIndex];
} // getBValue()

bool sc_building::operator == (const sc_building& rhs)
{
	if (mWValues != rhs.mWValues) return false;
	if (mDValues != rhs.mDValues) return false;
	if (mHValues != rhs.mHValues) return false;
	if (mBValues != rhs.mBValues) return false;
	return true;
} // operator ==

bool sc_building::operator != (const sc_building& rhs)
{
	return !(*this == rhs);
} // operator !=
	
std::ostream& operator << (std::ostream& stream, const sc_building& building)
{
	stream << building.writeToString(' ');
	return stream;
} // << operator

} // namespace spatial_design
} // namespace bso

#endif // MS_BUILDING_CPP