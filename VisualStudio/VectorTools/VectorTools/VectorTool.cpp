#include "VectorTool.h"
#include "VectorLocator.h"

#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MTransformationMatrix.h>

const MString START_HELP_STRING{ "VectorTool: Click to pick the origin point for the vector" };
const MString END_HELP_STRING{ "VectorTool: Click to pick the end point for the vector" };

void VectorTool::toolOnSetup(MEvent & event)
{
	isSelectingEndPoint = false;
	setHelpString(START_HELP_STRING);
}

MStatus VectorTool::doPress(MEvent & event)
{
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

		if (!isSelectingEndPoint) {
			basePoint = worldPosition;

			isSelectingEndPoint = true;
			setHelpString(END_HELP_STRING);
		}
		else {
			//Create the locators and set them to the correct world space coordinates
			MFnDagNode baseLocatorFn{};
			baseLocatorFn.create("locator");

			MTransformationMatrix baseLocatorMatrix{ baseLocatorFn.transformationMatrix() };
			baseLocatorMatrix.setTranslation(MVector(basePoint), MSpace::kWorld);

			MFnDagNode endLocatorFn{};
			endLocatorFn.create("locator");

			MTransformationMatrix endLocatorMatrix{ endLocatorFn.transformationMatrix() };
			endLocatorMatrix.setTranslation(MVector(basePoint), MSpace::kWorld);

			//Create the vectorLocator and connects the locators to its input
			MFnDagNode vectorNodeFn{};
			vectorNodeFn.create("VectorLocator");

			//Gets a plug to the needed attributes
			MObject vectorNode{ vectorNodeFn.dagPath().node() };
			MPlug vectorLocatorBasePointAttribute{ vectorNode, VectorLocator::basePoint  };
			MPlug vectorLocatorEndPointAttribute{ vectorNode, VectorLocator::endPoint };

			MObject baseLocatorNode{ baseLocatorFn.dagPath().node() };

		}
	}

	return MStatus();
}
