#pragma once
#include <cstdlib>
#include <vector>

class chromosome {
	int score = 10;
public:
	static const int arrSize = 3 * 3 * 5 * 7; // 0-6: Monday and so on, from class A1 to class C3

	chromosome() {}

	chromosome(std::vector<std::pair<int, int>>& lessonTeacher)
	{
		for (int i = 0; i < arrSize; i++)
		{
			int rand = std::rand();
			curriculumn[i] = lessonTeacher[rand % lessonTeacher.size()];
		}
	}

	chromosome(std::pair<int, int>* genes1, int size1, std::pair<int, int>* genes2, int size2)
	{
		for (int i = 0; i < size1; i++)
		{
			curriculumn[i] = genes1[i];
		}

		for (int i = 0; i < size2; i++)
		{
			curriculumn[i + size1] = genes2[i];
		}
	}	

	std::pair<int, int> curriculumn[arrSize];
	
	void addScore(int score);
	int getScore();
};