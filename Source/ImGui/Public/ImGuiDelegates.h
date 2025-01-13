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

#include <Delegates/Delegate.h>


class UWorld;

/**
 * Delegates to ImGui debug events. World delegates are called once per frame during world updates and have invocation
 * lists cleared after their worlds become invalid. Multi-context delegates are called once for every updated world.
 * Early debug delegates are called during world tick start and debug delegates are called during world post actor tick
 * or in engine versions below 4.18 during world tick start.
 *
 * Order of events is defined in a way that multi-context delegates can be used to draw headers and/or footers:
 * multi-context early debug, world early debug, world debug, multi-context debug.
 */
class IMGUI_API FImGuiDelegates
{
public:

	/**
	 * Get a delegate to ImGui world early debug event for current world (GWorld).
	 * @returns Simple multicast delegate to debug events called once per frame to debug current world
	 */
	static FSimpleMulticastDelegate& OnWorldEarlyDebug();

	/**
	 * Get a delegate to ImGui world early debug event for given world.
	 * @param World - World for which we need a delegate
	 * @returns Simple multicast delegate to debug events called once per frame to debug given world
	 */
	static FSimpleMulticastDelegate& OnWorldEarlyDebug(UWorld* World);

	/**
	 * Get a delegate to ImGui multi-context early debug event.
	 * @returns Simple multicast delegate to debug events called once per frame for every world to debug
	 */
	static FSimpleMulticastDelegate& OnMultiContextEarlyDebug();

	/**
	 * Get a delegate to ImGui world debug event for current world (GWorld).
	 * @returns Simple multicast delegate to debug events called once per frame to debug current world
	 */
	static FSimpleMulticastDelegate& OnWorldDebug();

	/**
	 * Get a delegate to ImGui world debug event for given world.
	 * @param World - World for which we need a delegate
	 * @returns Simple multicast delegate to debug events called once per frame to debug given world
	 */
	static FSimpleMulticastDelegate& OnWorldDebug(UWorld* World);

	/**
	 * Get a delegate to ImGui multi-context debug event.
	 * @returns Simple multicast delegate to debug events called once per frame for every world to debug
	 */
	static FSimpleMulticastDelegate& OnMultiContextDebug();
};


/** Enable to support legacy ImGui delegates API. */
#define IMGUI_WITH_OBSOLETE_DELEGATES 1

#if IMGUI_WITH_OBSOLETE_DELEGATES

/** Delegate that allows to subscribe for ImGui events.  */
typedef FSimpleMulticastDelegate::FDelegate FImGuiDelegate;

/**
 * Handle to ImGui delegate. Contains additional information locating delegates in different contexts.
 */
class FImGuiDelegateHandle
{
public:

	FImGuiDelegateHandle() = default;

	bool IsValid() const
	{
		return Handle.IsValid();
	}

	void Reset()
	{
		Handle.Reset();
		Index = 0;
	}

private:

	FImGuiDelegateHandle(const FDelegateHandle& InHandle, int32 InCategory, int32 InIndex = 0)
		: Handle(InHandle)
		, Category(InCategory)
		, Index(InIndex)
	{
	}

	friend bool operator==(const FImGuiDelegateHandle& Lhs, const FImGuiDelegateHandle& Rhs)
	{
		return Lhs.Handle == Rhs.Handle && Lhs.Category == Rhs.Category && Lhs.Index == Rhs.Index;
	}

	friend bool operator!=(const FImGuiDelegateHandle& Lhs, const FImGuiDelegateHandle& Rhs)
	{
		return !(Lhs == Rhs);
	}

	FDelegateHandle Handle;
	int32 Category = 0;
	int32 Index = 0;

	friend class FImGuiModule;
};

#endif // IMGUI_WITH_OBSOLETE_DELEGATES
