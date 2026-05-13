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

/// @file UIDlgImage.h
/// @brief 图片属性编辑对话框

#ifndef UIDLGIMAGE_H
#define UIDLGIMAGE_H

class DmImage;

#include "ui_UIDlgImage.h"

class UIDlgImage : public QDialog, public Ui::UIDlgImage
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否为模态对话框
    /// @param [in] fl 窗口标志
    UIDlgImage(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgImage();

public slots:
    /// @brief 设置要编辑的图片实体
    /// @param [in] e 图片实体引用
    virtual void setImage(DmImage& e);

    virtual void changeWidth();
    virtual void changeHeight();
    virtual void changeScale();
    virtual void changeDPI();

    /// @brief 更新图片实体以匹配用户修改
    virtual void updateImage();

protected slots:
    virtual void languageChange();

private:
    DmImage* m_pImage = nullptr;   ///< 待编辑的图片实体指针
    double m_dScale = 1.0;         ///< 当前缩放比例
    QDoubleValidator* val = nullptr; ///< 输入验证器
};

#endif // UIDLGIMAGE_H
