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

/// @file UIDlgPolyline.cpp
/// @brief 多段线属性编辑对话框

#include "UIDlgPolyline.h"

#include <QPushButton>

#include "DmPolyline.h"
#include "DmDocument.h"
#include "Transaction.h"

UIDlgPolyline::UIDlgPolyline(QWidget* parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);

    this->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    this->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
}

UIDlgPolyline::~UIDlgPolyline()
{
}

void UIDlgPolyline::languageChange()
{
    retranslateUi(this);
}

void UIDlgPolyline::setPolyline(DmPolyline& e)
{
    m_pPolyline = &e;
    wPen->setPen(m_pPolyline->getPen(false), true, tr("Pen"));
    DmDocument* document = m_pPolyline->getDocument();
    if (document != nullptr)
    {
        cbLayer->init(*(document->getLayerTable()), false);
    }
    DmLayer* lay = m_pPolyline->getLayer(false);
    if (lay != nullptr)
    {
        cbLayer->setLayer(*lay);
    }

    cbClosed->setChecked(m_pPolyline->isClosed());
}

void UIDlgPolyline::updatePolyline()
{
    Transaction t(tr("Modify polyline").toStdString(), m_pPolyline->getDocument());
    t.start();
    m_pPolyline->getDocument()->getEntityTable()->startModify(m_pPolyline);
    m_pPolyline->setClosed(cbClosed->isChecked());
    m_pPolyline->setPen(wPen->getPen());
    m_pPolyline->setLayer(cbLayer->currentText());
    m_pPolyline->update();
    t.commit();
}
