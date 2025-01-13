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

#include "DebugExecBindings.h"

#include "ImGuiModuleSettings.h"

#include <GameFramework/PlayerInput.h>
#include <UObject/UObjectIterator.h>

#include "EnhancedPlayerInput.h"
#include "GameFramework/InputSettings.h"


namespace
{
	FKeyBind CreateKeyBind(const FImGuiKeyInfo& KeyInfo, const FString& Command)
	{
		FKeyBind KeyBind;
		KeyBind.Command = Command;
		KeyBind.Key = KeyInfo.Key;
		KeyBind.bDisabled = false;

#define FILL_MODIFIER_DATA(KeyInfoProperty, BindProperty, BindIgnoreProperty)\
			if (KeyInfo.KeyInfoProperty == ECheckBoxState::Undetermined)\
			{\
				KeyBind.BindProperty = KeyBind.BindIgnoreProperty = false;\
			}\
			else\
			{\
				KeyBind.BindProperty = (KeyInfo.KeyInfoProperty == ECheckBoxState::Checked);\
				KeyBind.BindIgnoreProperty = !KeyBind.BindProperty;\
			}

		FILL_MODIFIER_DATA(Shift, Shift, bIgnoreShift);
		FILL_MODIFIER_DATA(Ctrl, Control, bIgnoreCtrl);
		FILL_MODIFIER_DATA(Alt, Alt, bIgnoreAlt);
		FILL_MODIFIER_DATA(Cmd, Cmd, bIgnoreCmd);

#undef FILL_MODIFIER_DATA

		return KeyBind;
	}

	bool IsBindable(const FKey& Key)
	{
#if ENGINE_COMPATIBILITY_LEGACY_KEY_AXIS_API
		return Key.IsValid() && Key != EKeys::AnyKey && !Key.IsFloatAxis() && !Key.IsVectorAxis()
			&& !Key.IsGamepadKey() && !Key.IsModifierKey() && !Key.IsMouseButton();
#else
		return Key.IsValid() && Key != EKeys::AnyKey && !Key.IsAxis1D() && !Key.IsAxis2D()
			&& !Key.IsAxis3D() && !Key.IsGamepadKey() && !Key.IsModifierKey() && !Key.IsMouseButton();
#endif
	}

	void UpdatePlayerInput(UPlayerInput* PlayerInput, const FKeyBind& KeyBind)
	{
		const int32 Index = PlayerInput->DebugExecBindings.IndexOfByPredicate([&](const FKeyBind& PlayerKeyBind)
		{
			return PlayerKeyBind.Command.Equals(KeyBind.Command, ESearchCase::IgnoreCase);
		});

		if (IsBindable(KeyBind.Key))
		{
			if (Index != INDEX_NONE)
			{
				PlayerInput->DebugExecBindings[Index] = KeyBind;
			}
			else
			{
				PlayerInput->DebugExecBindings.Add(KeyBind);
			}
		}
		else
		{
			if (Index != INDEX_NONE)
			{
				PlayerInput->DebugExecBindings.RemoveAt(Index);
			}
		}
	}
}

namespace DebugExecBindings
{
	void UpdatePlayerInputs(const FImGuiKeyInfo& KeyInfo, const FString& Command)
	{
		checkf(!Command.IsEmpty(), TEXT("Empty command."));

		const FKeyBind KeyBind = CreateKeyBind(KeyInfo, Command);

		// Update default player input, so changes will be visible in all PIE sessions created after this point.
		const UInputSettings* InputSettings = GetDefault<UInputSettings>();
		if (UPlayerInput* DefaultPlayerInput = GetMutableDefault<UPlayerInput>(InputSettings->GetDefaultPlayerInputClass()))
		{
			UpdatePlayerInput(DefaultPlayerInput, KeyBind);
		}

		// Update all existing player inputs to see changes in running PIE session.
		for (TObjectIterator<UEnhancedPlayerInput> It; It; ++It)
		{
			UpdatePlayerInput(*It, KeyBind);
		}
	}
}
