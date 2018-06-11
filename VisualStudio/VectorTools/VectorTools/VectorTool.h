// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#pragma once

#include <maya/MPxContext.h>
#include <maya/MPoint.h>
#include <maya/M3dView.h>
#include <maya/MPlug.h>

class VectorTool : public MPxContext {
public:
	VectorTool() :basePoint{}, endPoint{} {}

	static void* creator();
	virtual void toolOnSetup(MEvent& event) override;
	virtual MStatus doPress(MEvent& event, MHWRender::MUIDrawManager& drawManager,
		const MHWRender::MFrameContext& context) override;

private:
	MStatus getCursorWorldPosition(MEvent& event, MPoint& worldPt, MVector& worldVector);
	MVector getActiveCameraDirection(const MSpace::Space&);
	MStatus setTranslationsFromMObject(MObject& transform, MPoint& position,const MSpace::Space& space = MSpace::kWorld );
	MObject& shapeFromTransform(MObject& transform);
	MPlug plugFromMObject(MObject& node, MString attributeName);

private:
	MPoint basePoint;
	MPoint endPoint;

	// When false the user is choosing the origin for the vector. 
	// When active the user is choosing the end of the vector 
	// and if the left mouse is pressed the vector will be drawn. 
	bool isSelectingEndPoint;
};