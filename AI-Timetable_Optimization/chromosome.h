#pragma once
#include <cstdlib>
#include <vector>
#include <random>
#include <map>

class chromosome {
	long long score = 1LL; // should never be 0
	long long distribution = 0;
	int nConstraints = 8;
    public:
    static const int nClassesPerGrade = 3;
    static const int nGrades = 3;
    static const int nDaysPerWeek = 5;
    static const int nHoursPerDay = 7;
	static const int arrSize = nClassesPerGrade * nGrades * nDaysPerWeek * nHoursPerDay;

	chromosome() 
	{
		for (int i = 0; i < nConstraints; i++)
			passedConstraints[i] = false;
	}

	chromosome(std::vector<std::pair<int, int>>& lessonTeacher, std::uniform_int_distribution<long long>& distr, std::mt19937_64& gen) : chromosome()
	{
		for (int i = 0; i < arrSize; i++)
		{
			int rand = distr(gen);
			curriculum[i] = lessonTeacher[rand % lessonTeacher.size()];
		}
	}

	chromosome(std::pair<int, int>* genes1, int size1, std::pair<int, int>* genes2): chromosome()
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
	std::map<int, bool> passedConstraints;
	// possibly make id to constraint correlation with string table
	long long percent=0;
	long long satisfyLessonHoursScore = 0;
	void addScore(long long score);
	long long getScore() const;
	void setDistribution(long long val);
	long long getDistribution() const;

	static bool compareDistributionVal(long long val, chromosome* a);

	static int calculateIndex(int hour, int day, int cls, int grade) {
		return hour 
			+ nHoursPerDay * day 
			+ nHoursPerDay * nDaysPerWeek * cls 
			+ nHoursPerDay * nDaysPerWeek * nClassesPerGrade * grade;
    }
};