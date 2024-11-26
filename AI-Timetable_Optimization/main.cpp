#include "main.h"
#include <iostream>
#include <fstream>
#include <filesystem>
#include <random>
#include <climits>

//#define DEBUG

int main (int argc, char** argv)
{
    std::random_device                  rand_dev;
    std::mt19937                        generator(1);// seed
    std::uniform_int_distribution<>     distr(1, INT_MAX); //big number rand
    std::srand(1);// set seed for testing
    int populationSize = 10;
    int maxGen = 20;

    // get json data
    std::ifstream f("data/lessons.json");
    json lessons = json::parse(f)["Lessons"];
    f.close();
    f.open("data/teachers.json");
    json teachers = json::parse(f)["Teachers"];
    
    std::vector<std::pair<int, int>> lessonTeacher;
    lessonTeacher.push_back(std::make_pair(-1,-1)); // free period


    for (auto teacher : teachers.items())
    {
        for (auto lesson : teacher.value()["teaches"].items())
        {
            lessonTeacher.push_back(std::make_pair(stoi( (std::string) lesson.value()), stoi(teacher.key())));
        }
    }

    chromosome** population = new chromosome*[populationSize];
    chromosome** newPopulation = new chromosome*[populationSize];
    chromosome** tempPopulationPtr = nullptr;

    /* initialize population with randomized values

    */
    for (int i = 0; i < populationSize; i++)
        population[i] = new chromosome(lessonTeacher);


    #ifdef DEBUG
        std::cout << "All pairs\n";
        for (std::pair t : lessonTeacher)
        {
            std::cout << "lessons id: " << t.first << " teacher id: " << t.second << std::endl;
        }

        std::cout << "Check curriculumn data of 1st chromosome" << std::endl;
        for (int i = 0; i < population[0]->arrSize; i++)
        {
            std::cout << i << ": lesson id: " << population[0]->curriculumn[i].first << " teacher id: " << population[0]->curriculumn[i].second << std::endl;
        }
    #endif // DEBUG

    
    /* start of loop of new generations
    
    */
    for (int gen = 0; gen < maxGen; gen++)
    {

        /* score evaluation
        
        */
        int totalScore = 0;
        for (int i = 0; i < populationSize; i++)
        {
            scoreCalculation(population[i], lessons, teachers);
            totalScore += population[i]->getScore();
            population[i]->setDistribution(totalScore);
        }



        /* crossbreeding and mutation

        */

        int comparator;
        for (int i = 0; i < populationSize/2; i++)
        {
            comparator = distr(generator)% totalScore;
            chromosome* par1 = (*std::upper_bound(population, population + populationSize - 1, comparator, chromosome::compareDistributionVal));
           #ifdef DEBUG
                std::cout << "Comp: " << comparator << " |  Score: " << par1->getScore() << " with dis: " << par1->getDistribution() << std::endl;
           #endif // DEBUG

            comparator = distr(generator)% totalScore;
            chromosome* par2 = (*std::upper_bound(population, population + populationSize - 1, comparator, chromosome::compareDistributionVal));
            #ifdef DEBUG
                std::cout << "Comp: " << comparator << " |  Score: " << par2->getScore() << " with dis: " << par2->getDistribution() << std::endl;
            #endif // DEBUG        
            int size = distr(generator)% par1->arrSize;
            newPopulation[i] = new chromosome(par1->curriculumn, size, par2->curriculumn);
            newPopulation[populationSize / 2 + i] = new chromosome(par2->curriculumn, size, par1->curriculumn);

            for (int j = 0; j < par1->arrSize; j++)
            {
                if (distr(generator) < INT_MAX * 0.15)
                    newPopulation[i]->curriculumn[j] = lessonTeacher[distr(generator) % lessonTeacher.size()];
                
                if (distr(generator) < INT_MAX * 0.15) 
                    newPopulation[populationSize / 2 + i]->curriculumn[j] = lessonTeacher[distr(generator) % lessonTeacher.size()];   
            }
        }



        /* delete previous population, with allocated memory if needed

        */
        for (int i = 0; i < populationSize; i++)
        {
            if (population[i] == NULL) continue;
            delete population[i];
        }

        tempPopulationPtr = population;
        population = newPopulation;
        newPopulation = tempPopulationPtr;

    }

    // print Best
    chromosome* bestGenes = population[0];
    for (int i = 0; i < populationSize; i++)
    {
        scoreCalculation(population[i], lessons, teachers);
        if (bestGenes->getScore() > population[i]->getScore()) continue;
        bestGenes = population[i];
    }




    //jsonUseExample(lessons, teachers);

    //Memory cleanup
    for (int i = 0; i < populationSize; i++)
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

void scoreCalculation(chromosome* chrom, json& lessons, json& teachers)
{
    /* each lesson must appear x,y,z times for all classes, variable scoring
    
    */
    std::map <int, int> counterLH; // hold hours for each lesson
    std::string classYear[3] = { "A", "B", "C" };
    int year = -1;
    bool added;
    int evalVariance = 0;
    int total = 0;

    for (int i = 0; i < chrom->arrSize; i++)
    {
        if (i % (35 * 3) == 0) // year change
            year++;
        if (i % 35 == 0) // class change
        {                  
            for (auto lesson : lessons.items())
            {
                added = false;
                for (int k = 0; k < lesson.value()["classes"].size(); k++)
                {
                    if (lesson.value()["classes"][k]["year"] == classYear[year])
                    {
                        counterLH.insert({stoi(lesson.key()), (int)lesson.value()["classes"][k]["hours"]}); // times it must appear
                        added = true;
                        break;
                    }
                }
                if (!added)
                    counterLH.insert({ stoi(lesson.key()), 0}); // does not appear in year
            }
        }


        if (chrom->curriculumn[i].first != -1) // ignore free period
        { 
            counterLH[chrom->curriculumn[i].first] -= 1;
        }
        
        if (i % 35 == 34)
        {
            #ifdef DEBUG
                std::cout << "Year: " << year + 1 << " Class: " << (i/35) % 3 + 1 << " | ";
            #endif // DEBUG
            for (auto it = counterLH.begin(); it != counterLH.end(); it++)
            {
                #ifdef DEBUG
                    std::cout << it->second << " | ";
                #endif // DEBUG

               
                if (it->second == 0) // can have more cases depending on how close it was
                    evalVariance += 1;
                else
                    evalVariance -= 1;
                total++;
            }

            #ifdef DEBUG
                std::cout << std::endl;
            #endif // DEBUG


            if (evalVariance <= -50)
                chrom->addScore(1);
            else if (evalVariance <= -10)
                chrom->addScore(5);
            else if (evalVariance <= 0)
                chrom->addScore(10);
            else if (evalVariance <= total*0.3)
                chrom->addScore(30);
            else if (evalVariance <= total*0.5)
                chrom->addScore(80);
            else if (evalVariance <= total*0.8)
                chrom->addScore(200);
            else if (evalVariance < total)
                chrom->addScore(400);
            else if (evalVariance == total) // all lessons appear the specified hours
                chrom->addScore(10000);

            counterLH.clear();
        }
    }
    

    /* each teacher can not teach more than the daily/weekly limit, static scoring
    
    */
    std::map <std::string, std::pair<int, int>> teacherDayWeek;
    total = 0;
    for (auto teacher : teachers.items())
    {
        teacherDayWeek.insert({teacher.key(),
            std::make_pair( (int)teacher.value()["hoursPerDay"], (int)teacher.value()["hoursPerWeek"] )});
    }

    int hour = 0;
    int classvar = 0;
    int counter = 0;
    for (int i = 0; i < chrom->arrSize; i++)
    {
        #ifdef DEBUG
        std::cout << "Now teaches: " << chrom->curriculumn[hour + classvar].second <<std::endl;
        std::cout << "i: " << i << " cell: " << hour + classvar << std::endl;
        #endif // DEBUG

        if (chrom->curriculumn[hour + classvar].second != -1) // ignore free period
        {
            teacherDayWeek[std::to_string(chrom->curriculumn[hour + classvar].second)].first -= 1;
            teacherDayWeek[std::to_string(chrom->curriculumn[hour + classvar].second)].second -= 1;
        }

        hour++;

        if (hour >= 7)
        { 
            hour = 0;
            classvar += 35; // same day, next class
        }
        
        if (classvar >= chrom->arrSize) // use data, go next day
        {
            for (auto itr = teacherDayWeek.begin(); itr != teacherDayWeek.end(); itr++)
            {
                #ifdef DEBUG
                    std::cout << itr->first << " can teach " << itr->second.first << " more hours" << std::endl;
                #endif // DEBUG
                if (itr->second.first < 0)
                    counter++;
                total++;

                itr->second.first = teachers[itr->first]["hoursPerDay"]; //reset value
            }
            classvar = (classvar + 7) % 35;
        }
    }

    for (auto itr = teacherDayWeek.begin(); itr != teacherDayWeek.end(); itr++)
    {
        if (itr->second.second < 0)
            counter++; // or more
        total++;// increase by same amount
    }
 
    if (counter == 0)
        chrom->addScore(10000);
    else if (counter <= total*0.05)
        chrom->addScore(1000);
    else if (counter <= total*0.2)
        chrom->addScore(400);
    else if (counter <= total*0.4)
        chrom->addScore(200);
    else if (counter <= total*0.7)
        chrom->addScore(50);
    else if (counter <= total*0.85)
        chrom->addScore(10);
    else if (counter == total)
        chrom->addScore(1);


    /* a teacher can't be at the same time in 2 different classes, static scoring {tip: check every 35 cells, memory use recommended}
    
    */

    /* no free periods between classes, static scoring
    
    */

    /* a teacher shouldn't teach for more than 2 hours in a row, variable scoring
    
    */

    /* each classes programm should be uniformly spread out throughout the week, variable scoring
    
    */

    /* each lesson for each class should be uniformly spread out throughout the week, static scoring
    
    */

    /* all teacher should be teacher per week around the same amount of times, variable scoring
    
    */
}

