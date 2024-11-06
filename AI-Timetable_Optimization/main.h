#pragma once
#include <nlohmann/json.hpp>
using json = nlohmann::json;
//https://github.com/nlohmann/json
#include "chromosome.h"

void jsonUseExample(json& lessons, json& teachers, std::map<int, std::string>& lessonsIDs);
void scoreCalculation(chromosome* chrom);