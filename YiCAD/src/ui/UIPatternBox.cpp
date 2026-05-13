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

/// @file UIPatternBox.cpp
/// @brief 填充图案选择下拉框控件

#include "UIPatternBox.h"

#include <QPixmap>
#include <QStringList>

#include "DmPattern.h"
#include "DmPatternList.h"
#include "Debug.h"

// Default Constructor. You must call init manually if you choose to use this constructor.
UIPatternBox::UIPatternBox(QWidget* parent)
    : QComboBox(parent)
{
}

UIPatternBox::~UIPatternBox() = default;

// Initialisation (called manually and only once).
void UIPatternBox::init()
{
    QStringList patterns;

    for (auto const& pa : *DMPATTERNLIST)
    {
        patterns.append(pa.first);
    }

    patterns.sort();
    insertItems(0, patterns);

    connect(this, SIGNAL(activated(int)), this, SLOT(slotPatternChanged(int)));

    setCurrentIndex(0);
    slotPatternChanged(currentIndex());
}

// Sets the currently selected width item to the given width.
void UIPatternBox::setPattern(const QString& pName)
{
    setCurrentIndex(findText(pName));
    slotPatternChanged(currentIndex());
}

DmPattern* UIPatternBox::getPattern()
{
    if (nullptr == m_pCurrentPattern)
    {
        m_pCurrentPattern = DMPATTERNLIST->requestPattern(currentText());
    }
    return m_pCurrentPattern;
}

// Called when the pattern has changed. This method sets the current pattern to the value chosen.
void UIPatternBox::slotPatternChanged(int index)
{
    m_pCurrentPattern = DMPATTERNLIST->requestPattern(currentText());
    emit patternChanged();
}
