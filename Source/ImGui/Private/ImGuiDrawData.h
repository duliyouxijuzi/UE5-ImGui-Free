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

#include "ImGuiInteroperability.h"
#include "VersionCompatibility.h"

#include <Rendering/RenderingCommon.h>

#include <imgui.h>


// ImGui draw command data transformed for Slate.
struct FImGuiDrawCommand
{
	uint32 NumElements;
	FSlateRect ClippingRect;
	TextureIndex TextureId;
};

// Wraps raw ImGui draw list data in utilities that transform them for Slate.
class FImGuiDrawList
{
public:

	// Get the number of draw commands in this list.
	FORCEINLINE int NumCommands() const { return ImGuiCommandBuffer.Size; }

	// Get the draw command by number.
	// @param CommandNb - Number of draw command
	// @param Transform - Transform to apply to clipping rectangle
	// @returns Draw command data
	FImGuiDrawCommand GetCommand(int CommandNb, const FTransform2D& Transform) const
	{
		const ImDrawCmd& ImGuiCommand = ImGuiCommandBuffer[CommandNb];
		return { ImGuiCommand.ElemCount, TransformRect(Transform, ImGuiInterops::ToSlateRect(ImGuiCommand.ClipRect)),
			ImGuiInterops::ToTextureIndex(ImGuiCommand.TextureId) };
	}

#if ENGINE_COMPATIBILITY_LEGACY_CLIPPING_API
	// Transform and copy vertex data to target buffer (old data in the target buffer are replaced).
	// @param OutVertexBuffer - Destination buffer
	// @param Transform - Transform to apply to all vertices
	// @param VertexClippingRect - Clipping rectangle for transformed Slate vertices
	void CopyVertexData(TArray<FSlateVertex>& OutVertexBuffer, const FTransform2D& Transform, const FSlateRotatedRect& VertexClippingRect) const;
#else
	// Transform and copy vertex data to target buffer (old data in the target buffer are replaced).
	// @param OutVertexBuffer - Destination buffer
	// @param Transform - Transform to apply to all vertices
	void CopyVertexData(TArray<FSlateVertex>& OutVertexBuffer, const FTransform2D& Transform) const;
#endif // ENGINE_COMPATIBILITY_LEGACY_CLIPPING_API

	// Transform and copy index data to target buffer (old data in the target buffer are replaced).
	// Internal index buffer contains enough data to match the sum of NumElements from all draw commands.
	// @param OutIndexBuffer - Destination buffer
	// @param StartIndex - Start copying source data starting from this index
	// @param NumElements - How many elements we want to copy
	void CopyIndexData(TArray<SlateIndex>& OutIndexBuffer, const int32 StartIndex, const int32 NumElements) const;

	// Transfers data from ImGui source list to this object. Leaves source cleared.
	void TransferDrawData(ImDrawList& Src);

private:

	ImVector<ImDrawCmd> ImGuiCommandBuffer;
	ImVector<ImDrawIdx> ImGuiIndexBuffer;
	ImVector<ImDrawVert> ImGuiVertexBuffer;
};
