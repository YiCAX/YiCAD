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

/// @file GuiCommandEvent.h
/// @brief 命令事件类，封装命令行命令的传递与接受状态

#ifndef GUICOMMANDEVENT_H
#define GUICOMMANDEVENT_H

#include <QString>

/// @brief 命令事件
class GuiCommandEvent
{
public:
    /// @brief 创建一个尚未被接受的命令事件
    /// @param cmd 被触发的命令字符串
    GuiCommandEvent(const QString& cmd)
    {
        this->cmd = cmd;
        accepted = false;
    }

    /// @brief 获取被触发的命令
    /// @return 命令字符串（通常由用户输入触发）
    QString getCommand()
    {
        return cmd;
    }

    /// @brief 将事件状态设置为已接受
    void accept()
    {
        accepted = true;
    }

    /// @brief 检查事件是否已被接受
    /// @return true 表示事件已被接受，false 表示尚未接受
    bool isAccepted()
    {
        return accepted;
    }

protected:
    QString     cmd;
    bool        accepted;
};

#endif
