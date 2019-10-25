#ifndef BSO_GRAMMAR_SD_SPECIFICS_CPP
#define BSO_GRAMMAR_SD_SPECIFICS_CPP

#include <bso/utilities/trim_and_cast.hpp>

#include <boost/algorithm/string.hpp>

#include <fstream>
#include <stdexcept>
#include <sstream>

namespace bso { namespace grammar {

void grammar::mReadSDSettings(const std::string& fileName)
{
	std::fstream input(fileName.c_str());
	if (!input.is_open())
	{
		std::stringstream errorMessage;
		errorMessage << "\nCould not read SD grammar settings, failed to open file:"
								 << fileName << "\n(bso/grammar/grammar_sd_specifics.cpp)" << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}

	mSDWallProperties.clear();
	mSDFloorProperties.clear();
	mLoadPanel = structural_design::component::structure();
	mLoads.clear();
	std::map<std::pair<std::string,std::string>,std::pair<std::string,std::string> > wallStructureIDs;
	std::map<std::pair<std::string,std::string>,std::pair<std::string,std::string> > floorStructureIDs;
	std::map<std::string,std::pair<std::string,std::string> > spaceStructureIDs;
	std::map<std::string,structural_design::component::structure> flatShellStructures;
	std::map<std::string,structural_design::component::structure> beamStructures;
	std::map<std::string,structural_design::component::structure> trussStructures;
	std::map<std::string,structural_design::component::structure> quadhexStructures;

	std::string line;
	boost::char_separator<char> sep(","); // defines what separates tokens in a string
	typedef boost::tokenizer< boost::char_separator<char> > t_tokenizer; // settings for the boost::tokenizer
	char type_ID; // holds information about what type of information is described by the line currently read

	while(!input.eof())
	{
		getline(input, line); // get the next line from the file
		boost::algorithm::trim(line); // remove white space from start and end of line (to see if it is an empty line, remove any incidental white space)
		if (line == "") //skip empty lines (tokenizer does not like it)
		{
			continue; // continue to next line
		}
		t_tokenizer tok(line, sep); // tokenize the line
		t_tokenizer::iterator token = tok.begin(); // set iterator to first token

		try
		{
			type_ID = bso::utilities::trim_and_cast_char(*token);	++token; // interpret first token as type ID
		}
		catch (std::exception& e)
		{
			continue;
		}

		switch (type_ID)
		{
		case 'A':
		{
			std::string type = *token; ++token;
			boost::algorithm::trim(type);
			
			std::string structuralType = *token; ++token;
			boost::algorithm::trim(structuralType);
			std::string structuralTypeID = *token; ++token;
			boost::algorithm::trim(structuralTypeID);
			spaceStructureIDs[type] = {structuralType,structuralTypeID};
			break;
		}
		case 'B':
		{ // type settings
			std::string type1 = *token; ++token;
			boost::algorithm::trim(type1);
			std::string type2 = *token; ++token;
			boost::algorithm::trim(type2);
			auto typePair = std::minmax(type1,type2);

			std::string structuralType = *token; ++token;
			boost::algorithm::trim(structuralType);
			std::string structuralTypeID = *token; ++token;
			boost::algorithm::trim(structuralTypeID);
			wallStructureIDs[typePair] = {structuralType,structuralTypeID};

			structuralType = *token; ++token;
			boost::algorithm::trim(structuralType);
			structuralTypeID = *token; ++token;
			boost::algorithm::trim(structuralTypeID);
			floorStructureIDs[typePair] = {structuralType,structuralTypeID};
			break;
		}
		case 'C':
		{ // mesh setting
			mMeshSize = bso::utilities::trim_and_cast_uint(*token);
			break;
		}
		case 'D':
		{ // live load
			// do not break, continue to 'D' same implementation
		}
		case 'E':
		{ // wind load
			std::string loadCaseName = *token; ++token;
			boost::algorithm::trim(loadCaseName);
			structural_design::component::load_case lc(loadCaseName);

			double magnitude = utilities::trim_and_cast_double(*token); ++token;
			double azimuth = utilities::trim_and_cast_double(*token); ++token;
			double altitude = utilities::trim_and_cast_double(*token); ++token;
			utilities::geometry::vector direction(std::make_pair(azimuth,altitude));
			direction.normalize();

			std::string loadType = *token;
			boost::algorithm::trim(loadType);

			for (unsigned int i = 0; i < 3; ++i)
			{
				if (abs(direction(i)) > 1e-9)
				{
					mLoads.insert({loadType,structural_design::component::load(
						lc,magnitude*direction(i),i)});
				}
			}
			break;
		}
		case 'F':
		{ // truss property
			std::string trussID = *token; ++token;
			boost::algorithm::trim(trussID);
			double A = bso::utilities::trim_and_cast_double(*token); ++token;
			double E = bso::utilities::trim_and_cast_double(*token); ++token;
			trussStructures[trussID] = structural_design::component::structure("truss",{
				{"A",A},{"E",E}});
			break;
		}
		case 'G':
		{ // beam property
			std::string beamID = *token; ++token;
			boost::algorithm::trim(beamID);
			double width  = bso::utilities::trim_and_cast_double(*token); ++token;
			double height = bso::utilities::trim_and_cast_double(*token); ++token;
			double E = bso::utilities::trim_and_cast_double(*token); ++token;
			double v = bso::utilities::trim_and_cast_double(*token); ++token;
			beamStructures[beamID] = structural_design::component::structure("beam",{
				{"width",width},{"height",height},{"E",E},{"poisson",v}});
			break;
		}
		case 'H':
		{ // flat shell property
			std::string shellID = *token; ++token;
			boost::algorithm::trim(shellID);
			double thickness  = bso::utilities::trim_and_cast_double(*token); ++token;
			double E = bso::utilities::trim_and_cast_double(*token); ++token;
			double v = bso::utilities::trim_and_cast_double(*token); ++token;
			flatShellStructures[shellID] = structural_design::component::structure("flat_shell",{
				{"thickness",thickness},{"E",E},{"poisson",v}});
			break;
		}
		case 'I':
		{ // cuboid property
			std::string quadhexID = *token; ++token;
			boost::algorithm::trim(quadhexID);
			double E = bso::utilities::trim_and_cast_double(*token); ++token;
			double v = bso::utilities::trim_and_cast_double(*token); ++token;
			quadhexStructures[quadhexID] = structural_design::component::structure("quad_hexahedron",{
				{"E",E},{"poisson",v}});
			break;
		}
		case 'J':
		{ // low stiffness load panel
			double thickness  = bso::utilities::trim_and_cast_double(*token); ++token;
			double E = bso::utilities::trim_and_cast_double(*token); ++token;
			double v = bso::utilities::trim_and_cast_double(*token); ++token;
			mLoadPanel = structural_design::component::structure("flat_shell",{
				{"thickness",thickness},{"E",E},{"poisson",v}});
			break;
		}
		default:
			break;
		}
	} // end of file

	for (const auto& i : wallStructureIDs)
	{
		bool foundMatch = false;
		if (i.second.first == "flat_shell")
		{
			auto flatShellSearch = flatShellStructures.find(i.second.second);
			if (flatShellSearch != flatShellStructures.end())
			{
				mSDWallProperties[i.first] = flatShellSearch->second;
				foundMatch = true;
			}
		}
		else if (i.second.first == "beam")
		{
			auto beamSearch = beamStructures.find(i.second.second);
			if (beamSearch != beamStructures.end())
			{
				mSDWallProperties[i.first] = beamSearch->second;
				foundMatch = true;
			}
		}
		else if (i.second.first == "truss")
		{
			auto trussSearch = trussStructures.find(i.second.second);
			if (trussSearch != trussStructures.end())
			{
				mSDWallProperties[i.first] = trussSearch->second;
				foundMatch = true;
			}
		}
		else if (i.second.first == "none")
		{ // do nothing
			foundMatch = true;
			mSDWallProperties[i.first] = structural_design::component::structure();
		}
		if (!foundMatch)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when reading SD grammar settings, could not find\n"
									 << "the wall properties for the following structural type and type ID:\n"
									 << "\"" << i.second.first << "\" - \"" << i.second.second << "\"\n"
									 << "(bso/grammar/grammar_sd_specifics.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	}

	for (const auto& i : floorStructureIDs)
	{
		bool foundMatch = false;
		if (i.second.first == "flat_shell")
		{
			auto flatShellSearch = flatShellStructures.find(i.second.second);
			if (flatShellSearch != flatShellStructures.end())
			{
				mSDFloorProperties[i.first] = flatShellSearch->second;
				foundMatch = true;
			}
		}
		else if (i.second.first == "beam")
		{
			auto beamSearch = beamStructures.find(i.second.second);
			if (beamSearch != beamStructures.end())
			{
				mSDFloorProperties[i.first] = beamSearch->second;
				foundMatch = true;
			}
		}
		else if (i.second.first == "truss")
		{
			auto trussSearch = trussStructures.find(i.second.second);
			if (trussSearch != trussStructures.end())
			{
				mSDFloorProperties[i.first] = trussSearch->second;
				foundMatch = true;
			}
		}
		else if (i.second.first == "none")
		{ // do nothing
			foundMatch = true;
			mSDFloorProperties[i.first] = structural_design::component::structure();
		}
		if (!foundMatch)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when reading SD grammar settings, could not find\n"
									 << "the floor properties for the following structural type and type ID:\n"
									 << "\"" << i.second.first << "\" - \"" << i.second.second << "\"\n"
									 << "(bso/grammar/grammar_sd_specifics.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	}

	for (const auto& i : spaceStructureIDs)
	{
		bool foundMatch = false;
		if (i.second.first == "quad_hexahedron")
		{
			auto quadhexSearch = quadhexStructures.find(i.second.second);
			if (quadhexSearch != quadhexStructures.end())
			{
				mSDSpaceProperties[i.first] = quadhexSearch->second;
				foundMatch = true;
			}
		}
		if (!foundMatch)
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, when reading SD grammar settings, could not find\n"
									 << "the space properties for the following structural type and type ID:\n"
									 << "\"" << i.second.first << "\" - \"" << i.second.second << "\"\n"
									 << "(bso/grammar/grammar_sd_specifics.cpp)" << std::endl;
			throw std::runtime_error(errorMessage.str());
		}
	}

	input.close();
} // mReadSDSettings()

} // namespace grammar
} // namespace bso

#endif // BSO_GRAMMAR_SD_SPECIFICS_CPP
