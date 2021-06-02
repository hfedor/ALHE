#ifndef RHOMBUSWAREHOUSE_H
#define RHOMBUSWAREHOUSE_H


#include <iostream>
#include <time.h>
#include <cstdlib>
#include <algorithm>
#include "Ware.h"
#include "Individual.h"


#include <vector>

using namespace std;

class RhombusWarehouse
{
double leftLegA;
double rightLegA;
double rightLegB;
int hallwayWidth;
double warehouseHeight;

public:
    RhombusWarehouse(double leftLegA, double rightLegA, double rightLegB, int hallwayWidth, double warehouseHeight)
            : leftLegA(leftLegA), rightLegA(rightLegA), rightLegB(rightLegB), hallwayWidth(hallwayWidth),
              warehouseHeight(warehouseHeight) {}



    std::vector<Individual> calculateMiPlusLambda(int populationSize, int lambda, int generationCount, int mutateBySwapRate, int mutateByRotationRate,
                                 Individual &p) {
        vector<Individual> population;
        //generowanie populacji
        for(int i =0; i< populationSize;i++)
        {
            p.shuffle();
            population.push_back(p);
        }

        population = shuffle(population);
        for (int j = 0; j < generationCount; ++j)
        {
             vector<Individual> lambdaPopulation;
             //losowanie populacji lambda
             for(int i =0;i<lambda;i++)
             {
                 lambdaPopulation.push_back(population[rand()%populationSize]);
             }

            //reprodukcja
            for(int i =0;i<lambdaPopulation.size();i++)
            {
                Individual child =lambdaPopulation[i].crossover(lambdaPopulation[lambdaPopulation.size()-1-i]);

                //mutacja przez zamiane
                while(rand()%100 <  mutateBySwapRate)
                {
                    child.mutateBySwap();
                }
                while(rand()%100 <  mutateByRotationRate)
                {
                    child.mutateByRotation();
                }

                population.push_back(child);
            }

            //obliczanie funkcji przystosowania
            for (Individual &item : population)
            {
                if(j ==99)
                {
                    item.fillRhombusWarehouse(leftLegA,rightLegA,rightLegB,hallwayWidth,warehouseHeight);
                    item.calculateWaresArea();
                }
                else
                {
                item.fillRhombusWarehouse(leftLegA,rightLegA,rightLegB,hallwayWidth,warehouseHeight);
                item.calculateWaresArea();}
            }
            //sortowanie
            sort(population.begin(), population.end(), Individual::compare);
            population.erase(population.begin() + populationSize, population.end());

        }
        return population;
    }
    std::vector<Individual> calculateMiLambda(int populationSize, int lambda, int generationCount, int mutateBySwapRate, int mutateByRotationRate,
                                 Individual &p) {
        vector<Individual> population;
        //generowanie populacji
        for(int i =0; i< populationSize;i++)
        {
            p.shuffle();
            population.push_back(p);
        }

        population = shuffle(population);
        for (int j = 0; j < generationCount; ++j)
        {
             vector<Individual> lambdaPopulation;
             //losowanie populacji lambda
             for(int i =0;i<lambda;i++)
             {
                 lambdaPopulation.push_back(population[rand()%populationSize]);
             }
              population.erase(population.begin(),population.end());

            //reprodukcja
            for(int i =0;i<lambdaPopulation.size();i++)
            {
                Individual child =lambdaPopulation[i].crossover(lambdaPopulation[lambdaPopulation.size()-1-i]);

                //mutacja przez zamiane
                while(rand()%100 <  mutateBySwapRate)
                {
                    child.mutateBySwap();
                }
                while(rand()%100 <  mutateByRotationRate)
                {
                    child.mutateByRotation();
                }

                population.push_back(child);
            }

            //obliczanie funkcji przystosowania
            for (Individual &item : population)
            {
                if(j ==99)
                {
                    item.fillRhombusWarehouse(leftLegA,rightLegA,rightLegB,hallwayWidth,warehouseHeight);
                    item.calculateWaresArea();
                }
                else
                {
                item.fillRhombusWarehouse(leftLegA,rightLegA,rightLegB,hallwayWidth,warehouseHeight);
                item.calculateWaresArea();}
            }
            //sortowanie
            sort(population.begin(), population.end(), Individual::compare);
            population.erase(population.begin() + populationSize, population.end());

        }
        return population;
    }

private:
    std::vector<Individual> shuffle(std::vector<Individual> pop)
    {
        for(auto & item : pop)
        {
            std::swap(item,pop[rand()%pop.size()]);
        }
        return pop;
    }

};

#endif // RHOMBUSWAREHOUSE_H
