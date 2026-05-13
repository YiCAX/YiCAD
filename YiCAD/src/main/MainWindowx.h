/*
 * Copyright (C) 2026 YiCAD
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

/// @file MainWindowx.h
/// @brief 窗体基类头文件
/// @details 提供窗口停靠区域控制功能

#ifndef MAINWINDOWX_H
#define MAINWINDOWX_H

#include <QMainWindow>

/// @brief 窗体基类，提供停靠区域显示/隐藏控制功能
class MainWindowX : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindowX(QWidget* parent = nullptr);

    /// @brief 按标题排序停靠控件列表
    /// @param [in,out] list 待排序的停靠控件列表
    void sortWidgetsByTitle(QList<QDockWidget*>& list);

    /// @brief 按标题排序工具栏列表
    /// @param [in,out] list 待排序的工具栏列表
    void sortWidgetsByTitle(QList<QToolBar*>& list);

public slots:
    /// @brief 切换右侧停靠区域显示/隐藏
    /// @param [in] state true=显示，false=隐藏
    void toggleRightDockArea(bool state);

    /// @brief 切换左侧停靠区域显示/隐藏
    /// @param [in] state true=显示，false=隐藏
    void toggleLeftDockArea(bool state);

    /// @brief 切换顶部停靠区域显示/隐藏
    /// @param [in] state true=显示，false=隐藏
    void toggleTopDockArea(bool state);

    /// @brief 切换底部停靠区域显示/隐藏
    /// @param [in] state true=显示，false=隐藏
    void toggleBottomDockArea(bool state);

    /// @brief 切换浮动停靠控件显示/隐藏
    /// @param [in] state true=显示，false=隐藏
    void toggleFloatingDockwidgets(bool state);
};

#endif  // MAINWINDOWX_H
