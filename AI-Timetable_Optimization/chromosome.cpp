#include "chromosome.h"

void chromosome::addScore(int score)
{
	this->score += score;
}

int chromosome::getScore() const
{
	return score;
}

void chromosome::setDistribution(int val)
{
	distribution = val;
}

int chromosome::getDistribution() const
{
	return distribution;
}


bool chromosome::compareDistributionVal(int val, chromosome* a)
{
	return val < a->distribution;
}