#include "VectorToolCommand.h"
#include "VectorTool.h"

MPxContext * VectorToolCommand::makeObj()
{
	return new VectorTool;
}

void * VectorToolCommand::creator()
{
	return new VectorToolCommand;
}
