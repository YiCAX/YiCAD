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

/// @file UIDlgHatch.h
/// @brief 填充图案属性编辑对话框

#ifndef UIDLGHATCH_H
#define UIDLGHATCH_H

#include "ui_UIDlgHatch.h"

class DmHatch;

class UIDlgHatch : public QDialog, public Ui::UIDlgHatch
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否为模态对话框
    /// @param [in] fl 窗口标志
    UIDlgHatch(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgHatch();

    /// @brief 保存设置（如果需要）
    void saveSettingIfNeed();

public slots:
    virtual void showEvent(QShowEvent* e);

    /// @brief 设置要编辑的填充实体
    /// @param [in] h 填充实体引用
    /// @param [in] isNew 是否新建
    virtual void setHatch(DmHatch& h, bool isNew);

    /// @brief 更新填充实体以匹配用户修改
    virtual void updateHatch();

    /// @brief 设置填充模式
    /// @param [in] p 模式名称
    virtual void setPattern(const QString& p);

    virtual void resizeEvent(QResizeEvent*);
    virtual void updatePreview();
    virtual void reject();
    virtual void accept();

protected slots:
    virtual void languageChange();

private:
    void init();

private:
    DmEntityContainer* m_pPreview = nullptr; ///< 预览容器
    bool m_isNew = false;                    ///< 是否新建
    DmPattern* m_pPattern = nullptr;         ///< 填充模式
    DmHatch* m_pHatch = nullptr;             ///< 待编辑的填充实体指针
    bool m_saveSettings = true;              ///< 是否保存设置
};

#endif // UIDLGHATCH_H
