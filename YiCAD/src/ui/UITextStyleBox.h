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

/// @file UITextStyleBox.h
/// @brief 文字样式下拉列表框

#ifndef UITEXTSTYLEBOX_H
#define UITEXTSTYLEBOX_H

#include <QComboBox>

class DmTextStyle;
class DmTextStyleTable;

typedef bool (*ChangeQueryFunc)() ;
/// @brief 文字样式下拉列表框。支持用户选择确认提示
class UITextStyleBox : public QComboBox
{
Q_OBJECT
public:
    explicit UITextStyleBox(QWidget* parent = nullptr);
    virtual ~UITextStyleBox() = default;

    DmTextStyle* getStyle();
    void setStyle(const QString& style);
    void init(DmTextStyleTable* textStyleTable);
    /// @brief 选择提交时的确认回调
    void setChangeQueryFunc(ChangeQueryFunc callBack);
protected:
    /// @brief 判断是由用户在UI触发
    void mousePressEvent(QMouseEvent *e) override;
    void keyPressEvent(QKeyEvent *e) override;
private slots:
    void slotStyleChanged(const QString& text);
signals:
    void styleChanged();

private:
    bool m_userChoose = false; ///< 是否为用户在UI上触发
    int m_lastIndex = 0;	///< 选择改变之前的当前索引
    ChangeQueryFunc m_changeQueryFunc = nullptr;	///< 外部注册的回调函数，用来在选项改变前做提示，根据返回值来决定是否做改变
    DmTextStyle*		m_pStyle = nullptr;
    DmTextStyleTable*	m_pTextStyleTable = nullptr;
};
#endif //UITEXTSTYLEBOX_H