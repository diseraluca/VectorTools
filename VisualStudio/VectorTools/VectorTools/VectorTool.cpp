#include "VectorTool.h"
#include "VectorLocator.h"

#include <maya/MFnDagNode.h>
#include <maya/MDagPath.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MDagModifier.h>

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
			endPoint = worldPosition;

			//Create the locators and set them to the correct world space coordinates
			MDagModifier dagModifier{};
			MFnDagNode dagFn{};
			MTransformationMatrix locatorMatrix{};

			//Create the locators and position them at the correct position
			MObject baseLocatorTransform{ dagModifier.createNode("locator") };
			dagFn.setObject(baseLocatorTransform);
			locatorMatrix = dagFn.transformationMatrix();
			locatorMatrix.setTranslation(MVector(basePoint), MSpace::kWorld);

			// Retrieves the locator shape worl position attribute that will later be connected
			MObject baseLocatorShape{ dagFn.child(0) };
			dagFn.setObject(baseLocatorShape);
			MPlug baseLocatorShapeWorldPositionPlug{ dagFn.findPlug("worldPosition") };

			MObject endLocatorTransform{ dagModifier.createNode("locator") };
			dagFn.setObject(baseLocatorTransform);
			locatorMatrix = dagFn.transformationMatrix();
			locatorMatrix.setTranslation(MVector(endPoint), MSpace::kWorld);

			MObject endLocatorShape{ dagFn.child(0) };
			dagFn.setObject(endLocatorShape);
			MPlug endLocatorWorldPositionPlug{ dagFn.findPlug("worldPosition") };

			//Create the vectorLocator and prepare the plug to be connected
			MObject vectorLocatorTransform{ dagModifier.createNode(VectorLocator::id) };
			
			dagFn.setObject(vectorLocatorTransform);
			MObject vectorLocatorShape{ dagFn.child(0) };
			dagFn.setObject(vectorLocatorShape);

			MPlug vectorLocatorBasePointPlug{ dagFn.findPlug("basePoint") };
			MPlug vectorLocatorEndPointPlug{ dagFn.findPlug("endPoint") };

			//Connects the locator world position to the input attribute of the vector locator
			dagModifier.connect(baseLocatorShapeWorldPositionPlug, vectorLocatorBasePointPlug);
			dagModifier.connect(endLocatorWorldPositionPlug, vectorLocatorEndPointPlug);

			dagModifier.doIt();

			//Sets the flag back to the basePoint selection and sets the help string back to the base point one
			isSelectingEndPoint = false;
			setHelpString(START_HELP_STRING);
		}
	}

	return MStatus();
}
