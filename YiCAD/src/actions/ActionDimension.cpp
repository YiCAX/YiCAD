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

/// @file ActionDimension.cpp
/// @brief 标注操作基类实现

#include "ActionDimAligned.h"

#include "DmDimension.h"
#include "GuiDialogFactory.h"
#include "GuiDocumentView.h"
#include "DmDocument.h"

ActionDimension::ActionDimension(const char* name, DmDocument* doc,
                                 GuiDocumentView* docView) :
    PreviewActionInterface(name, doc, docView)
{
    reset();
}

ActionDimension::~ActionDimension() = default;

void ActionDimension::reset()
{
    data.reset(new DmDimensionData(DmVector(false), DmVector(false),
                EMTextVertMode::kTextVertMid, EMTextHorzMode::kTextCenter,
                1.0, "", 0.0,
                docView->getDocument()->getDimStyleTable()->getActive()));
    diameter = false;
}

void ActionDimension::init(int status)
{
    PreviewActionInterface::init(status);
}

void ActionDimension::hideOptions()
{
    ActionInterface::hideOptions();

    GUIDIALOGFACTORY->requestOptions(this, false);
}

void ActionDimension::showOptions()
{
    ActionInterface::showOptions();

    GUIDIALOGFACTORY->requestOptions(this, true, true);
}

void ActionDimension::updateMouseCursor()
{
    docView->setMouseCursor(DM::SelectCursor);
}

bool ActionDimension::isDimensionAction(DM::ActionType type)
{
    switch (type)
    {
        case DM::ActionDimAligned:
        case DM::ActionDimLinear:
        case DM::ActionDimAngular:
        case DM::ActionDimDiametric:
        case DM::ActionDimRadial:
            return true;
        default:
            return false;
    }
}

void ActionDimension::setText(const QString& t)
{
    data->text = t;
}

const QString& ActionDimension::getLabel() const
{
    return label;
}

void ActionDimension::setLabel(const QString& t)
{
    label = t;
}

const QString& ActionDimension::getTol1() const
{
    return tol1;
}

void ActionDimension::setTol1(const QString& t)
{
    tol1 = t;
}

const QString& ActionDimension::getTol2() const
{
    return tol2;
}

void ActionDimension::setTol2(const QString& t)
{
    tol2 = t;
}

bool ActionDimension::getDiameter() const
{
    return diameter;
}

void ActionDimension::setDiameter(bool d)
{
    diameter = d;
}

