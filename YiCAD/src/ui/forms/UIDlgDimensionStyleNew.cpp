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

/// @file UIDlgDimensionStyleNew.cpp
/// @brief 新建标注样式对话框实现

#include "UIDlgDimensionStyleNew.h"

#include "DmDocument.h"
#include "DmDimensionStyle.h"
#include "GuiDialogFactory.h"

#include <QPushButton>
#include <QMessageBox>

#include "Debug.h"

UIDlgDimensionStyleNew::UIDlgDimensionStyleNew(QWidget* parent /*= nullptr*/, bool modal /*= false*/, Qt::WindowFlags fl /*= 0*/)
    : QDialog(parent, fl)
    , ui(new Ui::UIDlgDimensionStyleNew())
    , m_pDocument(nullptr)
{
    setModal(modal);
    ui->setupUi(this);
    ui->buttonBox->button(QDialogButtonBox::Cancel)->setText(tr("Cancel"));
    ui->buttonBox->button(QDialogButtonBox::Ok)->setText(tr("OK"));
}

UIDlgDimensionStyleNew::~UIDlgDimensionStyleNew()
{
}

void UIDlgDimensionStyleNew::init(DmDimensionStyle* tempStyle, DmDocument* document)
{
    ui->cbTemplateStyle->init(document->getDimStyleTable(), tempStyle->getName());
    m_pDocument = document;
    ui->leNewStyle->setText(QString("%1 %2").arg(tempStyle->getName()).arg(tr("copy")));
}

void UIDlgDimensionStyleNew::done(int r)
{
    //在关闭窗体前做验证。参考：https://www.qtcentre.org/threads/8048-Validate-Data-in-QDialog
    if (QDialog::Accepted == r)  // ok was pressed
    {
        QString newStyle = ui->leNewStyle->text();
        if (newStyle.trimmed().isEmpty())
        {
            QMessageBox::critical(this, tr("New dimension style"), tr("Please input valid dimension style name!"), QMessageBox::Close);
            return;
        }
        if (m_pDocument->getDimStyleTable()->find(newStyle))
        {
            QMessageBox::critical(this, tr("New dimension style"), tr("%0 is already exist !").arg(newStyle), QMessageBox::Close);
            return;
        }
        DmDimensionStyle* pNewStyle = new DmDimensionStyle(*ui->cbTemplateStyle->getStyle(), newStyle);
        pNewStyle->setDocument(m_pDocument);
        bool res = GUIDIALOGFACTORY->requestDimStyleModifyDialog(pNewStyle, m_pDocument);
        if (!res)
        {
            delete pNewStyle;
            pNewStyle = nullptr;
        }
        QDialog::done(r);
    }
    else    // cancel, close or exc was pressed
    {
        QDialog::done(r);
        return;
    }
}

bool UIDlgDimensionStyleNew::isInputValid()
{
    return false;
}
