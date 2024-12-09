#pragma once
#include "chromosome.h"
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

void scoreCalculation(chromosome* chrom, json& lessons, json& teachers);
double calculateConsecutiveHoursScore(Chromosome* chrom);
double hoursPerDayUniformityScore(int hoursPerDay[], int maxHoursPerDay, int nDaysPerWeek);
double calculateAverageUniformityScore(Chromosome* chrom);