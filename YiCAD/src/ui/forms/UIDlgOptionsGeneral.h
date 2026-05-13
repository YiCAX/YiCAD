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

/// @file UIDlgOptionsGeneral.h
/// @brief 系统设置窗体头文件

#ifndef UIDLGOPTIONSGENERAL_H
#define UIDLGOPTIONSGENERAL_H

#include "ui_UIDlgOptionsGeneral.h"

/// @brief 系统设置窗体
class UIDlgOptionsGeneral : public QDialog, public Ui::UIDlgOptionsGeneral
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] modal 是否模态对话框
    /// @param [in] fl 窗口标志
    UIDlgOptionsGeneral(QWidget* parent = nullptr, bool modal = false, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIDlgOptionsGeneral();

    static int current_tab; ///< 当前选项卡索引

    /// @brief 设置并弹出颜色选择对话框
    /// @param [in,out] combo 颜色下拉框，用于读取当前颜色和写入新颜色
    /// @param [in] custom 自定义颜色预设值
    void set_color(QComboBox* combo, QColor custom);

public slots:
    virtual void setRestartNeeded();

    /// @brief 确认并保存设置
    virtual void ok();

protected slots:
    virtual void languageChange();

private slots:
    void on_tabWidget_currentChanged(int index);

    void on_pb_background_clicked();

    void on_pb_grid_clicked();

    void on_pb_meta_clicked();

    void on_pb_selected_clicked();

    void on_pb_highlighted_clicked();

    void on_pb_clear_all_clicked();

    void on_pb_clear_geometry_clicked();

    void slotKeyboardModifyClicked();

    //void on_comboBox_currentIndexChanged(int index);

private:
    bool restartNeeded = false; ///< 是否需要重启应用

    void init();
    void initComboBox(QComboBox* cb, const QString& text);

};

#endif // UIDLGOPTIONSGENERAL_H
