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

#include "ImGuiDelegatesContainer.h"

#include "ImGuiModule.h"
#include "Utilities/WorldContextIndex.h"


#if !WITH_EDITOR
//
// Non-editor version without container redirection
//

FImGuiDelegatesContainer& FImGuiDelegatesContainer::Get()
{
	static FImGuiDelegatesContainer* Container = new FImGuiDelegatesContainer();
	return *Container;
}

#endif // !WITH_EDITOR


#if WITH_EDITOR
//
// Editor version supporting container redirection needed for hot-reloading
//

#include "Utilities/RedirectingHandle.h"

// Redirecting handle which will always bind to a container from the currently loaded module.
struct FImGuiDelegatesContainerHandle : Utilities::TRedirectingHandle<FImGuiDelegatesContainer>
{
	FImGuiDelegatesContainerHandle(FImGuiDelegatesContainer& InDefaultContainer)
		: Utilities::TRedirectingHandle<FImGuiDelegatesContainer>(InDefaultContainer)
	{
		if (FImGuiModule* Module = FModuleManager::GetModulePtr<FImGuiModule>("ImGui"))
		{
			SetParent(Module->DelegatesContainerHandle);
		}
	}
};

FImGuiDelegatesContainer& FImGuiDelegatesContainer::Get()
{
	return GetHandle().Get();
}

FImGuiDelegatesContainerHandle& FImGuiDelegatesContainer::GetHandle()
{
	struct FContainerInstance
	{
		FContainerInstance() : Container(), Handle(Container) {}
		FImGuiDelegatesContainer Container;
		FImGuiDelegatesContainerHandle Handle;
	};
	static FContainerInstance* Instance = new FContainerInstance();
	return Instance->Handle;
}

void FImGuiDelegatesContainer::MoveContainer(FImGuiDelegatesContainerHandle& OtherContainerHandle)
{
	// Only move data if pointer points to default instance, otherwise our data has already been moved and we only
	// keep pointer to a more recent version.
	if (GetHandle().IsDefault())
	{
		OtherContainerHandle.Get() = MoveTemp(GetHandle().Get());
		GetHandle().Get().Clear();
	}

	// Update pointer to the most recent version.
	GetHandle().SetParent(&OtherContainerHandle);
}

#endif // WITH_EDITOR


int32 FImGuiDelegatesContainer::GetContextIndex(UWorld* World)
{
	return Utilities::GetWorldContextIndex(*World);
}

void FImGuiDelegatesContainer::Clear()
{
	WorldEarlyDebugDelegates.Empty();
	WorldDebugDelegates.Empty();
	MultiContextEarlyDebugDelegate.Clear();
	MultiContextDebugDelegate.Clear();
}
