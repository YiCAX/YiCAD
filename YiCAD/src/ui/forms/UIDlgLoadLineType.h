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

/// @file UIDlgLoadLineType.h
/// @brief 从线型文件加载线型的对话框

#ifndef UIDLGLOADLINETYPE_H
#define UIDLGLOADLINETYPE_H

#include "ui_UIDlgLoadLineType.h"

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QList>

#include "DmLineType.h"
#include "DmLineTypeTable.h"

class UIDlgLoadLineType : public QDialog, public Ui::UIDlgLoadLineType
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否为模态对话框
    /// @param [in] fl 窗口标志
    explicit UIDlgLoadLineType(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgLoadLineType();

    void init();

    /// @brief 获取表格中选中的线型
    /// @return 选中的线型指针，若未选中则返回nullptr
    DmLineType* selectTableWidget();

    /// @brief 设置文档对象
    /// @param [in] doc 文档指针
    void setDocument(DmDocument* doc)
    {
        m_doc = doc;
    }

protected:
    /// @brief 读取line文件，获得线型名称和线型数据
    /// @param [in] path 线型文件路径
    void readFile(const QString& path);

    void clearLineTypes();

private slots:
    void on_file_clicked();
    void on_Ok_clicked();
    void on_Cancel_clicked();

signals:
    void lineTypeSelected(DmLineType* data);

private:
    std::vector<DmLineType*> m_lineTypes; ///< 线型列表
    DmDocument* m_doc = nullptr;          ///< 文档指针
};

#endif // UIDLGLOADLINETYPE_H
