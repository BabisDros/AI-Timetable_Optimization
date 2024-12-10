#include "score.h"
#include <string>

void scoreCalculation(chromosome* chrom, json& lessons, json& teachers) {

    double totalScore = 0;

    // each lesson must appear x,y,z times for all classes, variable scoring
    totalScore += calculateSatisfyLessonHoursScore(chrom, lessons);

    // each teacher can not teach more than the daily/weekly limit, static scoring
    totalScore += calculateDailyWeeklyLimitScore(chrom, teachers);

    // a teacher can't be at the same time in 2 different classes, static scoring
    totalScore += calculateTeacherConflictScore(chrom);

    // no free periods between classes, variable scoring
    totalScore += calculateNoFreePeriodsScore(chrom);

    // a teacher shouldn't teach for more than 2 hours in a row, variable scoring
    totalScore += calculateConsecutiveHoursScore(chrom);

    // each class hours per day total should be uniformly spread out throughout the week, variable scoring
    totalScore += calculateAverageUniformityScore(chrom);

    // each lesson for each class should be uniformly spread out throughout the week, static scoring
    

    // all teachers should teach around the same hours per week, variable scoring
    totalScore += teachSimilarHoursPerWeek(chrom, teachers);

   chrom->addScore(totalScore);
}

double calculateSatisfyLessonHoursScore(chromosome* chrom, json& lessons) {
    // Each lesson has a required amount of hours for each grade.
    // eg. lesson_id = 1 : A=3, B=2, C=2, etc.
    // This info can be obtained from the json data.
    // Maximum Error: Total Required Hours in Curriculum = nGrades * nClassesPerGrade
    std::map<int, int> lessonID_totalRequiredHours; // total required hours for each lesson based on json data
    int allHours = 0;
    for (auto& lesson : lessons.items()) {
        int lessonID = std::stoi(lesson.key());
        auto& classes = lesson.value()["classes"];
        
        int totalHours = 0;
        for (int i=0; i < classes.size(); i++) {
            auto& yearHours = classes[i];
            int hours = yearHours["hours"];
            totalHours += hours;
            allHours += hours;
        }
        lessonID_totalRequiredHours[lessonID] = totalHours;
        // if (lessonID == 1) std::cout << "Lesson ID: " << lessonID << " Total Required Hours: " << totalHours << std::endl;
    }

    // Calculate the actual amount of hours per lesson in the chromosome curriculum
    // For each lesson, get the difference (can be either negative for underrepresented lessons, or positive for overrepresented lessons)
    // Score based on the absolute of the difference
    for (int i = 0; i < chrom->arrSize; i++) {
        std::pair<int, int> lessonTeacher = chrom->curriculum[i];
        int lessonID = lessonTeacher.first;
        if (lessonID == -1) continue;
        lessonID_totalRequiredHours[lessonID] -= 1;
        
    }

    // std::cout << "hoursLeft for l1: " << lessonID_totalRequiredHours[1] << std::endl;

    int totalErrors = 0;
    for (auto& lessonRH : lessonID_totalRequiredHours) {
        int hoursLeft = lessonRH.second;
        totalErrors += std::abs(hoursLeft);
    }
    // std::cout << "Errors: " << totalErrors << std::endl;
    double totalPossibleViolations = chrom->arrSize;

    int score = 0;
    if (totalErrors <= -50)
        score = 1;
    else if (totalErrors <= -10)
        score = 5;
    else if (totalErrors <= 0)
        score = 10;
    else if (totalErrors <= totalPossibleViolations*0.3)
        score = 30;
    else if (totalErrors <= totalPossibleViolations*0.5)
        score = 80;
    else if (totalErrors <= totalPossibleViolations*0.8)
        score = 200;
    else if (totalErrors < totalPossibleViolations)
        score = 400;
    else if (totalErrors == totalPossibleViolations) // all lessons appear the specified hours
        score = 1000;
    return score;
}


double calculateDailyWeeklyLimitScore(chromosome* chrom, json& teachers) {
    std::map <std::string, std::pair<int, int>> teacherDayWeek;
    int total = 0;
    for (auto teacher : teachers.items()) {
        teacherDayWeek.insert({teacher.key(),
            std::make_pair( (int)teacher.value()["hoursPerDay"], (int)teacher.value()["hoursPerWeek"] )});
    }

    int hour = 0;
    int classvar = 0;
    int nExceededLimit = 0;
    for (int i = 0; i < chrom->arrSize; i++) {
        #ifdef DEBUG
        std::cout << "Now teaches: " << chrom->curriculum[hour + classvar].second <<std::endl;
        std::cout << "i: " << i << " cell: " << hour + classvar << std::endl;
        #endif // DEBUG

        if (chrom->curriculum[hour + classvar].second != -1) // ignore free period
        {
            teacherDayWeek[std::to_string(chrom->curriculum[hour + classvar].second)].first -= 1;
            teacherDayWeek[std::to_string(chrom->curriculum[hour + classvar].second)].second -= 1;
        }

        hour++;

        if (hour >= 7) { 
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
                    nExceededLimit++;
                total++;

                itr->second.first = teachers[itr->first]["hoursPerDay"]; //reset value
            }
            classvar = (classvar + 7) % 35;
        }
    }

    for (auto itr = teacherDayWeek.begin(); itr != teacherDayWeek.end(); itr++)
    {
        if (itr->second.second < 0)
            nExceededLimit++; // or more
        total++;// increase by same amount
    }
    
    return (nExceededLimit>0) ? 0 : 1000;
}

double calculateTeacherConflictScore(chromosome* chrom) {
    // a teacher can't be at the same time in 2 different classes
    int nDaysPerWeek = chrom->nDaysPerWeek;
    int nHoursPerDay = chrom->nHoursPerDay; 
    int nClassesPerGrade = chrom->nClassesPerGrade;
    int nGrades = chrom->nGrades; 
    int conflicts = 0;

    for (int day = 0; day < nDaysPerWeek; day++) {
        for (int hour = 0; hour < nHoursPerDay; hour++) {
            std::set<int> teacherSet; // teachers for this (day, hour)
            for (int grade = 0; grade < nGrades; grade++) {
                for (int cls = 0; cls < nClassesPerGrade; cls++) {
                    int index = chrom->calculateIndex(cls, grade, day, hour);
                    int teacherID = chrom->curriculum[index].second;

                    // Check if teacher is already teaching
                    // .find() returns .end() if the element is not found
                    if (teacherSet.find(teacherID) == teacherSet.end()) {
                        teacherSet.insert(teacherID);
                    } else {
                        conflicts++;
                    }
                }
            }
        }
    }

    return (conflicts>0) ? 0 : 1000;
}

double calculateNoFreePeriodsScore(chromosome* chrom) {
    // loop through the curriculum, if (-1, -1) is found (free period) add penalty
    int freePeriods = 0;
    for (int i = 0; i < chrom->arrSize; i++) {
        std::pair<int, int> lessonTeacherPair = chrom->curriculum[i];
        if (lessonTeacherPair.first == -1 && lessonTeacherPair.second == -1) freePeriods++;
    }

    double score = 100.0;
    int totalLessons = chrom->arrSize;
    double totalPossibleViolations = totalLessons;
    double penaltyPerViolation = 100.0 / totalPossibleViolations;
    score -= freePeriods * penaltyPerViolation;

    if (score<0) score = 0;
    return score;
}

double calculateConsecutiveHoursScore(chromosome* chrom) {
    int moreThanThreeHours = 0;
    int totalLessons = chrom->arrSize;

    for (int i = 0; i < totalLessons-2; i++) {
        int teacherId1 = chrom->curriculum[i].second;
        int teacherId2 = chrom->curriculum[i+1].second;
        int teacherId3 = chrom->curriculum[i+2].second;
        if (teacherId1 == teacherId2 && teacherId2 == teacherId3) {
            moreThanThreeHours++;
        }
    }

    // TODO: Maybe use step scoring
    double score = 100.0;
    double totalPossibleViolations = totalLessons / 3.0;
    double penaltyPerViolation = 100.0 / totalPossibleViolations;
    score -= moreThanThreeHours * penaltyPerViolation;

    if (score<0) score = 0;
    return score;
}

double calculateAverageUniformityScore(chromosome* chrom) {
    // each class program should be uniformly spread out throughout the week, variable scoring    
    int nDaysPerWeek = chrom->nDaysPerWeek;
    int nHoursPerDay = chrom->nHoursPerDay; 
    int nClassesPerGrade = chrom->nClassesPerGrade;
    int nGrades = chrom->nGrades; 

    double totalUniformityScore = 0;

    for (int cls = 0; cls < nClassesPerGrade; ++cls) {
        for (int grade = 0; grade < nGrades; ++grade) {
            int hoursPerDay[nDaysPerWeek] = {0}; // For each unique (class, grade) calculate the total hours per day for each day of the week.
            for (int day = 0; day < nDaysPerWeek; ++day) {
                for (int hour = 0; hour < nHoursPerDay; ++hour) {
                    int index = chrom->calculateIndex(cls, grade, day, hour);
                    const auto& lessonTeacherPair = chrom->curriculum[index];
                    if (lessonTeacherPair.first != -1 || lessonTeacherPair.second != -1) {
                        hoursPerDay[day]++;
                    }
                }
            }

            // Calculate the uniformity score for this unique (class, grade).
            double uniformityScore = hoursPerDayUniformityScore(hoursPerDay, nHoursPerDay, nDaysPerWeek);
            totalUniformityScore += uniformityScore;

            // Debug output.
            debugHoursPerDay(cls, grade, hoursPerDay, nDaysPerWeek, uniformityScore);
        }
    }

    // Calculate the average uniformity score.
    int totalClasses = nClassesPerGrade * nGrades;
    return totalUniformityScore / totalClasses;
}

double hoursPerDayUniformityScore(int hoursPerDay[], int maxHoursPerDay, int nDaysPerWeek) {
    // 0-100 Score based on how uniform the hoursPerDay[] is
    int maxHours = *std::max_element(hoursPerDay, hoursPerDay + nDaysPerWeek);
    int minHours = *std::min_element(hoursPerDay, hoursPerDay + nDaysPerWeek);
    int diff = maxHours - minHours;

    // Normalize the difference to a score between 0 and 100
    int maxPossibleDiff = maxHoursPerDay;
    double score = (1.0 - (static_cast<double>(diff) / maxPossibleDiff)) * 100;
    return score;
}

double teachSimilarHoursPerWeek(chromosome* chrom, json& teachers) {
    std::map<int, int> teacherHoursTaught;

    for (int i=0; i < chrom->arrSize; i++) {
        std::pair<int, int> lessonTeacher = chrom->curriculum[i];
        if (lessonTeacher.second == -1) continue;
        teacherHoursTaught[lessonTeacher.second] += 1;
    }

    // calculate relative to each teacher's max hours
    // hoursTaught / maximumHours
    std::vector<double> intensity;
    for (const auto& tH : teacherHoursTaught) {
        int id = tH.first;
        int hoursTaught = tH.second;

        int maxHoursPossible = teachers[std::to_string(id)]["hoursPerWeek"];
        double intens = static_cast<double>(hoursTaught) / maxHoursPossible;
        intensity.push_back(intens);
    }

    // if (intensity.empty()) {
    //     std::cerr << "Error: No intensity data available." << std::endl;
    //     return 0.0; // Return a default score or handle the case appropriately
    // }


    // Find the overall maximum hours per week a teacher is available
    // to calculate the maximum possible difference
    // i.e. The overall maximum teacher works all of their hours, and another one 0
    int overallMaxHoursPerWeek = 0;
    for (auto& teacher : teachers.items()) {
        int maxHPerWeek = teacher.value()["hoursPerWeek"];
        if (maxHPerWeek > overallMaxHoursPerWeek) overallMaxHoursPerWeek = maxHPerWeek;
    }

    double maxHours = *std::max_element(std::begin(intensity), std::end(intensity));
    double minHours = *std::min_element(std::begin(intensity), std::end(intensity));
    double diff = maxHours - minHours;

    // Normalize the difference to a score between 0 and 100
    int maxPossibleDiff = overallMaxHoursPerWeek;
    double score = (1.0 - (static_cast<double>(diff) / maxPossibleDiff)) * 100;
    return score;
}

void debugHoursPerDay(int cls, int grade, const int hoursPerDay[], int nDaysPerWeek, double uniformityScore) {
#ifdef DEBUG
    std::cout << "(Class, Grade): (" << cls << ", " << grade << ") "
              << "Uniformity Score: " << uniformityScore
              << ", Hours Per Day: [";
    for (int day = 0; day < nDaysPerWeek; ++day) {
        std::cout << hoursPerDay[day];
        if (day < nDaysPerWeek - 1) std::cout << ", ";
    }
    std::cout << "]" << std::endl;
#endif
}