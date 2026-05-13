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

/// @file UIDlgCmdsSetting.h
/// @brief 命令快捷键设置对话框

#ifndef UIDLGCMDSSETTING_H
#define UIDLGCMDSSETTING_H

#include "ui_UIDlgCmdsSetting.h"
#include "Datamodel.h"

class UIDlgCmdsSetting : public QDialog, public Ui::UIDlgCmdsSetting
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UIDlgCmdsSetting(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 设置命令分组
    /// @param [in] group 分组名称
    void setGroup(const QString& group);

private:
    /// @brief 初始化数据
    void init();

    /// @brief 加载数据到界面
    void loadDataToUI();

private slots:
    /// @brief 确定按钮槽
    void slotOk();

    /// @brief 按钮点击槽
    /// @param [in] button 被点击的按钮指针
    void slotBtnClicked(QAbstractButton* button);

private:
    QString m_group;                                                   ///< 命令分组名称
    std::vector<std::tuple<DM::ActionType, QString, QStringList>> m_data; ///< 命令数据列表
};

#endif // UIDLGCMDSSETTING_H
