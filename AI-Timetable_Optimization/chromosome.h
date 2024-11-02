#pragma once


class chromosome {
	

public:
	chromosome() {}

	chromosome(int* genes1, int size1, int* genes2, int size2)
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

	int curriculumn[5 * 7]; // 0-6: Monday and so on 
};