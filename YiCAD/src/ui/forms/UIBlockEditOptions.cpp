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

/// @file UIBlockEditOptions.cpp
/// @brief 块在位编辑选项栏实现

#include "UIBlockEditOptions.h"

#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>

#include "ActionBlocksEdit.h"
#include "ActionInterface.h"

UIBlockEditOptions::UIBlockEditOptions(QWidget* parent, Qt::WindowFlags fl)
    : QWidget(parent, fl)
    , m_action(nullptr)
{
    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 2, 4, 2);
    layout->setSpacing(8);

    m_label = new QLabel(this);
    m_label->setText(tr("Editing Block:"));

    m_completeButton = new QPushButton(tr("Complete"), this);
    connect(m_completeButton, &QPushButton::clicked, this, &UIBlockEditOptions::onCompleteClicked);

    layout->addWidget(m_label);
    layout->addStretch();
    layout->addWidget(m_completeButton);

    setLayout(layout);
}

UIBlockEditOptions::~UIBlockEditOptions() = default;

void UIBlockEditOptions::setAction(ActionInterface* a)
{
    if (a && a->getEntityType() == DM::ActionBlocksEdit)
    {
        m_action = static_cast<ActionBlocksEdit*>(a);
        m_label->setText(tr("Editing Block: \"%1\"").arg(m_action->getBlockName()));
    }
    else
    {
        m_action = nullptr;
    }
}

void UIBlockEditOptions::onCompleteClicked()
{
    if (m_action)
    {
        m_action->completeEditing();
    }
}
