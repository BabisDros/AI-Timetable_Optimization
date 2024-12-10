#pragma once
#include "chromosome.h"
#include <nlohmann/json.hpp>
#include <iostream>
#include <set>
using json = nlohmann::json;

void scoreCalculation(chromosome* chrom, json& lessons, json& teachers);
double calculateSatisfyLessonHoursScore(chromosome* chrom, json& lessons);
double calculateDailyWeeklyLimitScore(chromosome* chrom, json& teachers);
double calculateTeacherConflictScore(chromosome* chrom);
double calculateNoFreePeriodsScore(chromosome* chrom);
double calculateConsecutiveHoursScore(chromosome* chrom);
double hoursPerDayUniformityScore(int hoursPerDay[], int maxHoursPerDay, int nDaysPerWeek);
double calculateAverageUniformityScore(chromosome* chrom);
void debugHoursPerDay(int cls, int grade, const int hoursPerDay[], int nDaysPerWeek, double uniformityScore);