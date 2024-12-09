#include "score.h"

void scoreCalculation(chromosome* chrom, json& lessons, json& teachers) {

    /* each lesson must appear x,y,z times for all classes, variable scoring
    
    */
    // std::map <int, int> counterLH; // hold hours for each lesson
    // std::string classYear[3] = { "A", "B", "C" };
    // int year = -1;
    // bool added;
    // int evalVariance = 0;
    // int total = 0;

    // for (int i = 0; i < chrom->arrSize; i++)
    // {
    //     if (i % (35 * 3) == 0) // year change
    //         year++;
    //     if (i % 35 == 0) // class change
    //     {                  
    //         for (auto lesson : lessons.items())
    //         {
    //             added = false;
    //             for (int k = 0; k < lesson.value()["classes"].size(); k++)
    //             {
    //                 if (lesson.value()["classes"][k]["year"] == classYear[year])
    //                 {
    //                     counterLH.insert({stoi(lesson.key()), (int)lesson.value()["classes"][k]["hours"]}); // times it must appear
    //                     added = true;
    //                     break;
    //                 }
    //             }
    //             if (!added)
    //                 counterLH.insert({ stoi(lesson.key()), 0}); // does not appear in year
    //         }
    //     }


    //     if (chrom->curriculum[i].first != -1) // ignore free period
    //     { 
    //         counterLH[chrom->curriculum[i].first] -= 1;
    //     }
        
    //     if (i % 35 == 34)
    //     {
    //         #ifdef DEBUG
    //             std::cout << "Year: " << year + 1 << " Class: " << (i/35) % 3 + 1 << " | ";
    //         #endif // DEBUG
    //         for (auto it = counterLH.begin(); it != counterLH.end(); it++)
    //         {
    //             #ifdef DEBUG
    //                 std::cout << it->second << " | ";
    //             #endif // DEBUG


    //             if (it->second == 0) // can have more cases depending on how close it was
    //                 evalVariance += 1;
    //             else
    //                 evalVariance -= 1;
    //             total++;
    //         }

    //         #ifdef DEBUG
    //             std::cout << std::endl;
    //         #endif // DEBUG


    //         if (evalVariance <= -50)
    //             chrom->addScore(1);
    //         else if (evalVariance <= -10)
    //             chrom->addScore(5);
    //         else if (evalVariance <= 0)
    //             chrom->addScore(10);
    //         else if (evalVariance <= total*0.3)
    //             chrom->addScore(30);
    //         else if (evalVariance <= total*0.5)
    //             chrom->addScore(80);
    //         else if (evalVariance <= total*0.8)
    //             chrom->addScore(200);
    //         else if (evalVariance < total)
    //             chrom->addScore(400);
    //         else if (evalVariance == total) // all lessons appear the specified hours
    //             chrom->addScore(10000);

    //         counterLH.clear();
    //     }
    // }
    

    // // each teacher can not teach more than the daily/weekly limit, static scoring
    // std::map <std::string, std::pair<int, int>> teacherDayWeek;
    // total = 0;
    // for (auto teacher : teachers.items())
    // {
    //     teacherDayWeek.insert({teacher.key(),
    //         std::make_pair( (int)teacher.value()["hoursPerDay"], (int)teacher.value()["hoursPerWeek"] )});
    // }

    // int hour = 0;
    // int classvar = 0;
    // int nExceededLimit = 0;
    // for (int i = 0; i < chrom->arrSize; i++)
    // {
    //     #ifdef DEBUG
    //     std::cout << "Now teaches: " << chrom->curriculum[hour + classvar].second <<std::endl;
    //     std::cout << "i: " << i << " cell: " << hour + classvar << std::endl;
    //     #endif // DEBUG

    //     if (chrom->curriculum[hour + classvar].second != -1) // ignore free period
    //     {
    //         teacherDayWeek[std::to_string(chrom->curriculum[hour + classvar].second)].first -= 1;
    //         teacherDayWeek[std::to_string(chrom->curriculum[hour + classvar].second)].second -= 1;
    //     }

    //     hour++;

    //     if (hour >= 7)
    //     { 
    //         hour = 0;
    //         classvar += 35; // same day, next class
    //     }
        
    //     if (classvar >= chrom->arrSize) // use data, go next day
    //     {
    //         for (auto itr = teacherDayWeek.begin(); itr != teacherDayWeek.end(); itr++)
    //         {
    //             #ifdef DEBUG
    //                 std::cout << itr->first << " can teach " << itr->second.first << " more hours" << std::endl;
    //             #endif // DEBUG
    //             if (itr->second.first < 0)
    //                 nExceededLimit++;
    //             total++;

    //             itr->second.first = teachers[itr->first]["hoursPerDay"]; //reset value
    //         }
    //         classvar = (classvar + 7) % 35;
    //     }
    // }

    // for (auto itr = teacherDayWeek.begin(); itr != teacherDayWeek.end(); itr++)
    // {
    //     if (itr->second.second < 0)
    //         nExceededLimit++; // or more
    //     total++;// increase by same amount
    // }
 
    // if (nExceededLimit == 0)
    //     chrom->addScore(10000);
    // else if (nExceededLimit <= total*0.05)
    //     chrom->addScore(1000);
    // else if (nExceededLimit <= total*0.2)
    //     chrom->addScore(400);
    // else if (nExceededLimit <= total*0.4)
    //     chrom->addScore(200);
    // else if (nExceededLimit <= total*0.7)
    //     chrom->addScore(50);
    // else if (nExceededLimit <= total*0.85)
    //     chrom->addScore(10);
    // else if (nExceededLimit == total)
    //     chrom->addScore(1);


    // a teacher can't be at the same time in 2 different classes, static scoring
    int nDaysPerWeek = chrom->nDaysPerWeek;
    int nHoursPerDay = chrom->nHoursPerDay; 
    int nClassesPerGrade = chrom->nClassesPerGrade;
    int nGrades = chrom->nGrades; 

    // int conflicts = 0;

    // for (int day = 0; day < nDaysPerWeek; day++) {
    //     for (int hour = 0; hour < nHoursPerDay; hour++) {
    //         std::set<int> teacherSet; // teachers for this (day, hour)
    //         for (int grade = 0; grade < nGrades; grade++) {
    //             for (int cls = 0; cls < nClassesPerGrade; cls++) {
    //                 int index = chrom->calculateIndex(cls, grade, day, hour);
    //                 int teacherID = chrom->curriculum[index].second;

    //                 // Check if teacher is already teaching
    //                 // .find() returns .end() if the element is not found
    //                 if (teacherSet.find(teacherID) == teacherSet.end()) {
    //                     teacherSet.insert(teacherID);
    //                 } else {
    //                     conflicts++;
    //                 }
    //             }
    //         }
    //     }
    // }

    // if (conflicts == 0)
    //     chrom->addScore(10000);
    // else {
    //     chrom->addScore(-1 * conflicts);
    // }

    // no free periods between classes, static scoring

    // loop through the curriculum, if (-1, -1) is found (free period) add penalty
    // int freePeriods = 0;
    // for (int i = 0; i < sizeof(chrom->curriculum); i++) {
    //     std::pair<int, int> lessonTeacherPair = chrom->curriculum[i];
    //     if (lessonTeacherPair.first == -1 && lessonTeacherPair.second == -1) freePeriods++;
    // }

    // if (freePeriods == 0) {
    //     chrom->addScore(10000);
    // }
    // else {
    //     int penaltyMultiplier = -5;
    //     chrom->addScore( penaltyMultiplier * freePeriods);
    // }

    // a teacher shouldn't teach for more than 2 hours in a row, variable scoring
    // int moreThanThreeHours = 0;
    // for (int i = 0; i < sizeof(chrom->curriculum); i++) {
    //     int teacherId1 = chrom->curriculum[i].second;
    //     int teacherId2 = chrom->curriculum[i+1].second;
    //     int teacherId3 = chrom->curriculum[i+2].second;
    //     if (teacherId1 == teacherId2 && teacherId2 == teacherId3) {
    //         moreThanThreeHours++;
    //     }
    // }

    // if (moreThanThreeHours == 0) {
    //     chrom->addScore(10000);
    // }
    // else {
    //     int penaltyMultiplier = -5;
    //     chrom->addScore( penaltyMultiplier * moreThanThreeHours);
    // }

    // each class program should be uniformly spread out throughout the week, variable scoring
    // std::cout << "\nEACH CLASS PROGRAM SHOULD BE UNIFORMLY SPREAD THROUGHOUT THE WEEK" << std::endl;
    double totalUniformityScore = 0;
    for (int cls = 0; cls < nClassesPerGrade; cls++) {
        for (int grade = 0; grade < nGrades; grade++) {
            // For each a given unique (class, grade) calculate the total hours per day this week
            int hoursPerDayArray[nDaysPerWeek] = {0};
            for (int day = 0; day < nDaysPerWeek; day++) {
                for (int hour = 0; hour < nHoursPerDay; hour++) {
                    int index = chrom->calculateIndex(cls, grade, day, hour);
                    std::pair<int, int> lessonTeacherPair = chrom->curriculum[index];
                    if (lessonTeacherPair.first == -1 && lessonTeacherPair.second == -1) continue;
                    hoursPerDayArray[day]++;
                }
            }

            // hoursPerDayUniformityScore for a unique class i.e. (cls, grade) pair
            double uniqueClassHoursPerDayUniformityScore = hoursPerDayUniformityScore(hoursPerDayArray, nHoursPerDay, nDaysPerWeek); // 0 - 100
            totalUniformityScore += uniqueClassHoursPerDayUniformityScore;
    
            // DEBUG: Output the score
            #ifdef DEBUG
            std::cout << "(cls, grade): (" << cls << ", " << grade << ") "
                    << "Uniformity Score: " << uniformityScore;
            std::cout << ", hoursPerDayArray: [";
            for (int day = 0; day < nDaysPerWeek; day++) {
                std::cout << hoursPerDayArray[day] << ", ";
            }
            std::cout << "]" << std::endl;
            #endif
            }
    }

    double avgUniformityScore = totalUniformityScore / (nClassesPerGrade*nGrades); // unique classes


    /* each lesson for each class should be uniformly spread out throughout the week, static scoring
    
    */

    /* all teacher should be teacher per week around the same amount of times, variable scoring
    
    */

   double totalScore = avgUniformityScore;
   chrom->addScore(totalScore);
}

double hoursPerDayUniformityScore(int hoursPerDay[], int maxHoursPerDay, int nDaysPerWeek) {
    int maxHours = *std::max_element(hoursPerDay, hoursPerDay + nDaysPerWeek);
    int minHours = *std::min_element(hoursPerDay, hoursPerDay + nDaysPerWeek);
    int diff = maxHours - minHours;

    // Normalize the difference to a score between 0 and 100
    int maxPossibleDiff = maxHoursPerDay;
    double score = (1.0 - (static_cast<double>(diff) / maxPossibleDiff)) * 100;
    return score;
}