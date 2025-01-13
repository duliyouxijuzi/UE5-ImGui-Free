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

#include "ImGuiModuleCommands.h"

#include "ImGuiModuleProperties.h"
#include "Utilities/DebugExecBindings.h"


const TCHAR* const FImGuiModuleCommands::ToggleInput = TEXT("ImGui.ToggleInput");
const TCHAR* const FImGuiModuleCommands::ToggleKeyboardNavigation = TEXT("ImGui.ToggleKeyboardNavigation");
const TCHAR* const FImGuiModuleCommands::ToggleGamepadNavigation = TEXT("ImGui.ToggleGamepadNavigation");
const TCHAR* const FImGuiModuleCommands::ToggleKeyboardInputSharing = TEXT("ImGui.ToggleKeyboardInputSharing");
const TCHAR* const FImGuiModuleCommands::ToggleGamepadInputSharing = TEXT("ImGui.ToggleGamepadInputSharing");
const TCHAR* const FImGuiModuleCommands::ToggleMouseInputSharing = TEXT("ImGui.ToggleMouseInputSharing");
const TCHAR* const FImGuiModuleCommands::SetMouseInputSharing = TEXT("ImGui.SetMouseInputSharing");
const TCHAR* const FImGuiModuleCommands::ToggleDemo = TEXT("ImGui.ToggleDemo");

FImGuiModuleCommands::FImGuiModuleCommands(FImGuiModuleProperties& InProperties)
	: Properties(InProperties)
	, ToggleInputCommand(ToggleInput,
		TEXT("Toggle ImGui input mode."),
		FConsoleCommandDelegate::CreateRaw(this, &FImGuiModuleCommands::ToggleInputImpl))
	, ToggleKeyboardNavigationCommand(ToggleKeyboardNavigation,
		TEXT("Toggle ImGui keyboard navigation."),
		FConsoleCommandDelegate::CreateRaw(this, &FImGuiModuleCommands::ToggleKeyboardNavigationImpl))
	, ToggleGamepadNavigationCommand(ToggleGamepadNavigation,
		TEXT("Toggle ImGui gamepad navigation."),
		FConsoleCommandDelegate::CreateRaw(this, &FImGuiModuleCommands::ToggleGamepadNavigationImpl))
	, ToggleKeyboardInputSharingCommand(ToggleKeyboardInputSharing,
		TEXT("Toggle ImGui keyboard input sharing."),
		FConsoleCommandDelegate::CreateRaw(this, &FImGuiModuleCommands::ToggleKeyboardInputSharingImpl))
	, ToggleGamepadInputSharingCommand(ToggleGamepadInputSharing,
		TEXT("Toggle ImGui gamepad input sharing."),
		FConsoleCommandDelegate::CreateRaw(this, &FImGuiModuleCommands::ToggleGamepadInputSharingImpl))
	, ToggleMouseInputSharingCommand(ToggleMouseInputSharing,
		TEXT("Toggle ImGui mouse input sharing."),
		FConsoleCommandDelegate::CreateRaw(this, &FImGuiModuleCommands::ToggleMouseInputSharingImpl))
	, SetMouseInputSharingCommand(SetMouseInputSharing,
		TEXT("Toggle ImGui mouse input sharing."),
		FConsoleCommandWithArgsDelegate::CreateRaw(this, &FImGuiModuleCommands::SetMouseInputSharingImpl))
	, ToggleDemoCommand(ToggleDemo,
		TEXT("Toggle ImGui demo."),
		FConsoleCommandDelegate::CreateRaw(this, &FImGuiModuleCommands::ToggleDemoImpl))
{
}

void FImGuiModuleCommands::SetKeyBinding(const TCHAR* CommandName, const FImGuiKeyInfo& KeyInfo)
{
	DebugExecBindings::UpdatePlayerInputs(KeyInfo, CommandName);
}

void FImGuiModuleCommands::ToggleInputImpl()
{
	Properties.ToggleInput();
}

void FImGuiModuleCommands::ToggleKeyboardNavigationImpl()
{
	Properties.ToggleKeyboardNavigation();
}

void FImGuiModuleCommands::ToggleGamepadNavigationImpl()
{
	Properties.ToggleGamepadNavigation();
}

void FImGuiModuleCommands::ToggleKeyboardInputSharingImpl()
{
	Properties.ToggleKeyboardInputSharing();
}

void FImGuiModuleCommands::ToggleGamepadInputSharingImpl()
{
	Properties.ToggleGamepadInputSharing();
}

void FImGuiModuleCommands::ToggleMouseInputSharingImpl()
{
	Properties.ToggleMouseInputSharing();
}

void FImGuiModuleCommands::SetMouseInputSharingImpl(const TArray<FString>& Args)
{
	bool bIsEnabled = false;
	if (Args.Num() > 0)
	{
		LexFromString(bIsEnabled, *Args[0]);
	}
	Properties.SetMouseInputShared(bIsEnabled);
}

void FImGuiModuleCommands::ToggleDemoImpl()
{
	Properties.ToggleDemo();
}
