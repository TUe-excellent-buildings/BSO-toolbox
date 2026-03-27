# README BSO toolbox

## What is this repository for?
   
This repository contains a C++ library named the Building Spatial design Optimization toolbox (BSO toolbox).
In the first place, the toolbox has been developed to support building spatial design simulation and optimization by providing tools to:
(i) represent and modify building spatial designs,
(ii) generate discipline specific designs from a building spatial design,
(iii) analyze structural designs,
(iv) analyze the building physics of a building (i.e. thermal performance),
(v) analyze data obtained from the evaluations, e.g. clustering and sorting algorithms.
The toolbox itself does not contain any optimization methods, however, this repository will be extended with applications, and these will be related to optimization as well.
The toolbox relates to research in two joint projects by the Department of the Built Environment of Eindhoven University of Technology, and the Leiden Institute of Advanced Computer Science (LIACS), both in The Netherlands.
Key publications within these research projects are given below. These publications provide a lot of background information on the purpose and setup of the code in this repository:
* Boonstra, S.; Van der Blom, K.; Hofmeyer, H.; Emmerich, M.T.M.: Hybridization of an evolutionary algorithm and simulations of co-evolutionary design processes for early-stage building spatial design optimization, Automation in Construction, Volume 124, Article 103522, pages 1-18, 2021
* Claessens, D.P.H.; Boonstra, S.; Hofmeyer, H.: Spatial Zoning for Better Structural Topology Design and Performance, Advanced Engineering Informatics, Volume 46, 101162, pages 1-16, 2020
* Boonstra, S.; Van der Blom, K.; Hofmeyer, H.; Emmerich, M.T.M.: Conceptual structural system layouts via design response grammars and evolutionary algorithms, Automation in Construction, Volume 116, Article 103009, pages 1-20, 2020
* Van der Blom K.; Boonstra S.; Wang H.; Hofmeyer H.; Emmerich M.T.M. Evaluating Memetic Building Spatial Design Optimisation Using Hypervolume Indicator Gradient Ascent. In: Trujillo L.; Schütze O.; Maldonado Y.; Valle P. (Eds): Numerical and Evolutionary Optimization – NEO 2017, Studies in Computational Intelligence 785, page 62-82, Springer Nature, Cham, Switzerland, 2019
* Boonstra, S.; Van der Blom, K.; Hofmeyer, H.; Emmerich, M.T.M.; Van Schijndel, A.W.M.; De Wilde, P.: Toolbox for super-structured and super-structure free multi-disciplinary building spatial design optimisation, Advanced Engineering Informatics, Volume 36, pages 86-100, 2018
* Hofmeyer, H.; Davila Delgado, J.M.: Coevolutionary and Genetic Algorithm Based Building Spatial and Structural Design, AIEDAM - Artificial Intelligence for Engineering Design, Analysis and Manufacturing, Volume 29, pages 351-370, 2015
* Davila Delgado, J.M.; Hofmeyer, H.: Automated Generation of Structural Solutions based on Spatial Designs, Automation in Construction, Volume 35 (November), pages 528-541, 2013

Moreover, three PhD theses related to the research are:
* Boonstra, S 2020, 'Multi-disciplinary optimization of building spatial designs: co-evolutionary design process simulations, evolutionary algorithms, hybrid approaches', PhD-thesis, Department of the Built Environment, Eindhoven University of Technology, The Netherlands.
* van der Blom, K 2019, 'Multi-objective mixed-integer evolutionary algorithms for building spatial design', PhD-thesis, Leiden Institute of Advanced Computer Science, The Netherlands.
* Davila Delgado, JM 2014, 'Building structural design generation and optimisation including spatial modification', PhD-thesis, Department of the Built Environment, Eindhoven University of Technology, The Netherlands.

Although the first aim of the toolbox was related to research carried out at Eindhoven University of Technology, the contributors now invite all interested researchers to download and use the toolbox; to apply it for innovative design support and optimisation, possibly also in other domains; and to explore and extend its possiblities. This is all possible thanks to the GNU Affero General Public License v3.0. If publications result from using this repository, the contributers would appreciate it if you cite this repository (via the Zenodo DOI) and esspecially the related journal papers above, and not the PhD-theses, although the latter of course provide an excellent source of background information in practice.

## How do I get it set up? ###

### Dependencies
The toolbox is written in C++ and makes use of elements defined in the C++14 standard.
The toolbox is developed to be cross-platform, and has been compiled and tested for Linux (Ubuntu 22.04 LTS) and Windows 10 (22H2,19045.2486).

Furthermore, the toolbox depends on the following external source and static/dynamic libraries:
* For linear algebra, the [Eigen](http://eigen.tuxfamily.org) C++ library is used (Linux: last tested is v3.4.0).
* Various utilities (e.g. [Odeint](https://www.odeint.com)) from the [Boost](https://www.boost.org/) C++ library (Linux: last tested is v1.80.0). Precompiling is not needed.
* Visualization is written in the OpenGL standard and makes use of freeglut (Linux: last tested is v2.8.1-6).   

First, an elaborate Linux installation tutorial is given. Although it only officially supports Linux, devices with macOS should also be able to run the toolbox. It may be usefull to read this first, even if a Windows 10 installation is desired. Hereafter, the Windows 10 installation is explained.

### Linux installation
For Linux, a tutuorial for the installation of the dependencies and the toolbox is given in the following steps.
1. Install C++ compiler (GCC) and make
2. Install the Eigen library.
3. Install (and compile) the Boost library.
4. Install visualization dependencies (GLUT)
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

##### 3. Install (and compile) the Boost library.
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

**NOTE.** Enter the above commands one by one. During Windows 10 installations, it appeared that the compiled parts of Boost are not needed in the toolbox so far, and so also for the Linux installation, precompilation may possibly be treated as an optional step.

##### 4. Install visualization dependencies (GLUT)
In this step the dependencies for the visualization in the toolbox are installed.
THe visualization makes use of OpenGL which is enabled by the GL Utility Toolkit (GLUT)

Install the dependencies by typing in the terminal:
```bash
$ sudo apt install freeglut3-dev
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

### How to run unit tests (general and Linux version, see further below for Windows 10)
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

#### (Optional) Development on macOS
The installation process follows in the same way as for Linux. Furthermore, GLUT should already be preinstalled with the C++ compiler on macOS. Other code dependent on the BSO toolbox can be compiled with the following command (please keep in mind that for your application, the paths and filenames will most likely be different), hence let this be a simple example, which rather visualizes how to properly link the necessary libraries.

```bash
g++ -std=c++17 tutorial.cpp -o BSO_tutorial -I/Users/john/BSO_dir -I/Users/john/includes/eigen -I/Users/john/includes/boost -framework GLUT -framework OpenGL -L /usr/local/Cellar/freeglut/3.4.0/lib -lglut -O3 -march=native -w
```

### Windows 10 installation

For this installation Code::Blocks has been used, but other strategies are possible as well.

##### 1. Installing Code::Blocks and starting the project

* Install codeblocks-20.03mingw-setup.exe, found via www.codeblocks.org

All in Code::Blocks:

* At Settings>Compiler>GNU GCC Compiler>Toolchain executables, check whether all "Programs Files" are indeed from C:\Program Files\CodeBlocks\MinGW. For instance, a previously installed Strawberry Perl may lead to other files and locations. The "Reset defaults" button may help in this case.

* Create a new project by File>New>Project>Console application, select a "Folder to create project in:" you prefer, here "D:\" has been chosen, and if options arise, use default options throughout. Note that here project title BSO111 was used, for which Code:Blocks then creates directory D:\BSO111 for the project.

##### 2. External libraries: eigen, boost, glut

All in Windows 10:

* Unzip eigen-3.4.0.zip, found via eigen.tuxfamily.org. In this example the resulting directory has been put in D:\BSO111, but it normally would make more sense to put it more closely to Code::Blocks or the compiler, e.g. in the conventional C:\Program Files\CodeBlocks\MinGW\lib.

* Unzip boost_1_81_0.zip found via www.boost.org. Further, the same applies as above for eigen. 

**NOTE** Windows 10 extraction can be quite slow, and using e.g. 7-Zip may be faster. Also, precompiling certain parts of boost is needed for certain functionalities, see www.boost.org, but not here.

* Unzip freeglut-MinGW-3.0.0-1.mp.zip found via www.transmissionzero.co.uk. Further, the same applies as above for eigen.

**NOTE** Different from possibly claimed elsewhere, the BSO toolbox (both for Linux and Windows 10) needs neither the OpenGL Shading Language (GLSL) nor the GNU Scientific (GSL) libraries.

All in Code::Blocks:

* In Settings>Compiler>Search directions>Compiler, add the above directories to enable the compiler to find header files, here these directories are:  
D:\BSO111\eigen-3.4.0   
D:\BSO111\boost_1_81_0   
D:\BSO111\freeglut\include\GL  

* In Settings>Compiler>Search directories>Linker, add the below directory to enable the linker to find libraries, in this example:  
D:\toolbox\freeglut\lib\x64

* In Settings>Compiler>Linker settings, add at "Other linker options" the following line to link the libraries below:  
-lfreeglut_static -lopengl32 -lwinmm -lgdi32 -Wl,--subsystem,console

**NOTE** "--subsystem,console" is used so the terminal reponses correctly to cout.

* In Settings>Compiler>Compiler settings>#defines, add:  
FREEGLUT_STATIC  
This is to statically link freeglut. Alternatively dynamical linking can be used, by leaving this define out, and linking -lfreeglut instead of -lfreeglut_static 

* To save the above settings, save the project (File>Save Project), workspace (File>Save Workspace), and perspective (View>Perspectives>Save current). Probably one will do, but not sure which one.

##### 3. Preparing the BSO-toolbox

In Windows 10:

* Unzip BSO-toolbox-v.1.1.1.zip found at github.com, and put the resulting directories bso, example, and unit_tests in the project folder, so here:  
D:\BSO111\bso   
D:\BSO111\example   
D:\BSO111\unit_tests  

All in Code::Blocks:

* Within the project (at the Projects tab in the left "Management" column), remove the initial main.cpp. Right (or left for a left-handed mouse) click on the project within the Projects tab in the left "Management" column, Add files ..., and select D:\BSO111\example\main.cpp. Select Debug and Release options. 

**NOTE** For editing and searching in the other hpp and cpp files, all other files in the folders bso, example, and unit_tests can be included (using Add files recursively). But then these files should be untagged "Compile file" and "Link file": If all directories are unfold, all files can be selected in one step, and Compile file and Link file can be switched off.

* In Settings>Compiler>Search directions>Compiler, add the directory of the BSO-toolbox to enable the compiler to find the toolbox cpp and hpp files, in the particular example here, this is:  
D:\BSO111

In Code::Blocks or any text-editor: 

* As a temporarily bugfix, add the following pre-compiler lines to D:\toolbox\BSO-toolbox-v.1.1.1/bso/utilities/geometry/vector.hpp. Here these three lines have been put after the first two lines (#ifndef VECTOR_HPP, #define VECTOR_HPP):  
~~~
#ifndef M_PI
#define M_PI 3.141592653589793238463
#endif // M_PI
~~~

**NOTE** This bug will be fixed in future versions.

All in Code::Blocks:

* In Settings>Compiler>Compiler settings>Compiler Flags, switch on:  
-std=c++17 (c++17 version)   
-m64 (64-bits executable)   
-Og (otherwise Debug executable too large)   
-O3 (optimised code)

* Right (or left) click the project (in the left Management column), choose Properties. At tab "Build targets" select for the "Execution working dir:" the location of the main.cpp file, so for this example:  
D:\BSO111\example    
Use the absolute path, so not the relative option. And note that this is to be done for both the Debug and Release options (left column).

##### 4. Build and run

In Code::Blocks:

* Build>Build and run

##### 5. Unit tests

All in Code::Blocks:

* Delete the main.cpp file in the project, and add instead:  
D:\BSO111\unit_tests\all_test.cpp   
Select Debug and Release options.

* Use for the "Execution working dir:" the following (see also above):  
D:\BSO111\unit_tests

* Build>Clean workspace

* Build>Build and run

##### 6. Running the executables outside Code::Blocks, i.e. in a console

All in Windows 10:

* Include the folder with neccessary dll's in the windows PATH:  
C:\Program Files\CodeBlocks\MinGW\bin  
This can be done by:
Win-X, search, type “env”, and choose “Edit the system environment variables”; 
Click the “Environment Variables…” button; 
Under the “System Variables” section (the lower half), find the row with “Path” in the first column, and click edit, and add the location.

* Move the executable file, now in:  
D:\BSO111\bin\Debug   
to the appropiate execution working directory as indicated above, i.e. for the unit tests in:  
D:\BSO111\unit_tests  
and for the example:  
D:\BSO111\example  

* Run the executable in a console.

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

#### Method of Moving Asymptotes
The MMA.hpp and MMA.cpp files in the directiory `../BSO-toolbox/bso/structural_design/topology_optimization` are the translation of the MMA-code written in MATLAB by Krister Svanberg.
The original work can be downloaded from http://www.smoptit.se/ under de GNU General Public License.
The user should refer to the academic work of Krister Svanberg when work will be published in which this code is used.
For a description of the original work see this [paper] (https://onlinelibrary.wiley.com/doi/abs/10.1002/nme.1620240207).

## Who do I talk to? ###

###### For technical issues, questions, and interests in the projects:  
Dr. H.(Hèrm) Hofmeyer MSc (project leader Eindhoven University of Technology)  
h.hofmeyer[at]tue.nl  

Dr. S.(Sjonnie) Boonstra MSc (former PhD student Eindhoven)  
s.boonstra[at]abt.eu

###### Other members of the projects:  

Dr. M.T.M. (Michael) Emmerich MSc (project leader Leiden Institute of Advanced Computer Science (LIACS))  
m.t.m.emmerich[at]liacs.leidenuniv.nl  

T.(Tessa) Ezendam MSc (PhD student Eindhoven)  
t.ezendam[at]tue.nl  

K.G. Pereverdieva MSc (PhD student Leiden)  
k.g.pereverdieva[at]liacs.leidenuniv.nl  

Dr. K. van der Blom MSc (former PhD student Leiden)  
koen.vdblom[at]lip6.fr

