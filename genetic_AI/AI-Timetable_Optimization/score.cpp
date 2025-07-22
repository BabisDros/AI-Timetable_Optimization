#include "score.h"
#include <string>
long long score::scoreRange = 10000000LL;
void scoreCalculation(chromosome* chrom, json& lessons, json& teachers) {

    long long totalScore = 0;

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
    totalScore += calculateAllLessonHourSpreadScore(chrom, lessons);

    // all teachers should teach around the same hours per week, variable scoring
    totalScore += teachSimilarHoursPerWeek(chrom, teachers);

   chrom->addScore(totalScore);
}

double calculateSatisfyLessonHoursScore(chromosome* chrom, json& lessons) {
    // Each lesson has a required amount of hours for each grade.
    // eg. lesson_id = 1 : A=3, B=2, C=2, etc.
    // This info can be obtained from the json data.
    long long score = 0;
    std::map <int, int> counterLH; // hold hours for each lesson
    std::string classYear[3] = { "A", "B", "C" };
    int year = -1;
    bool added;
    long long evalVariance = 0;
    long long total = 0;
    int weeklyHours = chrom->nDaysPerWeek * chrom->nHoursPerDay;
    int classCount = chrom->nClassesPerGrade;
    int yearChange = weeklyHours * classCount;

    for (int i = 0; i < chrom->arrSize; i++)
    {
        if (i % (yearChange) == 0) // year change
            year++;
        if (i % weeklyHours == 0) // class change
        {
            for (auto lesson : lessons.items())
            {
                added = false;
                for (int k = 0; k < lesson.value()["classes"].size(); k++)
                {
                    if (lesson.value()["classes"][k]["year"] == classYear[year])
                    {
                        counterLH.insert({ stoi(lesson.key()), (int)lesson.value()["classes"][k]["hours"] }); // times it must appear
                        added = true;
                        break;
                    }
            }
                if (!added)
                    counterLH.insert({ stoi(lesson.key()), 0 }); // does not appear in year
            }
    }


        if (chrom->curriculum[i].first != -1) // ignore free period
        {
            counterLH[chrom->curriculum[i].first] -= 1;
        }

        if (i % weeklyHours == weeklyHours-1) //gone through a class
        {
            for (auto it = counterLH.begin(); it != counterLH.end(); it++)
            {
                if (it->second == 0)
                    evalVariance++;
                total++;
            }
            counterLH.clear();
        }
    }

    score = (long long) (pow(evalVariance/total, 6) * score::scoreRange);

    if (evalVariance == total) // all lessons appear the specified hours
    {
        score *= 10;
        chrom->passedConstraints[0] = true;
    }
    chrom->percent = score;
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
    int nBelowLimit = 0;
    int nDailyHours = chrom->nHoursPerDay;
    int nWeeklyHours = nDailyHours * chrom->nDaysPerWeek;
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

        if (hour >= nDailyHours) {
            hour = 0;
            classvar += nWeeklyHours; // same day, next class
        }
        
        if (classvar >= chrom->arrSize) // use data, go next day
        {
            for (auto itr = teacherDayWeek.begin(); itr != teacherDayWeek.end(); itr++)
            {
                #ifdef DEBUG
                    std::cout << itr->first << " can teach " << itr->second.first << " more hours" << std::endl;
                #endif // DEBUG
                if (itr->second.first >= 0)
                    nBelowLimit++;
                total++;

                itr->second.first = teachers[itr->first]["hoursPerDay"]; //reset value
            }
            classvar = (classvar + nDailyHours) % nWeeklyHours; // pivot on next day
        }
    }

    for (auto itr = teacherDayWeek.begin(); itr != teacherDayWeek.end(); itr++)
    {
        if (itr->second.second >= 0)
            nBelowLimit++; // or more
        total++;// increase by same amount
    }
    if (nBelowLimit == total) chrom->passedConstraints[1] = true;
    return (nBelowLimit != total) ? 0 : score::scoreRange;
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
                    int index = chrom->calculateIndex(hour, day, cls, grade);
                    int teacherID = chrom->curriculum[index].second;

                    // Check if teacher is already teaching
                    // .second is false if it already exists
                    if (!teacherSet.insert(teacherID).second)
                        conflicts++;
                }
            }
        }
    }
    if (conflicts == 0) chrom->passedConstraints[2] = true;
    return (conflicts>0) ? 0 : score::scoreRange;
}

double calculateNoFreePeriodsScore(chromosome* chrom) {
    // no free periods between classes
    int freePeriodsBetweenClasses = 0;
    int total = 0;
    for (int grade=0; grade < chrom->nGrades; grade++) {
        for (int cls=0; cls < chrom->nClassesPerGrade; cls++) {
            // Check that there are no free periods in between lessons for this (cls, grade)
            for (int day=0; day < chrom->nDaysPerWeek; day++) {
                for (int hour=1; hour < chrom->nHoursPerDay-1; hour++) {

                    // Check that there's no free period (-1) between lessons
                    int idPrev = chrom->curriculum[chrom->calculateIndex(hour-1, day, cls, grade)].first;
                    int idMid = chrom->curriculum[chrom->calculateIndex(hour, day, cls, grade)].first;
                    int idNext = chrom->curriculum[chrom->calculateIndex(hour+1, day, cls, grade)].first;

                    if ((idPrev != -1) && (idMid == -1) && (idNext != -1))
                        freePeriodsBetweenClasses++;
                    total++;
                }
            }
        }
    }

    // score
    int valid = total - freePeriodsBetweenClasses;
    long double proportion = (long double) valid / (long double)total;
    long long score = (pow((proportion), 4) * score::scoreRange);
    if (freePeriodsBetweenClasses == 0) {
        score = score::scoreRange;
        chrom->passedConstraints[3] = true;
    }

    return score;
}

double calculateConsecutiveHoursScore(chromosome* chrom) {
    int moreThanThreeHours = 0;
    int totalLessons = chrom->arrSize;

    for (int grade=0; grade < chrom->nGrades; grade++) {
        for (int cls=0; cls < chrom->nClassesPerGrade; cls++) {
            for (int day=0; day < chrom->nDaysPerWeek; day++) {
                for (int hour=0; hour < chrom->nHoursPerDay-2; hour++) {
                    // A teacher shouldn't be teaching for more than 2 consecutive hours
                    int i = chrom->calculateIndex(hour, day, cls, grade);
                    int teacherId1 = chrom->curriculum[i].second;
                    int teacherId2 = chrom->curriculum[i+1].second;
                    int teacherId3 = chrom->curriculum[i+2].second;
                    if (teacherId1 == teacherId2 && teacherId2 == teacherId3) {moreThanThreeHours++;}
                }
            }
        }
    }

    // score
    int valid = totalLessons - moreThanThreeHours;
    long double proportion = (long double) valid / (long double) totalLessons;
    long long score = (pow((proportion), 4) * score::scoreRange);
    if (moreThanThreeHours == 0) {
        score = score::scoreRange;
        chrom->passedConstraints[4] = true;
    }

    return score;
}

double calculateAverageUniformityScore(chromosome* chrom) {
    // The daily lesson hours for each unique class should be uniform throughout the week, variable scoring 
    const int nDaysPerWeek = chrom->nDaysPerWeek;
    int nHoursPerDay = chrom->nHoursPerDay; 
    int nClassesPerGrade = chrom->nClassesPerGrade;
    int nGrades = chrom->nGrades; 

    double totalUniformityScore = 0;

    for (int cls = 0; cls < nClassesPerGrade; ++cls) {
        for (int grade = 0; grade < nGrades; ++grade) {
            int hoursPerDay[nDaysPerWeek] = {0}; // For each unique (class, grade) calculate the total hours per day for each day of the week.
            for (int day = 0; day < nDaysPerWeek; ++day) {
                for (int hour = 0; hour < nHoursPerDay; ++hour) {
                    int index = chrom->calculateIndex(hour, day, cls, grade);
                    const auto& lessonTeacherPair = chrom->curriculum[index];
                    if (lessonTeacherPair.first != -1 || lessonTeacherPair.second != -1) {
                        hoursPerDay[day]++;
                    }
                }
            }

            // uniformity score for this unique (class, grade).
            double uniformityScore = hoursPerDayUniformityScore(hoursPerDay, nHoursPerDay, nDaysPerWeek); // returns a value between 0 and 1
            totalUniformityScore += uniformityScore;

            // Debug output
            debugHoursPerDay(cls, grade, hoursPerDay, nDaysPerWeek, uniformityScore);
        }
    }

    // average uniformity score.
    int totalClasses = nClassesPerGrade * nGrades;
    long double averageUniformity = (long double) totalUniformityScore / (long double) totalClasses;
    long long finalScore = pow(averageUniformity, 4) * score::scoreRange;
    if (averageUniformity == 1.0L)
    {
        finalScore = score::scoreRange;
        chrom->passedConstraints[5] = true;
    }

    return finalScore;
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

double hoursPerDayUniformityScore(int hoursPerDay[], int maxHoursPerDay, int nDaysPerWeek) {
    // 0-1 Score based on how uniform the hoursPerDay[] is
    int maxHours = *std::max_element(hoursPerDay, hoursPerDay + nDaysPerWeek);
    int minHours = *std::min_element(hoursPerDay, hoursPerDay + nDaysPerWeek);
    int diff = maxHours - minHours;

    // Normalize the difference to a score between 0 and 1
    int maxPossibleDiff = maxHoursPerDay;
    double score = (1.0 - (static_cast<double>(diff) / maxPossibleDiff));
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

    // Find the overall maximum hours per week a teacher is available
    // to calculate the maximum possible difference
    // i.e. The overall maximum teacher works all of their hours, and another one 0
    int overallMaxHoursPerWeek = 0;
    for (auto& teacher : teachers.items()) {
        int maxHPerWeek = teacher.value()["hoursPerWeek"];
        if (maxHPerWeek > overallMaxHoursPerWeek) overallMaxHoursPerWeek = maxHPerWeek;
    }

    long double maxHours = *std::max_element(std::begin(intensity), std::end(intensity));
    long double minHours = *std::min_element(std::begin(intensity), std::end(intensity));
    long double diff = maxHours - minHours;

    int maxPossibleDiff = overallMaxHoursPerWeek;
    long double score = (1.0 - ((diff) / maxPossibleDiff)); // between 0 and 1
    
    long long finalScore = (pow((score), 4) * score::scoreRange);
    if (diff == 0) {
        finalScore = score::scoreRange;
        chrom->passedConstraints[6] = true;
    }
    return finalScore;
}

double calculateAllLessonHourSpreadScore(chromosome* chrom, json& lessons) {
    // Teaching hours for each lesson in each class should be uniformly spread out throughout the week
    int totalIdealSpread = 0;
    int totalConsidered = 0; // cls * grades * lessons
    std::string classYear[3] = { "A", "B", "C" };
    for (auto lesson : lessons.items()) {
        int lessonID = std::stoi(lesson.key());
        bool found = false;
        int maxHoursGradeA = 0;
        int maxHoursGradeB = 0;
        int maxHoursGradeC = 0;
        for (int k = 0; k < lesson.value()["classes"].size(); k++)
        {
            std::string val = lesson.value()["classes"][k]["year"];
            if (val == classYear[0])
                maxHoursGradeA = lesson.value()["classes"][k]["hours"];
            else if (val == classYear[1])
                maxHoursGradeB = lesson.value()["classes"][k]["hours"];
            else if (val == classYear[2])
                maxHoursGradeC = lesson.value()["classes"][k]["hours"];
        }

        // Calculate lesson hour spread score for all unique classes (cls, grade)
        for (int grade = 0; grade < chrom->nGrades; grade++) {
            int maxHours = (grade == 0) ? maxHoursGradeA : (grade == 1) ? maxHoursGradeB : maxHoursGradeC;
            for (int cls =0; cls < chrom->nClassesPerGrade; cls++) {
                std::vector<int> hoursPerDay(chrom->nDaysPerWeek, 0);
                for (int day = 0; day < chrom->nDaysPerWeek; day++) {
                    for (int hour = 0; hour < chrom->nHoursPerDay; hour++) {
                        int index = chrom->calculateIndex(hour, day, cls, grade);
                        if (chrom->curriculum[index].first == lessonID) {hoursPerDay[day]++;}
                    }
                }   
                totalIdealSpread += isLessonHoursSpreadIdeal(hoursPerDay, maxHours); // returns 1 if ideal, 0 if not
                totalConsidered += 1;
            }
        }
    }

    // Between 0 and 1000 based on how many lessons have a uniform spread of hours for each (cls, grade)
    long long score = pow((long double) (totalIdealSpread) / (long double)(totalConsidered), 4) *score::scoreRange;
    if (totalIdealSpread == totalConsidered)
    {
        score = score::scoreRange;
        chrom->passedConstraints[7] = true;
    }
    return score;
}

double isLessonHoursSpreadIdeal(const std::vector<int>& hoursPerDay, int totalHours) {
    int numDays = hoursPerDay.size();
    int idealHours = totalHours / numDays;
    int remainder = totalHours % numDays;

    // Evenly distribute the remainder (beginning to end)
    std::vector<int> target(numDays, idealHours);
    for (int i = 0; i < remainder; ++i) {
        target[i] += 1;
    }

    std::vector<int> sortedHoursPerDay = hoursPerDay;
    std::sort(sortedHoursPerDay.begin(), sortedHoursPerDay.end(), std::greater<int>());

    for (int i = 0; i < numDays; i ++) {
        if (sortedHoursPerDay[i] != target[i]) {return 0.0;}
    }
    return 1;
}