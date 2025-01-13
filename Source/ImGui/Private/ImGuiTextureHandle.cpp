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

#include "ImGuiTextureHandle.h"

#include "ImGuiInteroperability.h"


FImGuiTextureHandle::FImGuiTextureHandle()
	: FImGuiTextureHandle(NAME_None, ImGuiInterops::ToImTextureID(INDEX_NONE))
{
}

FImGuiTextureHandle::FImGuiTextureHandle(const FName& InName, ImTextureID InTextureId)
	: Name(InName)
	, TextureId(InTextureId)
{
	const TextureIndex Index = ImGuiInterops::ToTextureIndex(TextureId);
	checkf((Index == INDEX_NONE) == (Name == NAME_None),
		TEXT("Mismatch between Name and TextureId parameters, with only one indicating a null handle.")
		TEXT(" Name = '%s', TextureIndex(TextureId) = %d"), *Name.ToString(), Index);
}

// FImGuiTextureHandle::HasValidEntry() is implemented in ImGuiModule.cpp to get access to FImGuiModuleManager instance
// without referencing in this class.
