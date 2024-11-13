#include "chromosome.h"

void chromosome::addScore(int score)
{
	this->score += score;
}

int chromosome::getScore()
{
	return score;
}
