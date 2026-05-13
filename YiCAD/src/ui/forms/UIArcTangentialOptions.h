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

/// @file UIArcTangentialOptions.h
/// @brief 相切圆弧选项控件

#ifndef UIARCTANGENTIALOPTIONS_H
#define UIARCTANGENTIALOPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionDrawArcTangential;

namespace Ui
{
class Ui_ArcTangentialOptions;
}

class UIArcTangentialOptions : public QWidget
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UIArcTangentialOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    /// @brief 析构函数
    ~UIArcTangentialOptions();

public slots:
    /// @brief 设置当前 Action
    /// @param [in] a Action 接口指针
    /// @param [in] update 是否从 Action 更新界面
    virtual void setAction(ActionInterface* a, bool update);

    /// @brief 更新半径显示
    /// @param [in] s 半径字符串
    virtual void updateRadius(const QString& s);

    /// @brief 更新角度显示
    /// @param [in] s 角度字符串
    virtual void updateAngle(const QString& s);

    /// @brief 锁定角度切换槽
    /// @param [in] lock 是否锁定
    void slotLockAngle(bool lock);

    /// @brief 锁定半径切换槽
    /// @param [in] lock 是否锁定
    void slotLockRadius(bool lock);

protected:
    ActionDrawArcTangential* action;                     ///< 相切圆弧 Action 指针

protected slots:
    /// @brief 语言切换槽
    virtual void languageChange();

private slots:
    void on_leRadius_editingFinished();
    void on_leAngle_editingFinished();
    void on_rbRadius_clicked(bool checked);
    void on_rbAngle_clicked(bool checked);

private:
    std::unique_ptr<Ui::Ui_ArcTangentialOptions> ui;     ///< UI 对象
};

#endif // UIARCTANGENTIALOPTIONS_H
