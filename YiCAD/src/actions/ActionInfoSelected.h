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

/// @file ActionInfoSelected.h
/// @brief 选中实体信息 Action 类，显示当前选中实体的详细信息

#ifndef ACTIONINFOSELECTED_H
#define ACTIONINFOSELECTED_H

#include "ActionInterface.h"

class DmSpline;

/// @brief 显示选中实体信息的 Action
class ActionInfoSelected : public ActionInterface
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    ActionInfoSelected(DmDocument* doc, GuiDocumentView* docView);

    /// @brief 初始化并立即触发
    /// @param status 初始状态
    void init(int status = 0) override;

    /// @brief 触发信息显示
    void trigger() override;

private:
    /// @brief 获取实体信息
    /// @param e 实体指针
    /// @param info 输出信息字符串
    void getInfo(const DmEntity* e, QString& info);

    /// @brief 格式化向量信息
    /// @param vec 向量
    /// @param info 输出信息字符串
    void infoVector(const DmVector& vec, QString& info);

    /// @brief 获取样条曲线专用信息
    /// @param spline 样条曲线指针
    /// @param info 输出信息字符串
    void getInfoForSpline(const DmSpline* spline, QString& info);
};

#endif // ACTIONINFOSELECTED_H
