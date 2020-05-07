#include <bso/spatial_design/ms_building.hpp>
#include <bso/spatial_design/cf_building.hpp>
#include <bso/structural_design/sd_model.hpp>
#include <bso/building_physics/bp_model.hpp>
#include <bso/grammar/grammar.hpp>
#include <bso/visualization/visualization.hpp>

int main(int argc, char* argv[])
{
 // initialize a movable sizable model named MS
 bso::spatial_design::ms_building MS("ms_input_file.txt");

 // convert MS to a conformal model name CF
 bso::spatial_design::cf_building CF(MS);

 // create an instance of the grammar base class named gram using CF
 bso::grammar::grammar gram(CF);

 // use the default structural design grammar to create a structural
 // design named SD, using settings defined in
 // settings/sd_settings.txt
 bso::structural_design::sd_model SD = gram.sd_grammar <bso::grammar::DEFAULT_SD_GRAMMAR >(std::string("settings/sd_settings.txt"));

// analyze the structural model
SD.analyze();

// output result of the analysis
std::cout << "Structural compliance: " << SD.getTotalResults().mTotalStrainEnergy << std::endl;

 // use the default building physics design grammar to create a
 // building physics model named BP, using settings defined in
 // settings/bp_settings.txt
 bso::building_physics::bp_model BP = gram.bp_grammar <bso::grammar::DEFAULT_BP_GRAMMAR >(std::string("settings/bp_settings.txt"));

 // simulate the periods defined in bp_settings.txt
 // using the runge_kutta_dopri5 solver with both a
 // relative and an absolute error of 1e-3
 BP.simulatePeriods("runge_kutta_dopri5",1e-3,1e-3);

 // output result of the simulation
 std::cout << "Total heating energy: " << BP.getTotalResults().mTotalHeatingEnergy << std::endl;

std::cout << "Total cooling energy: " << BP.getTotalResults().mTotalCoolingEnergy << std::endl;

 // visualize the different models
 bso::visualization::initVisualization(argc, argv);
 bso::visualization::visualize(MS);
 bso::visualization::visualize(CF,"rectangle");
 bso::visualization::visualize(CF,"cuboid");
 bso::visualization::visualize(SD,"component");
 bso::visualization::visualize(SD,"element");
 bso::visualization::visualize(BP);
 bso::visualization::endVisualization();

 return 0;
}