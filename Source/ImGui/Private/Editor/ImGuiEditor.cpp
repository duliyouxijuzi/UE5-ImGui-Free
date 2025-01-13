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

#include "ImGuiEditor.h"

#if WITH_EDITOR

#include "ImGuiCanvasSizeInfoCustomization.h"
#include "ImGuiKeyInfoCustomization.h"
#include "ImGuiModuleSettings.h"

#include <ISettingsModule.h>
#include <Modules/ModuleManager.h>


#define LOCTEXT_NAMESPACE "ImGuiEditor"

#define SETTINGS_CONTAINER TEXT("Project"), TEXT("Plugins"), TEXT("ImGui")


namespace
{
	ISettingsModule* GetSettingsModule()
	{
		return FModuleManager::GetModulePtr<ISettingsModule>("Settings");
	}

	FPropertyEditorModule* GetPropertyEditorModule()
	{
		return FModuleManager::GetModulePtr<FPropertyEditorModule>("PropertyEditor");
	}
}

FImGuiEditor::FImGuiEditor()
{
	Register();

	// As a side effect of being part of the ImGui module, we need to support deferred registration (only executed if
	// module is loaded manually at the very early stage).
	if (!IsRegistrationCompleted())
	{
		CreateRegistrator();
	}
}

FImGuiEditor::~FImGuiEditor()
{
	Unregister();
}

void FImGuiEditor::Register()
{
	// Only register after UImGuiSettings class is initialized (necessary to check in early loading stages).
	if (!bSettingsRegistered && UImGuiSettings::Get())
	{
		if (ISettingsModule* SettingsModule = GetSettingsModule())
		{
			bSettingsRegistered = true;

			SettingsModule->RegisterSettings(SETTINGS_CONTAINER,
				LOCTEXT("ImGuiSettingsName", "ImGui"),
				LOCTEXT("ImGuiSettingsDescription", "Configure the Unreal ImGui plugin."),
				UImGuiSettings::Get());
		}
	}

	if (!bCustomPropertyTypeLayoutsRegistered)
	{
		if (FPropertyEditorModule* PropertyModule = GetPropertyEditorModule())
		{
			bCustomPropertyTypeLayoutsRegistered = true;

			PropertyModule->RegisterCustomPropertyTypeLayout("ImGuiCanvasSizeInfo",
				FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FImGuiCanvasSizeInfoCustomization::MakeInstance));
			PropertyModule->RegisterCustomPropertyTypeLayout("ImGuiKeyInfo",
				FOnGetPropertyTypeCustomizationInstance::CreateStatic(&FImGuiKeyInfoCustomization::MakeInstance));
		}
	}
}

void FImGuiEditor::Unregister()
{
	if (bSettingsRegistered)
	{
		bSettingsRegistered = false;

		if (ISettingsModule* SettingsModule = GetSettingsModule())
		{
			SettingsModule->UnregisterSettings(SETTINGS_CONTAINER);
		}
	}

	if (bCustomPropertyTypeLayoutsRegistered)
	{
		bCustomPropertyTypeLayoutsRegistered = false;

		if (FPropertyEditorModule* PropertyModule = GetPropertyEditorModule())
		{
			PropertyModule->UnregisterCustomPropertyTypeLayout("ImGuiCanvasSizeInfo");
			PropertyModule->UnregisterCustomPropertyTypeLayout("ImGuiKeyInfo");
		}
	}
}

void FImGuiEditor::CreateRegistrator()
{
	if (!RegistratorHandle.IsValid())
	{
		RegistratorHandle = FModuleManager::Get().OnModulesChanged().AddLambda([this](FName Name, EModuleChangeReason Reason)
		{
			if (Reason == EModuleChangeReason::ModuleLoaded)
			{
				Register();
			}

			if (IsRegistrationCompleted())
			{
				ReleaseRegistrator();
			}
		});
	}
}

void FImGuiEditor::ReleaseRegistrator()
{
	if (RegistratorHandle.IsValid())
	{
		FModuleManager::Get().OnModulesChanged().Remove(RegistratorHandle);
		RegistratorHandle.Reset();
	}
}


#undef SETTINGS_CONTAINER
#undef LOCTEXT_NAMESPACE

#endif // WITH_EDITOR
