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
	MStatus status{};

	if (event.mouseButton() == MEvent::kLeftMouse) {
		M3dView currentView = M3dView::active3dView();

		//Gets the position in port space where the left mouse button was pressed
		short portPositionX{};
		short portPositionY{};
		event.getPosition(portPositionX, portPositionY);

		//Convert the port space coordinates to worldSpace
		MPoint worldPosition{};
		MVector worldDirection{};
		currentView.viewToWorld(portPositionX, portPositionY, worldPosition, worldDirection);

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
			MFnTransform transformFn{};

			MObject baseLocatorTransform{ dagFn.create("locator") };

			MDagPath baseLocatorDagPath{};
			dagFn.getPath(baseLocatorDagPath);
			transformFn.setObject(baseLocatorDagPath);
			transformFn.setTranslation(basePoint, MSpace::kWorld);

			baseLocatorDagPath.extendToShape();
			MObject baseLocatorShape{ baseLocatorDagPath.node() };
			dagFn.setObject(baseLocatorShape);
			MPlug baseLocatorWorldPositionPlug{ dagFn.findPlug("worldPosition") };
			baseLocatorWorldPositionPlug = baseLocatorWorldPositionPlug.elementByLogicalIndex(0);

			MObject endLocatorTransform{ dagFn.create("locator") };

			MDagPath endLocatorDagPath{};
			dagFn.getPath(endLocatorDagPath);
			transformFn.setObject(baseLocatorDagPath);
			transformFn.setTranslation(endPoint, MSpace::kWorld);

			endLocatorDagPath.extendToShape();
			MObject endLocatorShape{ endLocatorDagPath.node() };
			dagFn.setObject(endLocatorShape);
			MPlug endLocatorWorldPositionPlug{ dagFn.findPlug("worldPosition") };
			endLocatorWorldPositionPlug = endLocatorWorldPositionPlug.elementByLogicalIndex(0);

			MObject vectorLocatorTransform{ dagFn.create("VectorLocator") };

			MDagPath vectorLocatorDagPath{};
			dagFn.getPath(vectorLocatorDagPath);
			vectorLocatorDagPath.extendToShape();

			MObject vectorLocatorShape{ vectorLocatorDagPath.node() };
			dagFn.setObject(vectorLocatorShape);
			MPlug vectorLocatorBasePointPlug{ dagFn.findPlug("basePoint") };
			MPlug vectorLocatorEndPointPlug{ dagFn.findPlug("endPoint") };

			MDagModifier dagModifier{};
			dagModifier.connect(baseLocatorWorldPositionPlug, vectorLocatorBasePointPlug);
			dagModifier.connect(endLocatorWorldPositionPlug, vectorLocatorEndPointPlug);
			dagModifier.doIt();

    		//Sets the flag back to the basePoint selection and sets the help string back to the base point one
			isSelectingEndPoint = false;
			setHelpString(START_HELP_STRING);
		}
	}

	return MStatus::kSuccess;
}