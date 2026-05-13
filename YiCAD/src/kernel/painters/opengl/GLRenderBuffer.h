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

/// @file GLRenderBuffer.h
/// @brief OpenGL渲染缓存封装（暂时不用）

#ifndef GLRENDERBUFFER_H
#define GLRENDERBUFFER_H

#include <GL/glew.h>

/// @brief 对RenderBuffer的封装（暂时不用）
class GLRenderBuffer
{
public:
    GLRenderBuffer();
    ~GLRenderBuffer();
    GLRenderBuffer(const GLRenderBuffer&) = delete;
    GLRenderBuffer& operator=(const GLRenderBuffer&) = delete;

    /// @brief 创建渲染缓存对象
    /// @param [in] width 宽度（像素）
    /// @param [in] height 高度（像素）
    /// @param [in] useMultiSample 是否使用多重采样
    void gen(int width, int height, bool useMultiSample = true);

    /// @brief 绑定渲染缓存
    void bind();

    /// @brief 取消绑定渲染缓存
    void unbind();

    /// @brief 释放渲染缓存对象
    void freeRBO();

    /// @brief 检查渲染缓存是否有效
    /// @return 有效返回true
    bool isValid() const;

private:
    GLuint id = 0;
    bool useMultiSample = false;
};

#endif //GLRENDERBUFFER_H
