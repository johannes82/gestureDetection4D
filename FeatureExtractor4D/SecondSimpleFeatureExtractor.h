#pragma once

#include "IFeatureExtractor4D.h"
#include "../GestureDetection4D/Constants.h"

class SecondSimpleFeatureExtractor:public IFeatureExtractor4D
{
public:
	std::vector<float> getFeatureVector(std::vector<Point3D> points);
};

