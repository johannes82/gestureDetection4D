#ifndef SIMPLEFEATUREEXTRACTOR_H_
#define SIMPLEFEATUREEXTRACTOR_H_

#include "IFeatureExtractor4D.h"
#include "../GestureDetection4D/Constants.h"

class SimpleFeatureExtractor :
	public IFeatureExtractor4D
{
public:
	std::vector<float> getFeatureVector(std::vector<Point3D> points);
	SimpleFeatureExtractor(void);
	virtual ~SimpleFeatureExtractor(void);

private:
	std::vector<Point3D> normalizeVector(std::vector<Point3D> points);
};

#endif
