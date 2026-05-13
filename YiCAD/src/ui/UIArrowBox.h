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

/// @file UIArrowBox.h
/// @brief 标注箭头形状选择下拉框控件

#ifndef UIARROWBOX_H
#define UIARROWBOX_H

#include <QComboBox>
#include "Datamodel.h"

class UIArrowBox : public QComboBox
{
    Q_OBJECT
public:
    UIArrowBox(QWidget* parent = nullptr);
    virtual ~UIArrowBox() = default;

    DM::ArrowType getArrowType() const;
    void setArrowType(DM::ArrowType arrowType);
private:
    void init();
private slots:
    void slotArrowChanged(int index);
signals:
    void arrowChanged(DM::ArrowType);

private:
    DM::ArrowType m_curArrowType;   // TODO: 需确认默认值并初始化
};
#endif // UIARROWBOX_H