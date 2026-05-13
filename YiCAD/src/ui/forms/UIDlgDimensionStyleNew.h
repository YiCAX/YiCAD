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

/// @file UIDlgDimensionStyleNew.h
/// @brief 新建标注样式对话框

#ifndef UIDLGDIMENSIONSTYLENEW_H
#define UIDLGDIMENSIONSTYLENEW_H

#include "ui_UIDlgDimensionStyleNew.h"
#include <memory>

namespace Ui
{
class UIDlgDimensionStyleNew;
}

class DmDocument;
class DmDimensionStyle;

class UIDlgDimensionStyleNew : public QDialog
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否模态
    /// @param [in] fl 窗口标志
    UIDlgDimensionStyleNew(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIDlgDimensionStyleNew();

    /// @brief 初始化对话框
    /// @param [in] tempStyle 模板样式指针
    /// @param [in] document 文档指针
    void init(DmDimensionStyle* tempStyle, DmDocument* document);

public slots:
    /// @brief 对话框结果处理
    /// @param [in] r 结果代码
    virtual void done(int r);

private:
    /// @brief 校验输入是否有效
    /// @return 有效返回 true
    bool isInputValid();

private:
    std::unique_ptr<Ui::UIDlgDimensionStyleNew> ui;  ///< UI 对象
    DmDocument* m_pDocument;                         ///< 文档指针
};

#endif
