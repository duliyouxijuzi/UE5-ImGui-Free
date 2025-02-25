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

#include "ImGuiModuleSettings.h"

#include "ImGuiModuleCommands.h"
#include "ImGuiModuleProperties.h"

#include <Engine/Engine.h>
#include <GameFramework/GameUserSettings.h>
#include <Misc/ConfigCacheIni.h>
#include "GenericPlatform/GenericApplication.h"
#include "Framework/Application/SlateApplication.h"

//====================================================================================================
// FImGuiDPIScaleInfo
//====================================================================================================

FImGuiDPIScaleInfo::FImGuiDPIScaleInfo()
{
	if (FRichCurve* Curve = DPICurve.GetRichCurve())
	{
		Curve->AddKey(   0.0f, 1.f);

		Curve->AddKey(2159.5f, 1.f);
		Curve->AddKey(2160.0f, 2.f);

		Curve->AddKey(4319.5f, 2.f);
		Curve->AddKey(4320.0f, 4.f);
	}
}

float FImGuiDPIScaleInfo::CalculateResolutionBasedScale() const
{
	float ResolutionBasedScale = 1.f;
	if (bScaleWithCurve)
	{
		if (const FRichCurve* Curve = DPICurve.GetRichCurveConst())
		{
			FDisplayMetrics DisplayMetrics;
			{
				DisplayMetrics.PrimaryDisplayWidth = 0;
				DisplayMetrics.PrimaryDisplayHeight = 0;
			}

			if (FSlateApplication::IsInitialized())
			{
				FSlateApplication::Get().GetInitialDisplayMetrics(DisplayMetrics);
			}

			ResolutionBasedScale *= Curve->Eval((float)DisplayMetrics.PrimaryDisplayHeight, 1.f);
		}
	}
	return ResolutionBasedScale;
}

//====================================================================================================
// UImGuiSettings
//====================================================================================================

UImGuiSettings* UImGuiSettings::DefaultInstance = nullptr;

FSimpleMulticastDelegate UImGuiSettings::OnSettingsLoaded;

void UImGuiSettings::PostInitProperties()
{
	Super::PostInitProperties();

	if (IsTemplate())
	{
		DefaultInstance = this;
		OnSettingsLoaded.Broadcast();
	}
}

void UImGuiSettings::BeginDestroy()
{
	Super::BeginDestroy();

	if (DefaultInstance == this)
	{
		DefaultInstance = nullptr;
	}
}

//====================================================================================================
// FImGuiModuleSettings
//====================================================================================================

FImGuiModuleSettings::FImGuiModuleSettings(FImGuiModuleProperties& InProperties, FImGuiModuleCommands& InCommands)
	: Properties(InProperties)
	, Commands(InCommands)
{
#if WITH_EDITOR
	FCoreUObjectDelegates::OnObjectPropertyChanged.AddRaw(this, &FImGuiModuleSettings::OnPropertyChanged);
#endif

	// Delegate initializer to support settings loaded after this object creation (in stand-alone builds) and potential
	// reloading of settings.
	UImGuiSettings::OnSettingsLoaded.AddRaw(this, &FImGuiModuleSettings::InitializeAllSettings);

	// Call initializer to support settings already loaded (editor).
	InitializeAllSettings();
}

FImGuiModuleSettings::~FImGuiModuleSettings()
{

	UImGuiSettings::OnSettingsLoaded.RemoveAll(this);

#if WITH_EDITOR
	FCoreUObjectDelegates::OnObjectPropertyChanged.RemoveAll(this);
#endif
}

void FImGuiModuleSettings::InitializeAllSettings()
{
	UpdateSettings();
	UpdateDPIScaleInfo();
}

void FImGuiModuleSettings::UpdateSettings()
{
	if (UImGuiSettings* SettingsObject = UImGuiSettings::Get())
	{
		SetImGuiInputHandlerClass(SettingsObject->ImGuiInputHandlerClass);
		SetShareKeyboardInput(SettingsObject->bShareKeyboardInput);
		SetShareGamepadInput(SettingsObject->bShareGamepadInput);
		SetShareMouseInput(SettingsObject->bShareMouseInput);
		SetUseSoftwareCursor(SettingsObject->bUseSoftwareCursor);
		SetToggleInputKey(SettingsObject->ToggleInput);
		SetCanvasSizeInfo(SettingsObject->CanvasSize);
	}
}

void FImGuiModuleSettings::UpdateDPIScaleInfo()
{
	if (UImGuiSettings* SettingsObject = UImGuiSettings::Get())
	{
		SetDPIScaleInfo(SettingsObject->DPIScale);
	}
}

void FImGuiModuleSettings::SetImGuiInputHandlerClass(const FSoftClassPath& ClassReference)
{
	if (ImGuiInputHandlerClass != ClassReference)
	{
		ImGuiInputHandlerClass = ClassReference;
		OnImGuiInputHandlerClassChanged.Broadcast(ClassReference);
	}
}

void FImGuiModuleSettings::SetShareKeyboardInput(bool bShare)
{
	if (bShareKeyboardInput != bShare)
	{
		bShareKeyboardInput = bShare;
		Properties.SetKeyboardInputShared(bShare);
	}
}

void FImGuiModuleSettings::SetShareGamepadInput(bool bShare)
{
	if (bShareGamepadInput != bShare)
	{
		bShareGamepadInput = bShare;
		Properties.SetGamepadInputShared(bShare);
	}
}

void FImGuiModuleSettings::SetShareMouseInput(bool bShare)
{
	if (bShareMouseInput != bShare)
	{
		bShareMouseInput = bShare;
		Properties.SetMouseInputShared(bShare);
	}
}

void FImGuiModuleSettings::SetUseSoftwareCursor(bool bUse)
{
	if (bUseSoftwareCursor != bUse)
	{
		bUseSoftwareCursor = bUse;
		OnUseSoftwareCursorChanged.Broadcast(bUse);
	}
}

void FImGuiModuleSettings::SetToggleInputKey(const FImGuiKeyInfo& KeyInfo)
{
	if (ToggleInputKey != KeyInfo)
	{
		ToggleInputKey = KeyInfo;
		Commands.SetKeyBinding(FImGuiModuleCommands::ToggleInput, ToggleInputKey);
	}
}

void FImGuiModuleSettings::SetCanvasSizeInfo(const FImGuiCanvasSizeInfo& CanvasSizeInfo)
{
	if (CanvasSize != CanvasSizeInfo)
	{
		CanvasSize = CanvasSizeInfo;
		OnCanvasSizeChangedDelegate.Broadcast(CanvasSize);
	}
}

void FImGuiModuleSettings::SetDPIScaleInfo(const FImGuiDPIScaleInfo& ScaleInfo)
{
	DPIScale = ScaleInfo;
	OnDPIScaleChangedDelegate.Broadcast(DPIScale);
}

#if WITH_EDITOR

void FImGuiModuleSettings::OnPropertyChanged(class UObject* ObjectBeingModified, struct FPropertyChangedEvent& PropertyChangedEvent)
{
	if (ObjectBeingModified == UImGuiSettings::Get())
	{
		UpdateSettings();
		if (PropertyChangedEvent.MemberProperty
			&& (PropertyChangedEvent.MemberProperty->GetFName() == GET_MEMBER_NAME_CHECKED(FImGuiModuleSettings, DPIScale)))
		{
			UpdateDPIScaleInfo();
		}
	}
}

#endif // WITH_EDITOR
