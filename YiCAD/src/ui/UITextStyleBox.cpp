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

/// @file UITextStyleBox.cpp
/// @brief 文字样式选择下拉框控件，管理和切换CAD文档中的文字样式

#include "UITextStyleBox.h"
#include "DmTextStyle.h"
#include "DmTextStyleTable.h"
#include <QKeyEvent>
#include <QMessageBox>


UITextStyleBox::UITextStyleBox(QWidget* parent)
    : QComboBox(parent)
    , m_lastIndex(-1)
    , m_changeQueryFunc(nullptr)
    , m_pStyle(nullptr)
    , m_pTextStyleTable(nullptr)
{

}

DmTextStyle* UITextStyleBox::getStyle()
{
    return m_pStyle;
}

void UITextStyleBox::setStyle(const QString& style)
{
    setCurrentText(style);
    auto oldStyle = m_pStyle;
    m_pStyle = m_pTextStyleTable->find(style);
    if (oldStyle != m_pStyle)
    {
        emit styleChanged();
    }
}

void UITextStyleBox::init(DmTextStyleTable* textStyleTable)
{
    m_pTextStyleTable = textStyleTable;
    clear();
    QStringList list;
    DmTextStyle* pActive = m_pTextStyleTable->getActive();
    int i = 0;
    int activeIdx = 0;
    for (auto it = m_pTextStyleTable->begin(); it != m_pTextStyleTable->end(); ++it)
    {
        list.append((*it)->getName());
        if (*it == pActive)
        {
            activeIdx = i;
        }
        i++;
    }
    addItems(list);
    connect(this, SIGNAL(currentTextChanged(const QString&)), this, SLOT(slotStyleChanged(const QString&)));
    if (pActive)
    {
        setCurrentIndex(activeIdx);
        m_lastIndex = activeIdx;
        setStyle(pActive->getName());
    }
}

void UITextStyleBox::setChangeQueryFunc(ChangeQueryFunc callBack)
{
    m_changeQueryFunc = callBack;
}

void UITextStyleBox::mousePressEvent(QMouseEvent* e)
{
    m_userChoose = true;
    QComboBox::mousePressEvent(e);
}

void UITextStyleBox::keyPressEvent(QKeyEvent* e)
{
    if (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down)
    {
        m_userChoose = true;
    }
    QComboBox::keyPressEvent(e);
}

void UITextStyleBox::slotStyleChanged(const QString& text)
{
    if (m_userChoose)
    {
        // 如果注册了回调函数，调用它
        bool checkVal = true;
        if (m_changeQueryFunc != nullptr)
        {
            checkVal = (*m_changeQueryFunc)();
        }

        // 取消选择变化
        if (!checkVal)
        {
            m_userChoose = false;
            if (m_lastIndex != -1 && count() != 0)
            {
                setCurrentIndex(m_lastIndex);
            }
            return;
        }
    }

    // 确认选择变化
    m_lastIndex = currentIndex();
    m_pStyle = m_pTextStyleTable->find(text);
    emit styleChanged();
}
