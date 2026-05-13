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

/// @file UIBlockEditOptions.h
/// @brief 块在位编辑选项栏
/// 显示正在编辑的块名称和"完成"按钮

#ifndef UIBLOCKEDITOPTIONS_H
#define UIBLOCKEDITOPTIONS_H

#include <QWidget>

class QLabel;
class QPushButton;
class ActionInterface;
class ActionBlocksEdit;

/// @class UIBlockEditOptions
/// @brief 块在位编辑选项栏
/// 显示正在编辑的块名称和"完成"按钮
class UIBlockEditOptions : public QWidget
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UIBlockEditOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIBlockEditOptions() override;

public slots:
    /// @brief 设置当前 Action
    /// @param [in] a Action 接口指针
    void setAction(ActionInterface* a);

private slots:
    /// @brief 完成按钮点击槽
    void onCompleteClicked();

private:
    ActionBlocksEdit* m_action;          ///< 块编辑 Action 指针
    QLabel* m_label;                     ///< 当前编辑块名称标签
    QPushButton* m_completeButton;       ///< 完成按钮
};

#endif // UIBLOCKEDITOPTIONS_H
