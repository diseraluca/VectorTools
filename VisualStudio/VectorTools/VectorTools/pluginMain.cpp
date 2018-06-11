// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#include "VectorLocator.h"
#include "VectorLocatorDrawOverride.h"
#include "VectorOperations.h"
#include "VectorScalarOperations.h"
#include "VectorToolCommand.h"

#include <maya/MFnPlugin.h>
#include <maya/MDrawRegistry.h>
#include <maya/MPxLocatorNode.h>

MStatus initializePlugin(MObject obj) {
	MStatus status{};
	MFnPlugin plugin{ obj, "Luca Di Sera", "1.0", "Any" };

	status = plugin.registerNode("VectorLocator", VectorLocator::id, VectorLocator::creator, VectorLocator::initialize, MPxNode::kLocatorNode, &VectorLocator::drawDbClassification);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.registerNode("VectorOperations", VectorOperations::id, VectorOperations::creator, VectorOperations::initialize, MPxNode::kDependNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.registerNode("VectorScalarOperations", VectorScalarOperations::id, VectorScalarOperations::creator, VectorScalarOperations::initialize, MPxNode::kDependNode);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = MHWRender::MDrawRegistry::registerDrawOverrideCreator(VectorLocator::drawDbClassification, VectorLocator::drawRegistrantId, VectorLocatorDrawOverride::Creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.registerContextCommand("VectorToolContext", VectorToolCommand::creator);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.registerUI("VectorToolCreateUI", "VectorToolDeleteUI");
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MStatus::kSuccess;
}

MStatus uninitializePlugin(MObject obj) {
	MStatus status{};
	MFnPlugin plugin(obj);

	status = MHWRender::MDrawRegistry::deregisterDrawOverrideCreator(VectorLocator::drawDbClassification, VectorLocator::drawRegistrantId);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.deregisterNode(VectorLocator::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.deregisterNode(VectorOperations::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.deregisterNode(VectorScalarOperations::id);
	CHECK_MSTATUS_AND_RETURN_IT(status);

	status = plugin.deregisterContextCommand("VectorToolContext");
	CHECK_MSTATUS_AND_RETURN_IT(status);

	return MStatus::kSuccess;
}