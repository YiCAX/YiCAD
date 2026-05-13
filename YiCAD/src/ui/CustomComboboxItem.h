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

/// @file CustomComboboxItem.h
/// @brief 自定义图层下拉框控件

#ifndef CUSTOMCOMBOBOXITEM_H
#define CUSTOMCOMBOBOXITEM_H

#include <QWidget>
#include <QColor>

class QLabel;
class QHBoxLayout;
class QToolButton;
class QPushButton;
class DmLayer;

struct ComboBoxData
{
    QToolButton*    btnOn = nullptr;       ///< 显示、隐藏
    QToolButton*    btnLock = nullptr;     ///< 锁定
    QToolButton*    btnPrint = nullptr;    ///< 打印
    QToolButton*    btnColor = nullptr;    ///< 颜色
    QPushButton*    labelName = nullptr;   ///< 名字
    QToolButton*    btnDelete = nullptr;   ///< 删除

    bool            isOn = false;
    bool            isLock = false;
    bool            isPrint = false;
    QColor          rgb;
    QString         strName;

    void setByData(ComboBoxData* data);
    void hide();
    void show();

    void setIsOn(const bool isOn);
    bool getIsOn();

    void setIsLock(const bool isLock);
    bool getIsLock();

    void setIsPrint(const bool isPrint);
    bool getIsPrint();

    void setColor(const QColor& color);
    QColor getColor();

    void setLayerName(const QString& name);
    QString getLayerName();
};


/// @brief 自定义图层下拉框
class CustomComboboxItem : public QWidget
{
    Q_OBJECT
public:
    /// @brief 下拉列表框的一行
    CustomComboboxItem(QWidget* parent, DmLayer* layer);
    ~CustomComboboxItem();

    ComboBoxData* getData() const;
    void setData(ComboBoxData* data);

    void setIsOn(const bool isOn);
    bool getIsOn();

    void setIsLock(const bool isLock);
    bool getIsLock();

    void setIsPrint(const bool isPrint);
    bool getIsPrint();

    void setColor(const QColor& color);
    QColor getColor();

    void setLayerName(const QString& name);
    QString getLayerName();
private:
    DmLayer* m_layer = nullptr;
    ComboBoxData* m_pItemData = nullptr;
};

#endif // CUSTOMCOMBOBOXITEM_H
