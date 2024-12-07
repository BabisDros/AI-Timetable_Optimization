#pragma once
#include <cstdlib>
#include <vector>

class chromosome {
	int score = 10;
	int distribution = 0;
public:
	static const int nClassesPerGrade = 3;
	static const int arrSize = nClassesPerGrade * 3 * 5 * 7; // classesPerGrade, Grades, DaysPerWeek, HoursPerDay

	chromosome() {}

	chromosome(std::vector<std::pair<int, int>>& lessonTeacher)
	{
		for (int i = 0; i < arrSize; i++)
		{
			int rand = std::rand();
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
	
	void addScore(int score);
	int getScore() const;
	void setDistribution(int val);
	int getDistribution() const;

	static bool compareDistributionVal(int val, chromosome* a);

};