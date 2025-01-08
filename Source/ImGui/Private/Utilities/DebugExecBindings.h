// Copyright (c) 2017-2021 Sebastian Gross. Distributed under the MIT License (MIT).
// See accompanying LICENSE file for details. This project is free to use under the terms of the MIT License.

#pragma once

class FString;
struct FImGuiKeyInfo;

namespace DebugExecBindings
{
	void UpdatePlayerInputs(const FImGuiKeyInfo& KeyInfo, const FString& Command);
}
