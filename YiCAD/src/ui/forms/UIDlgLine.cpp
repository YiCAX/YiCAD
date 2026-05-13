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

/// @file UIDlgLine.cpp
/// @brief 直线属性编辑对话框

#include "UIDlgLine.h"

#include <QPushButton>

#include "DmLine.h"
#include "DmDocument.h"
#include "Math2d.h"
#include "Transaction.h"

UIDlgLine::UIDlgLine(QWidget* parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);

    this->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    this->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
}

UIDlgLine::~UIDlgLine()
{
}

void UIDlgLine::languageChange()
{
    retranslateUi(this);
}

void UIDlgLine::setLine(DmLine& l)
{
    m_pLine = &l;
    wPen->setPen(m_pLine->getPen(false), true, tr("Pen"));
    DmDocument* document = m_pLine->getDocument();
    if (document != nullptr)
    {
        cbLayer->init(*(document->getLayerTable()), false);
    }
    DmLayer* lay = m_pLine->getLayer(false);
    if (lay != nullptr)
    {
        cbLayer->setLayer(*lay);
    }
    QString s;
    s.setNum(m_pLine->getStartpoint().x);
    leStartX->setText(s);
    s.setNum(m_pLine->getStartpoint().y);
    leStartY->setText(s);
    s.setNum(m_pLine->getEndpoint().x);
    leEndX->setText(s);
    s.setNum(m_pLine->getEndpoint().y);
    leEndY->setText(s);
}

void UIDlgLine::updateLine()
{
    Transaction t(tr("Modify line").toStdString(), m_pLine->getDocument());
    t.start();
    m_pLine->getDocument()->getEntityTable()->startModify(m_pLine);
    m_pLine->setStartpoint(DmVector(Math2d::eval(leStartX->text()), Math2d::eval(leStartY->text())));
    m_pLine->setEndpoint(DmVector(Math2d::eval(leEndX->text()), Math2d::eval(leEndY->text())));
    m_pLine->setPen(wPen->getPen());
    m_pLine->setLayer(cbLayer->currentText());
    t.commit();
}
