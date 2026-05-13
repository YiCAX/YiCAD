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

/// @file ConstrainedDelaunayTriangulation.h
/// @brief 约束Delaunay三角剖分类

#ifndef CONSTRAINEDDELAUNAYTRIANGUATION_H
#define CONSTRAINEDDELAUNAYTRIANGUATION_H

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>
#include "DmVector.h"
#include "DmTriangle.h"

/// @brief 三角面的信息，用作系统文字三角划分后剔除无效面
struct FaceInfo2
{
    FaceInfo2() : nesting_level(0) {}
    int nesting_level;
    bool in_domain()
    {
        return (nesting_level % 2) == 1;
    }
};

typedef CGAL::Exact_predicates_inexact_constructions_kernel       K;
typedef CGAL::Triangulation_vertex_base_2<K>                      Vb;
typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2, K>    Fbb;
typedef CGAL::Constrained_triangulation_face_base_2<K, Fbb>        Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb>               TDS;
typedef CGAL::Exact_predicates_tag                                Itag;
typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag>  CDT;
// typedef CDT::Point                                                Point;
typedef CGAL::Polygon_2<K>                                        Polygon_2;
typedef CDT::Face_handle                                          Face_handle;

/// @brief 约束Delaunay三角剖分类
class ConstrainedDelaunayTriangulation
{
public:
    /// @brief 三角化。参数T可为DmTriangle或DmTrianglePtr
    /// @details 外边界或内边界可以为空，保证有一个非空即可，函数内部判断三角形是内部还是外部，对内外边界采用相同的处理。
    /// @param [in] outBoundary 外边界点
    /// @param [in] holes 孔洞边界点列表
    /// @param [out] triangles 生成的三角形列表
    template<typename T>
    static void trianglulate(const std::vector<DmVector>& outBoundary, const std::vector<std::vector<DmVector>>& holes, std::vector<T>& triangles);

private:
    // 给CDT需要剔除的面打上标记
    // explore set of facets connected with non constrained edges,
    // and attribute to each such set a nesting level.
    // We start from facets incident to the infinite vertex, with a nesting
    // level of 0. Then we recursively consider the non-explored facets incident
    // to constrained edges bounding the former set and increase the nesting level by 1.
    // Facets in the domain are those with an odd nesting level.

    /// @brief 标记domain的面
    /// @param [in,out] cdt CDT对象
    static void mark_domains(CDT& cdt);

    // 来自：https://doc.cgal.org/latest/Triangulation_2/index.html#Section_2D_Triangulations_Constrained_Delaunay
    // 8.4 Example: Triangulating a Polygonal Domain

    /// @brief 标记domain的面（递归）
    /// @param [in,out] ct CDT对象
    /// @param [in] start 起始面
    /// @param [in] index 嵌套层级
    /// @param [in,out] border 边界列表
    static void mark_domains(CDT& ct, Face_handle start, int index, std::list<CDT::Edge>& border);
};

template<typename T>
void ConstrainedDelaunayTriangulation::trianglulate(const std::vector<DmVector>& outBoundary, const std::vector<std::vector<DmVector>>& holes, std::vector<T>& triangles)
{
    // CDT三角化
    // 用CGAL的Constrained Delaunay Triangulation做三角剖分
    CDT cdt;
    if (outBoundary.size() > 0)
    {
        Polygon_2 polygon1;
        for (auto& pt : outBoundary)
        {
            polygon1.push_back(CDT::Point(pt.x, pt.y));
        }
        cdt.insert_constraint(polygon1.vertices_begin(), polygon1.vertices_end(), true);
    }

    // 再添加孔洞边界
    for (auto hole : holes)
    {
        Polygon_2 polygon2;
        for (auto& pt : hole)
        {
            polygon2.push_back(CDT::Point(pt.x, pt.y));
        }
        cdt.insert_constraint(polygon2.vertices_begin(), polygon2.vertices_end(), true);
    }
    // 给需要剔除的面打上标记
    mark_domains(cdt);
    // 生成三角面
    for (Face_handle f : cdt.finite_face_handles())
    {
        if (f->info().in_domain())
        {
            CDT::Point p0 = f->vertex(0)->point();
            CDT::Point p1 = f->vertex(1)->point();
            CDT::Point p2 = f->vertex(2)->point();
            std::array<DmVector, 3> pts;
            pts.at(0) = DmVector(p0.x(), p0.y());
            pts.at(1) = DmVector(p1.x(), p1.y());
            pts.at(2) = DmVector(p2.x(), p2.y());
            TriangleData data;
            data.setPoints(pts);
            DmTriangle* triangle = new DmTriangle(nullptr, data);
            triangles.emplace_back(triangle);
        }
    }
}

#endif //CONSTRAINEDDELAUNAYTRIANGUATION_H
