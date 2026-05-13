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

/// @file ActionDrawHatch.h
/// @brief 填充图案（Hatch）绘制交互动作类

#ifndef ACTIONDRAWHATCH_H
#define ACTIONDRAWHATCH_H

#include "PreviewActionInterface.h"
#include "FindClosedRegion.h"

class HatchData;
class DmHatch;
class UIDlgHatchOptions;

/// @brief 通过点选封闭区域来创建填充图案
class ActionDrawHatch : public PreviewActionInterface
{
    Q_OBJECT

    /// @brief 动作状态枚举
    enum Status
    {
        ShowDialog, ///< 显示填充对话框
        PrepareAdd  ///< 准备添加（鼠标移动预览状态）
    };

public:
    /// @brief 构造函数
    /// @param doc 文档指针
    /// @param docView 文档视图指针
    /// @param isModify 是否为修改模式
    ActionDrawHatch(DmDocument* doc, GuiDocumentView* docView, bool isModify);
    ~ActionDrawHatch();

    void init(int status = 0) override;
    void trigger() override;
    void mouseMoveEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;
    void updateMouseButtonHints() override;
    void updateMouseCursor() override;

    /// @brief 用选中的实体初始化封闭区域查找算法
    void initFindMethodForSelectedEntities();

private slots:
    /// @brief 视图变化槽函数
    void slotViewChanged();

private:
    std::unique_ptr<HatchData> m_pData;              ///< 填充数据
    FindClosedRegion::FindClosedRegion m_findMethod; ///< 封闭区域查找算法
    DmVector m_pickPoint;                            ///< 鼠标点击位置
    bool m_hasSelectEntity{ false };                 ///< 命令开始时是否已选择实体
};

#endif
