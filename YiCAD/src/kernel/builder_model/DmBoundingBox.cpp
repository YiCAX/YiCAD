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

/// @file DmBoundingBox.cpp
/// @brief 包围框类实现

#include "DmBoundingBox.h"

DmBoundingBox::DmBoundingBox(const DmVector& pt1, const DmVector& pt2)
{
    add(pt1);
    add(pt2);
}

void DmBoundingBox::add(const DmVector& pt)
{
    if (!min.valid)
    {
        min.valid = true;
    }
    if (!max.valid)
    {
        max.valid = true;
    }
    min = DmVector::minimum(min, pt);
    max = DmVector::maximum(max, pt);
}

bool DmBoundingBox::contains(const DmBoundingBox& box) const
{
    if (min.x > box.min.x + DM_TOLERANCE)
    {
        return false;
    }
    if (min.y > box.min.y + DM_TOLERANCE)
    {
        return false;
    }
    if (max.x < box.max.x - DM_TOLERANCE)
    {
        return false;
    }
    if (max.y < box.max.y - DM_TOLERANCE)
    {
        return false;
    }
    return true;
}

bool DmBoundingBox::isCross(const DmBoundingBox& box) const
{
    auto boxMin = box.getMin();
    auto boxMax = box.getMax();
    if (boxMax.x < min.x - DM_TOLERANCE)
    {
        return false;
    }
    if (boxMax.y < min.y - DM_TOLERANCE)
    {
        return false;
    }
    if (boxMin.x > max.x + DM_TOLERANCE)
    {
        return false;
    }
    if (boxMin.y > max.y + DM_TOLERANCE)
    {
        return false;
    }
    return true;
}

bool DmBoundingBox::isPointInside(const DmVector& pt) const
{
    if (pt.x < min.x - DM_TOLERANCE)
    {
        return false;
    }
    if (pt.y < min.y - DM_TOLERANCE)
    {
        return false;
    }
    if (pt.x > max.x + DM_TOLERANCE)
    {
        return false;
    }
    if (pt.y > max.y + DM_TOLERANCE)
    {
        return false;
    }
    return true;
}

DmVector DmBoundingBox::getMin() const
{
    return min;
}

DmVector DmBoundingBox::getMax() const
{
    return max;
}
