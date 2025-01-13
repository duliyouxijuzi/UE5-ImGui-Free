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

struct ImFontConfig;

/** Properties that define state of the ImGui module. */
class IMGUI_API FImGuiModuleProperties
{
public:

	/** Check whether input is enabled. */
	bool IsInputEnabled() const { return bInputEnabled; }

	/** Enable or disable ImGui input. */
	void SetInputEnabled(bool bEnabled) { bInputEnabled = bEnabled; }

	/** Toggle ImGui input. */
	void ToggleInput() { SetInputEnabled(!IsInputEnabled()); }

	/** Check whether keyboard navigation is enabled. */
	bool IsKeyboardNavigationEnabled() const { return bKeyboardNavigationEnabled; }

	/** Enable or disable keyboard navigation. */
	void SetKeyboardNavigationEnabled(bool bEnabled) { bKeyboardNavigationEnabled = bEnabled; }

	/** Toggle keyboard navigation. */
	void ToggleKeyboardNavigation() { SetKeyboardNavigationEnabled(!IsKeyboardNavigationEnabled()); }

	/** Check whether gamepad navigation is enabled. */
	bool IsGamepadNavigationEnabled() const { return bGamepadNavigationEnabled; }

	/** Enable or disable gamepad navigation. */
	void SetGamepadNavigationEnabled(bool bEnabled) { bGamepadNavigationEnabled = bEnabled; }

	/** Toggle gamepad navigation. */
	void ToggleGamepadNavigation() { SetGamepadNavigationEnabled(!IsGamepadNavigationEnabled()); }

	/** Check whether keyboard input is shared with game. */
	bool IsKeyboardInputShared() const { return bKeyboardInputShared; }

	/** Set whether keyboard input should be shared with game. */
	void SetKeyboardInputShared(bool bShared) { bKeyboardInputShared = bShared; }

	/** Toggle whether keyboard input should be shared with game. */
	void ToggleKeyboardInputSharing() { SetKeyboardInputShared(!IsKeyboardInputShared()); }

	/** Check whether gamepad input is shared with game. */
	bool IsGamepadInputShared() const { return bGamepadInputShared; }

	/** Set whether gamepad input should be shared with game. */
	void SetGamepadInputShared(bool bShared) { bGamepadInputShared = bShared; }

	/** Toggle whether gamepad input should be shared with game. */
	void ToggleGamepadInputSharing() { SetGamepadInputShared(!IsGamepadInputShared()); }

	/** Check whether mouse input is shared with game. */
	bool IsMouseInputShared() const { return bMouseInputShared; }

	/** Set whether mouse input should be shared with game. */
	void SetMouseInputShared(bool bShared) { bMouseInputShared = bShared; }

	/** Toggle whether mouse input should be shared with game. */
	void ToggleMouseInputSharing() { SetMouseInputShared(!IsMouseInputShared()); }

	/** Check whether ImGui demo is visible. */
	bool ShowDemo() const { return bShowDemo; }

	/** Show or hide ImGui demo. */
	void SetShowDemo(bool bShow) { bShowDemo = bShow; }

	/** Toggle ImGui demo. */
	void ToggleDemo() { SetShowDemo(!ShowDemo()); }

	/** Adds a new font to initialize */
	void AddCustomFont(FName FontName, TSharedPtr<ImFontConfig> Font) { CustomFonts.Emplace(FontName, Font); }

	/** Removes a font from the custom font list */
	void RemoveCustomFont(FName FontName) { CustomFonts.Remove(FontName); }

	/** Gets the map of registered custom fonts */
	TMap<FName, TSharedPtr<ImFontConfig>>& GetCustomFonts() { return CustomFonts; }

private:

	bool bInputEnabled = false;

	bool bKeyboardNavigationEnabled = false;
	bool bGamepadNavigationEnabled = false;

	bool bKeyboardInputShared = false;
	bool bGamepadInputShared = false;
	bool bMouseInputShared = false;

	bool bShowDemo = false;

	TMap<FName, TSharedPtr<ImFontConfig>> CustomFonts;
};
