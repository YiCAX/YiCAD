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

/// @file UICommandWidget.h
/// @brief 命令行输入控件，提供命令输入、自动补全和历史记录功能

#ifndef UICOMMANDWIDGETNEW
#define UICOMMANDWIDGETNEW

#include <QWidget>
#include <QLineEdit>
#include <QTextEdit>
#include <QToolButton>
#include "DmVector.h"
#include "MDIWindow.h"
#include "qevent.h"
#include "UITabDrawWidget.h"

class UIActionHandler;
class QPropertyAnimation;

class UICommandWidget : public QWidget
{
    Q_OBJECT
public:
    UICommandWidget(QWidget* parent, UITabDrawWidget* tabDrawWidget);
    ~UICommandWidget();

    void createTriangleBtn();
    void createLineEdit();
    void createTempWin(QLineEdit* e);

    void setCompleterStrings(const QStringList& strs);
    QWidget* createTempEdit();
    QWidget* getCommandWidget();
    QWidget* getInfoWidget();
    QWidget* getTempWidget();
    QWidget* getTipWidget();

    void appCmdTempText(const QString text);

    void setActionHandler(UIActionHandler* pActionHandler);

    void doNothing();

    QLineEdit* getEditline();
    QLineEdit* getEdit();
    QTextEdit* getTipWin();

private slots:
    void pressShowTextEdit();
    void pressShowLineEdit();
    void btnShowTextEdit();

private:
    QCompleter*                     m_pCompleter = nullptr;
    QLineEdit*                      m_editline = nullptr;
    QLineEdit*                      m_pEdit = nullptr;                      ///< 输入栏
    QTextEdit*                      m_pTipWin = nullptr;

    QWidget*                        m_pWidget = nullptr;
    QWidget*                        m_cmdWin = nullptr;                     ///< 输入栏背板
    QWidget*                        m_infoWin = nullptr;                    ///< 信息保存栏
    QWidget*                        m_pCmdTempWin = nullptr;                ///< 临时信息显示栏
    QTextEdit*                      m_pTempTextEdit = nullptr;
    QTextEdit*                      m_infoTextEdit = nullptr;
    QString                         m_LineText;
    QString                         m_pLastText;                            ///< 记录上一次输入的值
    QString                         m_cmdInfo;
    UIActionHandler*                m_pActionHandler = nullptr;
    QWidget*                        editWidget = nullptr;
    QWidget*                        m_pTipWidget = nullptr;
    QString                         m_Coord;
    UITabDrawWidget*                m_pTabDrawWidget = nullptr;
    std::unique_ptr<QPropertyAnimation> m_pAnimation;
};
#endif // UICOMMANDWIDGETNEW