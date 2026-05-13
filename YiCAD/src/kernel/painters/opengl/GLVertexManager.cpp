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

/// @file GLVertexManager.cpp
/// @brief OpenGL顶点管理器实现，管理当前绘制路径的顶点和跳转信息

#include "GLVertexManager.h"

namespace
{
    constexpr float DEFAULT_PARA = 0.0f;            ///< 默认参数值
}

void GLVertexManager::addVertex(double x, double y)
{
    if (m_current_vertices.size() != 0)
    {
        glm::vec3 lastPt = m_current_vertices.back();
        float dist = glm::distance(lastPt, glm::vec3(x, y, 0.0));
        m_para += dist;
    }
    m_current_vertices.emplace_back(glm::vec4(x, y, 0.0, m_para));
}

void GLVertexManager::jump()
{
    if (m_current_vertices.size() > 0)
    {
        m_jumps.emplace_back(static_cast<int>(m_current_vertices.size()));
        appendCurrentVertex();
    }
    m_para = DEFAULT_PARA;
}

void GLVertexManager::clear()
{
    m_jumps.clear();
    m_vertex_data.clear();
    m_current_vertices.clear();
    m_para = DEFAULT_PARA;
}

std::vector<glm::vec4> GLVertexManager::getVertexes() const
{
    return m_vertex_data;
}

std::vector<int> GLVertexManager::getJumps() const
{
    return m_jumps;
}

void GLVertexManager::appendCurrentVertex()
{
    m_vertex_data.insert(m_vertex_data.end(), m_current_vertices.begin(), m_current_vertices.end());
    m_current_vertices.clear();
}
