#pragma once

#include <maya/MUserData.h>
#include <maya/MVector.h>

class VectorLocatorData : public MUserData {
public:
	VectorLocatorData() :MUserData(false) {}

	MVector basePoint;
	MVector endPoint;
	MColor  color;
};