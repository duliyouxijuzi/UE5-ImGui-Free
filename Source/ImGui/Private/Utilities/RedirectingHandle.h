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
#include <Delegates/DelegateCombinations.h>


namespace Utilities
{
	// Handle initialized as a pointer to a default value, but can be redirected to follow other handles.
	// When detached, it will revert to the default value. Intended for cross-module redirections.
	template<typename T>
	struct TRedirectingHandle
	{
		TRedirectingHandle(T& InDefaultValue)
			: Handle(&InDefaultValue)
			, DefaultHandle(&InDefaultValue)
		{
		}

		~TRedirectingHandle()
		{
			// Broadcast null pointer as a request to detach.
			OnRedirectionUpdate.Broadcast(nullptr);
		}

		// Check whether this handle points to the default value.
		bool IsDefault() const
		{
			return (Handle == DefaultHandle);
		}

		// Get the current value.
		T& Get() const
		{
			return *Handle;
		}

		// Set the other handle as a parent to this one. Passing null or itself will result with detaching from
		// the current parent (if any) and reverting back to the default value.
		void SetParent(TRedirectingHandle* InParent)
		{
			if (InParent != Parent)
			{
				if (Parent)
				{
					Parent->OnRedirectionUpdate.RemoveAll(this);
				}

				// Protecting from setting itself as a parent.
				Parent = (InParent != this) ? InParent : nullptr;

				if (Parent)
				{
					Parent->OnRedirectionUpdate.AddRaw(this, &TRedirectingHandle::UpdateRedirection);
				}

				SetHandle((Parent) ? Parent->Handle : DefaultHandle);
			}
		}

	protected:

		void UpdateRedirection(T* InHandle)
		{
			if (InHandle)
			{
				SetHandle(InHandle);
			}
			else
			{
				// Interpreting null as a signal to detach.
				SetParent(nullptr);
			}
		}

		void SetHandle(T* InHandle)
		{
			if (InHandle != Handle)
			{
				Handle = InHandle;
				OnRedirectionUpdate.Broadcast(Handle);
			}
		}

		T* Handle;
		T* DefaultHandle;

		TRedirectingHandle* Parent = nullptr;

		// Event with a new handle value or null pointer as a signal to detach.
		DECLARE_MULTICAST_DELEGATE_OneParam(FRedirectionUpdateDelegate, T*);
		FRedirectionUpdateDelegate OnRedirectionUpdate;
	};
}
