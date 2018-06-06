#pragma once

#include <maya/MPxNode.h>

class VectorOperations : public MPxNode {
public:
	static void * creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;

public:
	static MTypeId id;

	static MObject operation;
	static MObject vector1;
	static MObject vector2;
	static MObject normalizeOutput;

	static MObject result;
};