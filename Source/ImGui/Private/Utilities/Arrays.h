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

#include "Range.h"

#include <array>
#include <iterator>
#include <type_traits>


// Utilities to work with one-dimensional, statically bound arrays. Code relying on those utilities should work without
// modifications with fixed-sized arrays (currently used in ImGui) and with standard arrays.

namespace Utilities
{
	//====================================================================================================
	// Helper functions
	//====================================================================================================

	// Function to determine number of elements in fixed size array.
	template<class T, std::size_t N>
	constexpr std::size_t GetArraySize(const T(&)[N])
	{
		return N;
	}

	// Function to determine number of elements in std array.
	template<class T, std::size_t N>
	constexpr std::size_t GetArraySize(const std::array<T, N>&)
	{
		return N;
	}


	//====================================================================================================
	// Traits
	//====================================================================================================

	template<typename TArray>
	struct ArraySize;

	// Struct to determine number of elements in fixed size array.
	template<typename T, std::size_t N>
	struct ArraySize<T[N]> : std::extent<T[N]>
	{
	};

	// Struct to determine number of elements in std array.
	template<typename T, std::size_t N>
	struct ArraySize<std::array<T, N>> : std::tuple_size<std::array<T, N>>
	{
	};


	//====================================================================================================
	// Ranges
	//====================================================================================================

	// Array indices range. Limited by 0 and array size.
	template<typename TArray, typename SizeType>
	using TArrayIndexRange = TBoundedRange<SizeType, 0, ArraySize<TArray>::value>;
}
