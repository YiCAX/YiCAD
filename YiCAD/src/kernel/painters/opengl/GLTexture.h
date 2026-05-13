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

/// @file GLTexture.h
/// @brief OpenGL纹理封装（暂时不用）

#ifndef GLTEXTURE_H
#define GLTEXTURE_H

#include <GL/glew.h>

/// @brief 对纹理的封装（暂时不用）
class GLTexture
{
public:
    GLTexture();
    ~GLTexture();
    GLTexture(const GLTexture&) = delete;
    GLTexture& operator=(const GLTexture&) = delete;

    /// @brief 创建纹理对象
    /// @param [in] width 宽度（像素）
    /// @param [in] height 高度（像素）
    /// @param [in] useMultiSample 是否使用多重采样
    /// @param [in] useAlpha 是否使用Alpha通道
    void gen(int width, int height, bool useMultiSample = true, bool useAlpha = true);

    /// @brief 绑定纹理
    void bind();

    /// @brief 取消绑定纹理
    void unbind();

    /// @brief 释放纹理对象
    void freeTexture();

    /// @brief 获取纹理ID
    /// @return 纹理ID
    unsigned int getID() const { return id; }

    /// @brief 检查纹理是否有效
    /// @return 有效返回true
    bool isValid() const;

private:
    GLuint id = 0;
    bool useMultiSample = true;
    bool useAlpha = true;
};

#endif //GLTEXTURE_H
