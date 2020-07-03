#include <iostream>
#include <fstream>


#include <bso/spatial_design/ms_building.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/structural_design/sd_model.hpp>
#include <bso/grammar/grammar.hpp>
#include <bso/grammar/sd_grammars/optimizer_assignment.cpp>
#include <bso/visualization/visualization.hpp>


int main(int argc, char* argv[])
{
	double vol_frac = 0;
	double threshold = 0.01;
	unsigned int mesh_size = 1;
	
	bool visualise_on = false;
	bool rescaling_on = false;

	bool vol_frac_given = false;
	bool mesh_size_given = false;
	std::string ga_string = "";
	std::string input_file = "";

    std::vector<std::string> args(argv+1, argv+argc);
    auto it = args.begin();

    if (it == args.end())
    {
        std::cerr << "Error, expected arguments, got nothing, -h or --help for help, exiting now..." << std::endl;
        exit(1);
    }
    while (it != args.end())
    {
        if (*it == "-h" || *it == "--help")
        {
            std::cout << "\n\n\n\nThis program is made to subject a GA designed Structural design,"
                      << "\ntopology optimisation. format"
                      << "\nPossible input arguments are:"<< std::endl;

            std::cout << "-h\t--help\t\t" << "Will give you this help menu." << std::endl;

			std::cout << "-s\t--string\t" << "Followed by an string of parameters,"
                      << "\n\t\t\twhich represent the structural design for a given"
					  << "\n\t\t\tbuilding spatial design." << std::endl;
					  
			std::cout << "-f\t--fraction\t" << "Followed by a double, specifies"
                      << "\n\t\t\tthe volume fraction for the topology optimisation." << std::endl;
					  
			std::cout << "-m\t--mesh_size\t" << "Followed by an integer, specifies"
                      << "\n\t\t\tthe number of elements each structural component"
					  << "\n\t\t\tis divided into in each of its dimensions." << std::endl;
					  
			std::cout << "-t\t--threshold\t" << "Followed by a double, specifies the threshold"
					  << "\n\t\t\tat which the topology optimisation algorithm stops." << std::endl;
					  
			std::cout << "-i\t--input\t\t" << "Followed by a file name, specifies the"
					  << "\n\t\t\tfilename that defines the building spatial design" << std::endl;
					  
			std::cout << "-r\t--rescale\t" << "Flag to turn on rescaling, which increases"
					  << "\n\t\t\tthe design volume such that the optimised design has"
					  << "\n\t\t\tthe same volume as the initial design volume" << std::endl;

            std::cout << "-v\t--visualise\t" << "Flag to visualise the results" << std::endl;
					  
			std::cout << "\nBeware. The flag -c will end the program as soon as it is parsed." << std::endl;
					  
			std::cout << "\nPress ENTER to continue" << std::endl;
			std::cin.get();

            return 0; // break the while loop
        }
		else if (*it == "-v" || *it == "--visualise")
        {
            visualise_on = true;
            it++;
		}
		else if (*it == "-r" || *it == "--rescaling")
        {
            rescaling_on = true;
            it++;
		}
		else if (*it == "-s" || *it == "--string")
        {
            if (++it == args.end() || (*it)[0] == '-') 
				throw std::invalid_argument("Error. Expected a value after -s or --string");
			ga_string = *it;
			++it;
		}
		else if (*it == "-i" || *it == "--input")
        {
            if (++it == args.end() || (*it)[0] == '-') 
				throw std::invalid_argument("Error. Expected a value after -i or --input");
			input_file = *it;
			++it;
		}
		else if (*it == "-f" || *it == "--fraction")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value after -f or --fraction");
            vol_frac = bso::utilities::trim_and_cast_double(*it);
            if (vol_frac < 0 || vol_frac > 1) throw std::domain_error("expected a value between 0 and 1 for the volume fraction");
			vol_frac_given = true;
            it++;
		}
		else if (*it == "-t" || *it == "--threshold")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value after -t or --threshold");
            threshold = bso::utilities::trim_and_cast_double(*it);
            if (threshold < 0 || threshold > 1) throw std::domain_error("expected a value between 0 and 1 for the threshold");
            it++;
		}
		else if (*it == "-m" || *it == "--mesh_size")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value after -m or --mesh_size");
            mesh_size = bso::utilities::trim_and_cast_int(*it);
            if (mesh_size <= 1) throw std::domain_error("expected a value larger than 1 for the mesh size");
			mesh_size_given = true;
            it++;
		}
		else
		{
			throw std::invalid_argument("Error. Did not recognise argument \"" + *it + "\".");
		}
	}
	if (!vol_frac_given) 		throw std::invalid_argument("Error. Expected -f or --fraction to be specified. Use -h for help.");
	if (!mesh_size_given) 		throw std::invalid_argument("Error. Expected -m or --mesh_size to be specified. Use -h for help.");
	if (ga_string == "")		throw std::invalid_argument("Error. Expected -s or --string to be specified. Use -h for help.");
	if (input_file == "")		throw std::invalid_argument("Error. Expected -i or --input to be specified. Use -h for help.");


	bso::spatial_design::ms_building MS(input_file);
	bso::spatial_design::cf_building CF(MS);

	bso::grammar::grammar gram(CF);
	
	bso::structural_design::component::structure trussStructure("truss",{{"A",22500},{"E",3e4}});
	bso::structural_design::component::structure beamStructure("beam",{{"width",150},{"height",150},{"poisson",0.3},{"E",3e4}});
	bso::structural_design::component::structure flatShellStructure("flat_shell",{{"thickness",150},{"poisson",0.3},{"E",3e4}});

	//create the structural design
	bso::structural_design::sd_model SD = gram.sd_grammar<bso::grammar::OPTIMIZER_ASSIGNMENT>(std::string("settings/sd_settings.txt"),ga_string,trussStructure,beamStructure,flatShellStructure);
	
	// set mesh size
	if (mesh_size_given) SD.setMeshSize(mesh_size);
	// rescale dimensions is necessary
	if (rescaling_on) SD.rescaleStructuralVolume(1.0/vol_frac);
	// set element densities to do initial analysis
	SD.mesh();
	SD.setElementDensities(vol_frac,3.0);
	
	// store initial strain energy
	SD.analyze();
	double initStrain = SD.getTotalResults().mTotalStrainEnergy;
	std::cout << initStrain << std::endl;
	// do topology optimization
	SD.setTopOptOutputStream(std::cout);
	SD.topologyOptimization<bso::structural_design::topology_optimization::COMP_SIMP>(vol_frac,3.0,0.2,threshold);

	// store end strain energy
	SD.analyze();
	double endStrain = SD.getTotalResults().mTotalStrainEnergy;
	std::cout << SD.getTotalResults().mTotalStructuralVolume << "," << initStrain << "," << endStrain << std::endl;
	
	
	//visualize the design
	if (visualise_on)
	{
		
		bso::visualization::initVisualization(argc, argv);

		//bso::visualization::visualize(MS);
		//bso::visualization::visualize(CF, "rectangles");
		bso::visualization::visualize(SD, "component");
		bso::visualization::visualize(SD, "density");
		//bso::visualization::visualize(SD, "strain_energy");
		//bso::visualization::visualize(SD, 2,true);

		bso::visualization::endVisualization();

	}

    return 0;
}
