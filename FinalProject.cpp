//
//  FinalProject.cpp
//  FinalProject_493
//
//  Created by Cooper Richardson & Cruz Kerver on 5/7/17.
//  Copyright © 2017 Cooper Richardson & Cruz Kerver. All rights reserved.
//
#include <iostream>
#include <fstream>
#include <assert.h>
#include <random>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <limits>
#include <time.h>



#define crRand (double)rand()/RAND_MAX
using namespace std;

// Global Constants
int numStudents=0;
int numGenerations=0;
int populationSize = 100;
int numTutors;

class student{
public:
    double studentLocX;
    double studentLocY;
    void initStudent();
};

class policy{
public:
    vector<student> studentOrder;
    double fitness;
    double evalFitness(vector<student>, int);
    void initPolicy(vector<student>);
    vector<student> mutatePolicy(vector<student>);
};

void initStudents(vector<student>* allStudents){
    cout<<"Number of students?: "<<endl;
    cin>>numStudents;
    cout<<endl;
    
    cout<<"Number of tutors?: "<<endl;
    cin>>numTutors;
    cout<<endl;
    
    cout<<"How many Generations?: "<<endl;
    cin>>numGenerations;
    cout<<endl;

    
    for(int i=0; i<numStudents; i++){
        student A;
        A.initStudent();
        allStudents->push_back(A);
    }
};

void student::initStudent(){
    
    studentLocX = rand()% 15 + crRand - crRand;
    studentLocY = rand()% 15 + crRand - crRand;
};

void policy::initPolicy(vector<student> policyChange){
    studentOrder = policyChange;
    fitness = evalFitness(studentOrder, numTutors);
};

double calcDistance(double locX1, double locX2, double locY1, double locY2 )
{
    double distance;
    double xDist = locX1-locX2;
    double yDist = locY1-locY2;
    distance = sqrt((xDist*xDist)+(yDist*yDist));
    return distance;
}

double policy::evalFitness(vector<student> studentOrder, int numTutors)
{
    double tutorX = 7.5;
    double tutorY = 10;
    int numStudents = studentOrder.size();
    int num_Students_TS = numStudents / numTutors;
    int k = 0;
    double dist = 0;
    double total_dist = 0;
    for(int i=0; i<(numTutors-1); i++){
        int j = 0;
        dist = calcDistance(tutorX, studentOrder.at(k).studentLocX, tutorY, studentOrder.at(k).studentLocY );
        
        while(j<(num_Students_TS - 1)){ //number of distances is one less than number of cites
            dist = dist + calcDistance(studentOrder.at(k).studentLocX,studentOrder.at(k+1).studentLocX,studentOrder.at(k).studentLocY,studentOrder.at(k+1).studentLocY);
            j++;
            k++;
        }
        dist = dist + calcDistance(tutorX, studentOrder.at(k).studentLocX, tutorY, studentOrder.at(k).studentLocY );
        total_dist = total_dist + dist;
        k++;
    }
    
    dist = calcDistance(tutorX, studentOrder.at(k).studentLocX, tutorY, studentOrder.at(k).studentLocY ); //Distance calc from start first city
    while(k<numStudents-1){
        dist = dist + calcDistance(studentOrder.at(k).studentLocX,studentOrder.at(k+1).studentLocX,studentOrder.at(k).studentLocY,studentOrder.at(k+1).studentLocY);
        k++;
    };
    
    dist = dist + calcDistance(tutorX, studentOrder.at(k).studentLocX, tutorY, studentOrder.at(k).studentLocY );
    total_dist = total_dist + dist;
    
    fitness = total_dist;
    return fitness;
    
};

void initPolicies(vector<policy>* population, vector<student>* allStudents){
    vector<student> policyChange = *allStudents;
    population->clear();

    
    for (int i = 0; i < populationSize; i++)
    {
        for(int i=0; i<numStudents; i++)
        {
            vector<student> hold;
            student B;
            B.initStudent();
            hold.push_back(B);
            
            int pullLocation1 = rand()% numStudents;
            int pullLocation2 = rand()% numStudents;
            
            while( pullLocation1 == pullLocation2)
            {
                pullLocation1 = rand()% numStudents;
                pullLocation2 = rand()% numStudents;
            }
            
            hold[0]=policyChange.at(pullLocation1);
            
            policyChange.at(pullLocation1) = policyChange.at(pullLocation2);
            policyChange.at(pullLocation2) = hold[0];
        }
        
        // LR 6 - Doesnt visit same student twice                                                                      // LR 6
        for(int i =0; i<policyChange.size(); i++)
        {
            for(int j = 0; j<policyChange.size(); j++)
            {
                if(i != j){
                    assert(policyChange.at(i).studentLocX != policyChange.at(j).studentLocX);
                }
            }
        }
        
        policy A;
        A.initPolicy(policyChange);
        
        // MR 3 all policies have a fitness                                                                         // MR 3
        assert(A.fitness > 10);
        population->push_back(A);
    }
    
    
    // MR 1 - population is filled with policies and the size is greater than 1 (equals the desired size)              // MR 1
    assert(population->size() == populationSize);
};

vector<student> policy::mutatePolicy(vector<student> mutatePolicy){
    
    vector<student> mutate = mutatePolicy;
    int numChanges = numStudents * .1;
    if(numStudents<10){
        numChanges = 1;
    }
    
    for(int i = 0; i<numChanges; i++)
    {
        vector<student> hold;
        student B;
        B.initStudent();
        hold.push_back(B);
        
        int pullLocation1 = rand()% numStudents;
        int pullLocation2 = rand()% numStudents;
        
        while(pullLocation1 == 0 || pullLocation2 == 0 || pullLocation1 == pullLocation2)
        {
            pullLocation1 = rand()% numStudents;
            pullLocation2 = rand()% numStudents;
        }
        
        hold[0]=mutate.at(pullLocation1);
        
        mutate.at(pullLocation1) = mutate.at(pullLocation2);
        mutate.at(pullLocation2) = hold[0];
        hold.clear();
    }
    
    //Test that slight mutation occured                                                                           // LR 4
    int repeat=0;
    for(int i= 1; i<numStudents; i++){
        if(mutate.at(i).studentLocX == mutatePolicy.at(i).studentLocX){
            repeat++;
        }
        
    }
    
    assert(repeat<numStudents);
    assert(mutate.size()==numStudents);
    return mutate;
};

vector<policy> replicatePop(vector<policy>* population){
    
    // take in current population
    vector<policy> mutatePop = *population;
    assert(mutatePop.size() == populationSize);
    int pullLocation;
    
    // replicate and mutate
    for(int i =0; i<populationSize; i++)
    {
        policy B;
        pullLocation = rand()% mutatePop.size();
        vector<student> mutate = B.mutatePolicy(mutatePop.at(pullLocation).studentOrder);
        B.initPolicy(mutate);
        mutatePop.push_back(B);
    }
    
    // MR 5 - Can return to carrying capastudent (Mutated population doubles back to 200)                         // MR 5
    assert(mutatePop.size() == populationSize * 2);
    return mutatePop;
};

vector<policy> downSelect(vector<policy>* mutatedPopulation){
    vector<policy> population;
    population.clear();
    assert(population.size() == 0);
    
    int min = 10000;
    int minLoc = 0;
    
    for(int i=0; i<mutatedPopulation->size(); i++){
        if(mutatedPopulation->at(i).fitness<min){
            min = mutatedPopulation->at(i).fitness;
            minLoc = i;
        }
    }
    population.push_back(mutatedPopulation->at(minLoc));
    population.push_back(mutatedPopulation->at(minLoc));
    
    while(population.size()<populationSize)
    {
        int pullLocation1 = rand()% populationSize*2;
        int pullLocation2 = rand()% populationSize*2;
        
        while(pullLocation1 == pullLocation2)
        {
            pullLocation1 = rand()% populationSize*2;
        }
        
        if(mutatedPopulation->at(pullLocation1).fitness < mutatedPopulation->at(pullLocation2).fitness){
            population.push_back(mutatedPopulation->at(pullLocation1));
        }
        
        else if (mutatedPopulation->at(pullLocation1).fitness >= mutatedPopulation->at(pullLocation2).fitness)
        {
            population.push_back(mutatedPopulation->at(pullLocation2));
        }
    }
    
    // MR 4 - Downselect cuts parent population in half - Downselection                                           // MR 4
    assert(population.size() == mutatedPopulation->size()/2);
    return population;
};

void printInfo(vector<policy> population, vector<double>* averages, vector<double>* minimum, vector<double>* maximum){
    
    double average = 0.0;
    double min = 100000;
    double max = 0;
    int minRepeat = 0;
    for(int i=0; i<populationSize; i++){
        //cout<<"Policy Fitness: "<<population.at(i).fitness<<endl;
        average = average + population.at(i).fitness;
        if(population.at(i).fitness<min){
            min = population.at(i).fitness;
        }
        if(population.at(i).fitness>max){
            max = population.at(i).fitness;
        }
        if(population.at(i).fitness==min){
            minRepeat++;
        }
    }
    average = average/populationSize;
    cout<<"AVERAGE: "<<average<<" MIN: "<<min<<" MAX: "<<max<<" Min repeat: "<<minRepeat<<endl;
    averages->push_back(average);
    minimum->push_back(min);
    maximum->push_back(max);
};

void writeToFile(vector<double>* averages, vector<double>* minimum, vector<double>* maximum){
    
    ofstream allValueEA;
    allValueEA.open("EvoAlgValues");
    for(int i =0; i<numGenerations; i++)
    {
        allValueEA << averages->at(i) << "\t" << minimum->at(i) << "\t" << maximum->at(i) << "\n";
    }
    allValueEA.close();
};


int main() {
    srand(time(NULL));
    
    vector<policy> population;
    vector<student> allStudents;
    vector<policy> mutatedPopulation;
    vector<double> averages;
    vector<double> maximum;
    vector<double> minimum;
    
    vector<vector<double>> allAverages;
    vector<vector<double>> allMaximum;
    vector<vector<double>> allMinimum;
    
    
    // Initialize Students
    initStudents(&allStudents);

        // Initialize policies
        initPolicies(&population, &allStudents);
        
        cout<<"Original Fitnesses: "<<endl;
        printInfo(population, &averages, &minimum, &maximum);
        
        // Start EA (for)
        
        for(int i = 0; i< numGenerations; i++)
        {
            // Replicate -> Mutate
            mutatedPopulation = replicatePop(&population);
            
            // Downselect -> Binary tournament
            population.clear();
            population = downSelect(&mutatedPopulation);
            //cout<<endl;
            //printInfo(population, &averages, &minimum, &maximum);
        }
        cout<<endl<<" - - - - - - - - - - - - - - - - - - - - - - - - "<<endl<<"Final Fitnesses: "<<endl;
        printInfo(population, &averages, &minimum, &maximum);
        //writeToFile( &averages, &minimum, &maximum );
        

    double tutorX= 7.5;
    double tutorY = 10;
    int best = 100000;
    int bestLoc = 0;
    cout<< " POP SIZE: "<<population.size()<<endl;
    for(int i =0;i<population.size();i++)
    {
        if(population.at(i).fitness<best)
        {
            bestLoc = i;
            best = population.at(i).fitness;
        }
    }
    vector<student> bestPolicy = population.at(bestLoc).studentOrder;
    
    vector<double> bestX;
    vector<double> bestY;
    int k = 0;
    bestX.push_back(tutorX);
    bestY.push_back(tutorY);
    
    for(int j=0; j<numTutors-1; j++)
    {
        for(int h = 0; h<(numStudents/numTutors); h++)
        {
            bestX.push_back(bestPolicy.at(k).studentLocX);
            bestY.push_back(bestPolicy.at(k).studentLocY);
            k++;
        }
        bestX.push_back(tutorX);
        bestY.push_back(tutorY);
    }
    while(k<numStudents)
    {
        bestX.push_back(bestPolicy.at(k).studentLocX);
        bestY.push_back(bestPolicy.at(k).studentLocY);
        k++;
    }
    bestX.push_back(tutorX);
    bestY.push_back(tutorY);
    
    cout<<"here"<<endl;
    ofstream bestPath;
    bestPath.open("BestRoute");
    for(int j = 0; j<bestX.size(); j++)
    {
        bestPath << bestX.at(j) << "\t" << bestY.at(j) << "\n";
    }
    bestPath.close();
    
    

    
}














