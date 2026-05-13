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

/// @file UIDimLinearOptions.h
/// @brief 线性标注选项控件

#ifndef UIDIMLINEAROPTIONS_H
#define UIDIMLINEAROPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionDimLinear;

namespace Ui
{
class Ui_DimLinearOptions;
}

class UIDimLinearOptions : public QWidget
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UIDimLinearOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIDimLinearOptions();

public slots:
    /// @brief 设置当前 Action
    /// @param [in] a Action 接口指针
    /// @param [in] update 是否从 Action 更新界面
    virtual void setAction(ActionInterface* a, bool update);

    /// @brief 更新角度显示
    /// @param [in] a 角度字符串
    virtual void updateAngle(const QString& a);

protected:
    ActionDimLinear* action;                         ///< 线性标注 Action 指针

protected slots:
    /// @brief 语言切换槽
    virtual void languageChange();

private:
    /// @brief 保存设置到持久化存储
    void saveSettings();

    std::unique_ptr<Ui::Ui_DimLinearOptions> ui;     ///< UI 对象
};

#endif // UIDIMLINEAROPTIONS_H
