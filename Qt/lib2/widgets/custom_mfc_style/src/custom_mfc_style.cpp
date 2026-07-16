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
#include "custom_mfc_style.hpp"
//--------------------------------------------------------------------------------
void Custom_MFC_style::drawPrimitive(PrimitiveElement element,
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const
{
    if (element == PE_PanelButtonCommand || element == PE_FrameButtonBevel) {
        painter->save();

        QRect r = option->rect;
        QColor btnColor(212, 208, 200);   // Основной серый фон
        QColor whiteColor(255, 255, 255); // Блик (свет)
        QColor darkGray(128, 128, 128);   // Тень
        QColor blackColor(0, 0, 0);       // Глубокая тень

        // Заливаем саму кнопку базовым серым цветом
        painter->fillRect(r, btnColor);

        // Проверяем состояние: кнопка НАЖАТА (или зафиксирована)?
        if (option->state & QStyle::State_Sunken || option->state & QStyle::State_On) {
            // Эффект "вдавленной" кнопки: тени переходят наверх и влево
            painter->setPen(darkGray);
            painter->drawLine(r.topLeft(), r.topRight());
            painter->drawLine(r.topLeft(), r.bottomLeft());

            painter->setPen(blackColor);
            painter->drawLine(r.left() + 1, r.top() + 1, r.right() - 1, r.top() + 1);
            painter->drawLine(r.left() + 1, r.top() + 1, r.left() + 1, r.bottom() - 1);
        }
        else {
            // Эффект "выпуклой" кнопки (Обычное состояние)
            // Рисуем белые линии сверху и слева (блик)
            painter->setPen(whiteColor);
            painter->drawLine(r.topLeft(), r.topRight());
            painter->drawLine(r.topLeft(), r.bottomLeft());

            // Рисуем темно-серые линии снизу и справа (внутренняя тень)
            painter->setPen(darkGray);
            painter->drawLine(r.left() + 1, r.bottom() - 1, r.right() - 1, r.bottom() - 1);
            painter->drawLine(r.right() - 1, r.top() + 1, r.right() - 1, r.bottom() - 1);

            // Рисуем черные линии по самому внешнему краю снизу и справа (внешняя тень)
            painter->setPen(blackColor);
            painter->drawLine(r.bottomLeft(), r.bottomRight());
            painter->drawLine(r.topRight(), r.bottomRight());
        }

        painter->restore();
        return;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
//--------------------------------------------------------------------------------
int Custom_MFC_style::pixelMetric(PixelMetric metric,
                                    const QStyleOption *option,
                                    const QWidget *widget) const
{
    switch (metric) {
    case PM_ButtonMargin:
        return 4; // Маленькие, экономные отступы внутри кнопок
    case PM_DefaultFrameWidth:
        return 2; // Строгая толщина рамок окон ввода в 2 пикселя
    case PM_ScrollBarExtent:
        return 16; // Толстые, массивные олдскульные полосы прокрутки
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}
//--------------------------------------------------------------------------------
int Custom_MFC_style::styleHint(StyleHint hint,
                                  const QStyleOption *option,
                                  const QWidget *widget,
                                  QStyleHintReturn *returnData) const {
    switch (hint) {
    case SH_UnderlineShortcut:
        return 1; // Всегда подчеркивать буквы горячих клавиш (как в Win95)
    case SH_DialogButtonLayout:
        return QDialogButtonBox::WinLayout; // Строгий порядок кнопок: ОК, затем Отмена
    default:
        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
}
//--------------------------------------------------------------------------------
void Custom_MFC_style::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);
    if (!widget) return;

    QPalette mfcPalette = widget->palette();

    // Тот самый легендарный серо-бежевый фон окон и кнопок (3D-панелей)
    QColor mfcGray(212, 208, 200);

    mfcPalette.setColor(QPalette::Window,     mfcGray);
    mfcPalette.setColor(QPalette::Button,     mfcGray);

    // Текст всегда строго черный
    mfcPalette.setColor(QPalette::WindowText, Qt::black);
    mfcPalette.setColor(QPalette::ButtonText, Qt::black);

    // Фоны полей ввода и списков — чисто белые, текст внутри — черный
    mfcPalette.setColor(QPalette::Base,       Qt::white);
    mfcPalette.setColor(QPalette::Text,       Qt::black);

    // Легендарный темно-синий цвет активного заголовка окна (Classic Windows Navy)
    mfcPalette.setColor(QPalette::Highlight,  QColor(10, 36, 106));
    mfcPalette.setColor(QPalette::HighlightedText, Qt::white);

    widget->setPalette(mfcPalette);
}
//--------------------------------------------------------------------------------
