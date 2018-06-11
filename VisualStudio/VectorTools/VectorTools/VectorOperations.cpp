// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#include "VectorOperations.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MVector.h>

enum Operations{Addition, Subtraction, CrossProduct, DotProduct};

MTypeId VectorOperations::id{ 0x00016 };

MObject VectorOperations::operation;
MObject VectorOperations::vector1;
MObject VectorOperations::vector2;
MObject VectorOperations::normalizeOutput;

MObject VectorOperations::result;

void * VectorOperations::creator()
{
	return new VectorOperations();
}

MStatus VectorOperations::initialize()
{
	MFnNumericAttribute nAttr;
	MFnEnumAttribute    eAttr;

	operation = eAttr.create("operation", "op");
	eAttr.setConnectable(false);
	eAttr.addField("Addition", 0);
	eAttr.addField("Subtraction", 1);
	eAttr.addField("Cross Product", 2);
	eAttr.addField("Dot Product", 3);
	addAttribute(operation);

	vector1 = nAttr.createPoint("vector1", "v1");
	nAttr.setKeyable(true);
	addAttribute(vector1);

	vector2 = nAttr.createPoint("vector1", "v1");
	nAttr.setKeyable(true);
	addAttribute(vector2);

	normalizeOutput = nAttr.create("normalizeOutput", "no", MFnNumericData::kBoolean);
	addAttribute(normalizeOutput);

	result = nAttr.createPoint("result", "rs");
	nAttr.setStorable(false);
	nAttr.setWritable(false);
	addAttribute(result);

	attributeAffects(operation, result);
	attributeAffects(vector1, result);
	attributeAffects(vector2, result);
	attributeAffects(normalizeOutput, result);

	return MStatus::kSuccess;
}

MStatus VectorOperations::compute(const MPlug & plug, MDataBlock & data)
{
	if (plug != result) {
		return MStatus::kUnknownParameter;
	}

	int operationValue{ data.inputValue(operation).asInt() };
	MVector vector1Value{ data.inputValue(vector1).asFloatVector() };
	MVector vector2Value{ data.inputValue(vector2).asFloatVector() };
	bool normalizeOutputValue{ data.inputValue(normalizeOutput).asBool() };

	MVector resultValue{};

	switch (operationValue)
	{
	case Operations::Addition:
		resultValue = vector1Value + vector2Value;
		break;
	case Operations::Subtraction:
		resultValue = vector1Value - vector2Value;
		break;
	case Operations::CrossProduct:
		resultValue = vector1Value ^ vector2Value;
		break;
	case Operations::DotProduct:
		resultValue.x = vector1Value * vector2Value;
		resultValue.y = resultValue.x;
		resultValue.z = resultValue.x;
		break;
	default:
		break;
	}

	if (normalizeOutputValue) {
		resultValue.normalize();
	}

	data.outputValue(result).set(resultValue);
	data.outputValue(result).setClean();

	return MStatus::kSuccess;
}
