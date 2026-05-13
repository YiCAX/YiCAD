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

/// @file UIFontBox.cpp
/// @brief 字体选择下拉框控件，支持SHX字体、系统字体和缺失字体图标显示

#include "UIFontBox.h"

#include <QDebug>

#include "DmFont.h"
#include "DmFontList.h"

#include "Debug.h"

// Default Constructor. You must call init manually if you choose to use this constructor.
UIFontBox::UIFontBox(QWidget* parent)
    : QComboBox(parent)
{
    m_canEmitSignal = false;

    connect(this, SIGNAL(currentTextChanged(const QString&)), this, SLOT(slotCurrentTextChanged(QString)));
}

//// Initialisation (called from constructor or manually but only once).
//void UIFontBox::init()
//{
//	QStringList fonts;
//
//	for (auto const& f : *DMFONTLIST)
//	{
//		if (fonts.contains(f->getFileName()))
//		{
//			DEBUG_HEADER
//				qDebug() << __func__ << ": WARNING: duplicated font: " << f->getFileName();
//			continue;
//		}
//
//		fonts.append(f->getFileName());
//	}
//	addItems(fonts);
//
//	connect(this, SIGNAL(activated(int)), this, SLOT(slotFontChanged(int)));
//
//	setCurrentIndex(0);
//	slotFontChanged(currentIndex());
//}

//void UIFontBox::setFontBoxType(FontBoxType type)
//{
//	m_boxType = type;
//}
//
//FontBoxType UIFontBox::getFontBoxType() const
//{
//	return m_boxType;
//}

void UIFontBox::addFonts(const std::vector<std::pair<FontIconType, QString>>& fonts)
{
    m_canEmitSignal = false;
    for (auto item : fonts)
    {
        auto type = item.first;
        QString font = item.second;
        switch (type)
        {
        case FontIconType::None:
            QComboBox::addItem(font);
            break;
        case FontIconType::Shx:
            QComboBox::addItem(QIcon(":/ribbon/forms_dlg/shx_font.svg"), font);
            break;
        case FontIconType::SystemFont:
            QComboBox::addItem(QIcon(":/ribbon/forms_dlg/system_font.svg"), font);
            break;
        case FontIconType::NotExist:
            QComboBox::addItem(QIcon(":/ribbon/forms_dlg/warning.svg"), font);
            break;
        default:
            break;
        }
    }
    m_fonts.insert(m_fonts.end(), fonts.begin(), fonts.end());
    m_canEmitSignal = true;
}

std::vector<std::pair<FontIconType, QString>> UIFontBox::getFonts() const
{
    return m_fonts;
}

QString UIFontBox::firstFontOfType(FontIconType type) const
{
    for (auto item : m_fonts)
    {
        if (item.first == type)
        {
            return item.second;
        }
    }
    return "";
}

QString UIFontBox::firstValidFont() const
{
    for (auto item : m_fonts)
    {
        if (item.first != FontIconType::NotExist)
        {
            return item.second;
        }
    }
    return "";
}

QString UIFontBox::currentFont() const
{
    return currentText();
}

void UIFontBox::clearFonts()
{
    m_canEmitSignal = false;
    m_fonts.clear();
    QComboBox::clear();
    m_canEmitSignal = true;
}

void UIFontBox::slotCurrentTextChanged(QString text)
{
    // 用来屏蔽QComboBox::addItem在第一次调用时触发的currentTextChanged()
    if (m_canEmitSignal)
    {
        emit fontChanged(text);
    }
}
