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

/// @file UIDlgArc.cpp
/// @brief 圆弧属性对话框实现

#include "UIDlgArc.h"

#include <QPushButton>

#include "DmArc.h"
#include "DmDocument.h"
#include "Math2d.h"
#include "Transaction.h"

UIDlgArc::UIDlgArc(QWidget* parent, bool modal, Qt::WindowFlags fl)
    : QDialog(parent, fl)
{
    setModal(modal);
    setupUi(this);
}

UIDlgArc::~UIDlgArc()
{
    // no need to delete child widgets, Qt does it all for us
}

/// @brief 语言切换时刷新界面文本
void UIDlgArc::languageChange()
{
    retranslateUi(this);
}

void UIDlgArc::setArc(DmArc& a)
{
    arc = &a;
    wPen->setPen(arc->getPen(false), true, tr("Pen"));

    DmDocument* document = arc->getDocument();
    if (document)
    {
        cbLayer->init(*(document->getLayerTable()), false);
    }

    DmLayer* lay = arc->getLayer(false);
    if (lay)
    {
        cbLayer->setLayer(*lay);
    }

    QString s;
    s.setNum(arc->getCenter().x);
    leCenterX->setText(s);
    s.setNum(arc->getCenter().y);
    leCenterY->setText(s);
    s.setNum(arc->getRadius());
    leRadius->setText(s);
    s.setNum(Math2d::rad2deg(arc->getStartAngle()));
    leAngle1->setText(s);
    s.setNum(Math2d::rad2deg(arc->getEndAngle()));
    leAngle2->setText(s);
    s.setNum(arc->getNormal().x);
    leNormalX->setText(s);
    s.setNum(arc->getNormal().y);
    leNormalY->setText(s);
    s.setNum(arc->getNormal().z);
    leNormalZ->setText(s);
}

void UIDlgArc::updateArc()
{
    Transaction t(tr("Modify arc").toStdString(), arc->getDocument());
    t.start();
    arc->getDocument()->getEntityTable()->startModify(arc);
    arc->setCenter(DmVector(Math2d::eval(leCenterX->text()), Math2d::eval(leCenterY->text())));
    arc->setRadius(Math2d::eval(leRadius->text()));
    arc->setStartAngle(Math2d::deg2rad(Math2d::eval(leAngle1->text())));
    arc->setEndAngle(Math2d::deg2rad(Math2d::eval(leAngle2->text())));
    double normalX = Math2d::eval(leNormalX->text());
    double normalY = Math2d::eval(leNormalY->text());
    double normalZ = Math2d::eval(leNormalZ->text());
    arc->setNormal(DmVector(normalX, normalY, normalZ));
    arc->setPen(wPen->getPen());
    arc->setLayer(cbLayer->currentText());
    arc->update();
    arc->calculateBorders();
    t.commit();
}
