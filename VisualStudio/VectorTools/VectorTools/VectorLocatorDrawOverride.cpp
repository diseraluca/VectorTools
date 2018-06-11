// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#include "VectorLocatorDrawOverride.h"
#include "VectorLocatorData.h"

#include <maya/MEventMessage.h>
#include <maya/MFnDependencyNode.h>
#include <maya/MHardwareRenderer.h>

MVector _vectorFromPlug(const MPlug& plug) {
	float vector[3];

	for (int index{ 0 }; index < 3; index++) {
		MPlug vectorAxis{ plug.child(index) };
		vector[index] = vectorAxis.asFloat();
	}
	return MVector{ vector };
}

MColor _colorFromPlug(const MPlug& plug) {
	float color[3];

	for (int index{ 0 }; index < 3; index++) {
		MPlug vectorAxis{ plug.child(index) };
		color[index] = vectorAxis.asFloat();
	}
	return MColor{ color };
}


VectorLocatorDrawOverride::VectorLocatorDrawOverride(const MObject& obj) :MHWRender::MPxDrawOverride{ obj, NULL, false } {
	fModelEditorChangedCbId = MEventMessage::addEventCallback("onModelEditorChanged", VectorLocatorDrawOverride::OnModelEditorChanged, this);

	MStatus status{};
	MFnDependencyNode vectorLocatorNode{ obj, &status };
	fVectorLocator =  status ? dynamic_cast<VectorLocator*>(vectorLocatorNode.userNode()) : NULL;
}

VectorLocatorDrawOverride::~VectorLocatorDrawOverride()
{
	fVectorLocator = NULL;

	if (fModelEditorChangedCbId != 0) {
		MEventMessage::removeCallback(fModelEditorChangedCbId);
		fModelEditorChangedCbId = 0;
	}
}

void VectorLocatorDrawOverride::OnModelEditorChanged(void *clientData) {
	VectorLocatorDrawOverride* override = static_cast<VectorLocatorDrawOverride*>(clientData);
	if (override && override->fVectorLocator) {
		MHWRender::MRenderer::setGeometryDrawDirty(override->fVectorLocator->thisMObject());
	}
}

MHWRender::MPxDrawOverride * VectorLocatorDrawOverride::Creator(const MObject & obj)
{
	return new VectorLocatorDrawOverride(obj);
}

MHWRender::DrawAPI VectorLocatorDrawOverride::supportedDrawAPIs() const
{
	return (MHWRender::kOpenGL | MHWRender::kOpenGLCoreProfile | MHWRender::kDirectX11 );
}

MUserData * VectorLocatorDrawOverride::prepareForDraw(const MDagPath & objPath, const MDagPath & cameraPath, const MHWRender::MFrameContext & frameContext, MUserData * oldData)
{
	VectorLocatorData* data = dynamic_cast<VectorLocatorData*>(oldData);
	if (!oldData) {
		data = new VectorLocatorData;
	}

	MObject vectorLocatorNode{ objPath.node() };
	MPlug basePointPlug{ vectorLocatorNode, VectorLocator::basePoint };
	data->basePoint = _vectorFromPlug(basePointPlug);

	MPlug endPointPlug{ vectorLocatorNode, VectorLocator::endPoint };
	data->endPoint = _vectorFromPlug(endPointPlug);

	MPlug colorPlug{ vectorLocatorNode, VectorLocator::color };
	data->color = _colorFromPlug(colorPlug);

	return data;
}

bool VectorLocatorDrawOverride::hasUIDrawables() const
{
	return true;
}

void VectorLocatorDrawOverride::addUIDrawables(const MDagPath & objPath, MHWRender::MUIDrawManager & drawManager, const MHWRender::MFrameContext & frameContext, const MUserData * data)
{
	VectorLocatorData* vectorLocatorData = (VectorLocatorData*)data;

	if (!vectorLocatorData) {
		return;
	}

	drawManager.beginDrawable();

	drawManager.setColor(vectorLocatorData->color);

	drawManager.line(vectorLocatorData->basePoint, vectorLocatorData->endPoint);
	
	MVector line{ vectorLocatorData->endPoint - vectorLocatorData->basePoint };
	double lineLength{ line.length() };
	double coneRadius{ frameContext.getGlobalLineWidth() / 2};
	double coneHeigth = (lineLength > coneRadius) ? coneRadius : lineLength / 4;
	MVector coneDirection{ line.normal() };

	// Reduce the vector to position the base so that the cone tip lies on vectorLocatorData->endPoint
	MVector coneBase{ (coneDirection * (coneHeigth - lineLength) * -1) + vectorLocatorData->basePoint };

	drawManager.cone(coneBase, coneDirection, coneRadius, coneHeigth, true);

	drawManager.endDrawable();
}