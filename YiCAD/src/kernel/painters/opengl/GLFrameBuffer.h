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

/// @file GLFrameBuffer.h
/// @brief OpenGL帧缓存封装（暂时不用）

#ifndef GLFRAMEBUFFER_H
#define GLFRAMEBUFFER_H

#include <GL/glew.h>

/// @brief 对帧缓存的封装（暂时不用）
class GLFrameBuffer
{
public:
    GLFrameBuffer();
    ~GLFrameBuffer();
    GLFrameBuffer(const GLFrameBuffer&) = delete;
    GLFrameBuffer& operator=(const GLFrameBuffer&) = delete;

    /// @brief 创建帧缓存对象
    /// @param [in] default_id 默认帧缓存ID
    void gen(unsigned int default_id);

    /// @brief 绑定帧缓存
    void bind();

    /// @brief 取消绑定帧缓存
    void unbind();

    /// @brief 释放帧缓存对象
    void freeFBO();

    /// @brief 获取帧缓存ID
    /// @return 帧缓存ID
    unsigned int getID() const { return id; }

    /// @brief 检查帧缓存是否有效
    /// @return 有效返回true
    bool isValid() const;

private:
    GLuint id = 0;
    GLuint default_id = 0;
};

#endif //GLFRAMEBUFFER_H
