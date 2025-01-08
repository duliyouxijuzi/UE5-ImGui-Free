// Copyright (c) 2017-2021 Sebastian Gross. Distributed under the MIT License (MIT).
// See accompanying LICENSE file for details. This project is free to use under the terms of the MIT License.

#include "ImGuiDelegates.h"
#include "ImGuiDelegatesContainer.h"

#include <Engine/World.h>


FSimpleMulticastDelegate& FImGuiDelegates::OnWorldEarlyDebug()
{
	return OnWorldEarlyDebug(GWorld);
}

FSimpleMulticastDelegate& FImGuiDelegates::OnWorldEarlyDebug(UWorld* World)
{
	return FImGuiDelegatesContainer::Get().OnWorldEarlyDebug(World);
}

FSimpleMulticastDelegate& FImGuiDelegates::OnMultiContextEarlyDebug()
{
	return FImGuiDelegatesContainer::Get().OnMultiContextEarlyDebug();
}

FSimpleMulticastDelegate& FImGuiDelegates::OnWorldDebug()
{
	return OnWorldDebug(GWorld);
}

FSimpleMulticastDelegate& FImGuiDelegates::OnWorldDebug(UWorld* World)
{
	return FImGuiDelegatesContainer::Get().OnWorldDebug(World);
}

FSimpleMulticastDelegate& FImGuiDelegates::OnMultiContextDebug()
{
	return FImGuiDelegatesContainer::Get().OnMultiContextDebug();
}
