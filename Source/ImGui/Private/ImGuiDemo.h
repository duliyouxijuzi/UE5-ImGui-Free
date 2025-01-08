// Copyright (c) 2017-2021 Sebastian Gross. Distributed under the MIT License (MIT).
// See accompanying LICENSE file for details. This project is free to use under the terms of the MIT License.

#pragma once

#include <CoreMinimal.h>

#include <imgui.h>

class FImGuiModuleProperties;

// Widget drawing ImGui demo.
class FImGuiDemo
{
public:

	FImGuiDemo(FImGuiModuleProperties& InProperties)
		: Properties(InProperties)
	{
	}

	void DrawControls(int32 ContextIndex);

private:

	FImGuiModuleProperties& Properties;

	ImVec4 ClearColor = ImColor{ 114, 144, 154 };

	int32 ShowDemoWindowMask = 0;
	int32 ShowAnotherWindowMask = 0;

	int32 DemoWindowCounter = 0;
	uint32 LastDemoWindowFrameNumber = 0;
};
