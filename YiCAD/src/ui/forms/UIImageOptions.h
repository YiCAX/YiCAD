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

/// @file UIImageOptions.h
/// @brief 图片插入选项控件

#ifndef UIIMAGEOPTIONS_H
#define UIIMAGEOPTIONS_H

#include <memory>
#include <QWidget>

class ActionInterface;
class ActionDrawImage;
namespace Ui {
    class Ui_ImageOptions;
}

class UIImageOptions : public QWidget
{
    Q_OBJECT

public:
    /// @brief 构造函数
    /// @param [in] parent 父窗口指针
    /// @param [in] fl 窗口标志
    UIImageOptions(QWidget* parent = nullptr, Qt::WindowFlags fl = Qt::WindowFlags());

    ~UIImageOptions();

public slots:
    /// @brief 设置关联的动作
    /// @param [in] a 动作接口指针
    /// @param [in] update 是否更新数据
    virtual void setAction(ActionInterface* a, bool update);

    virtual void updateData();
    virtual void updateDPI();
    virtual void updateFactor();

protected:
    ActionDrawImage* action = nullptr; ///< 关联的图片绘制动作

protected slots:
    virtual void languageChange();

private:
    void saveSettings();

    std::unique_ptr<Ui::Ui_ImageOptions> ui; ///< UI 对象
};

#endif // UIIMAGEOPTIONS_H
