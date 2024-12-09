#pragma once
#include "chromosome.h"
#include <nlohmann/json.hpp>
#include <iostream>
using json = nlohmann::json;

void scoreCalculation(chromosome* chrom, json& lessons, json& teachers);
double hoursPerDayUniformityScore(int hoursPerDay[], int maxHoursPerDay, int nDaysPerWeek);