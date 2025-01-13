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

#include <Runtime/Launch/Resources/Version.h>

#define BELOW_ENGINE_VERSION(Major, Minor)  (ENGINE_MAJOR_VERSION < (Major) || (ENGINE_MAJOR_VERSION == (Major) && ENGINE_MINOR_VERSION < (Minor)))
#define FROM_ENGINE_VERSION(Major, Minor)   !BELOW_ENGINE_VERSION(Major, Minor)


// One place to define compatibility with older engine versions.


// Starting from version 4.17, Slate has an improved clipping API. Old version required to specify per-vertex clipping
// rectangle and unofficial GSlateScissorRect to correctly clip custom vertices made with FSlateDrawElement.
#define ENGINE_COMPATIBILITY_LEGACY_CLIPPING_API        BELOW_ENGINE_VERSION(4, 17)

// Starting from version 4.18, FPaths::GameSavedDir() has been superseded by FPaths::ProjectSavedDir().
#define ENGINE_COMPATIBILITY_LEGACY_SAVED_DIR           BELOW_ENGINE_VERSION(4, 18)

// Starting from version 4.18, we have support for dual key bindings.
#define ENGINE_COMPATIBILITY_SINGLE_KEY_BINDING         BELOW_ENGINE_VERSION(4, 18)

// Starting from version 4.18, FStringClassReference is replaced by FSoftClassPath. The new header contains a typedef
// that renames FStringClassReference to FSoftClassPath, so it is still possible tu use the old type name in code.
// The old header forwards to the new one but if used it outputs a warning, so we want to avoid it.
#define ENGINE_COMPATIBILITY_LEGACY_STRING_CLASS_REF    BELOW_ENGINE_VERSION(4, 18)

// Starting from version 4.18, engine has a world post actor tick event which if available, provides a good opportunity
// to call debug delegates after world actors are already updated.
#define ENGINE_COMPATIBILITY_WITH_WORLD_POST_ACTOR_TICK FROM_ENGINE_VERSION(4, 18)

// Starting from version 4.24, world actor tick event has additional world parameter.
#define ENGINE_COMPATIBILITY_LEGACY_WORLD_ACTOR_TICK    BELOW_ENGINE_VERSION(4, 24)

// Starting from version 4.26, FKey::IsFloatAxis and FKey::IsVectorAxis are deprecated and replaced with FKey::IsAxis[1|2|3]D methods.
#define ENGINE_COMPATIBILITY_LEGACY_KEY_AXIS_API        BELOW_ENGINE_VERSION(4, 26)

#define ENGINE_COMPATIBILITY_LEGACY_VECTOR2F            BELOW_ENGINE_VERSION(5, 0)
