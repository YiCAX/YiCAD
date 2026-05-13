/*
 * Copyright (C) 2024 YiCAD Team
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

/// @file UIWidthBox.h
/// @brief 线宽选择下拉列表框

#ifndef UIWIDTHBOX_H
#define UIWIDTHBOX_H

#include <QComboBox>

#include "Datamodel.h"

/// @brief 线宽选择下拉列表框
class UIWidthBox: public QComboBox 
{
    Q_OBJECT

public:
	UIWidthBox(QWidget* parent = nullptr, const char* name = nullptr);
    virtual ~UIWidthBox() = default;

    DM::LineWidth getWidth() const;
    void setWidth(DM::LineWidth w);

    void init(bool showByLayer);

protected:
    /// @brief 判断是由用户在UI触发
    void mousePressEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;

private slots:
    void slotWidthChanged(int index);

signals:
    void widthChanged(DM::LineWidth);

private:
    DM::LineWidth  m_currentWidth = DM::WidthDefault;
    bool            m_isShowByLayer = false;
    bool m_userChoose = false; ///< 是否为用户在UI上触发
};

#endif
