// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

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
