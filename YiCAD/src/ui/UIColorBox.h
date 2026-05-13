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

/// @file UIColorBox.h
/// @brief 颜色选择下拉框控件

#ifndef UICOLORBOX_H
#define UICOLORBOX_H

#include <QComboBox>
#include <memory>

class DmColor;

/// @brief A combobox for choosing a color.
class UIColorBox : public QComboBox
{
    Q_OBJECT
public:
    UIColorBox(QWidget* parent = nullptr, const char* name = nullptr);
    virtual ~UIColorBox();

    /// @brief 获得当前颜色
    DmColor getColor() const;

    /// @brief 设置当前颜色为指定颜色（不发送colorChanged信号）
    void setColor(const DmColor& color);
    /// @brief 初始化
    /// @param showByLayer 是否显示随层随块
    void init(bool showByLayer);

    /// @brief 初始化条目
    void initItems();

    /// @brief 获得指定颜色的索引，没有则返回-1
    int indexOfColor(const DmColor& c);

protected:
    /// @brief 添加指定颜色条目
    void addColor(QColor color, QString text);
    void addColor(Qt::GlobalColor color, QString text);
    /// @brief 添加颜色到历史
    void addColorToHistory(const DmColor& c);

private slots:
    void slotColorChanged(int index);

signals:
    void colorChanged(const DmColor& color);

private:
    std::unique_ptr<DmColor>    m_pCurrentColor;        ///< 当前颜色
    std::vector<DmColor>        m_historyColors;        ///< 历史颜色。用户选择过的自定义颜色，添加在列表后面
    int                         m_iColorIndexStart = 0; ///< 第一个非随层随块颜色的索引（”红色”的索引）
    bool                        m_isShowByLayer = false;    ///< 是否显示”随层””随块”
    bool                        m_isChangingByCode = false; ///< 是否由代码设置选项项，用于”自定义颜色”时避免死循环
};

#endif
