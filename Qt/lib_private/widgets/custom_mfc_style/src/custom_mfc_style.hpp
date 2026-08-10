/*********************************************************************************
**                                                                              **
**     Copyright (C) 2026                                                       **
**                                                                              **
**     This program is free software: you can redistribute it and/or modify     **
**     it under the terms of the GNU General Public License as published by     **
**     the Free Software Foundation, either version 3 of the License, or        **
**     (at your option) any later version.                                      **
**                                                                              **
**     This program is distributed in the hope that it will be useful,          **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of           **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the            **
**     GNU General Public License for more details.                             **
**                                                                              **
**     You should have received a copy of the GNU General Public License        **
**     along with this program.  If not, see http://www.gnu.org/licenses/.      **
**                                                                              **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich                            **
**********************************************************************************/
#ifndef CUSTOM_MFC_STYLE_HPP
#define CUSTOM_MFC_STYLE_HPP
//--------------------------------------------------------------------------------
#include <QPushButton>
#include <QToolButton>
#include <QRadioButton>
#include <QPlainTextEdit>
#include <QTextEdit>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QSpinBox>
#include <QDialogButtonBox>

#include <QProxyStyle>
#include <QStyleOption>
#include <QPainter>
//--------------------------------------------------------------------------------
class Custom_MFC_style : public QProxyStyle
{
    Q_OBJECT
public:
    explicit Custom_MFC_style(const QString &baseStyle = "fusion")
        : QProxyStyle(baseStyle) {}

    void drawPrimitive(PrimitiveElement element,
                       const QStyleOption *option,
                       QPainter *painter,
                       const QWidget *widget = nullptr) const override;
    int pixelMetric(PixelMetric metric,
                    const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override;
    int styleHint(StyleHint hint, const QStyleOption *option, const QWidget *widget, QStyleHintReturn *returnData) const override;

    // Стандартный метод Qt для точечной настройки палитры виджетов перед показом
    void polish(QWidget *widget) override;
};
//--------------------------------------------------------------------------------
#endif
