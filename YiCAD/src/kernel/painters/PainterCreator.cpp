/*
 * Copyright (C) 2026 YiCAD Contributors
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/// @file PainterCreator.cpp
/// @brief 画笔工厂类实现，根据不同后端创建画笔实例

#include "PainterCreator.h"

GLPainter* PainterCreator::createOpenGLPainter(const unsigned int width, const unsigned int height)
{
    return new GLPainter();
}

GLCachePainter* PainterCreator::createOpenGLCachePainter(const unsigned int width, const unsigned int height)
{
    return new GLCachePainter();
}

Painter* PainterCreator::createVulkanPainter(const unsigned int width, const unsigned int height)
{
    // TODO: Vulkan后端尚未实现
    return nullptr;
}

Painter* PainterCreator::createDirect3DPainter(const unsigned int width, const unsigned int height)
{
    // TODO: Direct3D后端尚未实现
    return nullptr;
}
