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

#include <Engine/Engine.h>
#include <Engine/GameInstance.h>
#include <Engine/GameViewportClient.h>
#include <UObject/WeakObjectPtr.h>


// Utilities helping to get a World Context.

namespace Utilities
{
	template<typename T>
	FORCEINLINE const FWorldContext* GetWorldContext(const T* Obj);

	FORCEINLINE const FWorldContext* GetWorldContext(const UGameInstance& GameInstance)
	{
		return GameInstance.GetWorldContext();
	}

	template<typename T>
	FORCEINLINE const FWorldContext* GetWorldContext(const TWeakObjectPtr<T>& Obj)
	{
		return Obj.IsValid() ? GetWorldContext(*Obj.Get()) : nullptr;
	}

	FORCEINLINE const FWorldContext* GetWorldContext(const UGameViewportClient& GameViewportClient)
	{
		return GetWorldContext(GameViewportClient.GetGameInstance());
	}

	FORCEINLINE const FWorldContext* GetWorldContext(const UWorld& World)
	{
		return GetWorldContext(World.GetGameInstance());
	}

	template<typename T>
	FORCEINLINE const FWorldContext* GetWorldContext(const T* Obj)
	{
		return Obj ? GetWorldContext(*Obj) : nullptr;
	}

	const FWorldContext* GetWorldContextFromNetMode(ENetMode NetMode);
}
