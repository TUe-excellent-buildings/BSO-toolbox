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
	bool penalty = false;
	bool counter = false;
	bool visualization = false;
	bool evaluate = false;
	std::string genome = "";
	double penalty_vol = 0;
	double penalty_strain = 0;
	std::string input_file = "MS_Input.txt";
	std::string output_file = "output.txt";

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
            std::cout << "\n\n\n\nThis program is made to let a GA design a Structural design,"
                      << "\ngiven a spatial design in \"Movable Sizable\" format"
                      << "\nOptions are:"<< std::endl;

            std::cout << "-h\t--help\t\t" << "Will give you this help menu." << std::endl;
			
			std::cout << "-p\t --penaty\t" << "Specify the penalties for the"
					  << "\n\t\t\tstructural volume and for the strain energy subsequently" << std::endl;

            std::cout << "-c\t--count\t\t" << "Will give you the number of variables in"
                      << "\n\t\t\tthe given spatial design." << std::endl;

            std::cout << "-e\t--evaluate\t" << "Expects a string of valid variables with a"
                      << "\n\t\t\tlength that is given by -c or --count."
                      << "\n\t\t\tValid characters for variables"
                      << "\n\t\t\tare: \"" << 1234 << "\"."
                      << "\n\t\t\tThis will result in a file with the evaluation values" << std::endl;

            std::cout << "-v\t--visualise\t" << "Expects a string of valid variables with a"
                      << "\n\t\t\tlength that is given by -c or --count."
                      << "\n\t\t\tValid characters for variables"
                      << "\n\t\t\tare: \"" << 1234 << "\"."
                      << "\n\t\t\tThis will result in a visualisation of the design" << std::endl;
					  
			std::cout << "\nBeware, order matters. If applicable, always start by specifying the penalty. "
					  << "\nAccordingly you choose only on of -c -e of -v. If you choose multiple, "
					  << "\nthen the later arguments are ignored and the program ends." << std::endl;
					  
			std::cout << "\nPress ENTER to continue\n\n" << std::endl;
			std::cin.get();

            break; // break the while loop
        }
		else if (*it == "-i" || *it == "--input")
		{
			if (++it == args.end() || (*it)[0] == '-') 
				throw std::invalid_argument("Error. Expected a value after -i or --input");
			input_file = *it;
			boost::trim(input_file);
			++it;
		}
		else if (*it == "-o" || *it == "--output")
		{
			if (++it == args.end() || (*it)[0] == '-') 
				throw std::invalid_argument("Error. Expected a value after -o or --output");
			output_file = *it;
			boost::trim(output_file);
			++it;
		}
		else if (*it == "-p" || *it == "--penalty")
		{
			if (++it == args.end() || (*it)[0] == '-') 
				throw std::invalid_argument("Error. Expected a second value after -p or --penalty");
			penalty_strain = bso::utilities::trim_and_cast_double(*it);
            if (penalty_strain <= 0) throw std::domain_error("expected a value larger than 0 for the second value of penalty");
			
			if (++it == args.end() || (*it)[0] == '-') 
				throw std::invalid_argument("Error. Expected a value after -p or --penalty");
			penalty_vol = bso::utilities::trim_and_cast_double(*it);
            if (penalty_vol <= 0) throw std::domain_error("expected a value larger than 0 for the first value of penalty");
			
			penalty = true;
			
			++it;
		}
		else if (*it == "-g" || *it == "--genome")
		{
			if (++it == args.end() || (*it)[0] == '-')
				throw std::invalid_argument("Error, Expected a value after -g or --genome");
			genome = *it;
			++it;
		}
        else if (*it == "-c" || *it == "--count")
        {
            counter = true;
			++it;
        }
        else if (*it == "-e" || *it == "--evaluate")
        {
			evaluate = true;
			++it;
        }
        else if (*it == "-v" || *it == "--visualise")
        {
            visualization = true;
			++it;
        }
        else
        {
            std::cerr << "Error, invalid argument, see -h or --help for help, exiting now..." << std::endl;
            exit(1);
        }
    } // end of while (end of arguments)


	bso::spatial_design::ms_building MS(input_file);
	bso::spatial_design::cf_building CF(MS);

	if (counter)
	{
		unsigned int varCount = 0;
		for (const auto& i : CF.cfRectangles())
		{
			if (i->cfSurfaces().size() == 1 || i->cfSurfaces().size() == 2) ++varCount;
		}
		std::cout << varCount << std::endl;
		return 0;
	}
	
	bso::grammar::grammar gram(CF);
	
	bso::structural_design::component::structure trussStructure("truss",{{"A",22500},{"E",3e4}});
	bso::structural_design::component::structure beamStructure("beam",{{"width",150},{"height",150},{"poisson",0.3},{"E",3e4}});
	bso::structural_design::component::structure flatShellStructure("flat_shell",{{"thickness",150},{"poisson",0.3},{"E",3e4}});


	//create the structural design
	bso::structural_design::sd_model SD = gram.sd_grammar<bso::grammar::OPTIMIZER_ASSIGNMENT>(std::string("settings/sd_settings.txt"),genome,trussStructure,beamStructure,flatShellStructure);
	
	std::ofstream out(output_file.c_str());
	if (!out.is_open())
	{
		std::cerr << "could not open output file: " << output_file << std::endl;
		exit(1);
	}
	
	//evaluate the structural design
	if (penalty && !SD.isStable())
	{
		out << penalty_strain << "," << penalty_vol;
	}
	else
	{
		SD.analyze();
		out << SD.getTotalResults().mTotalStrainEnergy << ","
				<< SD.getTotalResults().mTotalStructuralVolume;
	}
	out.close();
	//visualize the design
	if (visualization)
	{
		
		bso::visualization::initVisualization(argc, argv);

		//bso::visualization::visualize(MS);
		//bso::visualization::visualize(CF, "rectangles");
		bso::visualization::visualize(SD, "component");
		//bso::visualization::visualize(SD, "strain_energy");
		//bso::visualization::visualize(SD, 2,true);

		bso::visualization::endVisualization();

	}

    return 0;
}
