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

/// @file GLCacheUnit.h
/// @brief 缓存单元数据结构，存储同类型实体的顶点、索引和OpenGL对象

#ifndef GLCACHEUNIT_H
#define GLCACHEUNIT_H

#include <unordered_map>
#include <vector>
#include <GL/glew.h>
#include "GLVertexArray.h"
#include "GLVertexBuffer.h"

constexpr int GELEM_SIZE_FLOAT = 3; ///< 顶点三元组(x,y,z)的元素数

namespace opengl
{

/// @brief 缓冲单元。同一线型、颜色、线宽的同类型实体顶点缓存在一个缓冲单元
struct GLCacheUnit
{
    std::vector<float> vertexes;        ///< 顶点
    std::vector<int> jumps;             ///< 跳转
    std::vector<GLint> startIndices;    ///< 起始索引
    GLenum drawType = GL_POINTS;
    GLVertexArray vao;
    GLVertexBuffer vbo;

    /// @brief 释放缓存单元占用的OpenGL资源
    void free()
    {
        jumps.clear();
        if (vbo.isValid())
        {
            vbo.freeVBO();
        }
        if (vao.isValid())
        {
            vao.freeVAO();
        }
        vertexes.clear();
        startIndices.clear();
    }
};

using GLCacheUnitMap = std::unordered_map<int, GLCacheUnit>; ///< key为画笔的id

}

#endif //GLCACHEUNIT_H
