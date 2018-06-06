#pragma once

#include "VectorLocator.h"

#include <maya/MPxDrawOverride.h>
#include <maya/MCallbackIdArray.h>

class VectorLocatorDrawOverride : public MHWRender::MPxDrawOverride {
public:
	virtual ~VectorLocatorDrawOverride();

	static MHWRender::MPxDrawOverride* Creator(const MObject& obj);

	virtual MHWRender::DrawAPI supportedDrawAPIs() const override;

	virtual MUserData* prepareForDraw(const MDagPath& objPath, const MDagPath& cameraPath, const MHWRender::MFrameContext& frameContext, MUserData* oldData) override;

	virtual bool hasUIDrawables() const override;

	virtual void addUIDrawables(const MDagPath& objPath, MHWRender::MUIDrawManager& drawManager, const MHWRender::MFrameContext& frameContext, const MUserData* data) override;

private:
	//We call the base class Constructor to initialize the DrawOverride.
	VectorLocatorDrawOverride(const MObject& obj);

	//This method will be called by the callback to dirty the draw bit of the node
	static void OnModelEditorChanged(void *clientData);

	// Those two member will be initialized with the BasicLocator node to draw and with a callback to set the model to be drawn respectively
	VectorLocator*  fVectorLocator;
	MCallbackId fModelEditorChangedCbId;
};