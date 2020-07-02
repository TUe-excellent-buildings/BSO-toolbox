#include <cstdlib>
#include <stdexcept>

#include <boost/algorithm/string.hpp>

#include <bso/spatial_design/ms_building.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/structural_design/sd_model.hpp>
#include <bso/grammar/grammar.hpp>
#include <bso/grammar/sd_grammars/design_response_grammar.cpp>
#include <bso/visualization/visualization.hpp>


int main(int argc, char* argv[])
{
	bool penalty = false;
	double penalty_strain = 0;
	double penalty_vol = 0;
	double etaBend = 0;
	double etaAx = 0;
	double etaShear = 0;
	double etaNoise = 0;
	int etaConverge = 0;
	std::string checkingOrder = "123";
	bool visualise = false;
    std::vector<std::string> args(argv+1, argv+argc);
	std::string input_file = "MS_Input.txt"; // default value
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
            std::cout << "This program is made to do a parameter study with the iterative,"
                      << "\nstructural design grammar. Options are:" << std::endl;

            std::cout << "-h\t--help\t\t" << "Will give you this help menu." << std::endl << std::endl;
			
			std::cout << "-v\t--visualise\t" << "Will give you a visualisation of the input"
                      << "\n" << std::endl;
			
			std::cout << "-p\t--penalty\t" << "Specify the penalties for the"
					  << "\n\t\t\tstructural volume and for the strain energy subsequently" 
					  << "\n" << std::endl;

            std::cout << "-s\t--shear\t\t" << "Will give you the number of variables in"
                      << "\n" << std::endl;

            std::cout << "-a\t--axial\t\t" << "Expects a string of valid variables with a"
                      << "\n" << std::endl;

            std::cout << "-b\t--bending\t" << "Expects a real value x where 0 <= x <= 1"
                      << "\n" << std::endl;

            std::cout << "-c\t--convergence\t" << "Expects a positive integer larger than 0."
                      << "\n" << std::endl;

			std::cout << "-n\t--noise\t\t" << "Expects a real value x where 0 <= x <= 1"
                      << "\n" << std::endl;

            std::cout << "-o\t--order\t\t" << "expects a string of three integers with value 1, 2, or 3."
                      << "\n" << std::endl;

            return 0; // end the program
        }
		else if (*it == "-p" || *it == "--penalty")
		{
			if (++it == args.end() || (*it)[0] == '-') 
				throw std::invalid_argument("Error. Expected a value after -p or --penalty");
			penalty_strain = bso::utilities::trim_and_cast_double(*it);
            if (penalty_strain <= 0) throw std::domain_error("expected a value larger than 0 for the first value of penalty");
			
			if (++it == args.end() || (*it)[0] == '-') 
				throw std::invalid_argument("Error. Expected a value after -p or --penalty");
			penalty_vol = bso::utilities::trim_and_cast_double(*it);
            if (penalty_vol <= 0) throw std::domain_error("expected a value larger than 0 for the second value of penalty");
			
			penalty = true;
			++it;
		}
		else if (*it == "-v" || *it == "--visualise")
        {
            visualise = true;
            it++;
        }
        else if (*it == "-s" || *it == "--shear")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value after -s or --shear");
            double value = bso::utilities::trim_and_cast_double(*it);
            if (value < 0 || value > 1) throw std::domain_error("expected a value between 0 and 1 for eta_shear");
            etaShear = value;
            it++;
        }
        else if (*it == "-a" || *it == "--axial")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value afyer -a or --axial");
            double value = bso::utilities::trim_and_cast_double(*it);
            if (value < 0 || value > 1) throw std::domain_error("expected a value between 0 and 1 for eta_ax");
            etaAx = value;
            it++;
        }
        else if (*it == "-b" || *it == "--bend")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a balue after -b or --bend");
            double value = bso::utilities::trim_and_cast_double(*it);
            if (value < 0 || value > 1) throw std::domain_error("expected a value between 0 and 1 for eta_bend");
            etaBend = value;
            it++;
        }
		else if (*it == "-n" || *it == "--noise")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value after -n or --noise");
            double value = bso::utilities::trim_and_cast_double(*it);
            if (value < 0 || value > 1) throw std::domain_error("expected a value between 0 and 1 for eta_noise");
            etaNoise = value;
            it++;
        }
        else if (*it == "-c" || *it == "--convergence")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value after -c or --convergence");
            int value = bso::utilities::trim_and_cast_int(*it);
            if (value < 1) throw std::domain_error("Expected a positive int larger than zero for eta_convergence");
            etaConverge = value;
            it++;
        }
        else if (*it == "-o" || *it == "--order")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value after -o or --order");
            std::string value = *it;
            if (value.size() != 3) throw std::domain_error("Expected a string of three ints with value 1, 2, or 3 for checking order");
            for (auto c : value) if (c < '1' || c > '3') throw std::domain_error("Expected a string of three ints with value 1, 2, or 3 for checking order");
            checkingOrder = value; // add the four at the back for no structure
            it++;
        }
		else if (*it == "-i" || *it == "--input")
        {
            if (++it == args.end() || (*it)[0] == '-') throw std::domain_error("Expected a value after -i or --input");
			input_file = *it;
			boost::trim(input_file);
            it++;
        }
        else
        {
            throw std::invalid_argument("Argument: " + *it + " not recognised");
        }
    }
		
		bso::spatial_design::ms_building MS(input_file);
		bso::spatial_design::cf_building CF(MS);

		bso::grammar::grammar gram(CF);
		
		bso::structural_design::component::structure trussStructure("truss",{{"A",22500},{"E",3e4}});
		bso::structural_design::component::structure beamStructure("beam",{{"width",150},{"height",150},{"poisson",0.3},{"E",3e4}});
		bso::structural_design::component::structure flatShellStructure("flat_shell",{{"thickness",150},{"poisson",0.3},{"E",3e4}});
		bso::structural_design::component::structure substituteStructure("flat_shell",{{"thickness",150},{"poisson",0.3},{"E",3e-2}});
		
		// bso::structural_design::sd_model SD = gram.sd_grammar<bso::grammar::DEFAULT_SD_GRAMMAR>(std::string("settings/sd_settings.txt"));
		bso::structural_design::sd_model SD = gram.sd_grammar<bso::grammar::DESIGN_RESPONSE>(std::string("settings/sd_settings.txt"),etaBend,etaAx,etaShear,etaNoise,etaConverge,checkingOrder,trussStructure,beamStructure,flatShellStructure,substituteStructure);
	
	if (penalty && !SD.isStable())
	{
		std::cout << penalty_strain << "," << penalty_vol;
	}
	else
	{
		SD.analyze();
		std::cout << SD.getTotalResults().mTotalStrainEnergy << ","
							<< SD.getTotalResults().mTotalStructuralVolume;
	}
	
	std::cout << etaAx << ","
              << etaBend << ","
              << etaShear << ","
              << etaConverge << ","
			  << etaNoise << ","
              << checkingOrder << std::endl;
			  
	if (visualise)
	{
		bso::visualization::initVisualization(argc, argv);

		//bso::visualization::visualize(MS);
		//bso::visualization::visualize(CF, "rectangles");
		bso::visualization::visualize(SD, "component");
		bso::visualization::visualize(SD, "strain_energy");
		//bso::visualization::visualize(SD, 2,true);

		bso::visualization::endVisualization();
	}

    return 0;
}
