
# High Precision Particle Swarm Optimization (HiPPSO) in C++<a name="high-precision-particle-swarm-optimization-hippso-in-c"></a>

[![DOI](https://zenodo.org/badge/85169564.svg)](https://zenodo.org/badge/latestdoi/85169564)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Build Status](https://travis-ci.org/alexander-rass/HiPPSO.svg?branch=master)](https://travis-ci.org/alexander-rass/HiPPSO)
[![Coverage Status](https://coveralls.io/repos/github/alexander-rass/HiPPSO/badge.svg)](https://coveralls.io/github/alexander-rass/HiPPSO)

## Table of Contents<a name="table-of-contents"></a>
* [High Precision Particle Swarm Optimization (HiPPSO) in C++](#high-precision-particle-swarm-optimization-hippso-in-c)
* [Table of Contents](#table-of-contents)
* [Description](#description)
* [Features](#features)
* [Download](#download)
* [Installation Instructions](#installation-instructions)
  * [Installation on Windows](#installation-on-windows)
  * [Installation on a Linux operating system](#installation-on-a-linux-operating-system)
* [Usage](#usage)
* [Examples](#examples)
* [Documentation](#documentation)
* [Testing](#testing)
* [Extensibility](#extensibility)
* [Feedback](#feedback)
* [License](#license)
* [Scientific Results](#scientific-results)
* [References](#references)

## Description<a name="description"></a>

The High Precision Particle Swarm Optimization (HiPPSO) is an implementation of
the particle swarm optimization (PSO) with highly precise data types.

PSO is a popular nature inspired meta heuristic for solving PSO, introduced by
Kennedy and Eberhart [1,2], is a very popular nature-inspired meta-heuristic
for solving continuous optimization problems.  It is inspired by the social
interaction of individuals living together in groups and supporting and
cooperating with each other.  It is applied to continuous objective functions
on a multi-dimensional domain.  Those functions are likely not given in a
closed form, but by a *black box*.  The popularity of the PSO framework is due
to the fact that on the one hand it can be realized and, if necessary, adapted
to further needs easily, but on the other hand shows in experiments good
performance results with respect to the quality of the obtained solution and
the speed needed to obtain it.  A thorough discussion of PSO can be found in
[3].

To be precise, let an objective function f:IR<sup>D</sup>&rarr;IR on a
D-dimensional domain be given that (w. l. o. g.) has to be minimized.  A
population of *particles*, each consisting of a position (the candidate for a
solution), a velocity and a local attractor, moves through the search space
IR<sup>D</sup>. The local attractor of a particle is the best position with
respect to f this particle has encountered so far.  The best of all local
attractors is the global attractor.  The movement of a particle is governed by
so-called movement equations that depend on both the particle's velocity and
its two attractors and on some additional fixed algorithm parameters.

## Features<a name="features"></a>

The key feature of this implementation is the usage of data types with
arbitrary precision.  Those data types are used for all relevant calculations.
Here the precision is meant as the number of bits of the mantissa of the
floating point data type.  Either the precision can be set to a fixed value or
the precision can be set adjustable.  If the precision is set adjustable, then
the software autonomously increases the precision to avoid errors related to
imprecise evaluations. With this implementation one can get as close to actual
real numbers as possible.

In addition this software is highly adaptable.  Many options for the particle
swarm are already implemented.  There are a plenty of already defined objective
functions, neighborhood topologies and bound handling strategies.  For analysis
of the particle swarm there are also a plenty of statistical evaluations, which
can be displayed.  Furthermore the objective functions and the statistical
evaluations are highly modularized to enable the user to specify them on
demand.

If the available options are not sufficient, then it is possible to implement
further possibilities (Further objective functions, neighborhood topologies,
statistical evaluations, bound handling strategies, velocity adjustment
strategies, position and velocity update procedures, ...). Only the respective
interfaces need to be implemented and then new idea can be added to the
available features and used. Here the arbitrary precision data type needs to be
used, but to ease this process a large set of operations on this data type is
already implemented.

## Download<a name="download"></a>

The source code and all further data can be downloaded from the
[github-repository](https://github.com/alexander-rass/HiPPSO/).

## Installation Instructions<a name="installation-instructions"></a>

### Installation on Windows<a name="installation-on-windows"></a>

* Install Cygwin with components gcc, m4 and make
* Install the GMP library
  * Download and unzip the GMP sources and switch to that folder.
    * For example: download `gmp-x.x.x.tar.xz` from https://gmplib.org/
    * unzip the file with `tar -xJf gmp-x.x.x.tar.xz`
    * go to that folder with `cd gmp-x.x.x`
  * Execute `./configure`
  * Execute `make`
  * Execute `make install`
  * It might be necessary to copy the library `libgmp.a` and `libgmp.la` from
    `/usr/local/lib` to `/lib` to ensure that it is recognized.
* [Download](https://github.com/alexander-rass/HiPPSO/) and unzip the High Precision PSO sources and switch to that folder.
* Execute `make` to generate the executable file (`high_precision_pso.exe`).
* The executable file can be moved to any location - it contains the whole
  program. For example you can move it to a folder, which is included in your
  PATH variable. If you do this you can start the High Precision PSO program at
  any location without prefix.

### Installation on a Linux operating system<a name="installation-on-a-linux-operating-system"></a>

* Install g++ compiler, and make (if not already present).

  On systems like Ubuntu you can install them by the command
  `sudo apt-get install make g++`.
* Install the GMP library
  * On systems like Ubuntu you can install the GMP library by the command
    `sudo apt-get install libgmp-dev`.
  * Alternatively you can install it as described in the Windows section.
* [Download](https://github.com/alexander-rass/HiPPSO/) and unzip the High Precision PSO sources and switch to that folder.
* Execute `make` to generate the executable file (`high_precision_pso`).
* The executable file can be moved to any location - it contains the whole
  program. For example you can move it to a folder, which is included in your
  PATH variable. If you do this you can start the High Precision PSO program at
  any location without prefix.

## Usage<a name="usage"></a>

The program is a command line tool.  If the program is started without any
parameters, then the available options will be displayed.

The behavior is specified in a configuration file. Examples and explanations of
all options, which can be used in the configuration files, can be found in the
sub-folder
[guideline_configuration_files](https://github.com/alexander-rass/HiPPSO/tree/master/guideline_configuration_files).
It is recommended to use our
[online tool for configuration file generation](https://www12.informatik.uni-erlangen.de/downloads/eako/HiPPSOconfigurationFileGeneration/index.php).
There you can find additional information for each option. This is especially
helpful if you move your mouse over options of drop down menus.
An additional special file is the file
[runcheck.conf](https://github.com/alexander-rass/HiPPSO/blob/master/guideline_configuration_files/runcheck.conf).
If the runcheck option is activated in the normal configuration files, then it
can be specified, when the program is allowed to run.  As the runcheck
configuration file is parsed periodically during runtime this can be used to
terminate the program in a clean way without losing any information.

Usually the High Precision PSO program is started with parameter `c` and the name
of the configuration file.  For example to start the HiPPSO with the example
configuration file one can use

`./high_precision_pso c guideline_configuration_files/example_configuration_file.conf`

The HiPPSO generates various files.  At the start it generates a configuration
backup file (extension `confBU`), which stores the chosen configuration.  Also
at the start a log file (extension `log`) is created, which stores when the
HiPPSO starts / stops / restarts / finishes simulation of the particle swarm.
Approximately each minute (and at clean termination) a data backup file
(extension `backup`) is generated, which stores all information about the
current state such that it can be recovered. Additionally for each specified
statistic a data file will be generated, which contains the evaluated
statistical data. If also the debug swarm directive is activated in the
configuration file, then an additional folder is created with the name of the
prefix of the other files. In this folder gnuplot input files are created and a
script to create images out of them. gnuplot needs to be installed to generate
images.

If the HiPPSO is terminated by the runcheck configuration file, then it can be
restarted with parameter `r` and the name of the configuration backup file,
which has extension `confBU`.

If the HiPPSO has been illegally terminated, then one can try to restart with
parameter `rf` and the name of the configuration backup file, which has
extension `confBU`.

Additionally the HiPPSO can restart with a configuration file and a data backup
file.  For this purpose one can use the parameter `restart` followed by the
name of the configuration file followed by the name of the data backup file.
This option might be useful if you like to increase the number of iterations,
or change the statistical evaluations.  The HiPPSO will then start the
simulation at the stored iteration in the data backup file.

## Examples<a name="examples"></a>

In the [examples folder](https://github.com/alexander-rass/HiPPSO/tree/master/examples)
some examples can be found.

`example_01` starts a particle swarm with four particles on the sphere function
(f(x)=&sum;<sub>d=0..D-1</sub>x[d]<sup>2</sup>)
in two dimensions. The used configuration file is
[example_01.conf](https://github.com/alexander-rass/HiPPSO/blob/master/examples/example_01.conf).
A statistic is generated, which contains the objective function value of the
best found position.  In combination with the short gnuplot script
[example_01.STAT.GlBestFuncValue.gnuplot](https://github.com/alexander-rass/HiPPSO/blob/master/examples/example_01.STAT.GlBestFuncValue.gnuplot).
the following figure can be produced:

![trend of the objective function value of the best found position](https://github.com/alexander-rass/HiPPSO/raw/master/examples/example_01.STAT.GlBestFuncValue.png "trend of the objective function value of the best found position")

Additionally the debug swarm directive created a folder with gnuplot scripts.
After executing the script the images look like this:

![particles and attractors](https://github.com/alexander-rass/HiPPSO/raw/master/examples/example_01.animation.gif "particles and attractors")

(This gif is produced by the command
`convert -delay 50 -loop 0  *.png \( +clone -set delay 500 \) +swap +delete animation.gif`
in the folder of the generated png files)

But even in higher dimensions the swarm can be displayed very well by this
software.  `example_02` starts a particle swarm with eight particles on the
sphere function
(f(x)=&sum;<sub>d=0..D-1</sub>x[d]<sup>2</sup>)
in eight dimensions. The used configuration file is
[example_02.conf](https://github.com/alexander-rass/HiPPSO/blob/master/examples/example_02.conf).
The debug swarm directive created a folder with gnuplot scripts.  After
executing the script the images look like this:

![particles and attractors](https://github.com/alexander-rass/HiPPSO/raw/master/examples/example_02.animation.gif "particles and attractors")

(This gif is produced by the command
`convert -delay 100 -loop 0  *.png \( +clone -set delay 500 \) +swap +delete -resize 50% animation.gif`
in the folder of the generated png files)

If this visualization is not suitable, then the bare data of particle
positions, local and global attractor positions, velocities and anything else
can be extracted by statistics. Through this statistics everyone can display
the data in the preferred way.

The explicit advantage of this software is the opportunity to activate self
adjusting precision.  `example_03` displays the development of the global
attractor value of the HiPPSO optimizing the three-dimensional Rosenbrock
function
(f(x)=&sum;<sub>d=0..D-2</sub>(100(x[i+1]-x[i]<sup>2</sup>)<sup>2</sup>+(1-x[i])<sup>2</sup>) )
with four particles. The used configuration files are
[example_03a.conf](https://github.com/alexander-rass/HiPPSO/blob/master/examples/example_03a.conf)
(configuration with self adjusting precision) and
[example_03b.conf](https://github.com/alexander-rass/HiPPSO/blob/master/examples/example_03b.conf)
(configuration with constant precision).  A statistic is generated, which
contains the objective function value of the best found position.  In
combination with the short gnuplot script
[example_03.STAT.GlBestFuncValue.gnuplot](https://github.com/alexander-rass/HiPPSO/blob/master/examples/example_03.STAT.GlBestFuncValue.gnuplot)
the following figure can be produced:

![trend of the objective function value of the best found position](https://github.com/alexander-rass/HiPPSO/raw/master/examples/example_03.STAT.GlBestFuncValue.png "trend of the objective function value of the best found position")

This image visualizes the limitations of constant precision, which is present
in usual floating point calculations, compared to the actual behaviour.  As the
position differences become small the actual differences can not be stored
correctly to ensure correct transformation of the mathematical idea of PSO to
the actual optimization procedure. This results in a situation where
optimization is stopped completely. Therefore it is obvious that using larger
precision is essential to produce reliable examples. Furthermore HiPPSO
automatically increases precision on demand, which saves runtime on earlier
iterations, when lower precision is sufficient.

## Documentation<a name="documentation"></a>

The documentation including an API can be found on
the related [github-pages](https://alexander-rass.github.io/HiPPSO/) of this
repository. It is generated by doxygen. For customizing the output of doxygen
some files from the
[doxygen-bootstrapped](https://github.com/Velron/doxygen-bootstrapped)
repository of [Velron](https://github.com/Velron) are used. It is added as a
submodule and has an
[Apache 2.0 license](https://www.apache.org/licenses/LICENSE-2.0).

## Testing<a name="testing"></a>

This software uses Travis CI
[![Build Status](https://travis-ci.org/alexander-rass/HiPPSO.svg?branch=master)](https://travis-ci.org/alexander-rass/HiPPSO)
as continuous integration service and Coveralls
[![Coverage Status](https://coveralls.io/repos/github/alexander-rass/HiPPSO/badge.svg)](https://coveralls.io/github/alexander-rass/HiPPSO)
to ensure reasonable test coverage.

## Extensibility<a name="extensibility"></a>

By configuration file it is possible to define many properties and
characteristics of the HiPPSO. Especially for functions and statistics it is
additionally possible to specify new functions through a special syntax. By
this syntax it is possible to evaluate benchmark functions like Ackley function
even though it is not natively supplied. You can do element wise operations and
sum them up (E.g. sum of sine values `function reduce sum sin identity`). It is
also possible to access individual position entries (E.g. for third dimension
`reduce specific 2 identity`), perform further operations with them and combine
them with other values (E.g. the sine value of first dimension multiplied by
the cosine value of fourth dimension and then evaluating the square root of
the absolute value of that product:
`function sqrt abs combine * sin reduce specific 0 identity reduce specific 3 identity`).
Additionally there are many more options.

However, if the available options are still not sufficient it is possible to
implement your own versions.  You can find guidelines to implement your own
versions of the following modules beside the respective source files:
* [Function](https://github.com/alexander-rass/HiPPSO/blob/master/src/function/function.README)
* [Statistic](https://github.com/alexander-rass/HiPPSO/blob/master/src/statistics/statistic.README)
* [Neighborhood topology](https://github.com/alexander-rass/HiPPSO/blob/master/src/neighborhood/neighborhood.README)
* [Bound handling strategy](https://github.com/alexander-rass/HiPPSO/blob/master/src/bound_handling/bound_handling.README)
* [Velocity adjustment](https://github.com/alexander-rass/HiPPSO/blob/master/src/velocity_adjustment/velocity_adjustment.README) (after bound handling has been applied)
* [Position and velocity updater](https://github.com/alexander-rass/HiPPSO/blob/master/src/position_and_velocity_updater/position_and_velocity_updater.README) (specifies how the new velocity is calculated)

Please click on the respective item to go directly to the guideline description.

## Feedback<a name="feedback"></a>

Please feel free to contact Alexander Raß (Alexander.Rass@fau.de). Feedback or
a notice that you use this program is highly appreciated. We will also support
you in case you have problems during installation or usage of this program.

## License<a name="license"></a>

The MIT License (MIT)

Copyright (c) 2016 by Friedrich-Alexander-Universität Erlangen-Nürnberg and
Alexander Raß and Manuel Schmitt

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

See also the
[license file](https://github.com/alexander-rass/HiPPSO/blob/master/LICENSE).

## Scientific Results<a name="scientific-results"></a>

The following paper uses HiPPSO for experiments:

Raß A., Schmitt M., Wanka R.: <br>
Explanation of Stagnation at Points that are not Local Optima in Particle Swarm Optimization by Potential Analysis <br>
17th Genetic and Evolutionary Computation Conference (GECCO) <br>
DOI: 10.1145/2739482.2764654

## References<a name="references"></a>

[1] R. C. Eberhart and J. Kennedy.<br>
A new optimizer using particle swarm theory.<br>
In Proc. 6th International Symposium on Micro Machine and Human Science, pages 39–43, 1995.

[2] J. Kennedy and R. C. Eberhart.<br>
Particle swarm optimization.<br>
In Proc. IEEE International Conference on Neural Networks, volume 4, pages 1942–1948, 1995.

[3] B. K. Panigrahi, Y. Shi, and M.-H. Lim, editors.<br>
Handbook of Swarm Intelligence — Concepts, Principles and Applications.<br>
Springer, 2011.

