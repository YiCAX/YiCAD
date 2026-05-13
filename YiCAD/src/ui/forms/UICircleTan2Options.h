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

/// @file UICircleTan2Options.h
/// @brief 双切圆选项控件

#ifndef UICIRCLETAN2OPTIONS_H
#define UICIRCLETAN2OPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionDrawCircleTan2;

namespace Ui
{
class Ui_CircleTan2Options;
}

class UICircleTan2Options : public QWidget
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UICircleTan2Options(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UICircleTan2Options();

public slots:
    /// @brief 设置当前 Action
    /// @param [in] a Action 接口指针
    /// @param [in] update 是否从 Action 更新界面
    virtual void setAction(ActionInterface* a, bool update);

    /// @brief 更新半径显示
    /// @param [in] l 半径字符串
    virtual void updateRadius(const QString& l);

protected:
    ActionDrawCircleTan2* action;                        ///< 双切圆 Action 指针

protected slots:
    /// @brief 语言切换槽
    virtual void languageChange();

private:
    /// @brief 保存设置到持久化存储
    void saveSettings();

    std::unique_ptr<Ui::Ui_CircleTan2Options> ui;        ///< UI 对象
};

#endif // UICIRCLETAN2OPTIONS_H
