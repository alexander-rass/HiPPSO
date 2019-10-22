<?php

$targetHomepage='/downloads/eako/HiPPSOconfigurationFileGeneration/index.php';

class ConfigurationOptions{
	private static $initDone=false;
	private static $all_options;
	private static $command_explanation;
	private static $command_mapping;
	private static $command_default_value;

	private static function init(){
		if(self::$initDone) {
			return;
		}
		self::$initDone=true;
		self::$all_options=array();
		self::$command_explanation=array();
		self::$command_mapping=array();
		self::$command_default_value=array();


		self::$all_options['<positive integer value>']=array(array('<positive integer value>'));
		self::$all_options['<nonnegative integer value>']=array(array('<nonnegative integer value>'));
		self::$all_options['<alphanumeric value>']=array(array('<alphanumeric value>'));
		self::$all_options['<file name>']=array(array('<file name>'));
		self::$all_options['<float value>']=array(array('<float value>'));

		self::$all_options['<boolean value>']=array(array('true'), array('false'));

		self::$command_mapping['parameterChi']= '<float value>';
		self::$command_mapping['parameterCoefficientLocalAttractor']= '<float value>';
		self::$command_mapping['parameterCoefficientGlobalAttractor']= '<float value>';
		self::$command_default_value['parameterChi']= '0.72984';
		self::$command_default_value['parameterCoefficientLocalAttractor']= '1.496172';
		self::$command_default_value['parameterCoefficientGlobalAttractor']= '1.496172';
		self::$command_explanation['parameterCoefficientGlobalAttractor']='Set parameters for velocity update in PSO. v[i] = (chi) * v[i] + c_{loc} * r_1 * (l[i] - x[i]) + c_{glob} * r_2 * (g[i] - x[i]) for each dimension i separately, where v = velocity, c_{loc} = coefficient local attractor, c_{glob} = coefficient global attractor, r_1 and r_2 random values in [0,1], l = local attractor, x = position, g global attractor. A standard combination of parameters is chi=0.72984, c_{loc}=c_{glob}=1.496172.';

		self::$command_mapping['includeSystemTimeInFilename'] = '<boolean value>';
		self::$command_default_value['includeSystemTimeInFilename'] = array('false');
		self::$command_explanation['includeSystemTimeInFilename'] = 'Set whether the current date and time should be included in the prefix of all file names. WARNING: There is no check whether the respective prefix is already in use. If a prefix is used repeatedly in the same folder, then the program may produce wrong / useless data and may also destroy data of other executions of this program. Therefore it is highly recommended to set the "includeSystemTimeInFilename" value to true in such cases, because this automatically produces different prefixes for each execution, which has not started at the same second.';

		self::$command_mapping['updateGlobalAttractor'] = '<updateGlobalAttractor>';
		self::$command_default_value['updateGlobalAttractor'] = array('eachParticle');
		self::$command_explanation['updateGlobalAttractor'] = 'The option eachIteration means that the global attractor is updated after each iteration. Particles with higher index do not recognize improvements of earlier particles in the same iteration. The option eachParticle means that the global attractor is updated immediately after each particle has found a new position.';
		self::$all_options['<updateGlobalAttractor>']=array(array('eachIteration'), array('eachParticle'));
		
		self::$command_mapping['initialprecision']= '<positive integer value>';
		self::$command_default_value['initialprecision']= 128;
		self::$command_explanation['initialprecision']='Set initial precision (in bits) of the used floating point data type (Accepted values are positive integers).';
		self::$command_mapping['precision']= '<positive integer value>';
		self::$command_default_value['precision']= 128;
		self::$command_explanation['precision']='Set precision (in bits) of the used floating point data type (Accepted values are positive integers). According to this value checks are performed to ensure that calculations are reliable even if less bits (according to the specified number here) are used. If this is not the case then the precision will be increased.';

		self::$command_mapping['checkprecision']= '<checkprecision>';
		self::$command_default_value['checkprecision']= array('allExceptStatistics');
		self::$command_explanation['checkprecision']='Set when the precision is checked.';
		self::$all_options['<checkprecision>']=array(array('all'), array('allExceptStatistics'), array('never'));
		self::$command_explanation['<checkprecision>' . implode("_", array('all'))] = 'With this option precision checks are performed always.';
		self::$command_explanation['<checkprecision>' . implode("_", array('allExceptStatistics'))] = 'With this option precision checks are performed always except on statistical calculations. This is useful if runs should be reproducible not depending on whether statistics are done or not.';
		self::$command_explanation['<checkprecision>' . implode("_", array('never'))] = 'With this option precision checks are performed never.';
		
		self::$command_mapping['checkprecisionprobability']= '<ratio>';
		self::$command_default_value['checkprecisionprobability']= '1.0';
		self::$command_explanation['checkprecisionprobability']= 'Set the ratio how often the precision is checked expectedly (respecting the choice fo "checkprecision" command). A ratio of 0.0 means that the precision is never checked. A ratio of 1.0 means that the precision is always checked if it is triggered by "checkprecision" option. A ratio of 0.5 means that for 50% of the calculations the precision is checked if it is triggered by "checkprecision" option.';
		self::$command_mapping['<ratio>']='<float value>';
		self::$all_options['<ratio>']=array(array('<float value>'));

		self::$command_mapping['particles']= '<positive integer value>';
		self::$command_default_value['particles']= 5;
		self::$command_explanation['particles']='Set the number of particles.';
		self::$command_mapping['dimensions']= '<positive integer value>';
		self::$command_default_value['dimensions']= 8;
		self::$command_explanation['dimensions']='Set the number of dimensions of the search space.';
		self::$command_mapping['steps']= '<positive integer value>';
		self::$command_default_value['steps']= 1000;
		self::$command_explanation['steps']='Set the number of steps (iterations).';

		self::$command_mapping['srand']= '<rng description>';
		self::$command_explanation['srand']='Specify the random number generator (RNG) which should be used. Specifying only the seed uses the standard linear congruence random number generator.';
		self::$all_options['<rng description>']=array(array('<seed>'), array('linearCongruenceRNG', '<seed>', '<lcrng>'));
		self::$command_mapping['<seed>']='<nonnegative integer value>';
		self::$all_options['<seed>']=array(array('<nonnegative integer value>'));
		self::$command_explanation['<rng description>' . implode("_", array('<seed>'))] = 'This option uses a standard linear congruence RNG with multiplier 1571204578482947281, additive term 12345678901234567 and modulus 9223372036854775808.';
		self::$command_explanation['<rng description>' . implode("_", array('linearCongruenceRNG', '<seed>', '<lcrng>'))] = 'This option uses a linear congruence RNG with variable multiplier, additive term and modulus.';
		self::$command_default_value['srand']= array('<seed>');
		self::$command_default_value['<rng description>']= array('<seed>');
		self::$command_default_value['<seed>']= 42;
		self::$all_options['<lcrng>']=array(array('standard', '<floating point generation 1>'), array('mod2p63', '<multiplier>', '<adder>', '<floating point generation 1>'), array('specific', '<multiplier>', '<adder>', '<modulus>', '<floating point generation 2>'));
		self::$command_explanation['<lcrng>' . implode("_", array('standard', '<floating point generation 1>'))] = 'This option uses a standard linear congruence RNG with multiplier 1571204578482947281, additive term 12345678901234567 and modulus 9223372036854775808.';
		self::$command_explanation['<lcrng>' . implode("_", array('mod2p63', '<multiplier>', '<adder>', '<floating point generation 1>'))] = 'This option uses a standard linear congruence RNG with modulus 9223372036854775808=2^{63} and variable multiplier and additive term.';
		self::$command_explanation['<lcrng>' . implode("_", array('specific', '<multiplier>', '<adder>', '<modulus>', '<floating point generation 2>'))] = 'This option uses a standard linear congruence RNG with variable multiplier, additive term and modulus.';
		self::$all_options['<multiplier>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<multiplier>']= '<nonnegative integer value>';
		self::$command_default_value['<multiplier>']= '1571204578482947281';
		self::$all_options['<adder>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<adder>']= '<nonnegative integer value>';
		self::$command_default_value['<adder>']= '12345678901234567';
		self::$all_options['<modulus>']= array(array('<positive integer value>'));
		self::$command_mapping['<modulus>']= '<positive integer value>';
		self::$command_default_value['<modulus>']='9223372036854775808';
		self::$all_options['<used bits>']= array(array('<positive integer value>'));
		self::$command_mapping['<used bits>']= '<positive integer value>';
		self::$command_default_value['<used bits>']='2';
		self::$all_options['<floating point generation 1>'] = array(array('fast'), array('intense', '<used bits>'));
		self::$all_options['<floating point generation 2>'] = array(array('fast'), array('intense'));
		self::$command_explanation['<floating point generation 1>' . implode("_", array('fast'))] = 'If floating point generation is specified as fast then for floating point value generation a random double value is converted to a highly precise floating point value.';
		self::$command_explanation['<floating point generation 1>' . implode("_", array('intense', '<used bits>'))] = 'If floating point generation is specified as intense then the result is sum_{i=1..t} (highest <used bits> bits of a random integer value) / ( 2^( i * <used bits> ) ), where t is as large such that all bits of the random highly precise floating point value are randomly generated.';
		self::$command_explanation['<floating point generation 2>' . implode("_", array('fast'))] = 'If floating point generation is specified as fast then for floating point value generation a random double value is converted to a highly precise floating point value.';
		self::$command_explanation['<floating point generation 2>' . implode("_", array('intense'))] = 'If floating point generation is specified as intense then the result is sum_{i=1..t} (random 64 bit integer value) / (<modulus>^i), where t is as large such that all bits of the random highly precise floating point value are randomly generated.';

		self::$all_options['initializeVelocity']=array( array('zero'), array('halfDiff'), array('random'));
		self::$command_explanation['initializeVelocity']='Set how the velocity of each particle should be initialized.';
		self::$command_default_value['initializeVelocity']=array('zero');
		self::$command_explanation['initializeVelocity' . implode("_", array('zero'))] = 'This option initializes all velocity entries to zero.';
		self::$command_explanation['initializeVelocity' . implode("_", array('halfDiff'))] = 'This option samples a second position for each particle, subtracts the actual position and multiplies the result by 0.5.';
		self::$command_explanation['initializeVelocity' . implode("_", array('random'))] = 'Velocities are initialized similar to the position. See command initializationinformation in this case';
		
		self::$command_mapping['fileprefix']= '<alphanumeric value>';
		self::$command_default_value['fileprefix']='testrun001';
		self::$command_explanation['fileprefix' . 'once']='Set file prefixes of generated data. If none is specified then an automatic string is generated by specified values in this configuration file. Only alphanumeric characters are allowed here.';

		self::$all_options['initializationinformation']=array( array('<position or velocity>', '<affected particles and dimensions>', '<initialization type>'));
		self::$all_options['<position or velocity>']=array( array('position'), array('velocity'));
		self::$all_options['<affected particles and dimensions>']=array( array('<first particle id>' ,'<last particle id>' ,'<first dimension id>' ,'<last dimension id>'));
		self::$all_options['<first particle id>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<first particle id>']= '<nonnegative integer value>';
		self::$command_default_value['<first particle id>']='0';
		self::$all_options['<last particle id>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<last particle id>']= '<nonnegative integer value>';
		self::$command_default_value['<last particle id>']='0';
		self::$all_options['<first dimension id>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<first dimension id>']= '<nonnegative integer value>';
		self::$command_default_value['<first dimension id>']='0';
		self::$all_options['<last dimension id>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<last dimension id>']= '<nonnegative integer value>';
		self::$command_default_value['<last dimension id>']='0';
		self::$all_options['<initialization type>']= array(array('bounds', '<lower bound>', '<upper bound>'),
		   													array('centerandrange', '<center coordinate>', '<range>'),
															array('randomcenterandrange', '<center coordinate lower bound>', '<center coordinate upper bound>', '<range>'),
															array('scale', '<scale>'),
															array('powerscale', '<power scale>'));
		self::$all_options['<lower bound>']= array(array('<float value>'));
		self::$command_mapping['<lower bound>']= '<float value>';
		self::$command_default_value['<lower bound>']='-1.0';
		self::$all_options['<upper bound>']= array(array('<float value>'));
		self::$command_mapping['<upper bound>']= '<float value>';
		self::$command_default_value['<upper bound>']='1.0';
		self::$all_options['<center coordinate>']= array(array('<float value>'));
		self::$command_mapping['<center coordinate>']= '<float value>';
		self::$command_default_value['<center coordinate>']='2.0';
		self::$all_options['<range>']= array(array('<float value>'));
		self::$command_mapping['<range>']= '<float value>';
		self::$command_default_value['<range>']='2.0';
		self::$all_options['<center coordinate lower bound>']= array(array('<float value>'));
		self::$command_mapping['<center coordinate lower bound>']= '<float value>';
		self::$command_default_value['<center coordinate lower bound>']='3.0';
		self::$all_options['<center coordinate upper bound>']= array(array('<float value>'));
		self::$command_mapping['<center coordinate upper bound>']= '<float value>';
		self::$command_default_value['<center coordinate upper bound>']='4.0';
		self::$all_options['<scale>']= array(array('<float value>'));
		self::$command_mapping['<scale>']= '<float value>';
		self::$command_default_value['<scale>']='1e-3';
		self::$all_options['<power scale>']= array(array('<integer value>'));
		self::$command_mapping['<power scale>']= '<integer value>';
		self::$command_default_value['<power scale>']='-13';
		self::$command_explanation['initializationinformation' . 'once']='Set how positions and velocities are initialized. Please note: The commands which influences the velocity initialization are ignored if velocity initialization is not random. For example if the command "initializeVelocity halfDiff" or "initializeVelocity zero" is active then the command "initializationinformation" with subcommand "velocity" has no effect. Particles and dimensions are 0-indexed, the ids of the particles are in the range from 0 to one less than the respective number. Particle and dimension ranges are inclusive their bounds.';
		self::$command_explanation['<position or velocity>']='This option specifies whether the command influences the initialization of the position or the velocity.';
		self::$command_explanation['<affected particles and dimensions>']='This option specifies which particles and which dimensions are affected. The specified indices should be integers. The indices are treated zero indexed and inclusive specified index bounds.';
		self::$command_explanation['<initialization type>' . implode("_", array('bounds', '<lower bound>', '<upper bound>'))] = 'This option specifies that the coordinates of all particles and dimensions in the specified range are randomly initialized in the range from <lower bound> to <upper bound>. <lower bound> and <upper bound> can be floating point values.';
		self::$command_explanation['<initialization type>' . implode("_", array('centerandrange', '<center coordinate>', '<range>'))] = 'This option specifies that the coordinates of all particles and dimensions in the specified range are randomly initialized in the interval from (<center coordinate> - <range>) to (<center coordinate> + <range>). <center coordinate> and <range> can be floating point values.';
		self::$command_explanation['<initialization type>' . implode("_", array('randomcenterandrange', '<center coordinate lower bound>', '<center coordinate upper bound>', '<range>'))] = 'This option specifies that the coordinates of all particles and dimensions in the specified range are randomly initialized in the interval from (x - <range>) to (x + <range>), where x is a random value in the interval <lower bound> to <upper bound> and x is sampled for each dimension independently.';
		self::$command_explanation['<initialization type>' . implode("_", array('scale', '<scale>'))] = 'This option specifies that the length of the random interval of all particles and dimensions in the specified range are scaled by a factor of <scale>. The center of the interval stays constant. <scale> can be a floating point value.';
		self::$command_explanation['<initialization type>' . implode("_", array('powerscale', '<power scale>'))] = 'This option specifies that the length of the random interval of all particles and dimensions in the specified range are scaled by a factor of 2^(<power scale>). The center of the interval stays constant. <power scale> needs to be an integer (negative or positive).';


		self::$all_options['neighborhood'] = array( array('gBest'), array('wheel'), array('lBest', '<2 times k>'), array('ring'), array('grid', '<number of rows>', '<number of columns>'));
		self::$command_default_value['neighborhood']=array('gBest');
		self::$all_options['<2 times k>']= array(array('<positive integer value>'));
		self::$command_mapping['<2 times k>']= '<positive integer value>';
		self::$command_default_value['<2 times k>']='2';
		self::$all_options['<number of rows>']= array(array('<positive integer value>'));
		self::$command_mapping['<number of rows>']= '<positive integer value>';
		self::$command_default_value['<number of rows>']='2';
		self::$all_options['<number of columns>']= array(array('<positive integer value>'));
		self::$command_mapping['<number of columns>']= '<positive integer value>';
		self::$command_default_value['<number of columns>']='2';
		self::$command_explanation['neighborhood'] = 'This option sets the neighborhood topology. The neighborhood topology specifies which particles are known by other particles, i.e., if particle i knows particle j then the best position found by particle j is considered for the choice of global attractor for particle i.';
		self::$command_explanation['neighborhood' . implode("_", array('gBest'))] = 'All particles know each other. There is a single common global attractor for all particles.';
		self::$command_explanation['neighborhood' . implode("_", array('wheel'))] = 'All particles know the first particle and the first particle knows all particles. The global attractor of the first particle is the best position of all particles. The global attractor of all other particles is the best position of itself and the best position of the first particle.';
		self::$command_explanation['neighborhood' . implode("_", array('lBest', '<2 times k>'))] = 'Each particle knows the next and the previous <2 times k> / 2 particles. For example "neighborhood lBest 10" specifies that each particle knows the next and the previous 5 particles.';
		self::$command_explanation['neighborhood' . implode("_", array('ring'))] = 'Each particle knows the next and the previous particle. (equivalent to "lBest 2")';
		self::$command_explanation['neighborhood' . implode("_", array('grid', '<number of rows>', '<number of columns>'))] = 'The particles are assigned a position in a <number of rows> times <number of columns> grid. Each particle knows the four particles next to him, i.e., the particle above, below, left and right to him in a cyclical sense. Please note: <number of rows> multiplied by <number of columns> must be equal to the number of particles! (for example if there are 36 particles then one option is: "neighborhood grid 4 9")';

		self::$command_mapping['function']='<function type>';
		self::$all_options['<function type>']=array(array('<direct function type>'),array('<operation>','<function type>'),array('reduce', '<reduce type>','<specific function type>'),array('combine','<combination>','<function type>','<function type>'));
		self::$command_explanation['function'] = 'This options specifies the objective function which has to be minimized by the PSO. A <function type> is some specification of an evaluation which can be executed on a position vector and produces a single result value.';
		self::$command_explanation['<function type>'] = 'This options specifies the objective function which has to be minimized by the PSO. A <function type> is some specification of an evaluation which can be executed on a position vector and produces a single result value.';
		self::$command_explanation['<function type>' . implode("_", array('<direct function type>'))] = 'A <direct function type> describes a specific function directly. The solution vectors are evaluated to single values. Examples are standard functions (Sphere, Rastrigin, Rosenbrock, etc.) or constant functions.';
		self::$command_explanation['<function type>' . implode("_", array('<operation>','<function type>'))] = 'An <operation> is applied to the result delivered by the connected <function type> object. Examples for operations are the logarithm, the square root or trigonometric functions.';
		self::$command_explanation['<function type>' . implode("_", array('reduce', '<reduce type>','<specific function type>'))] = 'The reduce operator reduces the data vector received from some specific function to a single number. Some kind of aggregation can be done, e.g., sums or products. A <specific function type> calculates a vector of values by the given solution vector.';
		self::$command_explanation['<function type>' . implode("_", array('combine','<combination>','<function type>','<function type>'))] = 'The combine command "combine <combination> <function type> <function type>" can be used to evaluate complex functions. You can use add (+), subtract (-), multiply (*), divide (/), minimum (min) and maximum (max) as possible combinations to combine various functions. Each combination is executed element wise. The option is described by a prefix notation for the evaluation which uniquely specifies the order of execution.';
		self::$all_options['<direct function type>']= array(array('standard', '<standard function>'),array('constant', '<constant value>'));
		self::$command_default_value['<direct function type>']=array('standard', '<standard function>');
		self::$command_explanation['<direct function type>'] = 'A <direct function type> describes a specific function directly. The solution vectors are evaluated to single values. Examples are standard functions (Sphere, Rastrigin, Rosenbrock, etc.) or constant functions.';
		self::$command_explanation['<direct function type>' . implode("_", array('standard', '<standard function>'))] = 'This option offers the chance to use a variety of pre implemented functions (Sphere, Rastrigin, Rosenbrock, etc.).';
		self::$command_explanation['<direct function type>' . implode("_", array('constant', '<constant value>'))] = '"constant <constant value>" represents a function, which always evaluates to the same specified value.';

		self::$all_options['<standard function>'] =array(array('sphere'), array('scaledsphere'), array('scaledsphere2', '<scale>'), array('scaledsphererand','<rng description>'), array('scaledandhadamardrotatedshpere'), array('scaledandhadamardrotatedshpere2', '<scale>'), array('monosphere'), array('schwefel'), array('schwefel2'), array('rosenbrock'), array('movedrosenbrock'), array('rastrigin'), array('randomposdef', '<rng description>'), array('diagonal', '<scale>'), array('norm1'), array('normoo'), array('norm2'), array('norm4'), array('norm8'), array('norm2pk', '<positive int exponent>'), array('norm1pl2pmk', '<positive int exponent>'), array('sphereplus'), array('inclinedplane'), array('inclinedplaneasym'), array('twoCupsFunction'), array('sortFunction'), array('singleDifferentDirection', '<single dimension power>', '<remaining dimension power>', '<direction mode>'), array('testing'));

		self::$command_explanation['<standard function>' . implode("_", array('sphere'))] = '"sphere" is the well know sphere function Sphere(x) = sum ((x[i])^2).';
		self::$command_explanation['<standard function>' . implode("_", array('scaledsphere'))] = '"scaledsphere" is a scaled version of the Sphere function: scaledsphere(x) = sum ((10^6)^(i/(D-1))(x[i]^2)).';
		self::$command_explanation['<standard function>' . implode("_", array('scaledsphere2', '<scale>'))] = '"scaledsphere2 <scale>" is the same but the user can specify the scale as floating point number: scaledsphere2 <scale> (x) = sum ((<scale>)^(i/(D-1))(x[i]^2)).';
		self::$command_explanation['<standard function>' . implode("_", array('scaledsphererand','<rng description>'))] = '"scaledsphererand <rng description>" is a randomized version with randomly generated scales per dimension: scaledsphererand <random number generator description> (x) = sum (r_i(x[i]^2)) where r_i are once randomly generated non negative integer numbers with given RNG.';
		self::$command_explanation['<standard function>' . implode("_", array('scaledandhadamardrotatedshpere'))] = 'If the number of dimensions D = 2^d is a power of two then "scaledandhadamardrotatedshpere" is a "scaledsphere" with Hadamard rotated input x.';
		self::$command_explanation['<standard function>' . implode("_", array('scaledandhadamardrotatedshpere2', '<scale>'))] = '"scaledandhadamardrotatedshpere2 <scale>" is analogously to "scaledandhadamardrotatedshpere" but with user defined scale as floating point number.';
		self::$command_explanation['<standard function>' . implode("_", array('monosphere'))] = 'monosphere (x) = (x[0])^2.';
		self::$command_explanation['<standard function>' . implode("_", array('schwefel'))] = 'schwefel (x) = sum_{i=0..D-1} ((sum_{j=0..i} x[j])^2).';
		self::$command_explanation['<standard function>' . implode("_", array('schwefel2'))] = '"schwefel2" is also known as Schwefel function in the literature. schwefel2 (x) = sum_{i=0..D-1} (-x[i] * sin(sqrt(abs(x[i])))).';
		self::$command_explanation['<standard function>' . implode("_", array('rosenbrock'))] = 'rosenbrock (x) = sum{i=0..D-2} (100 * (x[i+1] - x[i] * x[i])^2 + (1 - x[i])^2.';
		self::$command_explanation['<standard function>' . implode("_", array('movedrosenbrock'))] = '"movedrosenbrock" is a moved version of the rosenbrock function such that the optimum is at the origin. movedrosenbrock (x) =rosenbrock([(x[0])+1,(x[1])+1,....]).';
		self::$command_explanation['<standard function>' . implode("_", array('rastrigin'))] = 'rastrigin (x) = sum{i=0..D-1} (10 + x[i]^2 - 10 * cos(2 * PI * x[i])).';
		self::$command_explanation['<standard function>' . implode("_", array('randomposdef', '<rng description>'))] = 'randomposdef <rng description> (x) = Transpose(x) * A * x with a once randomly generated positive definite matrix A with given RNG.';
		self::$command_explanation['<standard function>' . implode("_", array('diagonal', '<scale>'))] = 'diagonal <scale> (x) = (sphere (x)) + <scale> * (sum(x[i]))^2.';
		self::$command_explanation['<standard function>' . implode("_", array('norm1'))] = '"norm1" is the Manhattan norm. norm1 (x) = sum abs(x[i]).';
		self::$command_explanation['<standard function>' . implode("_", array('normoo'))] = '"normoo" is the infinity norm. normoo (x) = max abs(x[i]).';
		self::$command_explanation['<standard function>' . implode("_", array('norm2'))] = '"norm2" is equivalent to the Sphere function.';
		self::$command_explanation['<standard function>' . implode("_", array('norm4'))] = 'norm4 (x) = sum ((x[i])^4).';
		self::$command_explanation['<standard function>' . implode("_", array('norm8'))] = 'norm8 (x) = sum ((x[i])^8).';
		self::$command_explanation['<standard function>' . implode("_", array('norm2pk', '<positive int exponent>'))] = '"norm2pk <positive int exponent>" is equivalent to the 2^<positive int exponent> norm. norm2pk <positive int exponent> (x) = sum ((x[i])^(2^<positive int exponent>)).';
		self::$command_explanation['<standard function>' . implode("_", array('norm1pl2pmk', '<positive int exponent>'))] = '"norm1pl2pmk <positive int exponent>" is equivalent to the (1+2^(-<positive int exponent>)) norm. norm1pl2pmk <positive int exponent> (x) = sum ((abs(x[i]))^(1+2^(-<positive int exponent>))).';
		self::$command_explanation['<standard function>' . implode("_", array('sphereplus'))] = 'sphereplus (x) = (sphere (x)) if every entry of x is non-negative, and INFINITY otherwise.';
		self::$command_explanation['<standard function>' . implode("_", array('inclinedplane'))] = 'inclinedplane (x) = -sum_{i=0..D-1} x[i].';
		self::$command_explanation['<standard function>' . implode("_", array('inclinedplaneasym'))] = 'inclinedplaneasym (x) = -sum_{i=0..D-1} (i+1)*x[d].';
		self::$command_explanation['<standard function>' . implode("_", array('twoCupsFunction'))] = 'twoCupsFunction (x) = sum_{i=0..D-1} f(x[i]), where f(y) = (y+1)^2 if y <  0 and f(y) = (y-1)^4 if y >= 0.';
		self::$command_explanation['<standard function>' . implode("_", array('sortFunction'))] = 'sortFunction (x) = sum_{0<=d_1<d_2<D}(x[d_1]>=x[d_2])';
		self::$command_explanation['<standard function>' . implode("_", array('singleDifferentDirection', '<single dimension power>', '<remaining dimension power>', '<direction mode>'))] = 'Let "dir" be the specified direction specified by the <direction mode> then singleDifferentDirection (x) is evaluated by the following sequence of instructions: proj := ((x.dir)/(dir.dir)) * dir (proj is the projection of the current position to dir); orth := x - proj (orth is the orthogonal part of the position); return (proj.proj)^<single dimension power> + (orth.orth)^<remaining dimension power>; Here (a.b) is the scalar product of the vectors a and b.';
		self::$command_explanation['<standard function>' . implode("_", array('testing'))] = 'This function is just for testing and debugging. Behavior and shape is not defined and may change during the process or after software updates.';
		self::$all_options['<positive int exponent>']= array(array('<positive integer value>'));
		self::$command_mapping['<positive int exponent>']= '<positive integer value>';
		self::$command_default_value['<positive int exponent>']='2';
		self::$all_options['<single dimension power>']= array(array('<float value>'));
		self::$command_mapping['<single dimension power>']= '<float value>';
		self::$command_default_value['<single dimension power>']='2.0';
		self::$all_options['<remaining dimension power>']= array(array('<float value>'));
		self::$command_mapping['<remaining dimension power>']= '<float value>';
		self::$command_default_value['<remaining dimension power>']='5.0';
		self::$all_options['<direction mode>'] =array(array('firstDimension'), array('diagonalDirection'), array('randomdirection', '<rng description>'));
		self::$command_explanation['<direction mode>' . implode("_", array('firstDimension'))] = 'if "<direction mode>" is set to "firstDimension" then the special direction is dir=(1,0,...,0)';
		self::$command_explanation['<direction mode>' . implode("_", array('diagonalDirection'))] = 'if "<direction mode>" is set to "diagonalDirection" then the special direction is dir=(1,1,...,1)';
		self::$command_explanation['<direction mode>' . implode("_", array('randomdirection', '<rng description>'))] = 'if "<direction mode>" is set to "randomdirection <rng description>" then the special direction is dir=(y[0],...,y[D-1]), where each y[i] is an independent normal distributed random variable with expectation 0 and variance 1 generated with the given RNG and its random double values.';
		self::$command_default_value['<direction mode>']=array('diagonalDirection');
		self::$all_options['<constant value>']= array(array('E'), array('Pi'), array('plusInfinity'), array('minusInfinity'), array('<float value>'));
		self::$command_explanation['<constant value>' . implode("_", array('E'))] = 'Euler\'s number. It is approximately 2.71828. This value is always evaluated according to the current precision.';
		self::$command_explanation['<constant value>' . implode("_", array('Pi'))] = 'The number Pi. It is approximately 3.14159. This value is always evaluated according to the current precision.';

		self::$all_options['<operation>']=array(array('sqrt'), array('log2'), array('logE'), array('abs'), array('pow', '<float exponent>'), array('exp'), array('sin'), array('cos'), array('tan'), array('arcsin'), array('arccos'), array('arctan'));
		self::$all_options['<float exponent>']= array(array('<float value>'));
		self::$command_mapping['<float exponent>']= '<float value>';
		self::$command_default_value['<float exponent>']='0.5';
		self::$command_explanation['<operation>' . implode("_", array('sqrt'))] = '"sqrt" is the square root.';
		self::$command_explanation['<operation>' . implode("_", array('log2'))] = '"log2" is the logarithm with base 2.';
		self::$command_explanation['<operation>' . implode("_", array('logE'))] = '"logE" is the natural logarithm (logarithm with base E).';
		self::$command_explanation['<operation>' . implode("_", array('abs'))] = '"abs" is the absolute value.';
		self::$command_explanation['<operation>' . implode("_", array('pow', '<float exponent>'))] = '"pow <float exponent>" is the power of the value with the specified exponent.';
		self::$command_explanation['<operation>' . implode("_", array('exp'))] = '"exp" is the exponential function.';
		self::$command_explanation['<operation>' . implode("_", array('sin'))] = '"sin" is the sine function.';
		self::$command_explanation['<operation>' . implode("_", array('cos'))] = '"cos" is the cosine function.';
		self::$command_explanation['<operation>' . implode("_", array('tan'))] = '"tan" is the tangent function.';
		self::$command_explanation['<operation>' . implode("_", array('arcsin'))] = '"arcsin" is the respective arcus-functions to the sine function (inverse trigonometric function)';
		self::$command_explanation['<operation>' . implode("_", array('arccos'))] = '"arccos" is the respective arcus-functions to the cosine function (inverse trigonometric function)';
		self::$command_explanation['<operation>' . implode("_", array('arctan'))] = '"arctan" is the respective arcus-functions to the tangent function (inverse trigonometric function)';

		self::$all_options['<reduce type>']= array(array('specific', '<element id>'), array('increasingOrderNthObject', '<element id>'), array('arithmeticAverage'), array('geometricAverage'), array('sum'), array('product'), array('maximum'), array('minimum'), array('functionEvaluation', '<function type>'));
		self::$command_explanation['<reduce type>'] = 'The reduce operator specifies how values are combined.';
		self::$command_explanation['<reduce type>' . implode("_", array('specific', '<element id>'))] = '"specific <element id>" picks only the value at some specified index. Please note that the indices are 0-indexed (Example for <function type>: Let the position of the current particle be (7,6,5) then the following functions can be applied: "reduce specific 0 identity" produces the data 7, "reduce specific 1 identity" produces the data 6 and "reduce specific 2 identity" produces the data 5; Example for <statistic type>: Let the number of particles be 2 and the number of dimensions be 3. Let the position of particle 0 be pos(0)=(1,2,3) and let the position of particle 1 be pos(1)=(5,6,7) then the following statistics can be applied: "reduce particle specific 0 position" produces the data (1,2,3), "reduce particle specific 1 position" produces the data (5,6,7), "reduce dimension specific 0 position" produces the data (1,5), "reduce dimension specific 1 position" produces the data (2,6) and "reduce dimension specific 2 position" produces the data (3,7)).';
		self::$command_explanation['<reduce type>' . implode("_", array('increasingOrderNthObject', '<element id>'))] = '"increasingOrderNthObject <element id>" picks the value at the specified position after the values, which should be reduced, are sorted. Please note that the indices are 0-indexed (Example for <function type>: Let the position of the current particle be (7,6,5) then the following functions can be applied: "reduce increasingOrderNthObject 0 identity" produces the data 5, "reduce increasingOrderNthObject 1 identity" produces the data 6 and "reduce increasingOrderNthObject 2 identity" produces the data 7; Example for <statistic type>: Let the number of particles be 2 and the number of dimensions be 3. Let the position of particle 0 be pos(0)=(7,2,9) and let the position of particle 1 be pos(1)=(6,8,1) then the following statistics can be applied: "reduce particle increasingOrderNthObject 0 position" produces the data (6,2,1), "reduce particle increasingOrderNthObject 1 position" produces the data (7,8,9), "reduce dimension increasingOrderNthObject 0 position" produces the data (2,1), "reduce dimension increasingOrderNthObject 1 position" produces the data (7,6) and "reduce dimension increasingOrderNthObject 2 position" produces the data (9,8)).';
		self::$command_explanation['<reduce type>' . implode("_", array('arithmeticAverage'))] = '"arithmeticAverage" calculates the arithmetic average of the values which should be reduced (Example for <function type>: Let the position of the current particle be (7,6,5) then the following function can be applied: "reduce arithmeticAverage identity" produces the data 6; Example for <statistic type>: Let the number of particles be 2 and the number of dimensions be 3. Let the position of particle 0 be pos(0)=(2,4,3) and let the position of particle 1 be pos(1)=(6,8,1) then the following statistics can be applied: "reduce particle arithmeticAverage position" produces the data (4,6,2) and "reduce dimension arithmeticAverage position" produces the data (3,5)).';
		self::$command_explanation['<reduce type>' . implode("_", array('geometricAverage'))] = '"geometricAverage" calculates the geometric average of the values which should be reduced (Example for <function type>: Let the position of the current particle be (6,4,9) then the following function can be applied: "reduce geometricAverage identity" produces the data 6; Example for <statistic type>: Let the number of particles be 2 and the number of dimensions be 3. Let the position of particle 0 be pos(0)=(64,1,729) and let the position of particle 1 be pos(1)=(256,4,2916) then the following statistics can be applied: "reduce particle geometricAverage position" produces the data (128,2,1458) and "reduce dimension geometricAverage position" produces the data (36,144)).';
		self::$command_explanation['<reduce type>' . implode("_", array('sum'))] = '"sum" caculates the sum of the values which should be reduced (Example for <function type>: Let the position of the current particle be (6,4,9) then the following function can be applied: "reduce sum identity" produces the data 19; Example for <statistic type>: Let the number of particles be 2 and the number of dimensions be 3. Let the position of particle 0 be pos(0)=(2,4,3) and let the position of particle 1 be pos(1)=(6,8,1) then the following statistics can be applied: "reduce particle sum position" produces the data (8,12,4) and "reduce dimension sum position" produces the data (9,15)).';
		self::$command_explanation['<reduce type>' . implode("_", array('product'))] = '"product" calculates the product of the values which should be reduced.';
		self::$command_explanation['<reduce type>' . implode("_", array('maximum'))] = '"maximum" calculates the maximal value of the values which should be reduced.';
		self::$command_explanation['<reduce type>' . implode("_", array('minimum'))] = '"minimum" calculates the minimal value of the values which should be reduced.';
		self::$command_explanation['<reduce type>' . implode("_", array('functionEvaluation', '<function type>'))] = '"functionEvaluation <function type>" interprets the values which should be reduced as input of the specified function and evaluates this function. This might fail if the number of entries does not match the specified number of dimensions, because then the input can possibly not be interpreted as valid position.';
		self::$command_explanation['<reduce type>' . implode("_", array('objectiveFunctionEvaluation'))] = '"objectiveFunctionEvaluation" interprets the values which should be reduced as input of the specified objective function and evaluates this function. This might fail if the number of entries does not match the specified number of dimensions, because then the input can possibly not be interpreted as valid position. The objective function is specified by the keyword "function".';

		self::$all_options['<element id>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<element id>']= '<nonnegative integer value>';
		self::$command_default_value['<element id>']='0';
		self::$all_options['<specific function type>']= array(array('<direct specific function type>'), array('<operation>', '<specific function type>'), array('combine', '<combination>', '<specific function type>', '<specific function type>'));
		self::$command_explanation['<specific function type>'] = 'A "<specific function type>" represents a vector of values, which is calculated with the given position. It needs to be reduced in some way to receive an usable function, which can then be evaluated.';
		self::$command_explanation['<specific function type>' . implode("_", array('<direct specific function type>'))] = 'A "<direct specific function type>" describes a specific "<specific function type>" directly.';
		self::$command_explanation['<specific function type>' . implode("_", array('<operation>', '<specific function type>'))] = 'An <operation> is applied to the result delivered by the connected <specific function type> object. Examples for operations are the logarithm, the square root or trigonometric functions.';
		self::$command_explanation['<specific function type>' . implode("_", array('combine', '<combination>', '<specific function type>', '<specific function type>'))] = 'The combine command "combine <combination> <specific function type> <specific function type>" can be used to evaluate complex functions. You can use add (+), subtract (-), multiply (*), divide (/), minimum (min) and maximum (max) as possible combinations to combine various functions. Each combination is executed element wise. The option is described by a prefix notation for the evaluation which uniquely specifies the order of execution.';

		self::$all_options['<direct specific function type>']= array(array('identity'),array('constant', '<constant value>'));
		self::$command_explanation['<direct specific function type>' . implode("_", array('identity'))] = '"identity" supplies the bare position/input which should be evaluated as output.';
		self::$command_explanation['<direct specific function type>' . implode("_", array('constant', '<constant value>'))] = '"constant <constant value>" represents a function, which always evaluates to the same specified value. The number of dimensions of the result will be equal to the number of dimensions of the search space.';
		self::$all_options['<combination>'] = array(array('+'), array('-'), array('*'), array('/'), array('min'), array('max'));

		self::$all_options['functionbounds']= array(array('<first dimension id>', '<last dimension id>', '<lower bound>', '<upper bound>'));
		self::$command_explanation['functionbounds' . 'once']='Sets the lower and upper bound of the bounding box for the function to the given values for all dimensions form <first dimension id> to <last dimension id>. Relevant for initialization of the particles (if "initializationinformation" with subcommands "bounds", "centerandrange" and "randomcenterandrange" is not used) and it is relevant for bound handling and for the option "functionInfinityOutside". For standard functions the usual function bounds are implemented, but for all other functions or functions consisting of more than just a standard function the function bounds are set to [-100.0, 100.0] in all dimensions. If any other function bound is intended, then it need to be specified! Please note that dimension indices are 0-indexed and the ranges are inclusive bounds.';
		
		
		
		self::$all_options['positionAndVelocityUpdater']=array(array('default'), array('orientationChange', '<delta>'), array('delta', '<delta>'), array('delta', '<delta>', '<gamma>'), array('deltatcs', '<delta>'), array('deltatcs', '<delta>', '<gamma>'), array('dimensionindependent', '<reduction scale>', '<max directions>'), array('testing'));
		self::$command_default_value['positionAndVelocityUpdater']=array('default');
		self::$command_explanation['positionAndVelocityUpdater']='Set position and velocity update strategy.';
		self::$command_explanation['positionAndVelocityUpdater' . implode("_", array('default'))]='"positionAndVelocityUpdater default" means just that the well known movement equations are used in their standard form.';
		self::$command_explanation['positionAndVelocityUpdater' . implode("_", array('orientationChange', '<delta>'))]='"positionAndVelocityUpdater orientationChange <delta>" means the following: Random values of the standard movement equations are calculated in an orthogonal transformed space. Let p[i] = log_2 (abs(v[i]) + abs(x[i] - l[i]) + abs(x[i] - g[i])), where x[i], v[i], l[i] and g[i] are the position, velocity, local attractor and global attractor of "direction" i at the current iteration and particle. If in current orientation the minimal and maximal value of p[i] differ more than "<delta>" then the lowest and largest directions are rotated with a hadamard transformation. Beside that change the default behaviour is used.';
		self::$command_explanation['positionAndVelocityUpdater' . implode("_", array('delta', '<delta>'))]='"positionAndVelocityUpdater delta <delta>" means that if for a particle and all dimensions (abs(x[i]-g[i]) + abs(v[i])) < (<delta>) then the velocity is initialized randomly in [-<delta>, <delta>] for all dimensions. Otherwise default behavior is used. This variant is described in the paper "Particle Swarm Optimization Almost Surely Finds Local Optima" by M. Schmitt and R. Wanka published in Proceedings of GECCO 2013.';
		self::$command_explanation['positionAndVelocityUpdater' . implode("_", array('delta', '<delta>', '<gamma>'))]='"positionAndVelocityUpdater delta <delta> <gamma>" means the same but with limit and scale <delta>*(<gamma>^k) where k is the number of times the random initialization of the velocity is applied already. If <gamma> equals 1.0 then this updater performs equal to the updater with only one parameter.';
		self::$command_explanation['positionAndVelocityUpdater' . implode("_", array('deltatcs', '<delta>'))]='"positionAndVelocityUpdater deltatcs <delta>" means that if in the situation of an update for position and velocity for a particle p the new velocity is initialized randomly in [-<delta>, <delta>] in those dimensions, where for ALL particles the following condition is true in that dimension: (abs(x[i]-g[i]) + abs(v[i])) < (<delta>). For all other dimensions default behavior is used. This variant is described in the paper "Particle Swarm Optimization Almost Surely Finds Local Optima" by M. Schmitt and R. Wanka published in Journal TCS 2014.';
		self::$command_explanation['positionAndVelocityUpdater' . implode("_", array('deltatcs', '<delta>', '<gamma>'))]='"positionAndVelocityUpdater deltatcs <delta> <gamma>" means the same but with limit and scale <delta>*(<gamma>^k) where k is the number of times the random initialization of the velocity is applied already. If <gamma> equals 1.0 then this updater performs equal to the updater with only one parameter.';
		self::$command_explanation['positionAndVelocityUpdater' . implode("_", array('dimensionindependent', '<reduction scale>', '<max directions>'))]='"positionAndVelocityUpdater dimensionindependent <reduction> <max_directions>" means that for each update is performed in at most <max_directions> directions. Each update direction is calculated orthogonally to the previous directions and is performed in main movement direction plus some random direction scaled by <reduction>. The smaller the value of <reduction> is the greater is the portion of the main movement direction belonging to the first sampled direction. Formally: First the previous velocity is scaled by the specified swarm parameter; main movement direction = (difference vector of local and global attractor) if they are not equal and (difference vector of position and any of the equal attractors) otherwise. If this vector is a zero vector then position and attractors are equal and therefore there is no vector to local/global attractor, which can be scaled randomly. The directions are sampled iteratively: used_directions := {}; for d from 1 to <max_directions> - 1 do { random_direction := uniformly at random choose a direction orthogonal to used_directions; remaining_main_direction := part of main movement direction which is orthogonal to used_directions; normalize both directions; next_direction = random_direction * <reduction> + remaining_main_direction; // The smaller the value of <reduction> is the more of the remaining_main_direction will be covered by the next direction; perform standard PSO velocity update with local/global attractor in this direction; add next_direction to used_directions;} end for; calculate orthogonal part of difference vector to attractors; perform standard PSO velocity update in these directions;';
		self::$command_explanation['positionAndVelocityUpdater' . implode("_", array('testing'))]='The "positionAndVelocityUpdater testing" is just for testing and debugging. Behavior is not defined and may change during the process or after software updates.';
		self::$all_options['<delta>']= array(array('<float value>'));
		self::$command_mapping['<delta>']= '<float value>';
		self::$command_default_value['<delta>']='1e-7';
		self::$all_options['<gamma>']= array(array('<float value>'));
		self::$command_mapping['<gamma>']= '<float value>';
		self::$command_default_value['<gamma>']='0.1';
		self::$all_options['<reduction scale>']= array(array('<float value>'));
		self::$command_mapping['<reduction scale>']= '<float value>';
		self::$command_default_value['<reduction scale>']='0.1';
		self::$all_options['<max directions>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<max directions>']= '<nonnegative integer value>';
		self::$command_default_value['<max directions>']='2';

		self::$all_options['boundhandling']=array( array('noBounds'), array('absorption', '<adjust dimensions>'), array('randomforth', '<adjust dimensions>'), array('random' , 'resetAll'), array('random' ,'resetViolated'), array('hyperbolic'), array('nearest'), array('reflect'), array('torus') );
		self::$command_default_value['boundhandling']= array('noBounds');
		self::$command_explanation['boundhandling']='Set the bound handling strategy. The chosen strategy will be applied to handle/avoid positions outside the search space (outside the function bounds).';
		self::$all_options['<adjust dimensions>']=array(array('markAllOnChange'), array('markAllOutsideBoundsWithoutChange'), array('markAllOnBoundsAfterChange'));
		self::$command_default_value['<adjust dimensions>']= array('markAllOnChange');
		self::$command_explanation['boundhandling' . implode("_", array('noBounds'))]='With bound handling "noBounds" it is allowed that the new position is outside of the function bounds. No change of position or velocity happens in that case. In addition to that bound handling strategy with command "functionInfinityOutside" it can be specified whether all values outside of the function bounds are calculated with the specified function or are directly set to +infinity.';
		self::$command_explanation['boundhandling' . implode("_", array('absorption', '<adjust dimensions>'))]='With bound handling "absorption" the new position will be the old position + velocity if this lies within the bounds and old position + scale * velocity otherwise, where scale is the largest value in [0.0, 1.0] such that the new position lies within or on the function bounds. "<adjust dimensions>" specifies which dimensions are marked as to be adjusted with the velocity adjustment.';
		self::$command_explanation['boundhandling' . implode("_", array('randomforth', '<adjust dimensions>'))]='With bound handling "randomforth" the new position will be the old position + velocity if this lies within the bounds and old position + randval * scale * velocity otherwise, where randval is a random value in the interval [0, 1] (uniformly at random for each application) and scale is the largest value in [0.0, 1.0] such that the new position lies within or on the function bounds. "<adjust dimensions>" specifies which dimensions are marked as to be adjusted with the velocity adjustment.';
		self::$command_explanation['boundhandling' . implode("_", array('random' , 'resetAll'))]='With bound handling "random" the position is resampled uniformly inside the function bounds. With additional option "random resetAll" all dimensions are resampled.';
		self::$command_explanation['boundhandling' . implode("_", array('random' ,'resetViolated'))]='With bound handling "random" the position is resampled uniformly inside the function bounds. With additional option "random resetViolated" only for dimensions with violations resampling is applied.';
		self::$command_explanation['boundhandling' . implode("_", array('hyperbolic'))]='With bound handling "hyperbolic" the velocities are adjusted each time. For each particle and for each dimension the following is applied: If velocity is positive then it is multiplied by 1 / ( 1 + abs( velocity / ( function upper bound - position ) ) ) else it is multiplied by 1 / ( 1 + abs( velocity / ( position - function lower bound ) ) ) WARNING: Any specification of a velocity adjustment is ignored with hyperbolic, because it already adjusts velocity.';
		self::$command_explanation['boundhandling' . implode("_", array('nearest'))]='With bound handling "nearest" the position is adjusted to the nearest position inside or on the function bounds.';
		self::$command_explanation['boundhandling' . implode("_", array('reflect'))]='With bound handling "reflect" the movement is reflected at the function boundaries (also known as mirror).';
		self::$command_explanation['boundhandling' . implode("_", array('torus'))]='With bound handling "torus" the movement is wrapped around the function bounds. For the movement equations the directions to the attractors are calculated either directly or through the function bounds depending on which of the possible directions is shorter. I. e. let X = 0.8 , L = 0.1 and G = 0.6 be position, local attractor and global attractor in a single dimension and let 0 be the lower function bound and 1 be the upper function bound then the direction to the local attractor "(L-X)" is 0.3 and the direction to the global attractor is -0.2. The bound handling "bounded mirror" can not be stated directly because mirroring a function is not possible with functionality of bound handling. Instead it can be described by the boundhandling torus and manually mirroring the function. Mirroring the function can be easily achieved. Let <function description> be the <function type> which should be processed and let <lb> and <ub> be the lower and upper function bounds. Then you can use the following commands for mirroring:"function reduce functionEvaluation <function description> combine - constant <ub> abs identity" and "functionbounds 0 (<number of dimensions> - 1) (<lb> - <ub>) (<ub> - <lb>)", where expressions in round brackets "(" ")" need to be calculated by hand. With this commands the function is mirrored to the positive direction. For mirroring to the negative direction you can use "function reduce functionEvaluation <function description> combine + constant <lb> abs identity" and "functionbounds 0 (<number of dimensions> - 1) (<lb> - <ub>) (<ub> - <lb>)".';
		self::$command_explanation['<adjust dimensions>']='"<adjust dimensions>" specifies which dimensions are marked as to be adjusted with the velocity adjustment.';
		self::$command_explanation['<adjust dimensions>' . implode("_", array('markAllOnChange'))]='"markAllOnChange" specifies that all dimensions are marked if any scaling is necessary.';
		self::$command_explanation['<adjust dimensions>' . implode("_", array('markAllOutsideBoundsWithoutChange'))]='"markAllOutsideBoundsWithoutChange" specifies that all dimensions are marked where the new position would have been outside of the bounds without scaling.';
		self::$command_explanation['<adjust dimensions>' . implode("_", array('markAllOnBoundsAfterChange'))]='"markAllOnBoundsAfterChange" specifies that all dimensions are marked where the scaled new position lies on the boundary.';

		self::$all_options['functionBehaviorOutsideOfBounds']=array( array('normal'), array('infinity'), array('periodic'));
		self::$command_default_value['functionBehaviorOutsideOfBounds']= array('infinity');
		self::$command_explanation['functionBehaviorOutsideOfBounds']='This command decides how the function value is set if the position is outside of the function bounds.';
		self::$command_explanation['functionBehaviorOutsideOfBounds' . implode("_", array('normal'))]='The option "normal" tries to evaluate the function with specified evaluation even if the position is outside of the function bounds.';
		self::$command_explanation['functionBehaviorOutsideOfBounds' . implode("_", array('infinity'))]='The option "infinity" sets the value to +infinity if the objective function is evaluated on positions outside of the function bounds.';
		self::$command_explanation['functionBehaviorOutsideOfBounds' . implode("_", array('periodic'))]='If the position is outside of the function bounds then the option "periodic" moves the position with stepsize according to the distance from lower to upper function bound inside the search space and then evaluates the function. I.e. Let LB UB and X be lower function bound, upper function bound and position in current dimension. Then the evaluated position is (X - floor((X - LB) / (UB - LB)) * (UB - LB)). This is applied to all dimensions, where the position is outside of the function bounds.';

		self::$all_options['velocityAdjustment']=array( array('none'), array('zero'), array('mirror'), array('deterministicBack', '<lambda>'), array('randomBack'), array('adjust'));
		self::$command_default_value['velocityAdjustment']= array('none');
		self::$command_explanation['velocityAdjustment']='Set the strategy how the velocity is adjusted if the respective dimension is marked by the bound handling strategy. The respective velocity adjustment is applied after the bound handling strategy and it is applied to all dimensions which are marked by the bound handling strategy as adjusted.';
		self::$command_explanation['velocityAdjustment' . implode("_", array('none'))]='Velocity will not be adjusted.';
		self::$command_explanation['velocityAdjustment' . implode("_", array('zero'))]='Velocity of adjusted dimensions will be set to zero.';
		self::$command_explanation['velocityAdjustment' . implode("_", array('mirror'))]='Velocity of adjusted dimensions will be multiplied by -1.';
		self::$command_explanation['velocityAdjustment' . implode("_", array('deterministicBack', '<lambda>'))]='Velocity of adjusted dimensions will be multiplied by -<lambda>. <lambda> should be a floating point value. The value is most likely positive. A typical value for <lambda> is 0.5.';
		self::$command_explanation['velocityAdjustment' . implode("_", array('randomBack'))]='Same as deterministicBack but <lambda> is sampled independent and uniformly at random in [0,1].';
		self::$command_explanation['velocityAdjustment' . implode("_", array('adjust'))]='The new velocity is adjusted to the difference of the new position, which is already modified by a bound handling mechanism, and the old position.';
		self::$all_options['<lambda>']= array(array('<float value>'));
		self::$command_mapping['<lambda>']= '<float value>';
		self::$command_default_value['<lambda>']='0.5';

		self::$all_options['showStatistics']=array( array('<start step>', '<end step>', '<interval>'));
		self::$command_explanation['showStatistics' . 'once']='Set at which iterations the statistics should be stored. If more such times are specified all will be regarded. Statistics will be stored for all iterations t such that <start step> <= t <= <end step> and t = <start step> + i * <interval> for some integer i. For example: The commands "showStatistics 100 1222111222 100" and "showStatistics 10000 10100 1" together will show statistics each 100 steps (for a very long period) AND each step between 10000 and 10100.';
		self::$all_options['<interval>']= array(array('<positive integer value>'));
		self::$command_mapping['<interval>']= '<positive integer value>';
		self::$command_default_value['<interval>']='10';
		self::$all_options['<start step>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<start step>']= '<nonnegative integer value>';
		self::$command_default_value['<start step>']='0';
		self::$all_options['<end step>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<end step>']= '<nonnegative integer value>';
		self::$command_default_value['<end step>']='1000';

		self::$all_options['showStatistic']=array( array('<statistic type>'));
		self::$all_options['showNamedStatistic']=array( array('<statistic name>', '<statistic type>'));
		self::$all_options['<statistic name>']= array(array('<alphanumeric value>'));
		self::$command_mapping['<statistic name>']= '<alphanumeric value>';
		self::$command_default_value['<statistic name>']='statisticName';
		self::$command_explanation['showStatistic' . 'once']='This command can specify statistics that can be stored to analyze the PSO. Examples are the position of particles, function values and many more.';
		self::$command_explanation['showNamedStatistic' . 'once']='This command can specify statistics that can be stored to analyze the PSO. Examples are the position of particles, function values and many more. Additionally a name of this statistic can be defined. This name has to be unique and can contain characters A-Z, a-z and 0-9. The name is used as suffix for the file containing the data of this statistic.';
		self::$all_options['<statistic type>'] = array(array('<direct statistic type>'), array('<operation>', '<statistic type>'), array('<reduce operator>', '<specific statistic type>'), array('combine', '<combination>', '<statistic type>', '<statistic type>'));
		self::$command_explanation['<statistic name>']='You can specify the name of the statistic, which specifies the final part of the respective filename. Please do not use any whitespace characters. Furthermore only the letters A-Z, a-z and 0-9 are allowed.';
		self::$command_explanation['<statistic type>']='A <statistic type> is some specification of an evaluation which can be executed at the specified iterations (specified by command "showStatistics"). In general it produces at each evaluation a list of numbers which is written to the respective statistics file.';
		self::$command_explanation['<direct statistic type>'] = self::$command_explanation['<statistic type>' . implode("_", array('<direct statistic type>'))]='A <direct statistic type> describes a specific statistic directly. Examples are the global best position or the objective function value at the global best position.';
		self::$command_explanation['<statistic type>' . implode("_", array('<operation>', '<statistic type>'))]='An <operation> is applied to the result delivered by the connected <statistic type> object. Examples for operations are the logarithm, the square root or trigonometric functions.';
		self::$command_explanation['<statistic type>' . implode("_", array('<reduce operator>', '<specific statistic type>'))]='A <specific statistic type> produces a two-dimensional statistic (There is a value for each particle and dimension - An example would be the position of all particles). The reduce operator reduces the data received for each particle and dimension to some list of numbers. Some kind of aggregation can be applied over the particles or over the dimensions (Examples are the sum or the choice of a specific particle/dimension).';
		self::$command_explanation['<statistic type>' . implode("_", array('combine', '<combination>', '<statistic type>', '<statistic type>'))]='The combine command "combine <combination> <statistic type> <statistic type>" can be used to evaluate complex statistics. You can use add (+), subtract (-), multiply (*), divide (/), minimum (min) and maximum (max) as possible combinations to combine various statistics. Each combination is executed element wise. The option is described by a prefix notation for the evaluation which uniquely specifies the order of execution. WARNING: Undefined values can not be compared (e.g. for minimum). Undefined values can appear for example if there is a division by zero.';

		self::$all_options['<direct statistic type>']=array(array('globalBestPosition'), array('globalBestPositionDistanceTo1DOptimum'), array('globalBestPositionFunctionEvaluation'), array('localAttractorUpdates'), array('globalAttractorUpdates'), array('precision'), array('constant', '<dimensions>', '<constant value>'));
		self::$all_options['<dimensions>']= array(array('<positive integer value>'));
		self::$command_mapping['<dimensions>']= '<positive integer value>';
		self::$command_default_value['<dimensions>']='10';
		self::$command_explanation['<direct statistic type>' . implode("_", array('globalBestPosition'))]='"globalBestPosition" represents the best position found so far by the swarm.';
		self::$command_explanation['<direct statistic type>' . implode("_", array('globalBestPositionDistanceTo1DOptimum'))]='"globalBestPositionDistanceTo1DOptimum" represents the distance of the global best position to the nearest local optimum if only a single dimension is variable.';
		self::$command_explanation['<direct statistic type>' . implode("_", array('globalBestPositionFunctionEvaluation'))]='"globalBestPositionFunctionEvaluation" represents the function value of the specified objective function where the input position is the best position found so far.';
		self::$command_explanation['<direct statistic type>' . implode("_", array('localAttractorUpdates'))]='"localAttractorUpdates" counts for each particle the number of changes of its local attractor since the beginning.';
		self::$command_explanation['<direct statistic type>' . implode("_", array('globalAttractorUpdates'))]='"globalAttractorUpdates" counts for each particle the number of changes of a global attractor caused by that particle since the beginning.';
		self::$command_explanation['<direct statistic type>' . implode("_", array('precision'))]='"precision" represents the current precision in bits, which is used for all calculations.';
		self::$command_explanation['<direct statistic type>' . implode("_", array('constant', '<dimensions>', '<constant value>'))]='"constant <dimensions> <constant value>" represents a statistic, which always evaluates to the same specified value. The number of dimensions needs to be specified, because the generated vector needs to have the correct number of entries if it should be combined with other statistics.';
		self::$all_options['<reduce operator>'] = array(array('reduce', 'particle', '<reduce type>'), array('reduce', 'dimension', '<reduce type>'));
		self::$command_explanation['<reduce operator>'] = 'The reduce operator reduces the data received for each particle and dimension to some list of numbers. Some kind of aggregation can be applied over the particles or over the dimensions (Examples are the sum or the choice of a specific particle/dimension).';
		self::$command_explanation['<reduce operator>' . implode("_",array('reduce', 'particle', '<reduce type>'))] = '"reduce particle <reduce type>" reduces the data over the particles and produces one value for each dimension. (Example: Let the number of particles be 2 and the number of dimensions be 3. Let the position of particle 0 be pos(0)=(1,2,3) and let the position of particle 1 be pos(1)=(5,6,7) then the following statistics can be applied: "reduce particle specific 0 position" produces the data (1,2,3) and "reduce particle specific 1 position" produces the data (5,6,7)).';
		self::$command_explanation['<reduce operator>' . implode("_",array('reduce', 'dimension', '<reduce type>'))] = '"reduce dimension <reduce type>" reduces the data over the dimensions and produces one value for each particle. (Example: Let the number of particles be 2 and the number of dimensions be 3. Let the position of particle 0 be pos(0)=(1,2,3) and let the position of particle 1 be pos(1)=(5,6,7) then the following statistics can be applied: "reduce dimension specific 0 position" produces the data (1,5), "reduce dimension specific 1 position" produces the data (2,6) and "reduce dimension specific 2 position" produces the data (3,7)).';

		self::$all_options['<specific statistic type>']=array( array('<direct specific statistic type>'), array('<operation>', '<specific statistic type>'), array('combine', '<combination>', '<specific statistic type>', '<specific statistic type>'));
		self::$command_explanation['<specific statistic type>'] ='A <specific statistic type> produces a two-dimensional statistic (There is a value for each particle and dimension - An example would be the position of all particles). It needs to be reduced in some way to receive a list of numbers, which can then be displayed in a data file.';
		self::$command_explanation['<direct specific statistic type>'] = self::$command_explanation['<specific statistic type>' . implode("_", array('<direct specific statistic type>') )]='A "<direct specific statistic type>" describes a specific "<specific statistic type>" directly. Examples are the positions or the velocities of all particles.';
		self::$command_explanation['<specific statistic type>' . implode("_", array('<operation>', '<specific statistic type>') )]='An <operation> is applied to the result delivered by the connected <specific statistic type> object. Examples for operations are the logarithm, the square root or trigonometric functions.';
		self::$command_explanation['<specific statistic type>' . implode("_", array('combine', '<combination>', '<specific statistic type>', '<specific statistic type>') )]='The combine command "combine <combination> <specific statistic type> <specific statistic type>" can be used to evaluate complex statistics. You can use add (+), subtract (-), multiply (*), divide (/), minimum (min) and maximum (max) as possible combinations to combine various statistics. Each combination is executed element wise. The option is described by a prefix notation for the evaluation which uniquely specifies the order of execution. WARNING: Undefined values can not be compared (e.g. for minimum). Undefined values can appear for example if there is a division by zero.';

		self::$all_options['<direct specific statistic type>'] = array(array('position'), array('velocity'), array('localAttractor'), array('globalAttractor'), array('functionDifference'), array('absVelocityPlusDistToGlobalAttractor', '<scale>'), array('sqrtAbsVelocityPlusSqrtDistToGlobalAttractor', '<scale>'), array('constant', '<constant value>'), array('deltaUpdateCounters'));
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('position') )]='"position" represents the positions for each particle.';
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('velocity') )]='"velocity" represents the velocities for each particle.';
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('localAttractor') )]='"localAttractor" represents the local attractor for each particle.';
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('globalAttractor') )]='"globalAttractor" represents the global attractor for each particle. Please note that they can be different depending on the neighborhood topology.';
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('functionDifference') )]='"functionDifference" means abs(f(x)-f(x\')), where x is the position and x\'[dim] = x[dim] + v[dim] for the measured dimension and x\'[dim] = x[dim] for all other dimensions, where v is the velocity.';
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('absVelocityPlusDistToGlobalAttractor', '<scale>') )]= '"absVelocityPlusDistToGlobalAttractor <scale>" means <scale> * abs(v[dim]) + abs(x[dim]-g[dim]), where x is the position, v is the velocity, g is the global attractor and dim is the measured dimension.';
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('sqrtAbsVelocityPlusSqrtDistToGlobalAttractor', '<scale>') )]= '"sqrtAbsVelocityPlusSqrtDistToGlobalAttractor <scale>" means <scale> * sqrt( abs( v[dim] ) ) + sqrt( abs( x[dim] - g[dim] ) ), where x is the position, v is the velocity, g is the global attractor and dim is the measured dimension.';
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('constant', '<constant value>') )]= '"constant <constant value>" represents a statistic, which always evaluates to the same specified value. This specific statistic can be combined with other specific statistics. It has a value for each particle and each dimension.';
		self::$command_explanation['<direct specific statistic type>' . implode("_", array('deltaUpdateCounters') )]= '"deltaUpdateCounters" represents a statistic, which counts the number of delta updates for each particle and dimension. For obvious reasons if the position and velocity updater is not a delta updater (delta or deltatcs) then this statistic evaluates to zero.';

		self::$all_options['outputPrecision'] = array(array('<output precision in digits>'), array('-1'));
		self::$command_default_value['outputPrecision'] = array('-1');
		self::$command_explanation['outputPrecision'] = 'Set the precision (in digits) of numbers in the produced output (in general for all floating point numbers in statistical files). If the output precision is set to -1 then the full precision of the internal floating point values are displayed.';
		self::$all_options['<output precision in digits>']= array(array('<positive integer value>'));
		self::$command_mapping['<output precision in digits>']= '<positive integer value>';
		self::$command_default_value['<output precision in digits>']='10';

		self::$all_options['runcheck'] = array(array('<runcheck configuration file>'));
		self::$command_default_value['runcheck'] = array('<runcheck configuration file>');
		self::$command_explanation['runcheck' . 'once'] = 'Set the file name for configuration file which specifies when / whether the program is allowed to run. The program parses this file repeatedly. Therefore changes are recognized on runtime. If the program recognizes that it is not allowed to run then it stores its current data and terminates normally ensuring that no data loss appears. This can be used to terminate current PSO runs and restart them later. See sample file runcheck.conf for available possibilities. Allowed characters are 0-9, a-z, A-Z and the four characters "./_-". Please note that this command is optional.';
		self::$all_options['<runcheck configuration file>']= array(array('<file name>'));
		self::$command_mapping['<runcheck configuration file>']= '<file name>';
		self::$command_default_value['<runcheck configuration file>']='guideline_configuration_files/runcheck.conf';

		self::$command_mapping['preservebackup']= '<iteration index>';
		self::$command_explanation['preservebackup' . 'once'] = 'Set special iteration index/step, where a backup of the PSO should be preserved. For example use "preservebackup 0" to receive the backup directly after initialization.';
		self::$all_options['<iteration index>']= array(array('<nonnegative integer value>'));
		self::$command_mapping['<iteration index>']= '<nonnegative integer value>';
		self::$command_default_value['<iteration index>']='0';

		self::$all_options['debugswarm']= array(array(''));
		self::$command_explanation['debugswarm' . 'once']='For debugging and observation of the swarm this option can be used. This command activates the <debugswarm>-mode (generation of pictures which visualize the current state of the swarm and further output on terminal). The program generates a folder according to the current file prefix. Execute the small script in this folder to generate the image files for each visualized step. The prefix of the files are the referencing steps. WARNING 1: "gnuplot" has to be installed to create pictures by the created script. WARNING 2: It might be possible that you can not easily kill the PSO_program task with Ctrl+C in the terminal. You need to kill the process either by your specified runcheck file or by typing "killall PSO_program" in another terminal (this variant kills ALL(!!) instances of PSO_program) or you can use the programs "top" or "htop" to kill the respective process. WARNING 3: It might be possible that this option causes large memory usage on hard disk.';
		self::$command_explanation['debugswarm']='Please note that this option has no parameter.';

		self::$command_mapping['debugswarmShowGlobalAttractorTrajectory']= '<nonnegative integer value>';
		self::$command_default_value['debugswarmShowGlobalAttractorTrajectory']= 20;
		self::$command_explanation['debugswarmShowGlobalAttractorTrajectory']='This command specifies how many of the recent global best found positions will be visualized in the generated images as footprints.';

		self::$command_mapping['debugswarmImageGenerationFrequency']= '<nonnegative integer value>';
		self::$command_default_value['debugswarmImageGenerationFrequency']= 10;
		self::$command_explanation['debugswarmImageGenerationFrequency']='This command specifies how often the image will be generated. If the specified value is zero then no images will be generated. If the specified value "n" is positive each n-th iteration/step an image will be generated.';

		self::$command_mapping['debugswarmShowGlobalAttractor']= '<boolean value>';
		self::$command_default_value['debugswarmShowGlobalAttractor']= true;
		self::$command_explanation['debugswarmShowGlobalAttractor']='This command specifies whether the value and position of the current global best found position should be displayed on the terminal at each iteration.';

		self::$command_mapping['debugswarmXresolution']= '<positive integer value>';
		self::$command_default_value['debugswarmXresolution']= 2560;
		self::$command_explanation['debugswarmXresolution']='The following command specifies the resolution of the generated images. Please specify the width of the generated image in pixels.';
		self::$command_mapping['debugswarmYresolution']= '<positive integer value>';
		self::$command_default_value['debugswarmYresolution']= 1440;
		self::$command_explanation['debugswarmYresolution']='The following command specifies the resolution of the generated images. Please specify the height of the generated image in pixels.';

		self::$command_mapping['debugswarmGnuplotTerminal']= '<alphanumeric value>';
		self::$command_default_value['debugswarmGnuplotTerminal']= 'svg';
		self::$command_explanation['debugswarmGnuplotTerminal']='This command specifies which terminal should be used in gnuplot. The available terminals depend on your system. Usually available are terminals such as "svg", "png", "pngcairo" and many more.';
		self::$command_mapping['debugswarmOutput']= '<alphanumeric value>';
		self::$command_default_value['debugswarmOutput']= 'svg';
		self::$command_explanation['debugswarmOutput']='This command specifies the extension of the generated image file. The extension should match to the output file generated by the chosen gnuplot terminal.';
	}

	public static function add_statistic_options(){
		self::$all_options['<reduce type>'][]= array('objectiveFunctionEvaluation');
	}

	public static function get_options_explanation($select){
		if(!self::$initDone){
			self::init();
		}
		if(array_key_exists($select, self::$command_explanation)){
			return self::$command_explanation[$select];
		} else if(is_array($select)){
			if(count($select) == 1){
				return self::get_options_explanation($select[0]);
			}
		}
		return "";
	}
	public static function get_options($select){
		if(!self::$initDone){
			self::init();
		}
		if(array_key_exists($select, self::$all_options)){
			return self::$all_options[$select];
		} else {
			return array();
		}
	}
	public static function get_command_start($command){
		if(!self::$initDone){
			self::init();
		}
		if(array_key_exists($command, self::$command_mapping)){
			return self::$command_mapping[$command];
		}
		return $command;
	}
	public static function get_command_default($command){
		if(!self::$initDone){
			self::init();
		}
		if(array_key_exists($command, self::$command_default_value)){
			return self::$command_default_value[$command];
		}
		return "";
	}
}

function check_float($input){
	return preg_match("/\A((0|(-|)([1-9](\d*)))|(-|)[.](\d+)|(-|)(0|[1-9](\d*))[.](\d*))(|[Ee](-|)[1-9](\d*))\z/", "$input");
}

function check_positive_integer($input){
	return preg_match("/\A[1-9](\d{0,18})\z/", "$input");
}

function check_integer($input){
	return preg_match("/\A(0|(-|)[1-9](\d{0,18}))\z/", "$input");
}

function check_nonnegative64bitinteger($input){
	return preg_match("/\A(0|[1-9](\d{0,18}))\z/", "$input");
}

function check_alphanumeric($input){
	return preg_match("/\A([0-9]|[a-z]|[A-Z]){1,100}\z/", "$input");
}
function check_filename($input){
	return preg_match("/\A([0-9]|[a-z]|[A-Z]|[.]|[\/]|[_]|[-]){1,200}\z/", "$input");
}

function get_special_input($specifier, $hierarchy, $default_value, $inputtype){
	$value=$default_value;
	if(isset($_POST[$specifier])){
		$value=$_POST[$specifier];
	}
	$hiddenbutton_specifier = $specifier . '_HIDDEN_BUTTON';
	echo '<input type="submit" id="' . $hiddenbutton_specifier . '" name="' . $hiddenbutton_specifier . '" value="" hidden/>';
	$value_ok=true;
	echo '<input id="' . $specifier . '" ';
	if(strcmp("$inputtype","<float value>") == 0){
		$value_ok = check_float($value);
	} else if(strcmp("$inputtype","<integer value>") == 0){
		$value_ok = check_integer($value);
	} else if(strcmp("$inputtype","<positive integer value>") == 0){
		$value_ok = check_positive_integer($value);
	} else if(strcmp("$inputtype","<nonnegative integer value>") == 0){
		$value_ok = check_nonnegative64bitinteger($value);
	} else if(strcmp("$inputtype","<alphanumeric value>") == 0){
		$value_ok = check_alphanumeric($value);
	} else if(strcmp("$inputtype","<file name>") == 0){
		$value_ok = check_filename($value);
	} else {
		$value_ok = false;
	}
	if(!$value_ok){
		echo ' style="background-color:#FF8080;" ';
	}
	if(isset($_POST[$hiddenbutton_specifier])){
		echo ' autofocus ';
	}
	echo ' type="text" name="' . $specifier . '" value="' . htmlspecialchars($value) . '" onchange="document.getElementById(\'' . $hiddenbutton_specifier . '\').click();">';
	return array("$value", $value_ok);
}

function get_options($select){
	return ConfigurationOptions::get_options($select);
}

function get_hierarchy_string($hierarchy){
	return implode(".", $hierarchy);
}

function display_selection($prefix, $hierarchy, $current_variable, $print_inline, $default_value){
	$specifier="$prefix" . md5("$current_variable") . "_" . implode("_", $hierarchy);
	$chooseid=-2;
	if(isset($_POST[$specifier])){
		$chooseid=$_POST[$specifier];
	}
	if( $print_inline == false ){
		echo get_hierarchy_string($hierarchy);
		echo '. <label for="' . $specifier . '" ';
		$tooltip=ConfigurationOptions::get_options_explanation($current_variable);
		if($tooltip != ""){
			echo ' title="' . htmlspecialchars($tooltip) . '"';
		}
		echo '>';
		echo htmlspecialchars($current_variable);
		echo "</label>: \n";
	}
	$current_variable=ConfigurationOptions::get_command_start($current_variable);
	if("$current_variable" == "<float value>"){
		if("$default_value" == ""){
			$default_value="0.0";
		}
		return get_special_input($specifier, $hierarchy, $default_value, $current_variable);
	} elseif("$current_variable" == "<integer value>"){
		if("$default_value" == ""){
			$default_value="1";
		}
		return get_special_input($specifier, $hierarchy, $default_value, $current_variable);
	} elseif("$current_variable" == "<positive integer value>"){
		if("$default_value" == ""){
			$default_value="1";
		}
		return get_special_input($specifier, $hierarchy, $default_value, $current_variable);
	} elseif("$current_variable" == "<nonnegative integer value>"){
		if("$default_value" == ""){
			$default_value="0";
		}
		return get_special_input($specifier, $hierarchy, $default_value, $current_variable);
	} elseif("$current_variable" == "<alphanumeric value>"){
		if("$default_value" == ""){
			$default_value="text";
		}
		return get_special_input($specifier, $hierarchy, $default_value, $current_variable);
	} elseif("$current_variable" == "<file name>"){
		if("$default_value" == ""){
			$default_value="text";
		}
		return get_special_input($specifier, $hierarchy, $default_value, $current_variable);
	} else {
		$options=get_options($current_variable);
		$hiddenbutton_specifier = $specifier . '_HIDDEN_BUTTON';
		echo '<input type="submit" id="' . $hiddenbutton_specifier . '" name="' . $hiddenbutton_specifier . '" value="" hidden/>';
		$select_tooltip="Please choose an option.";
		for($i=0; $i < count($options); $i++) {
			$option=$options[$i];
			$tooltip=ConfigurationOptions::get_options_explanation("$current_variable" . implode("_", $option));
			if($tooltip != ""){
				if($i == "$chooseid" || ("$chooseid" == -2 && $option == $default_value)) {
					$select_tooltip = $tooltip;
				}	
			}
			if(count($options) == 1){
				$select_tooltip = $tooltip;
			}
		}
		echo '<select name="' . $specifier . '" ';
	   	if($select_tooltip != ""){
			echo ' title="' . htmlspecialchars($select_tooltip) . '" ';
		}
		echo ' id="' . $specifier . '" onchange="document.getElementById(\'' . $hiddenbutton_specifier . '\').click();"';
		if(isset($_POST[$hiddenbutton_specifier])){
			echo ' autofocus ';
		}
		if(count($options) == 1){
			echo ' disabled ';
		}
		echo '>';
		$selection=array();
		echo '<option value=-1>Please choose option</option>';
		echo "\n";
		for($i=0; $i < count($options); $i++) {
			$option=$options[$i];
			echo '<option value="'. $i . '" ';
			$tooltip=ConfigurationOptions::get_options_explanation("$current_variable" . implode("_", $option));
			if($tooltip != ""){
				echo ' title="' . htmlspecialchars($tooltip) . '" ';
			}
			if($i == "$chooseid" || ("$chooseid" == -2 && ($option == $default_value || count($options) == 1))) {
				$selection=$option;
				echo 'selected';
			}	
			echo '>' . htmlspecialchars(implode(" ", $option)) . '</option>';
			echo "\n";
		}
		echo '</select>';
		echo "\n";
		$hierarchy_counter=0;
		$resultarray=array("", true);
		$isfirst = true;
		for($i=0; $i < count($selection); $i++){
			$cselect=$selection[$i];
			$suboptions=get_options($cselect);
			if(count($suboptions) > 0){
				$hierarchy_counter++;
				$hierarchy_copy=$hierarchy;
				$hierarchy_copy[]=$hierarchy_counter;
				echo "<br/>\n";
				$currentresultarray=display_selection($prefix, $hierarchy_copy, $selection[$i], $print_inline, ConfigurationOptions::get_command_default($selection[$i]));
				if(is_array($currentresultarray)){
					if(count($currentresultarray) != 2){
						$resultarray[1] = false;
					} else {
						if($currentresultarray[1] != true){
							$resultarray[1] = false;
						}
						if($isfirst){
							$isfirst = false;
							$resultarray[0] =  $currentresultarray[0];
						} else {
							$resultarray[0] = $resultarray[0] . " " . $currentresultarray[0];
						}
					}
				} else {
					$resultarray[1] = false;
				}
			} else {
				if($isfirst){
					$isfirst = false;
					$resultarray[0] =  $cselect;
				} else {
					$resultarray[0] = $resultarray[0] . " " . $cselect;
				}
			}
		}
		if(count($selection) == 0){
			return array("<Please choose option>", false);
		}
		return $resultarray;
	}
}

function display_command_start($command, $hierarchy, $display_direct){
	if(!is_array($hierarchy)){
		$hierarchy=array($hierarchy);
	}
	$prefix=md5($command) . 'choice';
	$current_variable=ConfigurationOptions::get_command_start($command);
	$specifier="$prefix" . md5("$current_variable") . "_" . implode("_", $hierarchy);
	echo '<b>';
	echo get_hierarchy_string($hierarchy);
	echo '. ';
	if($display_direct){
		echo ' <label for="' . $specifier . '">';
	}
   	echo htmlspecialchars($command);
	if($display_direct){
		echo '</label>';
	}
	echo ':</b>' . "\n";
	if(!$display_direct){
		echo '<br/>' . "\n";
	}
	$tooltip=ConfigurationOptions::get_options_explanation($command);
	if(!$display_direct){
		if($tooltip != ""){
			echo htmlspecialchars($tooltip) . "<br/>\n";
		}
	}
	$result_array=display_selection($prefix, $hierarchy, $current_variable, $display_direct, ConfigurationOptions::get_command_default($command));
	if($display_direct){
		if($tooltip != ""){
			echo "<br/>\n" . htmlspecialchars($tooltip) . "\n";
		}
	}
	$result_array[0] = $command . " " . $result_array[0];
	if($result_array[1] == true){
		echo '<div class="hippsocommand-ok">';
	} else {
		echo '<div class="hippsocommand-fail">';
	}
	echo '<code>';
	echo htmlspecialchars($result_array[0]);
	echo '</code></div>';
	return $result_array;
}

function display_command($command, $hierarchy){
	return display_command_start($command, $hierarchy, false);
}

function display_command_direct($command, $hierarchy){
	return display_command_start($command, $hierarchy, true);
}

function display_command_multiple($command, $hierarchy, $max_appearance){
	if(!is_array($hierarchy)){
		$hierarchy=array($hierarchy);
	}
	echo '<b>';
	echo get_hierarchy_string($hierarchy);
	echo '. ' . htmlspecialchars($command) . ':</b><br/>' . "\n";
	$tooltip=ConfigurationOptions::get_options_explanation($command . "once");
	if($tooltip != ""){
		echo htmlspecialchars($tooltip) . "<br/>\n";
	}
	$specifier=md5($command) . "_" . implode("_", $hierarchy);
	$specifierButtonM = $specifier . 'buttonM';
	$specifierButtonP = $specifier . 'buttonP';
	$specifierNumber = $specifier . 'number';
	echo '<label for="' . $specifierNumber . '">Set the number of <b>' . htmlspecialchars($command) . '</b> commands.</label><br/>' . "\n";
	$num_commands=0;
	if(isset($_POST[$specifierNumber])){
		$postvalue=$_POST[$specifierNumber];
		if(check_integer("$postvalue")){
			$num_commands=$postvalue;
		}
	}
	$has_changed = false;
	if(isset($_POST[$specifierButtonP])){
		++$num_commands;
		$has_changed = true;
	}
	if(isset($_POST[$specifierButtonM])){
		--$num_commands;
		$has_changed = true;
	}
	if($num_commands < 0){
		$num_commands = 0;
	}
	if($max_appearance > 0 && $num_commands > $max_appearance){
		$num_commands = $max_appearance;
	}
	echo '<input type="submit" name="' . $specifierButtonM . '" value="-" ';
	if($num_commands == 0){
		echo ' disabled ';
	}
	echo '/>' . "\n";
	echo '<input type="text" id="' . $specifierNumber . '" name="' . $specifierNumber . '" value="'. $num_commands . '" readonly';
	if($has_changed){
		echo ' autofocus';
	}
    echo '/>' . "\n";
	echo '<input type="submit" name="' . $specifierButtonP . '" value="+" ';
	if($num_commands == $max_appearance){
		echo ' disabled ';
	}
	echo '/>' . "\n";
	echo '<br/>' . "\n";
	$list_result_array=array();
	for($i=1; $i <= $num_commands; $i++){
		$hierarchy_copy=$hierarchy;
		$hierarchy_copy[]=$i;
		$list_result_array[] = display_command($command, $hierarchy_copy);
	}
	return $list_result_array;
}
?>
<!DOCTYPE html>
<html class="no-js" lang="en-US">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <title>Interactive Configuration File Generation for the HiPPSO</title>
	<meta name="description" content="Language: English &nbsp; Short description: &nbsp; An interactive tool for the generation of configuration files for running the High Precision Particle Swarm Optimization (HiPPSO) framework." />
	<meta name="robots" content="index, follow" />
	<link rel="canonical" href="https://www12.informatik.uni-erlangen.de/downloads/eako/HiPPSOconfigurationFileGeneration/index.php" />

	<style>
	body {
		margin: 0;
	}

	a:link   { color: #6e7881;}
	a:visited{ color: #6e7881;}
	a:hover  { color: #3b4956;}
	a:active { color: #3b4956;}

	.header{
		background:#838c94;
		background-image: url("https://www12.informatik.uni-erlangen.de/downloads/eako/HiPPSOconfigurationFileGeneration/img/siegel-techfak-schnitt.png");
		background-repeat: no-repeat;
		background-position: right;
		height: 192px;
		margin-top: 20px;
		margin-bottom: 20px;
        color: #ffffff;
	}

	.header-inner{
        margin: auto;
        padding: 30px;
		font-family: Arial, Helvetica Neue, Helvetica, sans-serif;
        max-width: 900px;
	}

	.footer{
		margin-top: 20px;
		padding: 0px;
		background:#6e7881;
	}

	.footer-inner{
        max-width: 900px;
        margin: auto;
        padding: 30px;
        font-size: 12px;
		line-height: 16px;
		font-family: Arial, Helvetica Neue, Helvetica, sans-serif;
        color: #ffffff;
		display: grid;
		grid-gap: 20px;
	}
	.footer-inner a{
		color: #ffffff;
	}
	.footer-logo{
		grid-column: 1
	}
	.footer-address{
		grid-column: 2
	}
	.footer-meta{
		grid-column: 3
	}
    .invoice-box {
        max-width: 900px;
        margin: auto;
        padding: 30px;
        border: 1px solid #eee;
        box-shadow: 0 0 10px rgba(0, 0, 0, .15);
        font-size: 16px;
		line-height: 24px;
		font-family: Arial, Helvetica Neue, Helvetica, sans-serif;
        color: #555;
    }

	.hippsocommand-ok{
		border:2px;
		border-style:solid;
		padding: 4.5px;
		margin-top: 7px;
		margin-bottom: 7px;
		border-color: #555;
	}
	.hippsocommand-fail{
		border:2px;
		border-style:solid;
		padding: 4.5px;
		margin-top: 7px;
		margin-bottom: 7px;
		border-color: red;
		color: #ee0000;
	}
    </style>
</head>

<body>
<div class="header">
	<div class="header-inner">
		<h1>Interactive Configuration File Generation for the High Precision Particle Swarm Optimization (HiPPSO) Framework</h1>
	</div>
</div>
	<div class="invoice-box">
<h3>How to Use This Configuration File Generation Tool</h3>
In the following you can choose all available options for configuration of the
High Precision Particle Swarm Optimization
(<a href="https://github.com/alexander-rass/HiPPSO">HiPPSO</a>). You will
receive mouse-over-tooltips for almost all objects (this is especially helpful for
options in dropdown menus). Directly after each command specification you can see the
resulting command for the configuration file within a box. This box will be
colored red if the command is not specified completely or the specified data
does not conform to the expected data format (e.g., floating point numbers where
only integers are allowed). At the end you can find the complete condensed
content for the configuration file. Please note that this homepage works best
with Firefox. However, if you pretend to use chrome then do not use the Enter key
on text input fields as you will loose focus on them.
<?php

echo '<form id="submission_form" action="' . $targetHomepage .'" method="post">';


$command_list=array();
$running_index=1;
echo '<hr><h2>Objective Function Specification</h2>'."\n";
$command_list[] = display_command( "function", $running_index++);
$command_list[] = display_command_direct( "dimensions", $running_index++);
$command_list = array_merge($command_list, display_command_multiple( "functionbounds", $running_index++, -1));

echo '<hr><h2>Swarm Parameters and Swarm Configuration</h2>'."\n";
$command_list[] = display_command_direct( "particles", $running_index++);
$command_list[] = display_command_direct( "steps", $running_index++);
$command_list[] = display_command_direct( "parameterChi", $running_index++);
$command_list[] = display_command_direct( "parameterCoefficientLocalAttractor", $running_index++);
$command_list[] = display_command_direct( "parameterCoefficientGlobalAttractor", $running_index++);
$command_list[] = display_command_direct( "initializeVelocity", $running_index++);
$command_list = array_merge($command_list, display_command_multiple("initializationinformation", $running_index++, -1));
$command_list[] = display_command( "neighborhood", $running_index++);
$command_list[] = display_command( "positionAndVelocityUpdater", $running_index++);
$command_list[] = display_command( "boundhandling", $running_index++);
$command_list[] = display_command( "functionBehaviorOutsideOfBounds", $running_index++);
$command_list[] = display_command( "velocityAdjustment", $running_index++);
$command_list[] = display_command_direct( "updateGlobalAttractor", $running_index++);
$command_list[] = display_command( "srand", $running_index++);

echo '<hr><h2>Precision Configuration</h2>'."\n";
$command_list[] = display_command_direct( "initialprecision", $running_index++);
$command_list[] = display_command_direct( "precision", $running_index++);
$command_list[] = display_command_direct( "checkprecision", $running_index++);
$command_list[] = display_command_direct( "checkprecisionprobability", $running_index++);
$command_list[] = display_command( "outputPrecision", $running_index++);
	$tooltip=ConfigurationOptions::add_statistic_options();

echo '<hr><h2>Specification which Statistics are Written to Data Files</h2>'."\n";
$command_list = array_merge($command_list, display_command_multiple( "showStatistics", $running_index++, -1));
$command_list = array_merge($command_list, display_command_multiple( "showStatistic", $running_index++, -1));
$command_list = array_merge($command_list, display_command_multiple( "showNamedStatistic", $running_index++, -1));
$dbgswarm_index =$running_index++;
$dbgswarm_command = display_command_multiple( "debugswarm", $dbgswarm_index, 1);
$command_list = array_merge($command_list, $dbgswarm_command);
if(count($dbgswarm_command) == 1){
		$dbgswarm_runningindex=2;
		$command_list[] = display_command_direct( "debugswarmShowGlobalAttractorTrajectory", array($dbgswarm_index, $dbgswarm_runningindex++));
		$command_list[] = display_command_direct( "debugswarmImageGenerationFrequency", array($dbgswarm_index, $dbgswarm_runningindex++));
		$command_list[] = display_command_direct( "debugswarmShowGlobalAttractor", array($dbgswarm_index, $dbgswarm_runningindex++));
		$command_list[] = display_command_direct( "debugswarmXresolution", array($dbgswarm_index, $dbgswarm_runningindex++));
		$command_list[] = display_command_direct( "debugswarmYresolution", array($dbgswarm_index, $dbgswarm_runningindex++));
		$command_list[] = display_command_direct( "debugswarmGnuplotTerminal", array($dbgswarm_index, $dbgswarm_runningindex++));
		$command_list[] = display_command_direct( "debugswarmOutput", array($dbgswarm_index, $dbgswarm_runningindex++));
}

echo '<hr><h2>Miscellaneous Options</h2>'."\n";
$command_list[] = display_command_direct( "includeSystemTimeInFilename", $running_index++);
$command_list = array_merge($command_list, display_command_multiple("fileprefix", $running_index++, 1));
$command_list = array_merge($command_list, display_command_multiple( "runcheck", $running_index++, 1));
$command_list = array_merge($command_list, display_command_multiple( "preservebackup", $running_index++, -1));

echo '<hr><h2>Bare Configuration File for HiPPSO Containing All Specified Options:</h2>'. "\n";
echo '<code>';
for($i=0; $i < count($command_list); $i++){
	if($command_list[$i][1] != true){
		echo '<span style="color: #ee0000">';
	}
	echo htmlspecialchars($command_list[$i][0]);
	if($command_list[$i][1] != true){
		echo '</span>';
	}
	echo '<br/>' . "\n";
}
echo '</code>';
?>

	</form>
</div>
<div class="footer">
	<div class="footer-inner">
		<div class="footer-logo">
			<p><img src="https://www12.informatik.uni-erlangen.de/downloads/eako/HiPPSOconfigurationFileGeneration/img/logo-fau-inverse.png" width="185" height="35" alt="Friedrich-Alexander-Universit&auml;t Erlangen-N&uuml;rnberg"></p>
		</div>
		<div class="footer-address">
			<address itemscope itemtype="http://schema.org/PostalAddress">
				<meta itemprop="name" content="Lehrstuhl f&uuml;r Informatik 12 Hardware-Software-Co-Design">
				<span>Lehrstuhl f&uuml;r Informatik 12<br>Hardware-Software-Co-Design</span><br>
				<span itemprop="streetAddress">Cauerstra&szlig;e 11</span><br>
				<span itemprop="postalCode">91058</span> <span itemprop="addressLocality">Erlangen</span><br>
				<span itemprop="addressCountry"></span>
			</address>
		</div>
		<div class="footer-meta">
			<a href="https://www.cs12.tf.fau.de/impressum">Legal Notice</a>&emsp;
			<a href="https://www.cs12.tf.fau.de/datenschutz">Privacy</a>&emsp;
			<a href="https://www.cs12.tf.fau.de/barrierefreiheit">Accessibility</a>
		</div>
	</div>
</div>
<?php
#foreach ($_POST as $key => $value) {
#        echo "<tr>";
#        echo "<td>";
#        echo htmlspecialchars("$key");
#        echo "</td>   ";
#        echo "<td>";
#        echo htmlspecialchars("$value");
#        echo "</td>";
#        echo "</tr>";
#        echo "<br/>";
#    }
?>
</body>
</html>
