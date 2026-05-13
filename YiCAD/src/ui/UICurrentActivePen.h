/*
 * Copyright (C) 2026 YiCAD Team
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

/// @file UICurrentActivePen.h
/// @brief 当前文档激活的画笔UI控件

#ifndef UICURRENTACTIVEPEN_H
#define UICURRENTACTIVEPEN_H

#include <QWidget>
#include "Datamodel.h"
#include "DmLineType.h"

class UILineTypeBox;
class UIColorBox;
class UIWidthBox;
class DmDocument;
class DmPen;
class DmColor;

/// @brief 当前文档激活的画笔UI（显示当前文档激活的画笔颜色、线型、线宽）
class UICurrentActivePen : public QWidget
{
    Q_OBJECT

public:
    UICurrentActivePen(QWidget* parent);
    ~UICurrentActivePen();

public:
    void setPen(DmDocument* doc);

    DmLineType* getLineType();

    DM::LineWidth getLineWidth();
    DmColor getColor();
    void update(DmDocument* doc);

private slots:
    void slotSelectChanged();

private:
    UILineTypeBox*  m_pCurrentLineType = nullptr;   ///< 当前文档活动的线型
    UIColorBox*     m_pCurrentColor = nullptr;       ///< 当前文档活动的颜色
    UIWidthBox*     m_pCurrentWidth = nullptr;       ///< 当前文档活动的线宽
    DmDocument*     m_document = nullptr;            ///< doc
};

#endif