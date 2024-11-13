#pragma once
#include "main.h"
#include "chromosome.h"
#include <iostream>
#include <fstream>
#include <filesystem>

//#define DEBUG

int main (int argc, char** argv)
{
    std::srand(1);// set seed for testing
    int populationSize = 10;
    int maxGen = 20;

    // get json data
    std::ifstream f("data\\lessons.json");
    json lessons = json::parse(f)["Lessons"];
    f.close();
    f.open("data\\teachers.json");
    json teachers = json::parse(f)["Teachers"];
    
    //id to name correlation
    std::map<int, std::string> lessonsIDs;
    std::vector<std::pair<int, int>> lessonTeacher;
    lessonTeacher.push_back(std::make_pair(-1,-1)); // free period

    for (auto lesson: lessons.items())
    {
        lessonsIDs[lesson.value()["id"]] = lesson.key();
    }

    for (auto teacher : teachers.items())
    {
        for (auto lesson : teacher.value()["teaches"].items())
        {
            lessonTeacher.push_back(std::make_pair(lesson.value(), teacher.value()["id"]));
        }
    }

    chromosome** population = new chromosome*[populationSize];
    chromosome** tempNewPopulation = new chromosome*[populationSize];

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
        for (int i = 0; i < populationSize; i++)
        {
            scoreCalculation(population[i], lessons, teachers);
        }



        /* crossbreeding

        */


        /* mutation
        
        */


        /* delete previous population, with allocated memory if needed

        */

        //population = tempNewPopulation;
        //tempNewPopulation = nullptr;


        /* end of loop of new generations

        */



    }

    


   // jsonUseExample(lessons, teachers, lessonsIDs);

}

void jsonUseExample(json &lessons, json &teachers, std::map<int, std::string> &lessonsIDs)
{
    std::map<int, std::string>::iterator itr;
    for (itr = lessonsIDs.begin(); itr != lessonsIDs.end(); itr++)
    {
        std::cout << "Name: " << itr->second << " ID: " << itr->first << std::endl;
    }

    for (auto lesson : lessons.items())
    {
        std::cout << lesson.key() << std::endl;
        for (auto classYear : lesson.value()["classes"].items())
        {
            std::cout << "Year: " << classYear.value()["year"] << " hours: " << classYear.value()["hours"] << std::endl;
        }
    }

    for (auto teacher : teachers.items())
    {
        std::cout << teacher.key() << " has id " << teacher.value()["id"] << std::endl;
        for (auto teaching : teacher.value()["teaches"].items())
        {
            std::cout << "Teaches " << lessonsIDs[teaching.value()] << std::endl;
        }
    }
}

void scoreCalculation(chromosome* chrom, json& lessons, json& teachers)
{
    /* each lesson must appear x,y,z times for all classes, variable scoring
    
    */
    std::map <int, int> counter;
    std::string classYear[3] = { "A", "B", "C" };
    int year = -1;
    bool added;
    int evalVariance = 0;

    for (int i = 0; i < chrom->arrSize; i++)
    {
        if (i % (35 * 3) == 0) // year change
            year++;
        if (i % 35 == 0) // class change
        {                  
            for (auto lesson : lessons)
            {
                added = false;
                for (int k = 0; k < lesson["classes"].size(); k++)
                {
                    if (lesson["classes"][k]["year"] == classYear[year])
                    {
                        counter.insert({(int)lesson["id"], (int)lesson["classes"][k]["hours"]}); // times it must appear
                        added = true;
                        break;
                    }
                }
                if (!added)
                    counter.insert({ (int)lesson["id"], -1000}); // does not appear in year
            }
        }


        if (chrom->curriculumn[i].first != -1)
        { 
            counter[chrom->curriculumn[i].first] -= 1;
        }
        
        if (i % 35 == 34)
        {
            for (auto it = counter.begin(); it != counter.end(); it++)
            {
                #ifdef DEBUG
                    std::cout << it->second << " | ";
                #endif // DEBUG

               
                if (it->second == 0) // can have more cases depending on how close it was
                    evalVariance += 1;
                else
                    evalVariance -= 1;
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
            else if (evalVariance <= 10)
                chrom->addScore(30);
            else if (evalVariance <= 50)
                chrom->addScore(80);
            else if (evalVariance <= 100)
                chrom->addScore(200);
            else
                chrom->addScore(400);
           
            counter.clear();
        }
    }
    

    /* each teacher can not teach more than the daily/weekly limit, static scoring
    
    */

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

