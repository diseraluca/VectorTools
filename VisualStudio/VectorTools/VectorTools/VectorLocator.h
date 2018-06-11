// Copyright 2018 Luca Di Sera
// This code is licensed under the MIT License ( see LICENSE.txt for details )

#pragma once

#include <maya/MPxLocatorNode.h>

class VectorLocator : public MPxLocatorNode {
public:
	static void* creator();
	static MStatus initialize();
	virtual MStatus compute(const MPlug& plug, MDataBlock& data) override;
	virtual bool isBounded() const override;
	virtual void draw(M3dView&, const MDagPath&, M3dView::DisplayStyle, M3dView::DisplayStatus) override;
public:
	static MTypeId id;

	static  MString     drawDbClassification;
	static  MString     drawRegistrantId;

	static MObject basePoint;
	static MObject endPoint;
	static MObject color;

	static MObject magnitude;
	static MObject direction;
};