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

/// @file UIDlgCircle.cpp
/// @brief 圆属性对话框实现

#include "UIDlgCircle.h"

#include <QPushButton>

#include "DmCircle.h"
#include "DmDocument.h"
#include "Math2d.h"
#include "Transaction.h"

UIDlgCircle::UIDlgCircle(QWidget* parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);

    this->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    this->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
}

UIDlgCircle::~UIDlgCircle()
{
}

/// @brief 语言切换时刷新界面文本
void UIDlgCircle::languageChange()
{
    retranslateUi(this);
}

void UIDlgCircle::setCircle(DmCircle& c)
{
    m_pCircle = &c;
    wPen->setPen(m_pCircle->getPen(false), true, tr("Pen"));

    DmDocument* document = m_pCircle->getDocument();
    if (document)
    {
        cbLayer->init(*(document->getLayerTable()), false);
    }

    DmLayer* lay = m_pCircle->getLayer(false);
    if (lay)
    {
        cbLayer->setLayer(*lay);
    }

    QString s;
    s.setNum(m_pCircle->getCenter().x);
    leCenterX->setText(s);
    s.setNum(m_pCircle->getCenter().y);
    leCenterY->setText(s);
    s.setNum(m_pCircle->getRadius());
    leRadius->setText(s);
}

void UIDlgCircle::updateCircle()
{
    Transaction t(tr("Modify circle").toStdString(), m_pCircle->getDocument());
    t.start();
    m_pCircle->getDocument()->getEntityTable()->startModify(m_pCircle);
    m_pCircle->setCenter(DmVector(Math2d::eval(leCenterX->text()), Math2d::eval(leCenterY->text())));
    m_pCircle->setRadius(Math2d::eval(leRadius->text()));
    m_pCircle->setPen(wPen->getPen());
    m_pCircle->setLayer(cbLayer->currentText());
    m_pCircle->calculateBorders();
    m_pCircle->update();
    t.commit();
}
