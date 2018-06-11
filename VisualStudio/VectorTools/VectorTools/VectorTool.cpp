// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#include "VectorTool.h"
#include "VectorLocator.h"

#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDagModifier.h>
#include <maya/MMatrix.h>
#include <maya/MGlobal.h>
#include <maya/MFnTransform.h>
#include <maya/MFnCamera.h>

const MString TITLE_STRING{ "Vector Tool" };
const MString START_HELP_STRING{ "Click to pick the origin point for the vector" };
const MString END_HELP_STRING{ "Click to pick the end point for the vector" };

void * VectorTool::creator()
{
	return new VectorTool();
}

void VectorTool::toolOnSetup(MEvent & event)
{
	setTitleString(TITLE_STRING);

	isSelectingEndPoint = false;
	setHelpString(START_HELP_STRING);
}

MStatus VectorTool::doPress(MEvent & event, MHWRender::MUIDrawManager& drawManager,
	const MHWRender::MFrameContext& context)
{
	if (event.mouseButton() == MEvent::kLeftMouse) {
		MPoint worldPosition{};
		MVector worldDirection{};
		getCursorWorldPosition(event, worldPosition, worldDirection);

		MVector cameraDirection{ getActiveCameraDirection(MSpace::kWorld) };

		double dl{ worldDirection * cameraDirection };
		double d{ (MPoint(0, 0, 0) - worldPosition) * cameraDirection / dl };
		MPoint point{ worldPosition + d * worldDirection };

		MDagPath cameraPath{};
		currentView.getCamera(cameraPath);
		MFnCamera cameraFn{ cameraPath };
		MVector cameraDirection{ cameraFn.viewDirection(MSpace::kWorld) };

		double dl{ worldDirection * cameraDirection };
		double d{ (MPoint(0, 0, 0) - worldPosition) * cameraDirection / dl };
		MPoint point{ worldPosition + d * worldDirection };

		if (!isSelectingEndPoint) {
			basePoint = point;

			isSelectingEndPoint = true;
			setHelpString(END_HELP_STRING);
		}
		else {
			endPoint = point;

			MFnDagNode dagFn{};

			MObject baseLocatorTransform{ dagFn.create("locator") };
			setTranslationsFromMObject(baseLocatorTransform, basePoint);

			MObject baseLocatorShape{ shapeFromTransform(baseLocatorTransform) };
			MPlug baseLocatorWorldPositionPlug{ plugFromMObject(baseLocatorShape, "worldPosition") };
			baseLocatorWorldPositionPlug = baseLocatorWorldPositionPlug.elementByLogicalIndex(0);

			MObject endLocatorTransform{ dagFn.create("locator") };
			setTranslationsFromMObject(endLocatorTransform, endPoint);

			MObject endLocatorShape{ shapeFromTransform(endLocatorTransform) };
			MPlug endLocatorWorldPositionPlug{ plugFromMObject(endLocatorShape, "worldPosition") };
			endLocatorWorldPositionPlug = endLocatorWorldPositionPlug.elementByLogicalIndex(0);

			MObject vectorLocatorTransform{ dagFn.create("VectorLocator") };

			MObject vectorLocatorShape{ shapeFromTransform(vectorLocatorTransform) };
			MPlug vectorLocatorBasePointPlug{ plugFromMObject(vectorLocatorShape, "basePoint") };
			MPlug vectorLocatorEndPointPlug{ plugFromMObject(vectorLocatorShape, "endPoint") };

			MDagModifier dagModifier{};
			dagModifier.connect(baseLocatorWorldPositionPlug, vectorLocatorBasePointPlug);
			dagModifier.connect(endLocatorWorldPositionPlug, vectorLocatorEndPointPlug);
			dagModifier.doIt();

			M3dView activeView{ M3dView::active3dView() };
			activeView.refresh();

    		//Sets the flag back to the basePoint selection and sets the help string back to the base point one
			isSelectingEndPoint = false;
			setHelpString(START_HELP_STRING);
		}
	}

	return MStatus::kSuccess;
}

MStatus VectorTool::getCursorWorldPosition(MEvent & event, MPoint & worldPt, MVector & worldVector)
{
	short portPositionX{ 0 }, portPositionY{ 0 };
	event.getPosition(portPositionX, portPositionY);

	M3dView activeView{ M3dView::active3dView() };
	activeView.viewToWorld(portPositionX, portPositionY, worldPt, worldVector);

	return MS::kSuccess;
}

MVector VectorTool::getActiveCameraDirection(const MSpace::Space &space)
{
	M3dView activeView{ M3dView::active3dView() };
	MDagPath cameraPath{};
	activeView.getCamera(cameraPath);

	MFnCamera cameraFn{ cameraPath };

	return cameraFn.viewDirection(space);
}

MStatus VectorTool::setTranslationsFromMObject(MObject & transform, MPoint & position, const MSpace::Space & space)
{
	MFnDagNode dagFn{ transform };

	MDagPath transformPath{};
	dagFn.getPath(transformPath);

	MFnTransform transformFn{ transformPath };
	transformFn.setTranslation(MVector(position), space);

	return MStatus::kSuccess;
}

MObject & VectorTool::shapeFromTransform(MObject & transform)
{
	MDagPath transformDagPath{};
	MFnDagNode dagFn{ transform };
	dagFn.getPath(transformDagPath);
	
	transformDagPath.extendToShape();
	MObject shape{ transformDagPath.node() };
	return shape;
}

MPlug  VectorTool::plugFromMObject(MObject & node, MString attributeName)
{
	MFnDagNode dagFn{node};
	MPlug resultPlug{ dagFn.findPlug(attributeName) };

	return resultPlug;
}
