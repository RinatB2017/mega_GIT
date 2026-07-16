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
#include "custom_aqua_style.hpp"
//--------------------------------------------------------------------------------
void Custom_Aqua_style::drawPrimitive(PrimitiveElement element,
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const
{
    if (element == PE_PanelButtonCommand || element == PE_FrameButtonBevel) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing); // Идеально гладкие края

        QRectF rect = option->rect.adjusted(1, 1, -1, -1);

        // Магический радиус: делаем кнопку "капсульной" формы (половина высоты)
        qreal cornerRadius = rect.height() / 2;

        // 1. НАСТРАИВАЕМ ЦВЕТА СВЕЧЕНИЯ
        QColor baseColor(255, 255, 255); // Базовый белый
        QPen borderPen(QColor(218, 218, 222), 1); // Тонкая, едва заметная серая граница

        if (option->state & QStyle::State_MouseOver) {
            // При наведении мыши — кайма становится мягкой синей, кнопка слегка подсвечивается
            borderPen.setColor(QColor(0, 122, 255));
            borderPen.setWidthF(1.5);
        }

        if (option->state & QStyle::State_Sunken || option->state & QStyle::State_On) {
            // При нажатии или фиксации — кнопка плавно темнеет до благородного светло-серого
            baseColor = QColor(230, 230, 235);
        }

        // 2. РИСУЕМ КНОПКУ
        painter->setPen(borderPen);
        painter->setBrush(baseColor);
        painter->drawRoundedRect(rect, cornerRadius, cornerRadius);

        painter->restore();
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
//--------------------------------------------------------------------------------
void Custom_Aqua_style::drawControl(ControlElement element,
                                     const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *widget) const
{
    // Фон панели инструментов оставляем чисто белым
    if (element == CE_ToolBar) {
        painter->save();
        painter->fillRect(option->rect, Qt::white);
        painter->restore();
        return;
    }

    // Текст на кнопках, док-окна и всё остальное пусть рисуется абсолютно стандартно
    QProxyStyle::drawControl(element, option, painter, widget);
}
//--------------------------------------------------------------------------------
int Custom_Aqua_style::pixelMetric(PixelMetric metric,
                                    const QStyleOption *option,
                                    const QWidget *widget) const
{
    switch (metric) {
    case PM_ButtonMargin:
        return 8; // Больше свободного места вокруг текста кнопки
    case PM_DefaultFrameWidth:
        return 1; // Очень тонкие, изящные рамки полей ввода
    case PM_ScrollBarExtent:
        return 12; // Компактные, аккуратные полосы прокрутки
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}
//--------------------------------------------------------------------------------
void Custom_Aqua_style::polish(QWidget *app)
{
    QProxyStyle::polish(app);
    if (!app) return;

    QPalette aquaPalette = app->palette();

    QColor pureWhite(255, 255, 255);
    QColor softGray(245, 245, 247);     // Мягкий фоновый цвет для контраста панелей
    QColor darkText(45, 45, 48);        // Интеллигентный темный текст вместо жесткого черного
    QColor aquaBlue(0, 122, 255);       // Благородный синий акцент Apple/XP

    // Окна и общие фоны
    aquaPalette.setColor(QPalette::Window,         softGray);
    aquaPalette.setColor(QPalette::WindowText,     darkText);

    // Кнопки (сделаем их базово белыми)
    aquaPalette.setColor(QPalette::Button,         pureWhite);
    aquaPalette.setColor(QPalette::ButtonText,     darkText);

    // Поля ввода, списки, таблицы
    aquaPalette.setColor(QPalette::Base,           pureWhite);
    aquaPalette.setColor(QPalette::Text,           darkText);

    // Цвет выделения элементов
    aquaPalette.setColor(QPalette::Highlight,      aquaBlue);
    aquaPalette.setColor(QPalette::HighlightedText, pureWhite);

    app->setPalette(aquaPalette);
}
//--------------------------------------------------------------------------------
