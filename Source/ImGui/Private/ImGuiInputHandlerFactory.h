// Copyright (c) 2017-2021 Sebastian Gross. Distributed under the MIT License (MIT).
// See accompanying LICENSE file for details. This project is free to use under the terms of the MIT License.

#pragma once

#include "ImGuiModuleSettings.h"


class FImGuiModuleManager;
class UGameViewportClient;
class UImGuiInputHandler;

class FImGuiInputHandlerFactory
{
public:

	static UImGuiInputHandler* NewHandler(const FSoftClassPath& HandlerClassReference, FImGuiModuleManager* ModuleManager, UGameViewportClient* GameViewport, int32 ContextIndex);

	static void ReleaseHandler(UImGuiInputHandler* Handler);
};
