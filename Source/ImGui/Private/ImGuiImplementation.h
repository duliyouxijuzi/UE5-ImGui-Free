// Copyright (c) 2017-2021 Sebastian Gross. Distributed under the MIT License (MIT).
// See accompanying LICENSE file for details. This project is free to use under the terms of the MIT License.

#pragma once

struct FImGuiContextHandle;

// Gives access to selected ImGui implementation features.
namespace ImGuiImplementation
{
#if WITH_EDITOR
	// Get the handle to the ImGui Context pointer.
	FImGuiContextHandle& GetContextHandle();

	// Set the ImGui Context pointer handle.
	void SetParentContextHandle(FImGuiContextHandle& Parent);
#endif // WITH_EDITOR
}
