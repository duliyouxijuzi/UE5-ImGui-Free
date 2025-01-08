// Copyright (c) 2017-2021 Sebastian Gross. Distributed under the MIT License (MIT).
// See accompanying LICENSE file for details. This project is free to use under the terms of the MIT License.

#include "WorldContext.h"


namespace Utilities
{
	const FWorldContext* GetWorldContextFromNetMode(ENetMode NetMode)
	{
		checkf(GEngine, TEXT("GEngine required to get list of worlds."));

		for (const FWorldContext& WorldContext : GEngine->GetWorldContexts())
		{
			if (WorldContext.World() && WorldContext.World()->GetNetMode() == NetMode)
			{
				return &WorldContext;
			}
		}

		return nullptr;
	}
}
