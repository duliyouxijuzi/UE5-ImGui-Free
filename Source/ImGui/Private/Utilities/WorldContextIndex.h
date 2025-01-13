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

#include "Utilities/WorldContext.h"


// Utilities mapping worlds to indices that we use to identify ImGui contexts.

namespace Utilities
{
	// Invalid context index for parameters that cannot be resolved to a valid world.
	inline constexpr int32 INVALID_CONTEXT_INDEX = -10;

	// Standalone context index.
	inline constexpr int32 STANDALONE_GAME_CONTEXT_INDEX = -2;

#if WITH_EDITOR

	// Editor context index. We are lacking flexibility here, so we might need to change it somehow.
	inline constexpr int32 EDITOR_CONTEXT_INDEX = -1;

	FORCEINLINE int32 GetWorldContextIndex(const FWorldContext& WorldContext)
	{
		switch (WorldContext.WorldType)
		{
		case EWorldType::PIE:
			return WorldContext.PIEInstance;
		case EWorldType::Game:
			return STANDALONE_GAME_CONTEXT_INDEX;
		case EWorldType::Editor:
			return EDITOR_CONTEXT_INDEX;
		default:
			return INVALID_CONTEXT_INDEX;
		}
	}

	template<typename T>
	FORCEINLINE int32 GetWorldContextIndex(const T& Obj)
	{
		const FWorldContext* WorldContext = GetWorldContext(Obj);
		return WorldContext ? GetWorldContextIndex(*WorldContext) : INVALID_CONTEXT_INDEX;
	}

	FORCEINLINE int32 GetWorldContextIndex(const UWorld& World)
	{
		return (World.WorldType == EWorldType::Editor) ? EDITOR_CONTEXT_INDEX : GetWorldContextIndex(World.GetGameInstance());
	}

	FORCEINLINE int32 GetWorldContextIndex(const UWorld* World)
	{
		return World ? GetWorldContextIndex(*World) : INVALID_CONTEXT_INDEX;
	}

#else

	template<typename T>
	constexpr int32 GetWorldContextIndex(const T&)
	{
		// The only option is standalone game with one context.
		return STANDALONE_GAME_CONTEXT_INDEX;
	}

#endif // #if WITH_EDITOR
}
