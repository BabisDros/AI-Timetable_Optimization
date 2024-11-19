#pragma once
#include "main.h"
#include "chromosome.h"
#include <iostream>
#include <fstream>
#include <filesystem>

#define DEBUG

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

    


    //jsonUseExample(lessons, teachers);

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
            //std::cout << "Teaches " << lessons[teaching.value()]["name"] << std::endl;
            std::cout << "Teaches " << lessons[teaching.value()]["name"] << std::endl;
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
            for (auto lesson : lessons.items())
            {
                added = false;
                for (int k = 0; k < lesson.value()["classes"].size(); k++)
                {
                    if (lesson.value()["classes"][k]["year"] == classYear[year])
                    {
                        counter.insert({stoi(lesson.key()), (int)lesson.value()["classes"][k]["hours"]}); // times it must appear
                        added = true;
                        break;
                    }
                }
                if (!added)
                    counter.insert({ stoi(lesson.key()), 0}); // does not appear in year
            }
        }


        if (chrom->curriculumn[i].first != -1) // ignore free period
        { 
            counter[chrom->curriculumn[i].first] -= 1;
        }
        
        if (i % 35 == 34)
        {
            std::cout << "Year: " << year + 1 << " Class: " << (i/35) % 3 + 1 << std::endl;
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
    
    
    std::map <int, std::pair<int, int>> teacherDayWeek;

    for (auto teacher : teachers.items())
    {
        teacherDayWeek.insert({ stoi(teacher.key()),
            std::make_pair( (int)teacher.value()["hoursPerDay"], (int)teacher.value()["hoursPerWeek"] )});
    }

    int hour = 0;
    int classvar = 0;
    for (int i = 0; i < chrom->arrSize; i++)
    {
        if (chrom->curriculumn[hour + classvar].second != -1) // ignore free period
        {
            teacherDayWeek[chrom->curriculumn[hour + classvar].second].first -= 1;
            teacherDayWeek[chrom->curriculumn[hour + classvar].second].second -= 1;
        }


        hour++;

        if (hour >= 7)
        { 
            hour = 0;
            classvar += 35; // same day, next class
        }

        if (classvar >= chrom->arrSize) // use data, go next day
        {
            int counter = 0;
            for (auto itr = teacherDayWeek.begin(); itr != teacherDayWeek.end(); itr++)
            {
                if (itr->second.first < 0)
                    counter++;
               // itr->second.first = teachers
            }

        }
    }
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

