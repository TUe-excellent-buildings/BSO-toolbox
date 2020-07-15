#include <iostream>

#include <bso/spatial_design/ms_building.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/utilities/geometry.hpp>
#include <bso/utilities/data_point.hpp>
#include <bso/structural_design/sd_model.hpp>
#include <bso/building_physics/bp_model.hpp>
#include <bso/grammar/grammar.hpp>
#include <bso/visualization/visualization.hpp>

#include <boost/algorithm/string.hpp>
#include <vector>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <map>

std::vector<std::string> VIS_OPTIONS = {"ms","sc","rc","cb","sd","fe","bp"};
std::vector<std::string> MOD_OPTIONS = {"bp","sd","sdbp","bpsd"};
std::vector<std::string> OUT_OPTIONS = {"result_line", "verbose", "ms_files","best","all"};

int end, begin = 0;
template<class T>
void out(const T& t, const bool& e = false,
				 const bool& i = false, const bool& verbose = false){
	if (!verbose) return;
	std::cout << t;
	end = clock();
	if (i) std::cout << " (" << 1000*(end-begin)/CLOCKS_PER_SEC << " ms)";
	if (e) std::cout << std::endl;
	begin = end;
} // out()

using namespace bso;
int main(int argc, char* argv[])
{
	std::string inputFile = "";
	std::string designName = "";
	std::string inputLine = "";
	std::string modification = "";
	std::vector<std::string> visualizations;
	bool result_line = false;
	bool ms_files = false;
	bool verbose = false;
	bool outputAll = false;
	bool outputBest = false;
	unsigned int iterations = 0;
	
	std::vector<std::string> args(argv+1, argv+argc);
	auto arg = args.begin();
	if (arg == args.end())
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, expected arguments, received nothing.\n"
								 << "use -h or --help for help." << std::endl;
		throw std::invalid_argument(errorMessage.str());
	}
	while (arg != args.end())
	{
		if (*arg == "-h" || *arg == "--help")
		{
			std::cout << "This program ...." << std::endl;
			std::cout << std::endl;
			std::cout << "-i\t" << "or --input, to specify an input file,\n"
								<<   "\t" << "expects a string containing the file name\n"
								<<   "\t" << "of the input file. The input file should be\n"
								<<   "\t" << "in \'Movable Sizable\' format.\n";
			std::cout << "-l\t" << "or --line, to specify an input line for a\n"
								<<   "\t" << "supercube. Starts with design name, the a series\n"
								<<   "\t" << "integers and doubles that define the supercube\n";
			std::cout << "-m\t" << "or --modification, to specify which modification\n"
								<<   "\t" << "will be applied: \'sd\' for the sd-modifications;\n"
								<<   "\t" << "\'bp\' for the bp_modifications; \'sdbp\' to alternate\n"
								<<   "\t" << "between sd and bp, starting with sd; \'bpsd\', dito\n"
								<<   "\t" << "but now starting with bp. Only one option is possible.\n";
			std::cout << "-n\t" << "or --nModifications, to specify the number of modifications\n"
								<<   "\t" << "that will be applied. Expects only a positive integer\n";
			std::cout << "-o\t" << "or --output, to specify the diferent outputs:\n"
								<<   "\t" << "\'result_line\' prints a line with the input\n"
								<<   "\t" << "parameters and the results; \'verbose\' prints\n"
								<<   "\t" << "a descriptive outline of the process;\n"
								<<   "\t" << "\'ms_files\' will write the ms designs to files\n"
								<<   "\t" << "\'all\' will write all the designs to the specified format\n"
								<<   "\t" << "\'best\' (default) will only write the best all the designs to\n"
								<<   "\t" << "the specified format. More than one option can be given,\n"
								<<   "\t" << "but \'all\' and \'best\' cannot be given simultaneously.\n";
			std::cout << "-v\t" << "or --visualization, to specify which models are\n"
								<<   "\t" << "visualized: \'ms\' for \'Movable Sizable\',\n"
								<<   "\t" << "\'sc\' for \'Super Cube\', \'rc\' for \'conformal model with\n"
								<<   "\t" << "ReCtangles\', \'cb\' for \'conformal model with CuBoids\'\n"
								<<   "\t" << "\'sd\' for'\'Structural Design\', \'fe\' for \'Finite\n"
								<<   "\t" << "Elements of the structural design model\',\n"
								<<   "\t" << "and \'bp\' for the \'Building Physics model\'.\n"
								<<   "\t" << "More than one can be specified.\n";
			std::cout << "\n\n" << "Press ENTER to continue..." << std::endl;
			std::cin.get();
			return 0;
		}
		else if (*arg == "-i" || *arg == "--input")
		{
			if (++arg == args.end() || arg->operator[](0) == '-')
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, expected the name of the input file after -i or --input\n"
										 << "Use -h or --help for help" << std::endl;
				throw std::invalid_argument(errorMessage.str());
			}
			inputFile = *arg;
			boost::trim(inputFile);
			designName = inputFile;
			++arg;
		}
		else if (*arg == "-l" || *arg == "--line")
		{
			std::stringstream scInputLine;
			bool first = true;
			if (++arg == args.end() || arg->operator[](0) == '-')
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, expected a design name after -l or --line.\n";
				throw std::invalid_argument(errorMessage.str());
			}
			
			designName = *arg;
			boost::trim(designName);
			
			while (++arg != args.end() && arg->operator[](0) != '-')
			{
				std::string temp = *arg;
				boost::trim(temp);
				if (first) first = false;
				else scInputLine << "\t";
				scInputLine << temp;
			}
			if (first)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, expected a value for the input line\n"
										 << "specification after -l or --line.\n";
				throw std::invalid_argument(errorMessage.str());
			}
			inputLine = scInputLine.str();
		}
		else if (*arg == "-m" || *arg == "--modification")
		{
			if (++arg == args.end() || arg->operator[](0) == '-')
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, expected a modification argument to be\n"
										 << "specified after -m or --modification. Use -h or\n"
										 << "--help for help\n";
				throw std::invalid_argument(errorMessage.str());
			}
			modification = *arg;
			boost::trim(modification);
			if (std::find(MOD_OPTIONS.begin(),MOD_OPTIONS.end(),modification) == MOD_OPTIONS.end())
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, did not recognize: \"" << modification << "\"\n"
										 << "as an option for -m or --modification.\n"
										 << "use -h or --help for help\n";
				throw std::invalid_argument(errorMessage.str());
			}
			
			if (++arg != args.end() && arg->operator[](0) != '-')
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, only one argument can be specified after\n"
										 << "-m or --modification. Use -h or -- help for help\n";
				throw std::invalid_argument(errorMessage.str());
			}				
		}
		else if (*arg == "-n" || *arg == "--nModifications")
		{
			if (++arg == args.end() || arg->operator[](0) == '-')
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, expected a positive integer to be\n"
										 << "specified after -n or --nModifications. Use -h or\n"
										 << "--help for help\n";
				throw std::invalid_argument(errorMessage.str());
			}
			try
			{
				iterations = bso::utilities::trim_and_cast_uint(*arg);
			}
			catch (std::exception& e)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, could not parse argument specified after\n"
										 << "-n or --nModifications. received the following error:\n"
										 << e.what()
										 << "Use -h or -- help for help\n";
				throw std::invalid_argument(errorMessage.str());
			}
			
			if (++arg != args.end() && arg->operator[](0) != '-')
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, only one argument can be specified after\n"
										 << "-n or --nModifications. Use -h or -- help for help\n";
				throw std::invalid_argument(errorMessage.str());
			}				
		}
		else if (*arg == "-o" || *arg == "--output")
		{
			while (++arg != args.end() && arg->operator[](0) != '-')
			{
				std::string outSpec = *arg;
				boost::trim(outSpec);
				if 			(outSpec == "result_line") result_line = true;
				else if (outSpec == "verbose") verbose = true;
				else if (outSpec == "ms_files") ms_files = true;
				else if (outSpec == "all") outputAll = true;
				else if (outSpec == "best") outputBest = true;
				else
				{
					std::stringstream errorMessage;
					errorMessage << "\nError, did not recognize: " << outSpec << " as an\n"
											 << "argument for -o or --output. Use -h or\n"
											 << "--help for help\n";
					throw std::invalid_argument(errorMessage.str());
				}
			}
		}
		else if (*arg == "-v" || *arg == "--visualize")
		{
			while (++arg != args.end() && arg->operator[](0) != '-')
			{
				std::string visSpec = *arg;
				boost::trim(visSpec);
				if (std::find(VIS_OPTIONS.begin(),VIS_OPTIONS.end(),visSpec) == VIS_OPTIONS.end())
				{
					std::stringstream errorMessage;
					errorMessage << "\nError, did not recognize: \"" << visSpec << "\"\n"
											 << "as an option for -v or --visualization as a specification\n"
											 << "of a model to visualize, use -h or --help for help\n";
					throw std::invalid_argument(errorMessage.str());
				}
				visualizations.push_back(visSpec);
			}
			if (visualizations.size() == 0)
			{
				std::stringstream errorMessage;
				errorMessage << "\nError, expected at least one string after\n"
										 << "-v or --visualize, received nothing. Use -h or\n"
										 << "--help for help\n";
				throw std::invalid_argument(errorMessage.str());
			}
		}
		else
		{
			std::stringstream errorMessage;
			errorMessage << "\nError, did not recognize argument: " << *arg << std::endl;
			throw std::invalid_argument(errorMessage.str());
		}
	}
	if (inputFile == "" && inputLine == "")
	{
		std::stringstream errorMessage;
		errorMessage << "\nError, expected an input file or line to be specified.\n"
								 << "use -h or --help for help\n";
		throw std::invalid_argument(errorMessage.str());
	}
	if ((ms_files || result_line) && (outputAll && outputBest))
	{
		std::stringstream errorMessage;
		errorMessage << "Cannot specify both \'all\'  and \'best\' for output.\n"
								 << "use -h or --help for help\n";
		throw std::invalid_argument(errorMessage.str());
	}
	if (!outputAll && !outputBest) outputBest = true;
	out("Parsed program arguments", true, true, verbose);
	if (!visualizations.empty()) visualization::initVisualization(argc,argv);
	out("Initialized visualization", true, true, verbose);
	
	std::vector<spatial_design::ms_building> msDesigns;
	try
	{
		if (!inputFile.empty()) msDesigns.push_back(spatial_design::ms_building(inputFile));
		else
		{
			spatial_design::sc_building sc(inputLine);
			msDesigns.push_back(spatial_design::ms_building(sc));
		}
	}
	catch (std::exception& e)
	{
		std::stringstream errorMessage;
		errorMessage << "Error, could not initialize MS building design using\n";
		if (!inputFile.empty())
			errorMessage << "the following MS input file: " << inputFile << "\n";
		else
			errorMessage << "an SC model initialized by the following line:\n" << inputLine;
		errorMessage << "Received the following error:\n" << e.what() << std::endl;
		
		throw std::invalid_argument(errorMessage.str());
	}
	out("Initialized initial MS building model", true, true, verbose);
	
	unsigned int nSpaces = msDesigns.back().getSpacePtrs().size();
	double floorArea = msDesigns.back().getFloorArea();
	std::vector<spatial_design::sc_building> scDesigns;
	std::vector<structural_design::sd_model> sdModels;
	std::vector<building_physics::bp_model> bpModels;
	std::vector<utilities::data_point> designPerformances;
	for (unsigned int i = 0; i <= iterations; ++i)
	{
		out("Iteration: ",false,false,verbose); out(i,true,false,verbose);
		if (std::find(visualizations.begin(),visualizations.end(),"sc") != visualizations.end() ||
				result_line)
		{
			scDesigns.push_back(spatial_design::sc_building(msDesigns.back()));
			out("initialized sc building",true,true,verbose);
		}
		designPerformances.push_back(utilities::data_point(2));

		// step 1, generate SD and BP models and evaluate them.
		spatial_design::cf_building cf(msDesigns.back(),1e-6);
		out("Initialized conformal model",true,true,verbose);

		grammar::grammar grm(cf);
		out("Initialized grammar",true,true,verbose);
		
		sdModels.push_back(grm.sd_grammar<grammar::DEFAULT_SD_GRAMMAR>(std::string("settings/sd_settings.txt")));
		out("Generated structural model",true,true,verbose);
		
		sdModels.back().analyze();
		out("Evaluated structural model: ",false,false,verbose);
		double sdResult = sdModels.back().getTotalResults().mTotalStrainEnergy;
		out(sdResult,true,true,verbose);
		designPerformances.back()(0) = sdResult;
		
		bpModels.push_back(grm.bp_grammar<grammar::DEFAULT_BP_GRAMMAR>(std::string("settings/bp_settings.txt")));
		out("Generated building physics model",true,true,verbose);
		
		bpModels.back().simulatePeriods("runge_kutta_dopri5",1e-3,1e-3);
		out("Evaluated building physics model: ",false,false,verbose);
		double bpResult = bpModels.back().getTotalResults().mTotalEnergy;
		out(bpResult,true,true,verbose);
		designPerformances.back()(1) = bpResult;
		
		if (!visualizations.empty()) for (const auto& visOption : visualizations)
		{
			if (visOption == "ms") visualization::visualize(msDesigns.back(),"","ms_building",4.0);
			if (visOption == "sc") visualization::visualize(scDesigns.back());
			if (visOption == "rc") visualization::visualize(cf,"rectangle");
			if (visOption == "cb") visualization::visualize(cf,"cuboid");
			if (visOption == "sd") visualization::visualize(sdModels.back());
			if (visOption == "fe") visualization::visualize(sdModels.back(), "element");
			if (visOption == "bp") visualization::visualize(bpModels.back());
		}
		out("Sent models to visualization",true,true,verbose);
		if (i >= iterations) break;
		
		// create a new MS model
		spatial_design::ms_building newMS = msDesigns.back();
		
		// step 2, get performances per space
		std::map<spatial_design::ms_space*, utilities::data_point> performances;
		for (const auto& j : newMS.getSpacePtrs())
		{
			utilities::data_point tempDataPoint({0,0});
			std::stringstream spaceString;
			spaceString << *j;
			tempDataPoint.addLabel(spaceString.str());

			auto spaceGeom = j->getGeometry();
			if (modification != "bp") tempDataPoint(1) = sdModels.back().getPartialResults(&spaceGeom).mTotalStrainEnergy;
			if (modification != "sd") tempDataPoint(0) = bpModels.back().getPartialResults(&spaceGeom).mTotalEnergy;
			tempDataPoint /= j->getFloorArea();
			performances.emplace(j,tempDataPoint);
		}
		out("Collected performances for each space",true,true,verbose);

		// step 3, create geometric clusters
		std::vector<std::pair<utilities::geometry::vertex,
			utilities::geometry::vector> > geometricCutOff;
		std::vector<utilities::data_point> geometricPerformances;
		for (unsigned int j = 0; j < 2; ++j)
		{
			int dirMultiplier = (j==0)? 1:-1;
			double maxCoordValue;
			
			for (unsigned int k = 0; k < 3; ++k)
			{
				utilities::geometry::vector dir = {0,0,0};
				utilities::geometry::vertex loc;
				dir(k) = 1;
				dir *= dirMultiplier;

				bool firstCheck = true;
				for (const auto& l : newMS.getSpacePtrs())
				{
					utilities::geometry::vertex spaceOrigin = l->getCoordinates();
					utilities::geometry::vertex spaceMax = spaceOrigin + l->getDimensions();
					
					double checkValue;
					if (dirMultiplier < 0) checkValue = spaceOrigin(k);
					else checkValue = spaceMax(k);
					checkValue *= dirMultiplier;

					if (maxCoordValue < checkValue || firstCheck)
					{
						maxCoordValue = checkValue;
						if (dirMultiplier < 0) loc = spaceOrigin;
						else loc = spaceMax; 
						firstCheck = false;
					}
				}
				utilities::geometry::vertex tempLoc = loc - dir * 100;

				auto geometricCluster = newMS.selectSpacesGeometrically(tempLoc,dir,true);
				double maxDim = 0.0;
				for (const auto& l : geometricCluster)
				{
					if (l->getDimensions()(k) > maxDim) maxDim = l->getDimensions()(k);
				}
				loc -= dir * (maxDim);
				geometricCluster = newMS.selectSpacesGeometrically(loc,dir,false);
				if (geometricCluster.size() == nSpaces) continue;
				else
				{
					geometricCutOff.push_back({loc,dir});
					geometricPerformances.push_back(utilities::data_point(performances.begin()->second.size()));
					for (const auto& l : geometricCluster)
					{
						auto performanceSearch = performances.find(l);
						if (performanceSearch != performances.end())
						{
							geometricPerformances.back() += performanceSearch->second;
						}
						else
						{
							std::stringstream errorMessage;
							errorMessage << "Error, could not find performance for space: "
													 << l->getID() << std::endl;
							throw std::runtime_error(errorMessage.str());
						}
					}
					geometricPerformances.back() /= geometricCluster.size();
				}
			}
		}

		// step 4, normalize and select distopian point
		// normalize the performances of each geometric clustering
		utilities::data_point normalizedZero;
		normalizedZero.setToUtopia(geometricPerformances);
		utilities::data_point normalizedOne;
		normalizedOne.setToDistopia(geometricPerformances);
		std::swap(normalizedZero(1),normalizedOne(1)); // for sd a low performance is bad
		for (auto& j : geometricPerformances)
		{
			j.normalize(normalizedZero,normalizedOne);
		}

		// find out which performance(s) lies closest to the distopian point
		utilities::data_point distopianPerformance;
		distopianPerformance.setToDistopia(geometricPerformances);
		std::vector<utilities::data_point> worstPerformance = distopianPerformance.findClosestIn(geometricPerformances);

		// step 5, remove facade group closest to distopian point
		auto worstGeometricPerformance = std::find(geometricPerformances.begin(),
			geometricPerformances.end(),worstPerformance.front());

		auto cutOffPlane = geometricCutOff[std::distance(
			geometricPerformances.begin(),worstGeometricPerformance)];
		newMS.cutOff(cutOffPlane.first,cutOffPlane.second);
		// check if any constrain was exceeded
		std::vector<std::pair<unsigned int, double> > sweepDistances = {{0,0.0},{1,0.0},{2,0.0}};
		for (const auto& i : newMS.getSpacePtrs())
		{
			for (unsigned int j = 0; j < 3; ++j)
			{
				if (cutOffPlane.second(j) == 0) continue;
				double shortage = 0;
				if (j == 2 && i->getDimensions()(j) < 3000)
				{
					shortage = 3000 - i->getDimensions()(j);
				}
				else if ((j == 0 || j == 1) && i->getDimensions()(j) < 500)
				{
					shortage = 500 - i->getDimensions()(j);
				}
				if (sweepDistances[j].second < shortage) sweepDistances[j].second = shortage;
			}
		}
		// if one space is too small, expand
		newMS.sweep(cutOffPlane.first-0.5*cutOffPlane.second,sweepDistances);
		out("Removed spaces",true,true,verbose);
		
		// step 6, scale and split spaces to recover initial conditions
		// scale the dimensions in x and y direction
		double scaleFactor = sqrt(floorArea / newMS.getFloorArea());
		newMS.scale({{0,scaleFactor},{1,scaleFactor}});
		newMS.snapOn({{0,1},{1,1}});
		
		// split the spaces with the largest dimension in x or y direction (checking x first)
		while (newMS.getSpacePtrs().size() < nSpaces)
		{
			std::pair<unsigned int, double> largestDimension;
			spatial_design::ms_space* spaceWithLargestDimension;
			bool firstCheck = true;
			for (const auto& j : newMS.getSpacePtrs())
			{
				for (unsigned int k = 0; k < 3; ++k)
				{
					double dimension = j->getDimensions()(k);
					if (k == 2 && dimension < 6000) continue;
					if (largestDimension.second < dimension || firstCheck)
					{
						largestDimension.first = k;
						largestDimension.second = dimension;
						spaceWithLargestDimension = j;
						firstCheck = false;
					}
					else if (largestDimension.second == dimension)
					{ // if they both are the largest, check which one has a smaller z-coordinate
						if (spaceWithLargestDimension->getCoordinates()(2) > j->getCoordinates()(2))
						{
							spaceWithLargestDimension = j;
							largestDimension.first = k;
						}
					}
				}
			}
			newMS.splitSpace(spaceWithLargestDimension,{{largestDimension.first,2}});
		}
		// newMS.snapOn({{2,3000}});
		// scaleFactor = sqrt(floorArea / newMS.getFloorArea());
		// newMS.scale({{0,scaleFactor},{1,scaleFactor}});
		newMS.snapOn({{0,100},{1,100},{2,100}});
		out("Split spaces to restore number of spaces",true,true,verbose);
		
		// in case the lower spaces were removed reset minimum z coordinate to Z
		newMS.setZZero();
		out("Scaled design to initial volume",true,true,verbose);

		msDesigns.push_back(newMS);
		out("Modified building spatial design into a new one",true,true,verbose);
	}
	out("finished SCDP process",true,true,verbose);
	
	std::vector<unsigned int> outputSpaces;
	// if best, find which designs are best;
	if (outputAll)
	{
		for (unsigned int i = 0; i < msDesigns.size(); ++i)
		{
			outputSpaces.push_back(i);
		}
	}
	
	for (const auto& i : outputSpaces)
	{
		if (result_line)
		{
			std::cout << designName << "\t"
								<< modification << "\t"
								<< i << "\t"
								<< designPerformances[i](0) << "\t"
								<< designPerformances[i](1) << "\t"
								<< "NA" << "\t"
								<< scDesigns[i] << std::endl;
		}
		if (ms_files)
		{
			std::stringstream fileNameStream;
			fileNameStream << designName << "_"
										 << modification << "_"
										 << i << "_"
										 << designPerformances[i](0) << "_"
										 << designPerformances[i](1);
			std::string fileName = fileNameStream.str();
			msDesigns[i].writeToFile(fileName);
		}
	}
	
	if (!visualizations.empty()) visualization::endVisualization();
	
	return 0;
}
