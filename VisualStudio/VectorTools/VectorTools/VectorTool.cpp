#include "VectorTool.h"

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
			
		}
	}

	return MStatus();
}
