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

/// @file UIBottomWidget.h
/// @brief 底部状态栏控件，包含坐标显示、捕捉模式、单位设置、线宽开关等

#ifndef UIBOTTOMWIDGRT_H
#define UIBOTTOMWIDGRT_H
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include "UIActionHandler.h"
#include "QComboBox"
#include <QModelIndex>
#include "ApplicationWindow.h"

class DmVector;
class DmDocument;
class MDIWindow;
class QTableView;
class QStandardItemModel;

class UIBottomWindow : public QWidget
{
    Q_OBJECT
public:
    UIBottomWindow(QWidget* parent);
    ~UIBottomWindow();

public:
    void createBottomWindow(UIActionHandler* pActionHandler, MDIWindow* pCurrentMdiWindow);

    void createCoordLable();
    void createSelectNumberLable();
    void createButton();
    void createTool();
    void createComboBox();
    //void createThemeComboBox();

    UISnapWidget* getSnapToolBar();
    QWidget* getWidget();
    QWidget* getToolWidget();
    QWidget* getViewportsWidget();
    QWidget* getComboBoxWidget();
    //QWidget* getThemeComBoxWidget();

    void setCoordinates(const DmVector& abs);
    void setSelectNumber(const int num);

    /// @brief 清空状态栏
    void clearBottomWidget();

    /// @brief 刷新状态栏
    void redrawBottomWidget(UIActionHandler* pActionHandler, MDIWindow* pCurrentMdiWindow);

private slots:
    void showToolWidget();
    void showComboBoxWidget();
    void slotShowWidth();
    void unitBtnClicked(QString BtnName);
    void slotChangeTheme(QString BtnName);
    void showThemeComBoxWidget();

public:
    QToolButton*                    m_pSnapModeBtn = nullptr;       ///< 捕捉模式按钮
    QToolButton*                    m_pViewBtn = nullptr;           ///< 视口列表按钮
    QToolButton*                    m_pUnitBtn = nullptr;           ///< 单位设置按钮
    QToolButton*                    m_pTheme = nullptr;             ///< 主题切换开关
private:
    QWidget*                        m_pBottomWidget = nullptr;
    QWidget*                        m_pViewportWidget = nullptr;    ///< 视图列表背板
    QWidget*                        m_pBottomToolWidget = nullptr;  ///< 捕捉模式选择板
    QWidget*                        m_pComboBoxWidget = nullptr;    ///< 单位按钮自定义的ComBox
    //QWidget*                      m_pThemeComBoxWidget;            // 主题按钮自定义的comBox
    QWidget*                        m_pWidget = nullptr;            ///< 父组件
    QLabel*                         m_pCoordLabel = nullptr;        ///< 鼠标坐标
    QLabel*                         m_pNumberLabel = nullptr;       ///< 选中实体数量
    QToolButton*                    m_pOrthogonalBtn = nullptr;     ///< 正交开关
    QToolButton*                    m_pGridBtn = nullptr;           ///< 背景网格按钮
    QToolButton*                    m_pShowWidthBtn = nullptr;      ///< 显示线宽按钮
    QToolButton*                    m_pThemeDark = nullptr;
    QToolButton*                    m_pThemeLight = nullptr;

    QHBoxLayout*                    m_pHLayout = nullptr;           ///< 水平布局
    UIActionHandler*                m_pActionHandler = nullptr;
    MDIWindow*                      m_pCurrentMdiWindow = nullptr;
    std::map<QString, QToolButton*> m_mapUnitBtn;
    QTableView*                     m_pViewports = nullptr;
    QStandardItemModel*             m_pViewsModel = nullptr;
    UISnapWidget*                   m_pSnapWidget = nullptr;
};

#endif // UIBOTTOMWIDGRT_H
