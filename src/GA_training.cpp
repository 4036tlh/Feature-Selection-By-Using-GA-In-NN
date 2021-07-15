#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>

using namespace std;

// declare constant - problem specification, population size
const int GENE = 60;
const int POP_SIZE = 40;
const float CO_probability = 0.8; 
const float MUT_probability = 0.2; 
const int MAX_GENERATION = 30;

// declare chromosomes
int chromosome[POP_SIZE][GENE];
int newChromosome[POP_SIZE][GENE];

// declare new chromosome counter
int newChromoCounter = 0;

// define the basic recorded variables
double limit = 0.85;
double sum = 0.0;
int index = 99;
double bestFitness = 0.0;
double avgFitness = 0.0;
int bestChromosome[GENE];

// declare fitness
double fitness[POP_SIZE];

// declare parents
int parents[2][GENE];

// declare children
int children[2][GENE];

// define the required file
ofstream saveGeneFile, bestFitnessFile, avgFitnessFile, bestChromosomeFile;
fstream  readFitnessFile;

// initialize the population
void initializePopulation(){
	
	int randNum;
	//initialize random seed
	srand(time(NULL)); 
	for (int c = 0; c < POP_SIZE; c++){
		for (int i = 0; i < GENE; i++){
			chromosome[c][i] = rand() % 2;
		}
	}
}

// select parents for crossover & mutation
void parentSelection(){
    int player1, player2;
    int indexParents[2];

    do{	//tournament selection
        for (int p=0; p<2; p++){
            player1 = rand()  % POP_SIZE;
            do {
                player2 = rand()  % POP_SIZE;
            } while (player1==player2);

            if (fitness[player1] >= fitness[player2]){
                indexParents[p]=player1;
            }
            else{
                indexParents[p]=player2;
            }
            cout << "\n   Players: " << player1 << " vs " << player2;
            cout << "\n   Fitness: " << fitness[player1] << " vs " << fitness[player2];
            cout << "\n   Winner: " << indexParents[p];
        }
    } while(indexParents[0]==indexParents[1]);

    for (int p=0;p<2;p++){
            cout << "\n  Parents " << p+1 <<" : ";
        for (int g=0; g<GENE; g++){
            parents[p][g]=chromosome[indexParents[p]][g];
            cout << parents[p][g];
        }
    }
}

// 2-Point CrossOver
void crossover() {

	float prob = 0.0;
    int co_point1, co_point2;
    int min_co_p, max_co_p;

    for (int p=0; p<2; p++){//Copy parents to children
        for (int g=0; g<GENE; g++){
            children[p][g] = parents[p][g];
        }
    }
    
    prob = (rand() % 11)/10.0;
    if (prob < CO_probability){//crossover happen
        
        co_point1 = rand() % GENE;
        do{
	        co_point2 = rand() % GENE;
	    }while(co_point1 == co_point2);
	    
		min_co_p = min(co_point1, co_point2); 
		max_co_p = max(co_point1, co_point2) + 1; 
		
        cout << "\n  Children crossover between " << min_co_p << " and " << max_co_p;
        for (int co = min_co_p; co < max_co_p; co++ ){
            children[0][co] = parents[1][co];
            children[1][co] = parents[0][co];
        }
    }
	else cout << "\n  CrossOver did not happen";
	
	for(int c=0; c<2; c++){
		
		cout << "\n  Children " << c+1 << ": ";
			for (int g = 0; g < GENE; g++){
				cout << children[c][g];
			}
	}
}

// Mutation
void mutation(){
	
	float prob;
    int mut_point;

    for (int c=0; c<2; c++){
        prob = (rand() % 11)/10.0;
        if (prob < MUT_probability){
        	
            mut_point = rand() % GENE;
            cout << "\n  Children " << c <<" mutate at " << mut_point;
            children[c][mut_point]=abs(children[c][mut_point]-1);
        }
		else cout << "\n  Children " << c <<" =  Mutation do not occur";
	}

	for (int c = 0; c < 2; c++)
	{
		cout << "\n  Children " << c + 1 << " : ";
		for (int g = 0; g < GENE; g++)
		{
			cout << children[c][g];
		}
	}
}

// print all chromosomes
void printChromosome(){
	for (int c = 0; c < POP_SIZE; c++) {
		cout << "\n  C" << c << " = ";
		for (int i = 0; i < GENE; i++) {
			cout << chromosome[c][i];
		}
	}
}

// save chromose into file
void saveIntoFile(int saveItem[][GENE], int N){
	
	// open the file
	saveGeneFile.open("./data/gene.txt",ios::out);
	
	// save the chromose into the file
	for(int c = 0; c < N; c++){
		for(int g = 0; g < GENE; g++){
				saveGeneFile << saveItem[c][g];
		}
		saveGeneFile << endl;
	}
	// close the file
	saveGeneFile.close();
}

// calculate fitness by using NN
void cal_fitness(int cal_chromo[][GENE], int N){
	
	saveIntoFile(cal_chromo, POP_SIZE);
	
	// open python to calculate the fitness
	string str = "python ./training/NN.py";
	const char * command = str.c_str();
    system(command);
    
    cout << "\nReturn fitness from NN...\n";
}

// record the fitness
void evaluateChromosome(){
	
	cal_fitness(chromosome, POP_SIZE);
	
	readFitnessFile.open("./data/result.txt", std::ios_base::in);
	    
	double fit;
	    
	for (int c = 0; c < POP_SIZE; c++){
	    readFitnessFile >> fit;
	    fitness[c] = fit;
	}
	readFitnessFile.close();
}

// transfer the new children into newChromosome
void survivalSelection(){
	
	for (int c = 0; c < 2; c++){
		for (int g = 0; g < GENE; g++){
			newChromosome[newChromoCounter][g] = children[c][g];
		}
		newChromoCounter++;
	}

	for (int c = 0; c < newChromoCounter; c++)
	{
		cout << "\n  New c" << c << ": ";
		for (int g = 0; g < GENE; g++){
			cout << newChromosome[c][g];
		}
	}
}

// transfer the newChromosome into chromosomes
void copyChromosome(){
     for (int c=0; c<POP_SIZE; c++){
	 	//copy newChromosome to chromosome
        for (int g=0; g<GENE; g++){
            chromosome[c][g]=newChromosome[c][g];
        }
    }
}

// find the best fitness
void findBestFitness(){
	for (int c = 0; c < POP_SIZE; c++){
		if (bestFitness < fitness[c]){
			bestFitness = fitness[c];
			
			for (int g = 0; g < GENE; g++){
				bestChromosome[g] = chromosome[c][g];
				index = c;
			}
		}
	}
}

// calculate the average fitness
void calcAvgFitness(){
	
	sum = 0;
	for (int c = 0; c < POP_SIZE; c++){
		sum += fitness[c]; 
	}
	avgFitness = sum / POP_SIZE;
}

// find out and record the best fitness, average fitness & best chromosome
void record(){
	
	calcAvgFitness();
	findBestFitness();
	
	cout<<"\nDetail Information : ";
	for(int c = 0; c < POP_SIZE; c++){
		
		cout << "\nc" << c << " = " ;
		for(int g = 0; g < GENE; g++)cout<<chromosome[c][g];
		cout << " = " << fitness[c];
	}

	cout << endl;
	
	cout << "\n Best Fitness    = " << bestFitness;
	cout << "\n Best Chromosome = ";
	for (int g = 0; g < GENE; g++){
		cout << bestChromosome[g];
	}
	
	cout << "\n Average Fitness = " << avgFitness;
	avgFitnessFile << avgFitness << endl;
	
	cout << endl;

	bestFitnessFile << bestFitness << endl;
	for (int g = 0; g < GENE; g++){
		bestChromosomeFile << bestChromosome[g];
	}
	bestChromosomeFile << endl;
}

// checking terminate condition
bool terminateChecking(){
	if(bestFitness == 1){
			return true;
		}
	else return false;
}

int main() {
	
	// open file and prepare to record
	bestFitnessFile.open("./data/bestFitness.txt");
	avgFitnessFile.open("./data/avgFitness.txt");
	bestChromosomeFile.open("./data/bestChromosome.txt");

	cout << "\nGA START! \n";
	cout << "First generation \n\n";
	cout << "\nINITIALIZATION... \n";
	
	// initialize the population
	initializePopulation();
	
	// start training in GA
	for (int g=0;g<MAX_GENERATION;g++)
	{
		cout << "\n\n============================================================================";
		cout << "\nGENERATION " << g + 1 << endl;
		cout << "\nPRINT POPULATION \n";
		
		// show the chromosome
		printChromosome();

		// find fitness
		cout << "\n\nEVALUATE CHROMOSOME \n";
		evaluateChromosome();
		
		// record the variables that required to plot graph
		record();
		
		if(terminateChecking()){
			break;
		}
		
		// initialize the newChromoCounter
		newChromoCounter = 0;
		
		for (int i = 0; i < POP_SIZE/2; i++)
		{
			// apply tournament selection
			cout << "\n\nPARENT SELECTION \n";
			parentSelection();

			// apply 2-point crossover
			cout << "\n\nCROSS-OVER SELECTION \n";
			crossover();

			// apply mutation
			cout << "\n\nMUTATION \n";
			mutation();
			
			// update the child into newChromosome
			cout << "\n\nSURVIVAL SELECTION \n";
			survivalSelection();
			
		}

		cout <<"\n\nNEW CHROMOSOMES COPIED TO CHROMOSOME \n";
		// transfer into the formal chromosome to be new generation
        copyChromosome();
	}

	// close the file before exit the program
	bestFitnessFile.close();
	avgFitnessFile.close();
	bestChromosomeFile.close();

	cout << "\n\nGA Training Is Completed\n";
	
	// plot and save the result
	string str = "python ./training/plot_graph.py";
	const char * command = str.c_str();
	system(command);
	
	return 0;
}

