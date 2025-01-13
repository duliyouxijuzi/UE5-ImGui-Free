// Copyright (c) 2017-2021 Sebastian Gross. All Rights Reserved.
// Published by LeoGame in 2025.
// This project is distributed under the MIT License (MIT).

// MIT License

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include "ImGuiContextManager.h"
#include "ImGuiDemo.h"
#include "ImGuiModuleCommands.h"
#include "ImGuiModuleProperties.h"
#include "ImGuiModuleSettings.h"
#include "TextureManager.h"
#include "Widgets/SImGuiLayout.h"


// Central manager that implements module logic. It initializes and controls remaining module components.
class FImGuiModuleManager
{
	// Allow module to control life-cycle of this class.
	friend class FImGuiModule;

public:

	// Get interface to module settings.
	FImGuiModuleSettings& GetSettings() { return Settings; }

	// Get interface to module state properties. 
	FImGuiModuleProperties& GetProperties() { return Properties; }

	// Get ImGui contexts manager.
	FImGuiContextManager& GetContextManager() { return ContextManager; }

	// Get texture resources manager.
	FTextureManager& GetTextureManager() { return TextureManager; }

	// Event called right after ImGui is updated, to give other subsystems chance to react.
	FSimpleMulticastDelegate& OnPostImGuiUpdate() { return PostImGuiUpdateEvent; }

	void RebuildFontAtlas();

private:

	FImGuiModuleManager();
	~FImGuiModuleManager();

	FImGuiModuleManager(const FImGuiModuleManager&) = delete;
	FImGuiModuleManager& operator=(const FImGuiModuleManager&) = delete;

	FImGuiModuleManager(FImGuiModuleManager&&) = delete;
	FImGuiModuleManager& operator=(FImGuiModuleManager&&) = delete;

	void LoadTextures();
	void BuildFontAtlasTexture();

	bool IsTickRegistered() { return TickDelegateHandle.IsValid(); }
	void RegisterTick();
	void UnregisterTick();

	void CreateTickInitializer();
	void ReleaseTickInitializer();

	void Tick(float DeltaSeconds);

	void OnViewportCreated();

	void AddWidgetToViewport(UGameViewportClient* GameViewport);
	void AddWidgetsToActiveViewports();

	void OnContextProxyCreated(int32 ContextIndex, FImGuiContextProxy& ContextProxy);

	// Event that we call after ImGui is updated.
	FSimpleMulticastDelegate PostImGuiUpdateEvent;

	// Collection of module state properties.
	FImGuiModuleProperties Properties;

	// Tying module console commands to life-cycle of this manager and module.
	FImGuiModuleCommands Commands;

	// ImGui settings proxy (valid in every loading stage).
	FImGuiModuleSettings Settings;

	// Widget that we add to all created contexts to draw ImGui demo. 
	FImGuiDemo ImGuiDemo;

	// Manager for ImGui contexts.
	FImGuiContextManager ContextManager;

	// Manager for textures resources.
	FTextureManager TextureManager;

	// Slate widgets that we created.
	TArray<TWeakPtr<SImGuiLayout>> Widgets;

	FDelegateHandle TickInitializerHandle;
	FDelegateHandle TickDelegateHandle;
	FDelegateHandle ViewportCreatedHandle;

	bool bTexturesLoaded = false;
};
