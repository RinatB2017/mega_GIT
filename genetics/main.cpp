#include <iostream>					// для cout и т.п.
#include <vector>					// для класса vector
#include <string>					// для класса string
#include <algorithm>					// для алгоритма сортировки
#include <time.h>					// для случайных величин
#include <math.h>					// для abs()

#define GA_POPSIZE		2048		// размер популяции
#define GA_MAXITER		16384		// максимальное число итераций
#define GA_ELITRATE		0.10f		// элитарность
#define GA_MUTATIONRATE	0.25f			// мутации
#define GA_MUTATION		RAND_MAX * GA_MUTATIONRATE
#define GA_TARGET		std::string("Hello world!")

using namespace std;				

struct ga_struct 
{
	string str;						// строка
	unsigned int fitness;					// пригодность
};

typedef vector<ga_struct> ga_vector;			// для краткости

void init_population(ga_vector &population,
					 ga_vector &buffer ) 
{
	int tsize = GA_TARGET.size();

	for (int i=0; i<GA_POPSIZE; i++) {
		ga_struct citizen;
		
		citizen.fitness = 0;
		citizen.str.erase();

		for (int j=0; j<tsize; j++)
			citizen.str += (rand() % 90) + 32;

		population.push_back(citizen);
	}

	buffer.resize(GA_POPSIZE);
}

void calc_fitness(ga_vector &population)
{
	string target = GA_TARGET;
	int tsize = target.size();
	unsigned int fitness;

	for (int i=0; i<GA_POPSIZE; i++) {
		fitness = 0;
		for (int j=0; j<tsize; j++) {
			fitness += abs(int(population[i].str[j] - target[j]));
		}
		
		population[i].fitness = fitness;
	}
}

bool fitness_sort(ga_struct x, ga_struct y) 
{ return (x.fitness < y.fitness); }

inline void sort_by_fitness(ga_vector &population)
{ sort(population.begin(), population.end(), fitness_sort); }

void elitism(ga_vector &population, 
				ga_vector &buffer, int esize )
{
	for (int i=0; i<esize; i++) {
		buffer[i].str = population[i].str;
		buffer[i].fitness = population[i].fitness;
	}
}

void mutate(ga_struct &member)
{
	int tsize = GA_TARGET.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 32; 

	member.str[ipos] = ((member.str[ipos] + delta) % 122);
}

void mate(ga_vector &population, ga_vector &buffer)
{
	int esize = GA_POPSIZE * GA_ELITRATE;
	int tsize = GA_TARGET.size(), spos, i1, i2;

	elitism(population, buffer, esize);

	// Mate the rest
	for (int i=esize; i<GA_POPSIZE; i++) {
		i1 = rand() % (GA_POPSIZE / 2);
		i2 = rand() % (GA_POPSIZE / 2);
		spos = rand() % tsize;

		buffer[i].str = population[i1].str.substr(0, spos) + 
			            population[i2].str.substr(spos, esize - spos);

		if (rand() < GA_MUTATION) mutate(buffer[i]);
	}
}

inline void print_best(ga_vector &gav)
{ cout << "Best: " << gav[0].str << " (" << gav[0].fitness << ")" << endl; }

inline void swap(ga_vector *&population,
				 ga_vector *&buffer)
{ ga_vector *temp = population; population = buffer; buffer = temp; }

int main()
{
	srand(unsigned(time(NULL)));

	ga_vector pop_alpha, pop_beta;
	ga_vector *population, *buffer;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	buffer = &pop_beta;

	for (int i=0; i<GA_MAXITER; i++) {
		calc_fitness(*population);		// вычисляем пригодность
		sort_by_fitness(*population);		// сортируем популяцию
		print_best(*population);		// выводим лучшую популяцию

		if ((*population)[0].fitness == 0) break;

		mate(*population, *buffer);		// спариваем популяции
		swap(population, buffer);		// очищаем буферы
	}

	return 0;
}
