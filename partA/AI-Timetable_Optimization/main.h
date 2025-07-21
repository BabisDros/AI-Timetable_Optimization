#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json; //https://github.com/nlohmann/json
#include "chromosome.h"
#include "score.h"

void jsonUseExample(json& lessons, json& teachers);
void scoreCalculation(chromosome* chrom, json& lessons, json& teachers);
double calculateUniformityScore(int hoursPerDay[], int maxHoursPerDay, int nDaysPerWeek);