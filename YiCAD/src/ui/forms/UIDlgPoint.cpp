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

/// @file UIDlgPoint.cpp
/// @brief 点实体属性编辑对话框

#include "UIDlgPoint.h"

#include <QPushButton>

#include "DmPoint.h"
#include "DmDocument.h"
#include "Math2d.h"
#include "Transaction.h"

UIDlgPoint::UIDlgPoint(QWidget* parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);

    this->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    this->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
}

UIDlgPoint::~UIDlgPoint()
{
}

void UIDlgPoint::languageChange()
{
    retranslateUi(this);
}

void UIDlgPoint::setPoint(DmPoint& p)
{
    m_pPoint = &p;
    wPen->setPen(m_pPoint->getPen(false), true, tr("Pen"));
    DmDocument* document = m_pPoint->getDocument();
    if (document != nullptr)
    {
        cbLayer->init(*(document->getLayerTable()), false);
    }
    DmLayer* lay = m_pPoint->getLayer(false);
    if (lay != nullptr)
    {
        cbLayer->setLayer(*lay);
    }

    QString s;
    s.setNum(m_pPoint->getPos().x);
    lePosX->setText(s);
    s.setNum(m_pPoint->getPos().y);
    lePosY->setText(s);
}

void UIDlgPoint::updatePoint()
{
    Transaction t(tr("Modify point").toStdString(), m_pPoint->getDocument());
    t.start();
    m_pPoint->getDocument()->getEntityTable()->startModify(m_pPoint);
    m_pPoint->setPos(DmVector(Math2d::eval(lePosX->text()), Math2d::eval(lePosY->text())));
    m_pPoint->setPen(wPen->getPen());
    m_pPoint->setLayer(cbLayer->currentText());
    t.commit();
}
