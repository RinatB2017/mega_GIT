/*********************************************************************************
**  **
**     Copyright (C) 2026  **
**  **
**     This program is free software: you can redistribute it and/or modify  **
**     it under the terms of the GNU General Public License as published by  **
**     the Free Software Foundation, either version 3 of the License, or  **
**     (at your option) any later version.  **
**  **
**     This program is distributed in the hope that it will be useful,  **
**     but WITHOUT ANY WARRANTY; without even the implied warranty of  **
**     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the  **
**     GNU General Public License for more details.  **
**  **
**     You should have received a copy of the GNU General Public License  **
**     along with this program.  If not, see http://www.gnu.org/licenses/.  **
**  **
**********************************************************************************
**                   Author: Bikbao Rinat Zinorovich  **
**********************************************************************************/
#include <QPushButton>
//--------------------------------------------------------------------------------
#include "custom_cyber_style.hpp"
//--------------------------------------------------------------------------------
void Custom_cyber_style::drawPrimitive(PrimitiveElement element,
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const
{
    if (element == PE_PanelButtonCommand)
    {
        painter->setRenderHint(QPainter::Antialiasing);

        QRect rect = option->rect;
        QStyle::State state = option->state;

        // 1. Цвет фона кнопки
        QColor bgColor(0x0a, 0x11, 0x28); // Глубокий темно-синий
        if (state & State_Sunken)
        {
            bgColor = QColor(0x00, 0x1d, 0x4a); // Нажата
        }
        else if (state & State_MouseOver)
        {
            bgColor = QColor(0x1c, 0x25, 0x41); // Наведён курсор
        }

        // Рисуем тело кнопки
        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(rect, 6.0, 6.0);

        // 2. Цвет неоновой рамки
        QColor neonColor(0x00, 0xb4, 0xd8); // Голубой неон
        if (state & State_Sunken)
        {
            neonColor = QColor(0x00, 0x77, 0xb6);
        }
        else if (state & State_MouseOver)
        {
            neonColor = QColor(0x90, 0xe0, 0xef);
        }

        // Рисуем рамку
        QPen pen(neonColor, 2.0);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(rect.adjusted(1, 1, -1, -1), 6.0, 6.0);

        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
//--------------------------------------------------------------------------------
void Custom_cyber_style::drawComplexControl(ComplexControl control,
                                            const QStyleOptionComplex *option,
                                            QPainter *painter,
                                            const QWidget *widget) const
{
    // Проверяем, что Qt пытается нарисовать именно QSpinBox
    if (control == CC_SpinBox)
    {
        // Приводим базовую опцию к специальной структуре для спинбоксов
        auto sbOption = qstyleoption_cast<const QStyleOptionSpinBox *>(option);
        if (!sbOption)
            return;

        painter->setRenderHint(QPainter::Antialiasing);
        QRect rect = sbOption->rect;
        QStyle::State state = sbOption->state;

        // 1. Рисуем общий тёмно-синий фон для всего спинбокса
        QColor bgColor(0x0a, 0x11, 0x28);
        painter->setPen(Qt::NoPen);
        painter->setBrush(bgColor);
        painter->drawRoundedRect(rect, 4.0, 4.0);

        // 2. Рисуем неоновую рамку вокруг всего спинбокса
        QColor neonColor = (state & State_MouseOver) ? QColor(0x90, 0xe0, 0xef)
                                                     : QColor(0x00, 0xb4, 0xd8);
        if (state & State_HasFocus)
            neonColor = Qt::cyan; // Рамка горит ярче, если мы ввели туда курсор

        QPen pen(neonColor, 1.5);
        painter->setPen(pen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRoundedRect(rect.adjusted(1, 1, -1, -1), 4.0, 4.0);

        // 3. Рисуем стрелочки "Вверх" и "Вниз"
        // Запрашиваем у стиля точные координаты кнопок (они зависят от размера
        // шрифта)
        QRect upRect = subControlRect(CC_SpinBox, sbOption, SC_SpinBoxUp, widget);
        QRect downRect = subControlRect(CC_SpinBox, sbOption, SC_SpinBoxDown, widget);

        // Подсвечиваем стрелочку "Вверх", если мышь наведена конкретно на неё
        if (sbOption->activeSubControls & SC_SpinBoxUp)
        {
            painter->fillRect(upRect, QColor(0x1c, 0x25, 0x41));
        }
        // Подсвечиваем стрелочку "Вниз"
        if (sbOption->activeSubControls & SC_SpinBoxDown)
        {
            painter->fillRect(downRect, QColor(0x1c, 0x25, 0x41));
        }

        // Рисуем сами значки стрелок (треугольники)
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::white);

        // Стрелка вверх
        QPolygon upArrow;
        upArrow << QPoint(upRect.center().x(), upRect.top() + 4)
                << QPoint(upRect.left() + 4, upRect.bottom() - 4)
                << QPoint(upRect.right() - 4, upRect.bottom() - 4);
        painter->drawPolygon(upArrow);

        // Стрелка вниз
        QPolygon downArrow;
        downArrow << QPoint(upRect.center().x(), downRect.bottom() - 4)
                  << QPoint(downRect.left() + 4, downRect.top() + 4)
                  << QPoint(downRect.right() - 4, downRect.top() + 4);
        painter->drawPolygon(downArrow);

        return; // Мы полностью отрисовали каркас, прерываем стандартный Fusion
    }

    // Все остальные комплексные элементы (слайдеры, комбобоксы) рисуются базовым
    // стилем
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}
//--------------------------------------------------------------------------------
void Custom_cyber_style::drawControl(ControlElement element,
                                     const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *widget) const
{
    // Если Qt рисует иконку/текст внутри QToolButton
    if (element == CE_ToolButtonLabel)
    {
        auto tbOption = qstyleoption_cast<const QStyleOptionToolButton *>(option);
        if (tbOption)
        {
            // Создаем копию опции отрисовки, чтобы безопасно изменить цвета текста
            // внутри
            QStyleOptionToolButton labelOption(*tbOption);

            // Заставляем текст внутри ToolButton всегда быть белым
            labelOption.palette.setColor(QPalette::ButtonText, Qt::white);
            labelOption.palette.setColor(QPalette::WindowText, Qt::white);

            // Вызываем базовый Fusion для деликатного рендеринга иконки и текста.
            // Fusion сам правильно отцентрирует их, учтет размеры и не сломает
            // верстку.
            QProxyStyle::drawControl(element, &labelOption, painter, widget);
            return;
        }
    }

    QProxyStyle::drawControl(element, option, painter, widget);
}
//--------------------------------------------------------------------------------
void Custom_cyber_style::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);

    // Перехват QPushButton
    if (qobject_cast<QPushButton *>(widget))
    {
        QPalette pal = widget->palette();

        // Принудительно делаем текст на кнопках белым
        pal.setColor(QPalette::ButtonText, Qt::white);
        pal.setColor(QPalette::WindowText, Qt::white);

        widget->setPalette(pal);
    }

    // Перехват QSpinBox
    if (qobject_cast<QSpinBox *>(widget))
    {
        QPalette pal = widget->palette();
        pal.setColor(QPalette::Text, Qt::white);                // Белый текст цифр
        pal.setColor(QPalette::Base, QColor(0x0a, 0x11, 0x28)); // Тёмный фон под цифрами
        widget->setPalette(pal);
    }
}
//--------------------------------------------------------------------------------
