// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#pragma once

#include <maya/MPxNode.h>

class VectorScalarOperations : public MPxNode {
public:
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;
public:
	static MTypeId id;

	static MObject operation;
	static MObject vector;
	static MObject scalar;

	static MObject result;
};