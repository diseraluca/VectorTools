// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

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