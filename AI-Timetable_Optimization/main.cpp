#include "main.h"
#include "chromosome.h"
#include <iostream>
#include <fstream>
#include <filesystem>

int main (int argc, char** argv)
{
    int populationSize = 100;
    // get json data
    std::ifstream f("data\\lessons.json");
    json lessons = json::parse(f)["Lessons"];
    f.close();
    f.open("data\\teachers.json");
    json teachers = json::parse(f)["Teachers"];
    
    //id to name correlation
    std::map<int, std::string> lessonsIDs;

    for (auto lesson: lessons.items())
    {
        lessonsIDs[lesson.value()["id"]] = lesson.key();
    }

    chromosome* population = new chromosome[populationSize];
    chromosome* tempNewPopulation = new chromosome[populationSize];

    /* initialize population with randomized values

    */

    /* start of loop of new generations
    
    */

    /* crossbreeding
    
    */

    /* delete previous population, with allocated memory if needed
    
    */

    population = tempNewPopulation;
    tempNewPopulation = nullptr;
    

    /* end of loop of new generations

    */


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
            std::cout << "Class: " << classYear.value()["class"] << " hours: " << classYear.value()["hours"] << std::endl;
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

