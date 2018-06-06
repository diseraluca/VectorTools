#include "VectorLocator.h"

#include <maya/MFnNumericAttribute.h>

MTypeId VectorLocator::id{ 0x00015 };

MString VectorLocator::drawDbClassification{ "drawdb/geometry/VectorLocator" };
MString VectorLocator::drawRegistrantId{ "VectorLocatorPlugin" };

MObject VectorLocator::basePoint;
MObject VectorLocator::endPoint;
MObject VectorLocator::color;

MObject VectorLocator::magnitude;
MObject VectorLocator::direction;

void * VectorLocator::creator()
{
	return new VectorLocator();
}

MStatus VectorLocator::initialize()
{
	MStatus status{};

	MFnNumericAttribute nAttr;

	basePoint = nAttr.createPoint("basePoint", "bp", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	nAttr.setWritable(true);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);

	status = addAttribute(basePoint);
	CHECK_MSTATUS_AND_RETURN_IT(status);


	endPoint = nAttr.createPoint("endPoint", "ep", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	nAttr.setWritable(true);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);

	status = addAttribute(endPoint);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	color = nAttr.createColor("color", "cl", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	nAttr.setWritable(true);
	nAttr.setKeyable(true);
	nAttr.setStorable(true);

	status = addAttribute(color);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	magnitude = nAttr.create("magnitude", "mg", MFnNumericData::kFloat, 0.0f, &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	nAttr.setWritable(false);
	nAttr.setStorable(false);

	status = addAttribute(magnitude);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	direction = nAttr.createPoint("direction", "dr", &status);
	CHECK_MSTATUS_AND_RETURN_IT(status);
	nAttr.setWritable(false);
	nAttr.setStorable(false);

	status = addAttribute(direction);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	attributeAffects(basePoint, magnitude);
	attributeAffects(endPoint, magnitude);

	attributeAffects(basePoint, direction);
	attributeAffects(endPoint, direction);

	return MStatus::kSuccess;
}

MStatus VectorLocator::compute(const MPlug & plug, MDataBlock & data)
{
	if (plug != magnitude && plug != direction) {
		return MStatus::kUnknownParameter;
	}

	MVector basePointValue{ data.inputValue(basePoint).asFloatVector() };
	MVector endPointValue{ data.inputValue(endPoint).asFloatVector() };
	MVector resultVector{ endPointValue - basePointValue };

	data.outputValue(magnitude).set(resultVector.length());
	data.outputValue(direction).set(resultVector.normal());
	data.setClean(plug);

	return MStatus::kSuccess;
}

bool VectorLocator::isBounded() const
{
	return false;
}

void VectorLocator::draw(M3dView &, const MDagPath &, M3dView::DisplayStyle, M3dView::DisplayStatus)
{
}
