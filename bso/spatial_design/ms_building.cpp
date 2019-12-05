#ifndef MS_BUILDING_CPP
#define MS_BUILIDNG_CPP

#include <sstream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <utility>
#include <stdexcept>
#include <exception>
#include <boost/algorithm/string.hpp>
#include <bitset>

namespace bso { namespace spatial_design {

ms_building::ms_building()
{ // empty constructor
	mLastSpaceID = 0;
} // ms_building() (empty constructor)

ms_building::ms_building(std::string fileName)
{ // initilization by string or text file
	mLastSpaceID = 0;
	std::ifstream input;
	if (!fileName.empty()) input.open(fileName.c_str());
	
	if (fileName.empty() || !input.is_open())
	{
		std::stringstream errorMessage;
		errorMessage << "Could not initialize an MS building spatial design with the following input file:" << std::endl
								 << ((fileName.empty())? "no input file given" : fileName) << std::endl
								 << "(bso/spatial_design/ms_building.cpp). " << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	
	std::string line;

	while (!input.eof()) // Parse the input file line by line
	{
		getline(input,line); // get next line from the file
		try
		{
			boost::algorithm::trim(line); // remove white space from start and end of line (to see if it is an empty line, removes any incidental white space)
			if (line == "") //skip empty lines (tokenizer does not like it)
			{
				continue; // continue to next line
			}
			else if (line.substr(0,2) == "R," || line.substr(0,2) == "r,")
			{
				line.erase(0,2);
				mSpaces.push_back(new ms_space(line));
			}
			else
			{
				continue;
			}
		}
		catch(std::exception& e)
		{
			std::stringstream errorMessage;
			errorMessage << "Encountered an error while parsing the following line of an MS input file:" << std::endl
									 << line << std::endl
									 << "(bso/spatial_design/ms_building.cpp). Got the following error: " << std::endl
									 << e.what() << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	}
	
	mLastSpaceID = this->getLastSpaceID();
	this->checkValidity();
} // ms_building() (constructor using input file)

ms_building::ms_building(const ms_building& rhs)
{ // copy constructor
	mLastSpaceID = rhs.mLastSpaceID;
	for (auto i : rhs.mSpaces)
	{
		mSpaces.push_back(new ms_space(*i));
	}
	
	this->checkValidity();
} // ms_building() (copy constructor)

ms_building::ms_building(const sc_building& sc)
{ // convert SC to MS
	try
	{
		// initialize variables and variable size
		mLastSpaceID = 0;
		std::vector<unsigned int> originIndices = {sc.getWSize(), sc.getDSize(), sc.getHSize()};
		std::vector<std::vector<double> > globalCoords(3);
		std::vector<std::vector<double> > dimensions(3);
		
		for (unsigned int i = 0; i < 3; i++)
		{
			globalCoords[i] = std::vector<double>(originIndices[i]+1);
			dimensions[i] = std::vector<double>(originIndices[i]);
			globalCoords[i][0] = 0.0;
			for (unsigned int j = 0; j < originIndices[i]; ++j)
			{
				if (i == 0) dimensions[i][j] = sc.getWValue(j);
				if (i == 1) dimensions[i][j] = sc.getDValue(j);
				if (i == 2) dimensions[i][j] = sc.getHValue(j);
				globalCoords[i][j+1] = globalCoords[i][j] + dimensions[i][j];
			}
		}

		for (unsigned int i = 0; i < sc.getBSize(); ++i)
		{ // for each space i
			// find the indices of the minium and maximum cell index that is active for space i
			unsigned int maxW = 0, maxD = 0, maxH = 0, minW = originIndices[0],
									 minD = originIndices[1], minH = originIndices[1];
			bool empty = true;
			for (unsigned int j = 1; j < sc.getBRowSize()+1; ++j)
			{
				if (sc.getBValue(i,j) != 1) continue;
				empty = false;
				unsigned int wInd = sc.getWIndex(j);
				unsigned int dInd = sc.getDIndex(j);
				unsigned int hInd = sc.getHIndex(j);
				
				if (minW > wInd) minW = wInd;
				if (minD > dInd) minD = dInd;
				if (minH > hInd) minH = hInd;
				
				if (maxW < wInd) maxW = wInd;
				if (maxD < dInd) maxD = dInd;
				if (maxH < hInd) maxH = hInd;
			}
			if (empty)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, while converting SC building into MS building\n"
										 << "encountered a space without cell assignments\n"
										 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
				throw std::runtime_error(errorMessage.str());
			}

			//get the locations and dimensions from these indices
			utilities::geometry::vertex location;
			location << globalCoords[0][minW],
									globalCoords[1][minD],
									globalCoords[2][minH];
			location.round(0);
			
			utilities::geometry::vector dimensions;
			dimensions << globalCoords[0][maxW+1],
										globalCoords[1][maxD+1],
										globalCoords[2][maxH+1];
			dimensions -= location;
			dimensions.round(0);
			
			//initialize a new space with the found location and dimensions
			mSpaces.push_back(new ms_space(sc.getBValue(i,0), location, dimensions));
			// check if the last space ID is up to date.
			if (mLastSpaceID < mSpaces.back()->getID())
			{ // if it is not, up date it
				mLastSpaceID = mSpaces.back()->getID();
			}
		}
		
		this->resetOrigin();
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "Could not convert SC building into MS building" << std::endl
								 << "(bso/spatial_design/ms_building.cpp)." << std::endl
								 << "Got the following error: " << std::endl
								 << e.what() << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	
	
} // ms_building() (convert SC to MS)

ms_building::~ms_building()
{ // destructor
	for (auto i : mSpaces) delete i;
} // ~ms_building()

void ms_building::checkValidity() const
{
	
} // checkValidity()

void ms_building::writeToFile(std::string fileName) const
{
	std::ofstream output;
	if (!fileName.empty()) output.open(fileName.c_str());
	
	if (fileName.empty() || !output.is_open())
	{
		std::stringstream errorMessage;
		errorMessage << "Could not open the following file to write an MS building spatial design to:" << std::endl
								 << ((fileName.empty())? "no input file given" : fileName) << std::endl
								 << "(bso/spatial_design/ms_building.cpp). " << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	
	output << *this;
} // writeToFile()

std::vector<ms_space*> ms_building::getSpacePtrs() const
{
	return mSpaces;
} //getSpacePtrs()

ms_space* ms_building::getSpacePtr(const ms_space& space) const
{
	for (auto i : mSpaces)
	{
		if (/*i == &space ||*/ *i == space)
		{
			return i;
		}
	}
	
	// if this part of code is reached, the space was not found in the preceding for loop
	std::stringstream errorMessage;
	errorMessage << "Could not find the following space in an MS building spatial design:" << std::endl
							 << space << std::endl
							 << "(bso/spatial_design/ms_building.cpp). " << std::endl;
	throw std::runtime_error(errorMessage.str());
} // getSpacePtr()

ms_space* ms_building::getSpacePtr(const ms_space* spacePtr) const
{
	return getSpacePtr(*spacePtr);
}

unsigned int ms_building::getLastSpaceID() const
{
	for (auto i : mSpaces)
	{
		if (mLastSpaceID < i->getID()) mLastSpaceID = i->getID();
	}
	return mLastSpaceID;
} // getLastSpaceID()

double ms_building::getVolume() const
{
	double volume = 0;
	for (auto i : mSpaces) volume += i->getVolume();
		
	return volume;
} // getVolume()

double ms_building::getFloorArea() const
{
	double area = 0;
	for (const auto& i : mSpaces) area += i->getFloorArea();
	return area;
}

std::vector<ms_space*> ms_building::selectSpacesGeometrically(
		const bso::utilities::geometry::vertex& location,
		const bso::utilities::geometry::vector& direction,
		const bool includePartialSpaces /*= false*/) const
{
	std::vector<ms_space*> spaceSelection;
	for (const auto& i : mSpaces)
	{
		bool allVerticesSelected = true;
		bool oneOrMoreVerticesSelected = false;
		for (const auto& j : i->getGeometry())
		{
			double checkValue = direction.dot(j - location);
			if (checkValue >= -1e-3)
			{ // check if point j lies behind the plane defined by the location and the direction
				if (checkValue >= 1e-3) oneOrMoreVerticesSelected = true;
			}
			else
			{
				allVerticesSelected = false;
			}
		}
		if (allVerticesSelected) spaceSelection.push_back(i);
		else if (oneOrMoreVerticesSelected && includePartialSpaces) spaceSelection.push_back(i);
	}
	return spaceSelection;
}

void ms_building::setZZero()
{
	this->resetOrigin({2});
} // setZZero()

void ms_building::resetOrigin(const std::vector<unsigned int>& indices /*= {0,1,2}*/)
{
	utilities::geometry::vector coordDifference = {0,0,0};
	for (const auto& index : indices)
	{
		double min = mSpaces[0]->getCoordinates()(index); // set an initial value to the minimum
	
		// find the minimum value of the z-coordinates in the building
		for (auto i : mSpaces) 
		{
			double coord = i->getCoordinates()(index);
			if (min > coord) min = coord;
		}			

		coordDifference(index) -= min;
	}
	
	if (coordDifference(2) > 0) coordDifference(2) = 0;
	
	for (auto i : mSpaces) i->setCoordinates(i->getCoordinates() + coordDifference);
	this->snapOn({{0,1},{1,1},{2,1}});
} // resetOrigin()

void ms_building::addSpace(const ms_space& space)
{
	try
	{
		mSpaces.push_back(new ms_space(space));
		checkValidity();
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "Could not add the following space to an MS building spatial design: " << std::endl
								 << space << std::endl
								 << "(bso/spatial_design/ms_building.cpp). Got the following error message: " << std::endl
								 << e.what() << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // addSpace

void ms_building::deleteSpace(ms_space* spacePtr)
{
	if (std::find(mSpaces.begin(), mSpaces.end(), spacePtr) == mSpaces.end()) spacePtr = getSpacePtr(spacePtr);
	try
	{
		mSpaces.erase(std::remove(mSpaces.begin(), mSpaces.end(), spacePtr), mSpaces.end());
	}
	catch(std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "Could not delete the following space from an MS building spatial design: " << std::endl
								 << *spacePtr << std::endl
								 << "(bso/spatial_design/ms_building.cpp). Got the following error message: " << std::endl
								 << e.what() << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // deleteSpace()

void ms_building::deleteSpace(ms_space& space)
{
	this->deleteSpace(this->getSpacePtr(space));
}

void ms_building::cutOff(const bso::utilities::geometry::vertex& location,
		const bso::utilities::geometry::vector& direction, const double& tol /*= 1e-3*/)
{
	// normalize the direction for convenience of later use
	auto n = direction.normalized();
	// check if the cutting plane is orhtogonal
	bool isOrthogonal = false;
	for (unsigned int i = 0; i < 3; ++i)
	{
		bso::utilities::geometry::vector check = {0,0,0};
		check(i) = 1;
		if (check.isParallel(n, tol))
		{
			isOrthogonal = true;
			break;
		}
	}
	if (!isOrthogonal)
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, could not cut-off part of the building in following location:\n"
								 << location << ", and the following direction:\n"
								 << direction << ". Direction is not orthogonal.\n"
								 << "(bso/spatial_design/ms_building.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	
	// remove spaces that are completely included by the cut-off plane
	auto selection = this->selectSpacesGeometrically(location,n,false);
	for (auto& i : selection) this->deleteSpace(i);
	
	// select spaces that are cut by the cut-off plane
	selection = this->selectSpacesGeometrically(location,n,true);
	for (auto& i : selection)
	{// cut each space at the cutting plane
		bso::utilities::geometry::vertex coords = i->getCoordinates();
		bso::utilities::geometry::vector tempSum = coords + i->getDimensions();
		bso::utilities::geometry::vector vec1 = coords - location;

		if (n.dot(vec1) < 0)
		{
			vec1 = tempSum - location;
			double distance = n.dot(vec1);
			tempSum -= n* distance;
		}
		else
		{
			double distance = n.dot(vec1);
			coords -= n * distance;
		}
		
		i->setCoordinates(coords);
		i->setDimensions(tempSum - coords);
	}
}

void ms_building::sweep(const bso::utilities::geometry::vertex& location,
		const std::vector<std::pair<unsigned int, double> >& distances 
		/*= {{0,1.0},{1,0.0},{2,0.0}}*/, const double& tol /*=1e-3*/)
{
	bso::utilities::geometry::vertex coords;
	bso::utilities::geometry::vector tempSum;
	
	unsigned int axis;
	double distance;
	bool checkDouble[3] = {false};
	
	for (auto i : distances)
	{
		axis = i.first;
		if (axis != 0 && axis != 1 && axis != 2)
		{
			std::stringstream errorMessage;
			errorMessage << "Trying to sweep an MS building spatial design over the \n"
									 << "following non-existent axis: " << axis << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		if (checkDouble[axis])
		{
			std::stringstream errorMessage;
			errorMessage << "Defined a scaling factor to scale and MS building \n"
									 << "spatial design twice for the same axis, axis: " << axis << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		else checkDouble[axis] = true;
	}
	
	for (auto i : mSpaces)
	{
		coords = i->getCoordinates();
		tempSum = coords + i->getDimensions();

		for (auto j : distances)
		{
			axis = j.first;
			distance = j.second;
			
			if (distance < 0)
			{
				if (coords(axis)  + tol < location(axis)) coords(axis)  += distance;
				if (tempSum(axis) + tol < location(axis)) tempSum(axis) += distance;
			}
			else 
			{
				if (coords(axis)  - tol > location(axis)) coords(axis)  += distance;
				if (tempSum(axis) - tol > location(axis)) tempSum(axis) += distance;
			}
		}

		i->setCoordinates(coords);
		i->setDimensions(tempSum - coords);
	}
} // sweep()

void ms_building::scale(const std::vector<std::pair<unsigned int, double> >& scales /*= {{0,sqrt(2.0)},{1,sqrt(2.0)}}*/)
{
	utilities::geometry::vertex coords;
	utilities::geometry::vector tempSum;
	
	unsigned int axis;
	double n;
	bool checkDouble[3] = {false};
	
	for (auto i : scales)
	{
		axis = i.first;
		if (axis != 0 && axis != 1 && axis != 2)
		{
			std::stringstream errorMessage;
			errorMessage << "Trying to sweep an MS building spatial design over the \n"
									 << "following non-existent axis: " << axis << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		if (checkDouble[axis])
		{
			std::stringstream errorMessage;
			errorMessage << "Defined a scaling factor to scale and MS building \n"
									 << "spatial design twice for the same axis, axis: " << axis << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		else checkDouble[axis] = true;
	}
	
	for (auto i : mSpaces)
	{
		coords = i->getCoordinates();
		tempSum = coords + i->getDimensions();

		for (auto j : scales)
		{
			axis = j.first;
			n = j.second;
			
			coords(axis)  *= n;
			tempSum(axis) *= n;
		}

		i->setCoordinates(coords);
		i->setDimensions(tempSum - coords);
	}
} // scale()

void ms_building::splitSpace(ms_space* spacePtr, const std::vector<std::pair<unsigned int, unsigned int> >& splits /*= {{0,2},{1,2}}*/)
{	
	this->getLastSpaceID();
	utilities::geometry::vertex coords  = spacePtr->getCoordinates();
	utilities::geometry::vector tempSum = coords + spacePtr->getDimensions();
	
	std::vector<double> splitValues[3];
	for (unsigned int i = 0; i < 3; i++)
	{
		splitValues[i] = std::vector<double>();
		splitValues[i].push_back(coords(i));
		splitValues[i].push_back(tempSum(i));
	}
	
	unsigned int axis, div;
	bool checkDouble[3] = {false};
	
	for (auto i : splits)
	{
		axis = i.first;
		div = i.second;
		
		if (axis != 0 && axis != 1 && axis != 2)
		{
			std::stringstream errorMessage;
			errorMessage << "Trying to split space \"" << spacePtr->getID() << "\" over a non-existing axis: " << axis << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		if (div < 2)
		{
			std::stringstream errorMessage;
			errorMessage << "Trying to split space \"" << spacePtr->getID() << "\" into less than two new spaces: n_split = " << div << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		if (checkDouble[axis])
		{
			std::stringstream errorMessage;
			errorMessage << "Defined a split for space \"" << spacePtr->getID() << "\" twice for the same axis, axis: " << axis << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		else checkDouble[axis] = true;
		
		double delta = floor((splitValues[axis][1] - splitValues[axis][0])/div);

		for (unsigned int i = 0; i < div-1; i++)
		{
			double nextCoord;
			nextCoord = splitValues[axis][i] + delta;
			splitValues[axis].insert(splitValues[axis].begin() + i + 1,nextCoord);
		}
	}
	
	ms_space* temp;
	for (unsigned int i = 0; i < splitValues[0].size() - 1; i++)
	{
		for (unsigned int j = 0; j < splitValues[1].size() - 1; j++)
		{
			for (unsigned int k = 0; k < splitValues[2].size() - 1; k++)
			{
				temp = new ms_space(*spacePtr);
				temp->setID(++mLastSpaceID);
				coords << splitValues[0][i], splitValues[1][j], splitValues[2][k];
				temp->setCoordinates(coords);
				tempSum << splitValues[0][i+1], splitValues[1][j+1], splitValues[2][k+1];
				temp->setDimensions(tempSum - coords);
				mSpaces.push_back(temp);
			}
		}
	}
	
	deleteSpace(spacePtr);
} // splitSpace()

void ms_building::splitSpace(ms_space& space, const std::vector<std::pair<unsigned int, unsigned int> >& splits /*= {{0,2},{1,2}}*/)
{
	this->splitSpace(this->getSpacePtr(space), splits);
} // splitSpace()

void ms_building::snapOn(const std::vector<std::pair<unsigned int, double> >& snaps /*= {{0,10},{1,10}}*/)
{
	utilities::geometry::vertex coords;
	utilities::geometry::vector tempSum;
	
	unsigned int axis;
	double n;
	bool checkDouble[3] = {false};
	
	for (auto i : snaps)
	{
		axis = i.first;
		if (axis != 0 && axis != 1 && axis != 2)
		{
			std::stringstream errorMessage;
			errorMessage << "Trying to snap-on an MS building spatial to a grid over the following non-existent axis: " << axis << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		
		if (checkDouble[axis])
		{
			std::stringstream errorMessage;
			errorMessage << "Defined a snap-on for an MS building spatial design twice for the same axis, axis: " << axis << std::endl
									 << "(bso/spatial_design/ms_building.cpp)." << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
		else checkDouble[axis] = true;
	}
	
	for (auto i : mSpaces)
	{
		coords = i->getCoordinates();
		tempSum = coords + i->getDimensions();
		
		for (auto j : snaps)
		{
			axis = j.first;
			n = j.second;

			coords(axis)  = round(coords(axis)  / n) * n;
			tempSum(axis) = round(tempSum(axis) / n) * n;
		}
		
		i->setCoordinates(coords);
		i->setDimensions(tempSum - coords);
	}
} // snapOn()

bool ms_building::hasOverlappingSpaces(std::multimap<ms_space*,ms_space*>& overlappingSpaces,
	const double tol /*= 1e-3*/) const
{
	std::vector<bso::utilities::geometry::quad_hexahedron> spaceGeoms;
	for (const auto& i : mSpaces)
	{
		spaceGeoms.push_back(i->getGeometry());
	}
	
	for (unsigned int i = 0; i < spaceGeoms.size(); ++i)
	{
		for (unsigned int j = 0; j < spaceGeoms.size(); ++j)
		{
			if (i == j) continue;
			bool foundOverlap = false;
			for (const auto& k : spaceGeoms[i])
			{
				if (spaceGeoms[j].isInside(k,tol))
				{
					overlappingSpaces.emplace(mSpaces[j], mSpaces[i]);
					foundOverlap = true;
					break;
				}
			}
			if (foundOverlap) break;
			unsigned int linePolygonIntersections = 0;
			for (const auto& k : spaceGeoms[j].getLines())
			{
				for (const auto& l : spaceGeoms[i].getPolygons())
				{
					if (l->intersectsWith(k,tol))
					{
						++linePolygonIntersections;
						if (linePolygonIntersections == 2)
						{
							overlappingSpaces.emplace(mSpaces[j], mSpaces[i]);
							foundOverlap = true;
							break;
						}
					}
				}
				if (foundOverlap) break;
			}
		}
	}
	
	return (overlappingSpaces.size() != 0);
} // hasOverlappingSpaces()

bool ms_building::hasFloatingSpaces(std::vector<ms_space*>& floatingSpaces, 
	const double tol /*= 1e-3*/) const
{
	std::vector<bso::utilities::geometry::quad_hexahedron> groundedSpaceGeoms;
	floatingSpaces = mSpaces;
	bool hasFloatingSpaces = false;
	bool addedSpaceToGroundedSpaceGeoms = true; // initial
	while (addedSpaceToGroundedSpaceGeoms)
	{
		addedSpaceToGroundedSpaceGeoms = false;
		auto spaceIt = floatingSpaces.begin();
		while (spaceIt != floatingSpaces.end())
		{
			auto spaceGeom = (*spaceIt)->getGeometry();

			// check if this space has any vertex with a z-coordinate at or below zero
			bool isGrounded = false;
			for (const auto& j : spaceGeom)
			{
				if (j(2) < abs(tol))
				{
					isGrounded = true;
					break;
				}
			}
			if (isGrounded)
			{
				groundedSpaceGeoms.push_back(spaceGeom);
				addedSpaceToGroundedSpaceGeoms = true;
				floatingSpaces.erase(spaceIt);
				continue;
			}
			
			// check if this space is connected to any of the grounded spaces
			bool isConnectedToGroundedSpace = false;
			for (const auto& i : groundedSpaceGeoms)
			{
				for (const auto& j : spaceGeom)
				{
					if (i.isInside(j,tol))
					{ // if any vertex j of the space is inside a grounded space
						isConnectedToGroundedSpace = true;
						break;
					}
					for (const auto& k : i.getPolygons())
					{
						if (k->isInside(j,tol))
						{ // if any vertex j of the space is inside the surface of a grounded space
							isConnectedToGroundedSpace = true;
							break;
						}
					}
					if (isConnectedToGroundedSpace) break;
					for (const auto& k : i.getLines())
					{
						if (k.isOnLine(j,tol))
						{ // if any vertex j of the space is on a line segment of a grounded space
							isConnectedToGroundedSpace = true;
							break;
						}
					}
					if (isConnectedToGroundedSpace) break;
					for (const auto& k : i)
					{
						if (k.isSameAs(j,tol))
						{ // if any vertex j of the space is collocated with a vertex of a grounded space
							isConnectedToGroundedSpace = true;
							break;
						}
					}
					if (isConnectedToGroundedSpace) break;
				}
				for (const auto& j : i)
				{
					if (spaceGeom.isInside(j,tol))
					{ // if any vertex j of a grounded space i is inside the space
						isConnectedToGroundedSpace = true;
						break;
					}
				}
				if (isConnectedToGroundedSpace) break;
			}
			
			if (isConnectedToGroundedSpace)
			{
				groundedSpaceGeoms.push_back(spaceGeom);
				addedSpaceToGroundedSpaceGeoms = true;
				floatingSpaces.erase(spaceIt);
				continue;
			}
			
			++spaceIt;
		}
	}
	
	
	return (floatingSpaces.size() != 0);
} // hasFloatingSpaces()

bool ms_building::operator == (const ms_building& rhs)
{
	if (mSpaces.size() != rhs.mSpaces.size()) return false;
	
	// check if rhs contains the same spaces, assuming they may be in different order
	std::vector<ms_space*> cmp = rhs.mSpaces; // copy the vector containing the spaces to safe computation time, matches can be removed from this
	std::reverse(cmp.begin(), cmp.end()); // reverse the vector, since items will be removed we want to iterate from the back, however if vectors are same order we want to benefit from that to safe computation time

	for (auto i : mSpaces)
	{
		bool found = false;
		for (unsigned int j = cmp.size(); j > 0; j--)
		{
			if (*i == *(cmp[j - 1]))
			{
				found = true;
				cmp.erase(cmp.begin() + j - 1);
				break;
			}
		}
		if (!found) return false;
	}
	return true;
}

bool ms_building::operator != (const ms_building& rhs)
{
	return !(*this == rhs);
}

ms_building::operator sc_building() const
{ // convert MS to SC
	try
	{
		sc_building sc;
		bso::utilities::geometry::vertex minMSPoint = {0,0,0};
		std::vector<std::vector<double> > coordValues(3); // {xValues,yValues,Zvalues}
		
		// store all coordinate values in this MS building model in the three std::vectors
		for (auto i : mSpaces)
		{
			utilities::geometry::vertex p1 = i->getCoordinates();
			utilities::geometry::vertex p2 = i->getDimensions() + p1;
			for (unsigned int j = 0; j < 3; j++)
			{
				coordValues[j].push_back(p1(j));
				coordValues[j].push_back(p2(j));
			}
		}
		
		// sort the three vectors with coordinate values, and remove duplicates
		for (auto& i : coordValues)
		{
			sort(i.begin(), i.end());
			i.erase(std::unique(i.begin(), i.end()), i.end());
		}
		
		// create the width, depth and height vectors of the supercube, and compute the size of the bitmask of the b vectors
		unsigned int cubeSize = 1;
		for (unsigned int i = 0; i < coordValues.size(); i++)
		{
			std::vector<double> tempDimensions;
			for (unsigned int j = 0; j < coordValues[i].size()-1; j++)
			{
				tempDimensions.push_back(coordValues[i][j+1] - coordValues[i][j]);
			}
			
			cubeSize *= tempDimensions.size();
			if (i == 0) sc.mWValues = tempDimensions;
			else if (i == 1) sc.mDValues = tempDimensions;
			else if (i == 2) sc.mHValues = tempDimensions;
		}
		
		// for each space, create an emtpy bit mask, and subsequently find out which cells in that mask belong to the space, and should be activated
		for (auto i : mSpaces)
		{
			sc.mBValues.push_back(std::vector<int>(cubeSize+1));
			sc.mBValues.back()[0] = i->getID();
			
			utilities::geometry::vertex p1 = i->getCoordinates();
			utilities::geometry::vertex p2 = i->getDimensions() + p1;

			for (unsigned int j = 1; j < sc.mBValues.back().size(); j++)
			{
				utilities::geometry::vertex pCheck;
				pCheck[0] = coordValues[0][sc.getWIndex(j)] + coordValues[0][sc.getWIndex(j)+1];
				pCheck[1] = coordValues[1][sc.getDIndex(j)] + coordValues[1][sc.getDIndex(j)+1];
				pCheck[2] = coordValues[2][sc.getHIndex(j)] + coordValues[2][sc.getHIndex(j)+1];
				pCheck /= 2;
			
				bool belongsToSpace = true;
				for (unsigned int k = 0; k < 3; k++)
				{
					if (!(pCheck[k] > p1[k] && pCheck[k] < p2[k]))
					{
						belongsToSpace = false;
						break;
					}
				}
				sc.mBValues.back()[j] = belongsToSpace;
			}
		}
		sc.checkValidity();

		return sc;
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "Could not convert MS building into SC building" << std::endl
								 << "(bso/spatial_design/ms_building.cpp)." << std::endl
								 << "Got the following error: " << std::endl
								 << e.what() << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
} // sc_building() (convert MS to SC)

std::ostream& operator <<(std::ostream& stream, const ms_building& building)
{
	bool first = true;
	for (auto i : building.getSpacePtrs())
	{
		if (!first) stream << std::endl;
		else first = false;
		stream << *i;
	}
	
	return stream;
} // << operator

} // namespace spatial_design
} // namespace bso

#endif // MS_BUILDING_CPP