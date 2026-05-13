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

/// @file ConstrainedDelaunayTriangulation.cpp
/// @brief 约束Delaunay三角剖分类实现

#include "ConstrainedDelaunayTriangulation.h"

void ConstrainedDelaunayTriangulation::mark_domains(CDT& cdt)
{
    for (CDT::Face_handle f : cdt.all_face_handles())
    {
        f->info().nesting_level = -1;
    }
    std::list<CDT::Edge> border;
    mark_domains(cdt, cdt.infinite_face(), 0, border);
    while (!border.empty())
    {
        CDT::Edge e = border.front();
        border.pop_front();
        Face_handle n = e.first->neighbor(e.second);
        if (n->info().nesting_level == -1)
        {
            mark_domains(cdt, n, e.first->info().nesting_level + 1, border);
        }
    }
}

constexpr int TRIANGULATION_EDGE_COUNT = 3; ///< 三角面边数

void ConstrainedDelaunayTriangulation::mark_domains(CDT& ct, Face_handle start, int index, std::list<CDT::Edge>& border)
{
    if (start->info().nesting_level != -1)
    {
        return;
    }
    std::list<Face_handle> queue;
    queue.push_back(start);
    while (!queue.empty())
    {
        Face_handle fh = queue.front();
        queue.pop_front();
        if (fh->info().nesting_level == -1)
        {
            fh->info().nesting_level = index;
            for (int i = 0; i < TRIANGULATION_EDGE_COUNT; i++)
            {
                CDT::Edge e(fh, i);
                Face_handle n = fh->neighbor(i);
                if (n->info().nesting_level == -1)
                {
                    if (ct.is_constrained(e))
                    {
                        border.push_back(e);
                    }
                    else
                    {
                        queue.push_back(n);
                    }
                }
            }
        }
    }
}
