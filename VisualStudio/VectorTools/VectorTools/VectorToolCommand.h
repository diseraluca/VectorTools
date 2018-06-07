#pragma once

#include <maya/MPxContextCommand.h>

class VectorToolCommand : public MPxContextCommand {
public:
	virtual MPxContext* makeObj() override;
	static void* creator();
};