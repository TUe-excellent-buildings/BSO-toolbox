# README BSO toolbox

## What is this repository for?

This repository contains a C++ library named the Building Spatial design Optimization toolbox (BSO toolbox).
In the first place, the toolbox has been developed to support building spatial design optimization by providing tools to:
(i) represent and modify building spatial designs,
(ii) generate discipline specific designs from a building spatial design,
(iii) analyze structural designs,
(iv) analyze the building physics of a building (i.e. thermal performance),
(v) analyze data obtained from the evaluations, e.g. clustering and sorting algorithms.
The toolbox itself does not contain any optimization methods, however, this repository will be extended with toolbox applications soon, and these applications will be related to optimization as well.
The current version toolbox has been developed to support research in a joint project by the Department of the Built Environment of Eindhoven University of Technology, and the Leiden Institute of Advanced Computer Science (LIACS) of Leiden University, both in The Netherlands.
Key publications within this research project are given below. These publications provide a lot of background information on the purpose and setup of the code in this repository:
* Boonstra, S, van der Blom, K, Hofmeyer, H, Emmerich, MTM, van Schijndel, AWM & de Wilde, P 2018, 'Toolbox for super-structured and super-structure free multi-disciplinary building spatial design optimisation', Advanced Engineering Informatics, vol. 36, pp. 86-100. https://doi.org/10.1016/j.aei.2018.01.003
* van der Blom, K, Boonstra, S, Hofmeyer, H & Emmerich, MTM 2019, Analysing optimisation data for multicriteria building spatial design. in CA Coello Coello, P Reed, K Deb, E Goodman, K Klamroth, K Miettinen & S Mostaghim (eds), Evolutionary Multi-Criterion Optimization - 10th International Conference, EMO 2019, Proceedings. Lecture Notes in Computer Science , vol. 11411, Springer, pp. 671-682, Evolutionary Multi-Criterion Optimization, 10th International Conference, (EMO2019), East Lansing, United States, 10/03/19. https://doi.org/10.1007/978-3-030-12598-1_53
* Boonstra, S, van der Blom, K, Hofmeyer, H, van den Buijs, J & Emmerich, MTM 2018, Coupling between a building spatial design optimisation toolbox and BouwConnect BIM. in I Mutis & T Hartman (eds), Advances in Informatics and Computing in Civil and Construction Engineering: Proceedings of the 35th CIB W78 2018 Conference: IT in Design, Construction, and Management. Springer, Cham, pp. 95-102, 35th CIB W78 2018 Conference, Chicago, United States, 1/10/18. https://doi.org/10.1007/978-3-030-00220-6_12
* van der Blom, K, Boonstra, S, Hofmeyer, H, Bäck, T & Emmerich, MTM 2017, Configuring advanced evolutionary algorithms for multicriteria building spatial design optimisation. in 2017 IEEE Congress on Evolutionary Computation (CEC) Proceedings, June 5-8, 2017, Donostia - San Sebastián, Spain., 7969520, Institute of Electrical and Electronics Engineers, pp. 1803-1810, 2017 IEEE Congress on Evolutionary Computation (CEC2017) June 5-8, 2017, Donostia - San Sebastián, Spain, San Sebastian, Spain, 5/06/17. https://doi.org/10.1109/CEC.2017.7969520
* Boonstra, S, van der Blom, K, Hofmeyer, H & Emmerich, MTM 2019, Co-evolutionary design processes applied to building spatial design optimization. in X Guo & H Huang (eds), Advances in Structural and Multidisciplinary Optimization: Proceedings of the 13th World Congress of Structural and Multidisciplinary Optimization (WCSMO13). ISSMO, Beijing, pp. 110-115, World Congress of Structural and Multidisciplinary Optimization, Beijing, China, 20/05/19.

Moreover, three PhD theses related to the research are:
* Boonstra, S 2020, 'Multi-disciplinary optimization of building spatial designs: co-evolutionary design process simulations, evolutionary algorithms, hybrid approaches', PhD-thesis, Department of the Built Environment, Eindhoven University of Technology, The Netherlands.
* van der Blom, K 2019, 'Multi-objective mixed-integer evolutionary algorithms for building spatial design', PhD-thesis, Leiden Institute of Advanced Computer Science, The Netherlands.
* Davila Delgado, JM 2014, 'Building structural design generation and optimisation including spatial modification', PhD-thesis, Department of the Built Environment, Eindhoven University of Technology, The Netherlands.

Although the first aim of the toolbox was related to research carried out at Eindhoven University of Technology, the contributors now invite all interested researchers to download and use the toolbox; to apply it for innovative design support and optimisation, possibly also in other domains; and to explore and extend its possiblities. This is all possible thanks to the GNU Affero General Public License v3.0. If publications result from using this repository, the contributers would appreciate it if you cite this repository and esspecially the related journal papers above, and not the PhD-theses, although the latter of course provide an excellent source of background information in practice.


## How do I get it set up? ###

### Dependencies
The toolbox is written in C++ and makes use of elements defined in the C++14 standard.
Although the toolbox is developed to be cross-platform, it has only been compiled and tested for Linux (Ubuntu 18.04).
Compilation on a Windows machine has been successfull in the past, but is not explained in this readme.

Furthermore, the toolbox depends on the following external software:
* For linear algebra (Geometry and FEM) the [Eigen](http://eigen.tuxfamily.org) C++ library is used (last tested for v3.2.3).
* Various utilities from the [Boost](https://www.boost.org/) C++ library (last tested for v1.70.0)
* For solving systems of ODE's (thermal simulation) the [Odeint](https://www.odeint.com) library is used (also contained in the Boost library; last tested for v1.70.0).
* Visualization is written in the openGL standard and makes use of GSL (last tested for v2.4+dfsg-6 amd64) and freeglut3 (last tested for v2.8.1-3 amd64)   

### Installation
A tutuorial for the installation of the dependencies and the toolbox is given in the following steps.
1. Install C++ compiler (GCC) and make
2. Install the Eigen library.
3. Install and compile the Boost library.
4. Install visualization dependencies (GSL and GLUT)
5. Install GIT
6. Clone the repository to your local machine
7. Compile and run the example code.

##### 1. Install C++ compiler (GCC) and make
In this step, a compiler to compile the toolbox into executable code is installed.
Additionally, make is installed, which can parse and execute compiler commands that are given in a so-called makefile

To install the GCC C++ compiler, type in the terminal:
```bash
$ sudo apt install g++
```
To install make, type in the terminal:
```bash
$ sudo apt install make
```

##### 2. Install the Eigen library.
In this step, the [Eigen](http://eigen.tuxfamily.org) library is installed.

Go to the website of the Eigen library (http://eigen.tuxfamily.org) and download the tar.gz of the desired version (e.g. latest stable release).

Extract the package (the downloaded tar.gz) to /usr/include, by typing in the terminal:
```bash
$ sudo tar -xf ~/Downloads/eigen-eigen-[version_number].tar.gz -C /usr/include
```

**OPTIONAL.** Rename the directory so that it is easier to link to the directory, by typing in the terminal:
```bash
$ sudo mv eigen-[version_number] eigen
```

**NOTE.** You may change the directory in which you place Eigen, however, this may have consequences for later steps.

##### 3. Install and compile the Boost library.
In this step, the [Boost]() library is installed and parts of it are pre-compiled.

Go to the website of the Boost library (https://boostorg.jfrog.io/artifactory/main/release/1.77.0/source/) and download the tar.gz of the desired version (e.g. latest stable release).

Extract the package (the downloaded tar.gz) to /usr/include, by typing in the terminal:
```bash
$ sudo tar -xf ~/Downloads/boost_[version_number].tar.gz -C /usr/include
```

**OPTIONAL.** Rename the directory so that it is easier to link to the directory, by typing in the terminal:
```bash
$ sudo mv boost_[version_number] boost
```

**NOTE.** You may change the directory in which you place Boost, however, this may have consequences for later steps.

Pre-compile parts of the boost library (e.g. the boost test framework), by typing in the terminal:
```bash
$ cd /usr/include/boost # note that this directory has been defined in the preceding steps
$ sudo ./bootstrap.sh
$ sudo ./b2
$ sudo ./b2 install
```

**NOTE.** Enter the above commands one by one.

##### 4. Install visualization dependencies (GSL and GLUT)
In this step the dependencies for the visualization in the toolbox are installed.
THe visualization makes use of OpenGL which is enabled by GL Utility Toolkit (GLUT) and GL Shading Language (GSL)

Install the dependencies by typing in the terminal:
```bash
$ sudo apt install libgsl-dev freeglut3-dev
```

##### 5. Install GIT
In this step, the version control software GIT is installed, together with a GUI for GIT.

To install GIT and an GUI type the following in the terminal:
```bash
$ sudo apt install git git-gui
```

##### 6. Clone the repository to your local machine
In this step, the BSO toolbox is cloned to your local machine. This allows you to compile/develop/use the code in the toolbox on your local machine.

In your terminal go to the directory where you would like the toolbox to be placed. For example by typing in the terminal:

```bash
$ cd /usr/include
```

On the homepage of the BSO toolbox click on *clone*.

A dropdown box will appear with two links, copy the link under **Clone with HTTPS** and enter it in the terminal as follows:

```bash
$ git clone <link>
```

The command line on the terminal should then look similar to the line below.

```bash
$ git clone https://github.com/TUe-excellent-buildings/BSO-toolbox.git
```

If the clone was successful, the BSO toolbox is now cloned onto your local machine, e.g. in the directory: `/usr/include/BSO-toolbox`.

##### 7. Compile and run the example.

An example of how the toolbox can be used has been added to the repository as well (`../BSO-toolbox/example`).
The source code of the example is as follows:

```cpp
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
	bso::structural_design::sd_model SD = gram.sd_grammar<bso::grammar::DEFAULT_SD_GRAMMAR>(std::string("settings/sd_settings.txt"));
	
	// analyze the structural model
	SD.analyze();

	// output result of the analysis
	std::cout << "Structural compliance: " << SD.getTotalResults().mTotalStrainEnergy << std::endl;
	
	// use the default building physics design grammar to create a
	// building physics model named BP, using settings defined in 
	// settings/bp_settings.txt
	bso::building_physics::bp_model BP = gram.bp_grammar<bso::grammar::DEFAULT_BP_GRAMMAR>(std::string("settings/bp_settings.txt"));
	
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
```

A make file has been added to the directory of the example, which can be used to compile the code.
In the makefile, check if the dependencies (Eigen and Boost) are linked to the locations of your local machine.
For instance, check if the line `BOOST = /usr/include/boost` is correctly linking to the Boost library on your local machine.

The example can then be compiled by typing the following command in the terminal:

```bash
$ make clean all
```

Run the example by typing the following command in the terminal:

```bash
$ ./example
```

The output should then look as follows.

```bash
> Structural compliance: 4054.5
> Total heating energy: 106.819
> Total cooling energy: 15.9503
```

Note that the directory also includes settings files to define building spatial designs, structural design settings, and building physics settings.
For more information about these the reader is referred to the [paper](https://doi.org/10.1016/j.aei.2018.01.003) in which the toolbox is presented.

### How to run unit tests
To ensure the functionality of the BSO toolbox before and after developing new tools, unit testing is employed.
The unit tests are also included in the repository, and in case of developing/contributing to the BSO toolbox it is adviced to add and test the unit tests regularly.
Depending on the location of the repository on your local machine, moving to the unit test directory may be achieved as follows.

```bash
$ cd /usr/include/BSO-toolbox/unit_tests
```

The unit test directory contains a makefile with which the unit tests can be compiled.
In the makefile, check if the dependencies (Eigen and Boost) are linked to the locations of your local machine.
For instance, check if the line `BOOST = /usr/include/boost` is correctly linking to the Boost library on your local machine.

To run all tests of the BSO toolbox, make sure the directory mentioned above is the working directory of your terminal and type in the terminal:
```bash
$ make clean cls all
./all_test
```

Possible output of running the unit tests successfully may look as follows:
```bash
> Running 359 test cases...
> 
> *** No errors detected
```

Alternatively, also sub-parts of the toolbox can be tested. This can be achieved by replacing the `all` argument in the `make` command above by any of the following arguments:
* `all` tests all unit tests
* `ms_space` tests only the unit tests concerning the class ms_space (space in Movable Sizable representation)
* `ms_building` tests only the unit tests concering the class ms_building (building in Movable Sizable representation)
* `sc_building` tests only the unit tests concerning the class sc_building (building in SuperCube representation)
* `conformal` tests only the unit tests concerning the cf_building class (conformal building model)
* `trim_cast` tests only the unit tests concerning the trim and cast functions
* `geometry` tests only the unit tests concerning the geometry utilities in the
* `building_physics` tests only the unit tests concerning the building physics simulation tools
* `structural_design` tests only the unit tests concerning the structural design analysis tools
* `grammar` tests only the unit tests concerning the design grammars
* `visualization` tests only the unit tests concerning the visualization in the toolbox
* `xml` tests only the unit tests concerning XML data im- and export
* `data` tests only the unit tests concerning data points, e.g. Eulerian distance and clustering algorithm

For example to test the geometry package of the toolbox type the following in the terminal:
```bash
$ make clean cls geometry
$ ./geometry_test
```

## Contribution guidelines

The authors of this toolbox encourage others to contribute to the toolbox.
People who would like to contribute are asked to follow the guidelines described in this section.

### Write and run unit tests

To ensure the functionality of the contribution, but also of the existing code, unit tests should be written and run.
Make sure that all existing unit tests are succesfull before sending in your contribution.
Additionally, make sure to write meaningful unit tests to test your contribution.

### Review of code

Contributing to the toolbox is achieved by means of pull requests (also termed merge request on GitLab).
How to open a pull request is explained [here](https://docs.gitlab.com/ee/user/project/merge_requests/getting_started.html).
Once a pull request is opened, an admin of the toolbox can review your code and possibly discuss or modify your code.
If the proposed contribution is considered to be a meaningful addition to the toolbox, an admin can then merge your contribution with the repository.

### Other guidelines

Would you like to propose a change, or are you not sure that your potential contribution will be considered to be meaningfull by the admin?
In this case, you can create an issue, or contact an admin or the project manage (see information below).

## Code accreditation

Parts of the code that are used in the BSO toolbox are based on codes that are originally developed by other authors.
This section is used to clarify which parts of code in the BSO toolbox are based on other codes, and refers to the original work.
For any contributions to the BSO toolbox that use code that is originally written by other authors, add the accreditation to this section.

#### Method of Moving Asymptotes
The MMA.hpp and MMA.cpp files in the directiory `../BSO-toolbox/bso/structural_design/topology_optimization` are the translation of the MMA-code written in MATLAB by Krister Svanberg.
The original work can be downloaded from http://www.smoptit.se/ under de GNU General Public License.
The user should refer to the academic work of Krister Svanberg when work will be published in which this code is used.
For a description of the original work see this [paper] (https://onlinelibrary.wiley.com/doi/abs/10.1002/nme.1620240207).

## Who do I talk to? ###

###### For technical issues and/or questions, contact the repository admin:
  dr.ir. S. (Sjonnie) Boonstra  
  s.boonstra[at]abt.eu

###### For questions and other interests concerning the project, contact the project manager:
  dr.ir. H. (Hèrm) Hofmeyer  
  h.hofmeyer[at]tue.nl

###### Other members of the project.
Project manager at Leiden Institute of Advanced Computer Science (LIACS):  
  dr.ir. M.T.M. (Michael) Emmerich  
  m.t.m.emmerich[at]liacs.leidenuniv.nl


PhD graduate at LIACS:  
  dr.ir. K. (Koen) van der Blom  
  k.van.der.blom[at]liacs.leidenuniv.nl
