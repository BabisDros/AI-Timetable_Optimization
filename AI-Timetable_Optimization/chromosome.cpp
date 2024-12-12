#include "chromosome.h"

void chromosome::addScore(long long score)
{
	this->score += score;
}

long long chromosome::getScore() const
{
	return score;
}

void chromosome::setDistribution(long long val)
{
	distribution = val;
}

long long chromosome::getDistribution() const
{
	return distribution;
}


bool chromosome::compareDistributionVal(long long val, chromosome* a)
{
	return val < a->distribution;
}