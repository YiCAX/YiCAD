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

/// @file UIFileDialog.h
/// @brief 文件 打开/保存 对话框

#ifndef UIFILEDIALOG_H
#define UIFILEDIALOG_H

#include <QFileDialog>

/// @class UIFileDialog
/// @brief 文件 打开/保存 对话框
class UIFileDialog : public QFileDialog
{
public:
    enum FileType
    {
        DrawingFile = 0,
        BlockFile = 1
    };

    UIFileDialog(QWidget* parent = nullptr, Qt::WindowFlags f = Qt::WindowType::Widget, FileType type = DrawingFile);
    virtual ~UIFileDialog();

    /// @brief 获取打开文件路径
    QString getOpenFile();
    /// @brief 获取保存文件路径
    /// @param [out] formatType 保存格式类型名
    QString getSaveFile(QString& formatType);

private:
    QString         m_strName;      ///< 窗体名
    static const QString  m_strDefaultFilter; ///< Types
};

#endif
