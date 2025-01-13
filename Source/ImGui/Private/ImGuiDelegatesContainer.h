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

#include <Containers/Map.h>
#include <Delegates/Delegate.h>


#if WITH_EDITOR
struct FImGuiDelegatesContainerHandle;
#endif

struct FImGuiDelegatesContainer
{
public:

	// Get the current instance (can change during hot-reloading).
	static FImGuiDelegatesContainer& Get();

#if WITH_EDITOR
	// Get the handle to the container instance (can attach to other handles in hot-reloaded modules).
	static FImGuiDelegatesContainerHandle& GetHandle();

	// Redirect to the other container and if this one is still active move its data to the other one.
	static void MoveContainer(FImGuiDelegatesContainerHandle& OtherContainerHandle);
#endif

	// Get delegate to ImGui world early debug event from known world instance.
	FSimpleMulticastDelegate& OnWorldEarlyDebug(UWorld* World) { return OnWorldEarlyDebug(GetContextIndex(World)); }

	// Get delegate to ImGui world early debug event from known context index.
	FSimpleMulticastDelegate& OnWorldEarlyDebug(int32 ContextIndex) { return WorldEarlyDebugDelegates.FindOrAdd(ContextIndex); }

	// Get delegate to ImGui multi-context early debug event.
	FSimpleMulticastDelegate& OnMultiContextEarlyDebug() { return MultiContextEarlyDebugDelegate; }

	// Get delegate to ImGui world debug event from known world instance.
	FSimpleMulticastDelegate& OnWorldDebug(UWorld* World) { return OnWorldDebug(GetContextIndex(World)); }

	// Get delegate to ImGui world debug event from known context index.
	FSimpleMulticastDelegate& OnWorldDebug(int32 ContextIndex) { return WorldDebugDelegates.FindOrAdd(ContextIndex); }

	// Get delegate to ImGui multi-context debug event.
	FSimpleMulticastDelegate& OnMultiContextDebug() { return MultiContextDebugDelegate; }

private:

	int32 GetContextIndex(UWorld* World);

	void Clear();

	TMap<int32, FSimpleMulticastDelegate> WorldEarlyDebugDelegates;
	TMap<int32, FSimpleMulticastDelegate> WorldDebugDelegates;
	FSimpleMulticastDelegate MultiContextEarlyDebugDelegate;
	FSimpleMulticastDelegate MultiContextDebugDelegate;
};
