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

#include <CoreMinimal.h>

#include <imgui.h>


/**
 * Handle to texture resources registered in module instance. Returned after successful texture registration.
 * Can be implicitly converted to ImTextureID making it possible to use it directly with ImGui interface.
 * Once texture is not needed handle can be used to release resources.
 */
class IMGUI_API FImGuiTextureHandle
{
public:

	/** Creates an empty (null and not valid) texture handle. */
	FImGuiTextureHandle();

	/**
	 * Checks whether this handle is null. Can be used as a quick test whether it points to any resources but it does
	 * not check whether those resources are valid (see @ IsValid).
	 *
	 * @returns True, if this handle is null (Name is NAME_None and TextureId is invalid) and false otherwise.
	 */
	bool IsNull() const { return Name == NAME_None; }

	/**
	 * Checks whether this handle is not null and valid. Valid handle points to valid texture resources.
	 * It is slower but safer test, more useful when there is no guarantee that resources haven't been released.
	 *
	 * @returns True, if this handle is not null and valid, false otherwise.
	 */
	bool IsValid() const { return !IsNull() && HasValidEntry(); }

	/** Get the name of the texture resources (NAME_None if handle is null). */
	const FName& GetName() const { return Name; }

	/** Get the ImGui texture id for this texture (invalid if handle is null). */
	ImTextureID GetTextureId() const { return TextureId; }

	/** Implicit conversion to ImTextureID. */
	operator ImTextureID() const { return GetTextureId(); }

private:

	/**
	 * Creates a texture handle with known name and texture id.
	 * @param InName - Name of the texture
	 * @param InTextureId - ImGui id of texture
	 */
	FImGuiTextureHandle(const FName& InName, ImTextureID InTextureId);

	/** Checks if texture manager has entry that matches this name and texture id index. */
	bool HasValidEntry() const;

	FName Name;
	ImTextureID TextureId;

	// Give module class a private access, so it can create valid handles.
	friend class FImGuiModule;
};
