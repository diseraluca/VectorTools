// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#pragma once

#include <maya/MPxContextCommand.h>

class VectorToolCommand : public MPxContextCommand {
public:
	virtual MPxContext* makeObj() override;
	static void* creator();
};