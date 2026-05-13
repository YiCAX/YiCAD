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

/// @file UICloudLineOptions.h
/// @brief 云线选项控件

#ifndef UICLOUDLINEOPTIONS_H
#define UICLOUDLINEOPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;

namespace Ui
{
class UICloudLineOptions;
}

class UICloudLineOptions : public QWidget
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UICloudLineOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UICloudLineOptions();

public slots:
    /// @brief 设置当前 Action
    /// @param [in] a Action 接口指针
    /// @param [in] update 是否从 Action 更新界面
    virtual void setAction(ActionInterface* a, bool update);

    /// @brief 撤销操作
    virtual void undo();

    /// @brief 更新最小长度
    /// @param [in] s 长度字符串
    virtual void updateMinLength(const QString& s);

    /// @brief 更新最大长度
    /// @param [in] s 长度字符串
    virtual void updateMaxLength(const QString& s);

    /// @brief 更新方向反转
    /// @param [in] reverse 反转状态值
    virtual void updateReverse(int reverse);

protected:
    ActionInterface* action;                          ///< Action 接口指针

protected slots:
    /// @brief 语言切换槽
    virtual void languageChange();

private:
    /// @brief 销毁时保存设置
    void destroy();

    std::unique_ptr<Ui::UICloudLineOptions> ui;       ///< UI 对象
};

#endif // UICLOUDLINEOPTIONS_H
