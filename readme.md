# README BSO toolbox

## What is this repository for?

Quick summary


## How do I get it set up? ###

### Dependencies
The toolbox is written in C++ and makes use of elements defined in the C++14 standard.
Although the toolbox is developed to be cross-platform, it has only been compiled and tested for Linux (Ubuntu 18.04).
Compilation on a Windows machine has been successfull in the past, but is not explained in this readme.

Furthermore, the toolbox depends on the following external software:
* Linear algebra (Geometry and FEM): the [Eigen](http://eigen.tuxfamily.org) C++ library.
* Various utilities (Tokenizers: the [Boost](https://www.boost.org/) C++ library.
* Solving systems of ODE's (thermal simulation): the [Odeint](https://www.odeint.com) library (also contained in the Boost library).
* Visualization:  

### Installation
A tutuorial for the installation of the dependencies and the toolbox is given in the following steps.
1. Install C++ compiler (GCC) and make
2. Install the Eigen library.
3. Install and compile the Boost library.
4. Install visualization dependencies (GSL and GLUT)
5. Install GIT
6. Clone the repository to your local machine
7. Compile and run the example code.

#### 1. Install C++ compiler (GCC) and make
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

#### 2. Install the Eigen library.
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

#### 3. Install and compile the Boost library.
In this step, the [Boost]() library is installed and parts of it are pre-compiled.

Go to the website of the Eigen library (http://eigen.tuxfamily.org) and download the tar.gz of the desired version (e.g. latest stable release).

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

#### 4. Install visualization dependencies (GSL and GLUT)
In this step the dependencies for the visualization in the toolbox are installed.
THe visualization makes use of OpenGL which is enabled by GL Utility Toolkit (GLUT) and GL Shading Language (GSL)

Install the dependencies by typing in the terminal:
```bash
$ sudo apt install libgsl-dev freeglut3-dev
```

#### 5. Install GIT
In this step, the version control software GIT is installed, together with a GUI for GIT.

To install GIT and an GUI type the following in the terminal:
```bash
$ sudo apt install git git-gui
```

#### 6. Clone the repository to your local machine
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
$ git clone https://gitlab.tue.nl/sboonstra/bso-toolbox.git
```

If the clone was successful, the BSO toolbox is now cloned onto your local machine, e.g. in the directory: `/usr/include/bso-toolbox`.

#### 7. Compile and run the example code.

### How to run unit tests
To ensure ... 
The unit tests are

```bash
$ cd /usr/include/bso-toolbox/unit_tests
```

This directory contains a makefile with which the unit tests can be compiled.
In the makefile, check if the dependencies (Eigen and Boost) are linked to the locations of your local machine.
For instance, check if the line `BOOST = /usr/include/boost` is correctly linking to the Boost library on your local machine/

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

### Write tests

### Review code

### Other guidelines

## Who do I talk to? ###

** Repo owner or admin
* Admin: S. Boonstra ([e-mail](TU/e: s.boonstra@tue.nl))
* 
** Other community or team contact
*
* Project Manager:
* dr.ir. H. (Hèrm) Hofmeyer
* e-mail: h.hofmeyer@tue.nl
* 
* dr.ir. M. (Michael) T.M. Emmerich
* e-mail: m.t.m.emmerich@liacs.leidenuniv.nl
*
* PhD at Eindhoven University of Technology:
* S. (Sjonnie) Boonstra MSc.
* e-mail: s.boonstra@tue.nl
*
* PhD at Leiden Institute of Advanced Computer Science, Leiden University:
* K. (Koen) van der Blom MSc.
* e-mail: k.van.der.blom@liacs.leidenuniv.nl