#include "general/includes.h"
#include <fstream>
#include <gmp.h>

int main(){
	std::vector<mpf_t*> v2;
	std::vector<std::string> rngdesc2(1,"42");
	unsigned int parpar = 0;
	highprecisionpso::RandomNumberGenerator * rng2 = highprecisionpso::parse::ParseRandomNumberGenerator(rngdesc2, parpar);
	for(int i = 0; i < 8; i++)v2.push_back(highprecisionpso::mpftoperations::ToMpft(highprecisionpso::mpftoperations::GetGaussianRandomMpft(0.0, 1.0, rng2)));
	std::ofstream myres ("myres.txt");
	myres << "vec\n";
	for(int i = 0; i < 8; i++){
		highprecisionpso::mpftoperations::StoreMpft(v2[i], &myres);	
	}
	myres.close();
}
