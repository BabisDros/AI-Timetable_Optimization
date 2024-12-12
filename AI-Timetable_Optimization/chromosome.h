#pragma once
#include <cstdlib>
#include <vector>
#include <random>

class chromosome {
	long long score = 1LL; // should never be 0
	long long distribution = 0;
    public:
    static const int nClassesPerGrade = 3;
    static const int nGrades = 3;
    static const int nDaysPerWeek = 5;
    static const int nHoursPerDay = 7;
	static const int arrSize = nClassesPerGrade * nGrades * nDaysPerWeek * nHoursPerDay;

	chromosome() {}

	chromosome(std::vector<std::pair<int, int>>& lessonTeacher, std::uniform_int_distribution<long long>& distr, std::mt19937_64& gen)
	{
		for (int i = 0; i < arrSize; i++)
		{
			int rand = distr(gen);
			curriculum[i] = lessonTeacher[rand % lessonTeacher.size()];
		}
	}

	chromosome(std::pair<int, int>* genes1, int size1, std::pair<int, int>* genes2)
	{
		for (int i = 0; i < size1; i++)
		{
			curriculum[i] = genes1[i];
		}

		for (int i = 0; i < arrSize - size1; i++)
		{
			curriculum[i + size1] = genes2[i];
		}
	}	

	std::pair<int, int> curriculum[arrSize];
	
	void addScore(long long score);
	long long getScore() const;
	void setDistribution(long long val);
	long long getDistribution() const;

	static bool compareDistributionVal(long long val, chromosome* a);

	static int calculateIndex(int cls, int grade, int day, int hour) {
		return cls 
			+ nClassesPerGrade * grade 
			+ nClassesPerGrade * nGrades * day 
			+ nClassesPerGrade * nGrades * nDaysPerWeek * hour;
    }

    
};