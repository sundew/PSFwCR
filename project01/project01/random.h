#pragma once
#include <iostream>
using namespace std;
float randFloat(float low, float high)
{
	float temp;
	if (low > high){
		temp = low;
		low = high;
		high = temp;
	}
	temp = (rand() / (static_cast<double>(RAND_MAX) + 1.0)) * (high - low) + low;
	return temp;
}