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

#include "ImGuiDrawData.h"


#if ENGINE_COMPATIBILITY_LEGACY_CLIPPING_API
void FImGuiDrawList::CopyVertexData(TArray<FSlateVertex>& OutVertexBuffer, const FTransform2D& Transform, const FSlateRotatedRect& VertexClippingRect) const
#else
void FImGuiDrawList::CopyVertexData(TArray<FSlateVertex>& OutVertexBuffer, const FTransform2D& Transform) const
#endif // ENGINE_COMPATIBILITY_LEGACY_CLIPPING_API
{
	// Reset and reserve space in destination buffer.
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 4)
	OutVertexBuffer.SetNumUninitialized(ImGuiVertexBuffer.Size, EAllowShrinking::No);
#else
	OutVertexBuffer.SetNumUninitialized(ImGuiVertexBuffer.Size, false);
#endif

	// Transform and copy vertex data.
	for (int Idx = 0; Idx < ImGuiVertexBuffer.Size; Idx++)
	{
		const ImDrawVert& ImGuiVertex = ImGuiVertexBuffer[Idx];
		FSlateVertex& SlateVertex = OutVertexBuffer[Idx];

		// Final UV is calculated in shader as XY * ZW, so we need set all components.
		SlateVertex.TexCoords[0] = ImGuiVertex.uv.x;
		SlateVertex.TexCoords[1] = ImGuiVertex.uv.y;
		SlateVertex.TexCoords[2] = SlateVertex.TexCoords[3] = 1.f;

#if ENGINE_COMPATIBILITY_LEGACY_CLIPPING_API
		const FVector2D VertexPosition = Transform.TransformPoint(ImGuiInterops::ToVector2D(ImGuiVertex.pos));
		SlateVertex.Position[0] = VertexPosition.X;
		SlateVertex.Position[1] = VertexPosition.Y;
		SlateVertex.ClipRect = VertexClippingRect;
#else
#if ENGINE_COMPATIBILITY_LEGACY_VECTOR2F
		SlateVertex.Position = Transform.TransformPoint(ImGuiInterops::ToVector2D(ImGuiVertex.pos));
#else
		SlateVertex.Position = (FVector2f)Transform.TransformPoint(ImGuiInterops::ToVector2D(ImGuiVertex.pos));
#endif // ENGINE_COMPATIBILITY_LEGACY_VECTOR2F
#endif // ENGINE_COMPATIBILITY_LEGACY_CLIPPING_API

		// Unpack ImU32 color.
		SlateVertex.Color = ImGuiInterops::UnpackImU32Color(ImGuiVertex.col);
	}
}

void FImGuiDrawList::CopyIndexData(TArray<SlateIndex>& OutIndexBuffer, const int32 StartIndex, const int32 NumElements) const
{
#if (ENGINE_MAJOR_VERSION >= 5 && ENGINE_MINOR_VERSION >= 4)
	OutIndexBuffer.SetNumUninitialized(NumElements, EAllowShrinking::No);
#else
	OutIndexBuffer.SetNumUninitialized(NumElements, false);
#endif

	// Copy elements (slow copy because of different sizes of ImDrawIdx and SlateIndex and because SlateIndex can
	// have different size on different platforms).
	for (int i = 0; i < NumElements; i++)
	{
		OutIndexBuffer[i] = ImGuiIndexBuffer[StartIndex + i];
	}
}

void FImGuiDrawList::TransferDrawData(ImDrawList& Src)
{
	// Move data from source to this list.
	Src.CmdBuffer.swap(ImGuiCommandBuffer);
	Src.IdxBuffer.swap(ImGuiIndexBuffer);
	Src.VtxBuffer.swap(ImGuiVertexBuffer);
}
