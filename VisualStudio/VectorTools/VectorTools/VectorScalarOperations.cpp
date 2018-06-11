// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#include "VectorScalarOperations.h"

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MVector.h>

enum Operations{multiplication, division};

MTypeId VectorScalarOperations::id{ 0x00017 };

MObject VectorScalarOperations::operation;
MObject VectorScalarOperations::vector;
MObject VectorScalarOperations::scalar;

MObject VectorScalarOperations::result;

void * VectorScalarOperations::creator()
{
	return new VectorScalarOperations();
}

MStatus VectorScalarOperations::initialize()
{
	MFnNumericAttribute nAttr;
	MFnEnumAttribute    eAttr;

	operation = eAttr.create("operations", "op");
	eAttr.setConnectable(false);
	eAttr.addField("Multiplication", 0);
	eAttr.addField("Division", 1);
	addAttribute(operation);

	vector = nAttr.createPoint("vector", "vc");
	nAttr.setKeyable(true);
	addAttribute(vector);

	scalar = nAttr.create("scalar", "sc", MFnNumericData::kDouble);
	nAttr.setKeyable(true);
	addAttribute(scalar);

	result = nAttr.createPoint("result", "rs");
	nAttr.setWritable(false);
	nAttr.setStorable(false);
	addAttribute(result);

	attributeAffects(operation, result);
	attributeAffects(vector, result);
	attributeAffects(scalar, result);

	return MStatus::kSuccess;
}

MStatus VectorScalarOperations::compute(const MPlug & plug, MDataBlock & data)
{
	int operationValue{ data.inputValue(operation).asInt() };
	MVector  vectorValue{ data.inputValue(vector).asVector() };
	double scalarValue{ data.inputValue(scalar).asDouble() };

	MVector resultValue{};
	switch (operationValue)
	{
	case Operations::multiplication:
		resultValue = vectorValue * scalarValue;
		break;
	case Operations::division:
		resultValue = vectorValue * scalarValue;
		break;
	default:
		break;
	}

	data.outputValue(result).set(resultValue);
	data.outputValue(result).setClean();

	return MStatus::kSuccess;
}
