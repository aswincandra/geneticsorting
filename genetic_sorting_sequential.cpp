#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <stdio.h>
#include <iomanip>    
#include <algorithm>   
#include <array>        
#include <random>       
#include <chrono> 
#include <cstdlib>
#include <stdlib.h>
#include <math.h>

//#include <mpi.h>


using namespace std;

const int jumlahData = 999999; // Ganti sesuai jumlah data asli
double data[jumlahData] = {0};


	




struct chromosome{
	//public:							
	//double gnome[jumlahData]={};      				// data tunggal (gnome) tiap satu kromosom
	//double* gnome[jumlahData];
	double* gnome;
	double fitness; 						// kualitas untuk satu kromosom
};

int random_num(int start, int end){
	int range = (end-start)+1; 
	int random_int = start+(rand()%range); 
	return random_int; 
}

double create_gnome(){	
	double gnomes;
	int randomIndex = random_num(0, jumlahData-1);
	gnomes=data[randomIndex];
	
	
	return gnomes;

}

void cetakData(double* data, int len){
	for(int i=0; i<len; i++){
		printf("%f\n", data[i]);
	}
}

double hitungFitness(double* individu) {
	double counter=0;
	
	/*
	for(int i=0; i<jumlahData; i++){
		if(dataSorted[i] == individu[i]) {
			counter++;
			printf("Data Sorted: %f  Individu: %f data ke: %d\n", dataSorted[i], individu[i], i);
			
		}
	}
	cout << "Yang sama ada: " << counter << endl;
	*/

	
	for(int i=0; i<jumlahData; i++){
		if(individu[i]<individu[i+1]){
			counter++;		
		}
	}
	

	return (counter/jumlahData)*100;
}


void crossover_v1(chromosome ortu1, chromosome ortu2, chromosome anak1, chromosome anak2) {

	int point;
	point = 1 + rand() % (jumlahData - 1); // Cutting Point mulai indeks ke: 1 dan berakhir di indeks: jumlahData-1

	for (int i = 0; i < point; i++) {
		anak1.gnome[i] = ortu1.gnome[i];
		anak2.gnome[i] = ortu2.gnome[i];
	}
	for (int i = point; i < jumlahData; i++) {
		anak1.gnome[i] = ortu2.gnome[i];
		anak2.gnome[i] = ortu1.gnome[i];
	}

}

void crossover_v2(chromosome ortu1, chromosome ortu2, chromosome anak1, chromosome anak2)
{
	// uniform crossover can also be used
	// for each gene we decide randomly where it goes
	// (to the first or second offspring)

	for (int i = 0; i < jumlahData; i++){
		if (rand() % 2) {// flip
			anak1.gnome[i] = ortu2.gnome[i];
			anak2.gnome[i] = ortu1.gnome[i];
		}
		else {
			anak1.gnome[i] = ortu1.gnome[i];
			anak2.gnome[i] = ortu2.gnome[i];
		}
	}
}


int tournament_selection(int tournament_size, chromosome *population, int pop_size){ // ----------------------- FIND THE BEST SINGLE INDIVIDUAL FUNCTION
	int selected_index;
	int temp=0;
	
	selected_index=rand() % (pop_size/2) + (pop_size/2);
	// selected_index=rand() % pop_size;
	
	for (int i = 1; i < tournament_size; i++) {
		int r = rand() % (pop_size/2) + (pop_size/2);
		selected_index = population[r].fitness < population[selected_index].fitness ? r : selected_index;
	}
	
	
	return selected_index;
}


void doMutation(chromosome newGen, double mutation_rate){

	for(int a=0; a<jumlahData; a++){
		double p = rand() / (double)RAND_MAX;
		//printf("Double random: %f\n", p);
		if (p < mutation_rate){
			int indexRandom1=rand() % jumlahData;
			int indexRandom2=rand() % jumlahData;
			swap(newGen.gnome[indexRandom1], newGen.gnome[indexRandom2]);
		}
	}
}


void loadData(const char* namaFile) {
	string line;
	ifstream myfile (namaFile);
	
	if (myfile.is_open()) {
		int counter = 0;
		while (getline(myfile, line, ',')) {
			//cout << line << '\n';
			data[counter] = stod(line);
			counter++;
		}
		myfile.close();
	}
	int size = *(&data + 1) - data; 
}




int main(){
	srand((unsigned)(time(0))); 
	
	loadData("dataDemo.csv");
	//cetakData(data, jumlahData);

// -------------------------------------------------------------------------------- CREATE A TARGET THAT ALREADY SORTED 
	
	double dataSorted[jumlahData]={};

	for(int i=0; i<jumlahData; i++){
		dataSorted[i]=data[i];
	}
	
	//cout << "Data Sorted: " << endl;
	sort(dataSorted, dataSorted+jumlahData);
	// cetakData(dataSorted, jumlahData);

// -------------------------------------------------------------------------------------------------------------------

	int pop_size = 12;			// Harus kelipatan 4
	int num_gens=30;
	double mutation_rate=0.600000;
	int j=0;

	chromosome* population;
	population = (chromosome*) malloc(pop_size * sizeof(chromosome));

	for (int j = 0; j < pop_size; j++)
		population[j].gnome = (double*)malloc(jumlahData * sizeof(double));

	chromosome anak1, anak2;
	anak1.gnome = (double*)malloc(jumlahData * sizeof(double));
	anak2.gnome = (double*)malloc(jumlahData * sizeof(double));
		
			
	// -------------------------------------------------------------------------------- INITIALIZE BEGINNING POPULATION
	
	
	do{
		printf("Population: %d\n", j+1);
 
		
		
		random_shuffle(&data[0], &data[jumlahData]);

		for(int i=0; i<jumlahData; i++){
			population[j].gnome[i]=data[i];
		}

						
		/*
		for(int i=0; i<jumlahData; i++){
			printf("%f\n", population[j].gnome[i]);
		}
		cout << endl;
		*/
				
		population[j].fitness=hitungFitness(population[j].gnome);
		cout << "Fitness: " << population[j].fitness << endl;
		
				
		j++;
		
	}while(j<pop_size);

	// -------------------------------------------------------------------------------- START REPRODUCTION
	
	chromosome bestGen;
	bestGen.gnome = (double*)malloc(jumlahData * sizeof(double));

int pop=0;

// -------------------------- RULE 1 : Looping till reach the (x) generations; crossover: crossover_v2; pop: 0 to half of population ---------------------------
cout << "MULAI RULE 1" << endl;

chromosome bestGen1;
	bestGen1.gnome = (double*)malloc(jumlahData * sizeof(double));

for(int g=1; g<num_gens; g++){	
	int selected_index=0;
		cout << "Pop ke: " << pop << endl;
		// DO CROSSOVER
		//crossover_v1(population[pop], population[pop+1], anak1, anak2);
		crossover_v2(population[pop], population[pop+1], anak1, anak2);

		// DO MUTATION
		doMutation(anak1, mutation_rate);
		anak1.fitness=hitungFitness(anak1.gnome);
		//cout << "Fitness Anak1: " << anak1.fitness << endl;

		doMutation(anak2, mutation_rate);
		anak2.fitness=hitungFitness(anak2.gnome);
		//cout << "Fitness Anak2: " << anak2.fitness << endl;

		// SURVIVAL OF FITTEST 
		for(int j=0; j<pop_size/2; j++){
			if(population[j].fitness<anak1.fitness) population[j]=anak1;
			if(population[j].fitness<anak2.fitness && population[j].fitness != anak1.fitness) population[j]=anak2;	
		}

		// FIND THE BEST GEN
		double temp=0;
	

		for(int i=0; i<pop_size/2; i++){
			if(population[i].fitness>temp){
				temp=population[i].fitness;
				selected_index=i;
			}
		}

		
		if(pop==pop_size/2) pop=0;
		else pop=pop+2;
		printf("Generation: %d\n", g);
		bestGen1=population[selected_index];
		printf("Final fitness: %f\n", bestGen1.fitness);


	
}



// -------------------------- RULE 2 : Looping till reach the (x) fitness; crossover: crossover_v1 ------------------------------------------------------------
	cout << "MULAI RULE 2" << endl;
	
	chromosome bestGen2;
	bestGen2.gnome = (double*)malloc(jumlahData * sizeof(double));
	

	int a=1;

	do{
		printf("Process: %d\n", a);
		// FIND THE TWO BESTS INDIVIDUALS
		int r1 = tournament_selection(2, population, pop_size);
		int r2 = tournament_selection(2, population, pop_size);
	

		// DO CROSSOVER
		crossover_v1(population[r1], population[r2], anak1, anak2);

		// DO MUTATION
		doMutation(anak1, mutation_rate);
		anak1.fitness=hitungFitness(anak1.gnome);
		//cout << "Fitness Anak1: " << anak1.fitness << endl;

		doMutation(anak2, mutation_rate);
		anak2.fitness=hitungFitness(anak2.gnome);
		//cout << "Fitness Anak2: " << anak2.fitness << endl;

		// SURVIVAL OF FITTEST 
		for(int j=0; j<pop_size; j++){
			if(population[j].fitness<anak1.fitness) population[j]=anak1;
			if(population[j].fitness<anak2.fitness && population[j].fitness != anak1.fitness) population[j]=anak2;	
		}

		// FIND THE BEST GEN


		double temp=0;
		int selected_index=0;

		for(int i=0; i<pop_size; i++){
			if(population[i].fitness>temp){
				temp=population[i].fitness;
				selected_index=i;
			}
		}
		bestGen2=population[selected_index];
		printf("Final fitness: %f\n", bestGen2.fitness);
		//cetakData(bestGen.gnome, jumlahData);
		a++;
	}while(bestGen2.fitness<=50.08);


	// -------------------------------------------------------------------------------------------------------------FINAL GENERATION
	// ~ Mengawinkan hasil dari generasi terbaik dari Cluster 1 dan Cluster 2

	crossover_v2(bestGen1, bestGen2, anak1, anak2);					// Menggunakan crossover tipe-2
	
	doMutation(anak1, mutation_rate);
	anak1.fitness=hitungFitness(anak1.gnome);
	//cout << "Fitness Anak1: " << anak1.fitness << endl;

	doMutation(anak2, mutation_rate);
	anak2.fitness=hitungFitness(anak2.gnome);
	//cout << "Fitness Anak2: " << anak2.fitness << endl;

	// SURVIVAL OF FITTEST 
	for(int j=0; j<pop_size; j++){
		if(population[j].fitness<anak1.fitness) population[j]=anak1;
		if(population[j].fitness<anak2.fitness && population[j].fitness != anak1.fitness) population[j]=anak2;	
	}

	// FIND THE BEST GEN

	
	int temp=0;
	int selected_index=0;

	for(int i=0; i<pop_size; i++){
		if(population[i].fitness>temp){
		temp=population[i].fitness;
		selected_index=i;
		}
	}
	bestGen1=population[selected_index];

	cetakData(bestGen1.gnome, jumlahData);
	printf("Final fitness: %f\n", bestGen2.fitness);
	

	
}

	


