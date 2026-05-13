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

/// @file PainterCreator.h
/// @brief 画笔工厂类，根据渲染后端创建对应的画笔实例

#ifndef PAINTERCREATOR_H
#define PAINTERCREATOR_H

#include "GLPainter.h"
#include "GLCachePainter.h"

using namespace opengl;

/// @brief 画笔工厂类，负责创建不同后端的画笔实例
class PainterCreator
{
public:
    /// @brief 创建OpenGL画笔
    /// @param [in] width 设备宽度（像素）
    /// @param [in] height 设备高度（像素）
    /// @return OpenGL画笔实例
    static GLPainter* createOpenGLPainter(const unsigned int width, const unsigned int height);

    /// @brief 创建带缓存的OpenGL画笔
    /// @param [in] width 设备宽度（像素）
    /// @param [in] height 设备高度（像素）
    /// @return 带缓存的OpenGL画笔实例
    static GLCachePainter* createOpenGLCachePainter(const unsigned int width, const unsigned int height);

    /// @brief 创建Vulkan画笔（预留）
    /// @param [in] width 设备宽度（像素）
    /// @param [in] height 设备高度（像素）
    /// @return Vulkan画笔实例（当前返回nullptr）
    static Painter* createVulkanPainter(const unsigned int width, const unsigned int height);

    /// @brief 创建Direct3D画笔（预留）
    /// @param [in] width 设备宽度（像素）
    /// @param [in] height 设备高度（像素）
    /// @return Direct3D画笔实例（当前返回nullptr）
    static Painter* createDirect3DPainter(const unsigned int width, const unsigned int height);
};

#endif //PAINTERCREATOR_H
