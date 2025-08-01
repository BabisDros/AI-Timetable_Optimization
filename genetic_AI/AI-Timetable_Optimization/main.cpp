#pragma once
#include "main.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <climits>
#include <set>
#include <unordered_set>
#include <cmath>
#include <iomanip> // for setting precision
#include <algorithm>
#include <execution>


// #define DEBUG

int main (int argc, char** argv)
{
    std::random_device                          rand_dev;// used for randomised seed
    std::mt19937_64                             generator(1);// seed
    std::uniform_int_distribution<long long>    distr(1, LLONG_MAX); //big number rand
    
    // Hyperparameters
    int POPULATION_SIZE = 40000;
    int MAX_GEN = 8000;
    long double MUTATION_PROB = 0.25L;
    if (chromosome::nGrades != 3)
    {
        std::cout << "Need to set chromosome grade to 3 to continue\n";
        exit(-1);
    }
    // Data paths
    std::string LESSON_DATA_PATH = "data/lessons.json";
    std::string TEACHER_DATA_PATH = "data/teachers.json";

    // Save results for evaluation
    std::string EVAL_METRICS_PATH = "data_analysis/evaluation_metrics.csv";
    std::ofstream evalMetricsFile(EVAL_METRICS_PATH, std::ios::trunc); // overwrite
    if (evalMetricsFile.tellp() == 0) {
        evalMetricsFile << "Generation,BestScore,AvgScore\n";
    }

    // get json data
    std::ifstream f(LESSON_DATA_PATH);
    json lessons = json::parse(f)["Lessons"];
    f.close();
    f.open(TEACHER_DATA_PATH);
    json teachers = json::parse(f)["Teachers"];
    
    std::vector<std::pair<int, int>> lessonTeacher;
    lessonTeacher.push_back(std::make_pair(-1,-1)); // free period
    for (auto teacher : teachers.items()) {
        for (auto lesson : teacher.value()["teaches"].items()) {
            lessonTeacher.push_back(std::make_pair(stoi( (std::string) lesson.value()), stoi(teacher.key())));
        }
    }

    chromosome** population = new chromosome*[POPULATION_SIZE];
    chromosome** newPopulation = new chromosome*[POPULATION_SIZE];
    chromosome** tempPopulationPtr = nullptr;

    // initialize population with randomized values
    for (int i = 0; i < POPULATION_SIZE; i++)
        population[i] = new chromosome(lessonTeacher, distr, generator);


    #ifdef DEBUG
        std::cout << "All possible (lessonID, teacherID) pairs\n";
        for (std::pair t : lessonTeacher)
        {
            std::cout << "lessons id: " << t.first << " teacher id: " << t.second << std::endl;
        }
        
        std::cout << "\nCurriculum Params: " 
            << "\nClasses per Grade: " << population[0]->nClassesPerGrade
            << "\nGrades: " << population[0]->nGrades
            << "\nDays per Week: " << population[0]->nDaysPerWeek
            << "\nHours per Day: " << population[0]->nHoursPerDay 
            << std::endl;
        std::cout << "\nCheck curriculum data of 1st chromosome. \nFormat: (curriculumIndex, lessonId, teacherId)" << std::endl;
        for (int i = 0; i < population[0]->arrSize; i++)
        {
            std::cout << i << ": (" << population[0]->curriculum[i].first << ", " << population[0]->curriculum[i].second << ")" << std::endl;
        }
    #endif // DEBUG

    
    // start of loop of new generations
    long long bestScoreOverall = 0;
    chromosome* totalBestChrom = nullptr;
    for (int gen = 0; gen < MAX_GEN; gen++)
    {
        if (gen == 400)
            MUTATION_PROB = 0.15L;
        if (gen == 800)
            MUTATION_PROB = 0.1L;
        if (gen == 1000)
            MUTATION_PROB = 0.05L;
        if (gen == 1300)
            MUTATION_PROB = 0.03L;
        if (gen == 1800)
            MUTATION_PROB = 0.02L;
        if (gen == 2200)
            MUTATION_PROB = 0.01L;
        if (gen == 2800)
            MUTATION_PROB = 0.005L;
        if (gen == 3500)
            MUTATION_PROB = 0L;
        // score evaluation
        long long totalScore = 0;
        long long bestScore = 0;

        std::for_each(std::execution::par, population, population+ POPULATION_SIZE, [&](chromosome* chrom)
            {
                scoreCalculation(chrom, lessons, teachers);
            });

        for (int i = 0; i < POPULATION_SIZE; i++) {
            #ifdef DEBUG
                std::cout << "population[" << i << "] score: " << population[i]->getScore() << " total: " << totalScore << std::endl;
            #endif // DEBUG
            totalScore += population[i]->getScore();
            population[i]->setDistribution(totalScore);
            if (population[i]->getScore() > bestScore) bestScore = population[i]->getScore();
            if (population[i]->getScore() > bestScoreOverall)
            {
                bestScoreOverall = population[i]->getScore();
                totalBestChrom = population[i];
            }
            if (totalScore < 0) exit(-2); // overflow, impossible with long long
        }

        long long avgScore = totalScore/ (long long)POPULATION_SIZE;

        // EVALUATION METRICS
        std::cout << "\n== GEN: " << gen << " ==" << std::endl;
        std::cout << "Best Score: " << bestScore << ", " << "Avg Score: " << avgScore << std::endl;
        evalMetricsFile << gen << "," << std::fixed << std::setprecision(2) << bestScore << "," << avgScore << "\n";

        // crossbreeding and mutation
        long long comparator;
        for (int i = 0; i < POPULATION_SIZE/2; i++) {
            comparator = distr(generator)% totalScore;
            chromosome* par1 = (*std::upper_bound(population, population + POPULATION_SIZE, comparator, chromosome::compareDistributionVal));
           #ifdef DEBUG
                std::cout << "Parent1: " << "Score: " << par1->getScore() << ", Comp: " << comparator << " with dis: " << par1->getDistribution() << std::endl; 
           #endif // DEBUG
            comparator = distr(generator)% totalScore;
            chromosome* par2 = (*std::upper_bound(population, population + POPULATION_SIZE, comparator, chromosome::compareDistributionVal));
            #ifdef DEBUG
                std::cout << "Parent2: " << "Score: " << par2->getScore() << ", Comp: " << comparator << " with dis: " << par2->getDistribution() << std::endl; 
            #endif // DEBUG        
            int size = (int) (distr(generator)% (long long)par1->arrSize);
            newPopulation[i] = new chromosome(par1->curriculum, size, par2->curriculum);
            newPopulation[POPULATION_SIZE / 2 + i] = new chromosome(par2->curriculum, size, par1->curriculum);

            for (int j = 0; j < par1->arrSize; j++) {
                if (distr(generator) < LLONG_MAX * MUTATION_PROB)
                    newPopulation[i]->curriculum[j] = lessonTeacher[distr(generator) % lessonTeacher.size()];
                
                if (distr(generator) < LLONG_MAX * MUTATION_PROB)
                    newPopulation[POPULATION_SIZE / 2 + i]->curriculum[j] = lessonTeacher[distr(generator) % lessonTeacher.size()];   
            }
        }
        // delete previous population, with allocated memory if needed
        for (int i = 0; i < POPULATION_SIZE; i++) {
            if (population[i] == NULL) continue;
            delete population[i];
        }
        tempPopulationPtr = population;
        population = newPopulation;
        newPopulation = tempPopulationPtr;
    }


    // print Best
    chromosome* bestGenes = population[0];
    for (int i = 0; i < POPULATION_SIZE; i++) {
        scoreCalculation(population[i], lessons, teachers);
        if (bestGenes->getScore() > population[i]->getScore()) continue;
        bestGenes = population[i];
    }
    std::cout << "Best Score (last gen) after " << MAX_GEN << " generations: " << bestGenes->getScore() << std::endl;

    
    evalMetricsFile.close();
    std::cout << "Best Score (overall) after " << MAX_GEN << " generations: " << bestScoreOverall << std::endl;
    std::cout << "Pass to eval.py to calculate Constraint0 percentage: " << totalBestChrom->percent << std::endl;


    //jsonUseExample(lessons, teachers);

    //Memory cleanup
    for (int i = 0; i < POPULATION_SIZE; i++)
    {
        if (population[i] == NULL) continue;
        delete population[i];
    }
    delete[] population;
    delete[] newPopulation;
    
}

void jsonUseExample(json &lessons, json &teachers)
{


    for (auto lesson : lessons.items())
    {
        std::cout << lesson.value()["name"] << std::endl;
        for (auto classYear : lesson.value()["classes"].items())
        {
            std::cout << "Year: " << classYear.value()["year"] << " hours: " << classYear.value()["hours"] << std::endl;
        }
    }

    for (auto teacher : teachers.items())
    {
        std::cout << teacher.key() << " is named " << teacher.value()["name"] << std::endl;
        for (auto teaching : teacher.value()["teaches"].items())
        {
            std::cout << "Teaches " << lessons[teaching.value()]["name"] << std::endl;
        }
    }
}