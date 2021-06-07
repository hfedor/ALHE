#ifndef POPULATION_H
#define POPULATION_H

#include <iostream>
#include <vector>
#include <algorithm>

/*
 * Szablon klasy odpowiedzialną za działanie algorytmu ewolucyjnego
*/
template <class Individ>
class Population
{
    std::vector<Individ> population; // tablica na populację osobników
    int lambda;
    int generationCount;
    int mutateBySwapRate;
    int mutateByRotationRate;
    int populationSize;
public:
    Population(int populationSize,
               int lambda,
               int generationCount,
               int mutateBySwapRate,
               int mutateByRotationRate,
               Individ p) :
        populationSize(populationSize),
        lambda(lambda),
        generationCount(generationCount),
        mutateBySwapRate(mutateBySwapRate),
        mutateByRotationRate(mutateByRotationRate)
    {
        for(int i =0; i< populationSize;i++)
            population.push_back(p);
    }

    void calculateMiPlusLambda()
    {
        shuffle();

        for (int j = 0; j < generationCount; ++j)
        {
             std::vector<Individ> lambdaPopulation;
             //losowanie populacji lambda
             for(int i =0;i<lambda;i++)
                 lambdaPopulation.push_back(population[rand()%populationSize]);

            //reprodukcja
            for(int i =0;i<lambdaPopulation.size();i++)
            {
                Individ child = lambdaPopulation[i].crossover(lambdaPopulation[lambdaPopulation.size()-1-i]);

                //mutacja przez zamiane
                while(rand()%100 <  mutateBySwapRate)
                    child.mutateBySwap();

                while(rand()%100 <  mutateByRotationRate)
                    child.mutateByRotation();

                population.push_back(child);
            }

            //obliczanie funkcji przystosowania
            for (auto &item : population)
                item.adaptationFunction();

            //sortowanie
            sort(population.begin(), population.end(), Individ::compare);

            population.erase(population.begin() + populationSize, population.end());
        }
    }

    void calculateMiLambda()
    {
        shuffle();

        for (int j = 0; j < generationCount; ++j)
        {
            std::vector<Individ> lambdaPopulation;
             //losowanie populacji lambda
             for(int i =0;i<lambda;i++)
             {
                 lambdaPopulation.push_back(population[rand()%populationSize]);
             }
              population.erase(population.begin(),population.end());

            //reprodukcja
            for(int i =0;i<lambdaPopulation.size();i++)
            {
                Individ child =lambdaPopulation[i].crossover(lambdaPopulation[lambdaPopulation.size()-1-i]);

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
            for (Individ &item : population)
                item.adaptationFunction();

            //sortowanie
            sort(population.begin(), population.end(), Individ::compare);
            population.erase(population.begin() + populationSize, population.end());

        }
        return population;
    }

    Individ GetBest() { return population[0]; }

    void shuffle()
    {
        for(auto & item : population)
            std::swap(item,population[rand()%populationSize]);
    }
};

#endif // POPULATION_H
