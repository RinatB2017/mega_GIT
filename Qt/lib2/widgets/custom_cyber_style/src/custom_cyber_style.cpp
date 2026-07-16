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
#include "custom_cyber_style.hpp"
//--------------------------------------------------------------------------------
void Custom_cyber_style::drawPrimitive(PrimitiveElement element,
                                       const QStyleOption *option,
                                       QPainter *painter,
                                       const QWidget *widget) const
{
    // Проверяем, что Qt пытается нарисовать именно фон кнопки (обычной или ToolButton)
    if (element == PE_PanelButtonCommand || element == PE_FrameButtonBevel) {

        painter->save();
        painter->setRenderHint(QPainter::Antialiasing); // Чтобы скругление было гладким, без зазубрин

        // Берем цвета кнопок, которые мы ранее настроили в палитре приложения
        QColor buttonColor = option->palette.color(QPalette::Button);
        painter->setBrush(buttonColor);
        painter->setPen(Qt::NoPen); // Рисуем без обводки рамки

        // Проверяем состояние: наведена ли мышь прямо сейчас?
        if (option->state & QStyle::State_MouseOver) {
            // МЫШЬ НАВЕДЕНА: Рисуем красивый скругленный прямоугольник (радиус 12 пикселей)
            painter->drawRoundedRect(option->rect, 12, 12);
        } else {
            // МЫШЬ УШЛА: Рисуем обычную квадратную кнопку
            painter->drawRect(option->rect);
        }

        painter->restore();
        return; // Выходим, чтобы базовый стиль не нарисовал свою квадратную форму поверх
    }

    // Все остальные элементы интерфейса (окна, меню и т.д.) отдаем стандартному стилю
    QProxyStyle::drawPrimitive(element, option, painter, widget);
}
//--------------------------------------------------------------------------------
void Custom_cyber_style::drawComplexControl(ComplexControl control,
                                            const QStyleOptionComplex *option,
                                            QPainter *painter,
                                            const QWidget *widget) const
{
    // Проверяем, что это слайдер
    if (control == CC_Slider) {
        if (const QStyleOptionSlider *sliderOpt = qstyleoption_cast<const QStyleOptionSlider *>(option)) {

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);

            // 1. РИСУЕМ ДОРОЖКУ (Groove)
            if (sliderOpt->subControls & SC_SliderGroove) {
                // Получаем геометрию дорожки
                QRect grooveRect = subControlRect(CC_Slider, sliderOpt, SC_SliderGroove, widget);

                painter->setPen(Qt::NoPen);
                painter->setBrush(QColor(230, 230, 230)); // Светло-серый фон дорожки
                painter->drawRoundedRect(grooveRect, 3, 3);

                // Заполняем пройденную часть дорожки КРАСНЫМ
                QRect activeRect = grooveRect;
                if (sliderOpt->orientation == Qt::Horizontal) {
                    // Рассчитываем ширину от начала до текущего положения ручки
                    QRect handleRect = subControlRect(CC_Slider, sliderOpt, SC_SliderHandle, widget);
                    activeRect.setRight(handleRect.center().x());
                }
                painter->setBrush(Qt::red); // Наш красный акцент
                painter->drawRoundedRect(activeRect, 3, 3);
            }

            // 2. РИСУЕМ ПОДВИЖНУЮ РУЧКУ (Handle)
            if (sliderOpt->subControls & SC_SliderHandle) {
                QRect handleRect = subControlRect(CC_Slider, sliderOpt, SC_SliderHandle, widget);

                painter->setBrush(Qt::white); // Наша БЕЛАЯ ручка
                painter->setPen(QPen(Qt::red, 2)); // С красным контуром

                // Рисуем ручку в виде круглого элемента
                painter->drawEllipse(handleRect.center(), handleRect.width() / 2, handleRect.height() / 2);
            }

            painter->restore();
            return; // Готово! Базовый системный стиль Windows больше не вмешивается
        }
    }

    // Для всех остальных сложных элементов вызываем базу
    QProxyStyle::drawComplexControl(control, option, painter, widget);
}
//--------------------------------------------------------------------------------
void Custom_cyber_style::drawControl(ControlElement element,
                                     const QStyleOption *option,
                                     QPainter *painter,
                                     const QWidget *widget) const
{
    if (element == CE_ToolBar) {
        painter->save();

        // Закрашиваем весь прямоугольник панели инструментов чистым белым цветом
        painter->fillRect(option->rect, Qt::white);

        painter->restore();
        return; // Выходим, чтобы базовый Fusion не наложил поверх серую заливку или полосы
    }

    if (element == CE_DockWidgetTitle) {
        painter->save();
        painter->setRenderHint(QPainter::Antialiasing);

        // Красим фон заголовка в белый цвет
        painter->fillRect(option->rect, Qt::white);

        // Рисуем вокруг заголовка тонкую синюю рамку для кибер-эффекта
        QPen borderPen(Qt::blue, 1);
        painter->setPen(borderPen);
        painter->drawRect(option->rect.adjusted(0, 0, -1, -1));

        painter->restore();

        // ВАЖНО: Мы вызываем базу ПОСЛЕ нашей заливки,
        // чтобы Qt сам нарисовал текст названия дока и стандартные иконки поверх белого фона!
        QProxyStyle::drawControl(element, option, painter, widget);
        return;
    }

    // 1. Сначала всегда рисуем саму кнопку
    QProxyStyle::drawControl(element, option, painter, widget);

    // 2. Рисуем крест Х, только если мышь НЕ наведена на кнопку
    if (element == CE_PushButton || element == CE_ToolButtonLabel) {

        // Проверяем: если флаг State_MouseOver ОТСУТСТВУЕТ
        if (!(option->state & QStyle::State_MouseOver)) {

            painter->save();
            painter->setRenderHint(QPainter::Antialiasing);

            QPen crossPen(Qt::black, 2);
            painter->setPen(crossPen);

            QRect rect = option->rect;

            // Проводим линии из угла в угол
            painter->drawLine(rect.topLeft(), rect.bottomRight());
            painter->drawLine(rect.topRight(), rect.bottomLeft());

            painter->restore();
        }
    }
}
//--------------------------------------------------------------------------------
int Custom_cyber_style::pixelMetric(PixelMetric metric,
                                    const QStyleOption *option,
                                    const QWidget *widget) const
{
    switch (metric) {

    // 1. Сделаем все полосы прокрутки (ScrollBar) ультра-тонкими (например, 8 пикселей вместо стандартных 15)
    case PM_ScrollBarExtent:
        return 8;

    // 2. Увеличим внутренние отступы (поля) внутри кнопок, чтобы текст не прижимался к краям
    case PM_ButtonMargin:
        return 10; // 10 пикселей отступа со всех сторон текста

    // 3. Зададим фиксированный размер для всех иконок на кнопках и в меню (например, сделаем их крупнее — 24х24)
    case PM_ButtonIconSize:
    case PM_SmallIconSize:
        return 24;

    // Для всех остальных сотен метрик обязательно вызываем базовый Fusion-стиль
    default:
        return QProxyStyle::pixelMetric(metric, option, widget);
    }
}
//--------------------------------------------------------------------------------
QSize Custom_cyber_style::sizeFromContents(ContentsType type,
                                           const QStyleOption *option,
                                           const QSize &contentsSize,
                                           const QWidget *widget) const
{
    QSize size = QProxyStyle::sizeFromContents(type, option, contentsSize, widget);

    // Если Qt рассчитывает размер для обычной кнопки
    if (type == CT_PushButton) {
        size.setHeight(45); // Принудительно делаем ВСЕ кнопки высотой 45 пикселей
    }

    return size;
}
//--------------------------------------------------------------------------------
void Custom_cyber_style::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);
    if (!widget) return;

    if (qobject_cast<QPushButton*>(widget) || qobject_cast<QToolButton*>(widget))
    {
        QPalette palette = widget->palette();
        palette.setColor(QPalette::Button,      Qt::white);
        palette.setColor(QPalette::ButtonText,  Qt::red);

        widget->setPalette(palette);
    }
    else if (qobject_cast<QAbstractSpinBox*>(widget))
    {
        QPalette palette = widget->palette();

        palette.setColor(QPalette::Base, Qt::white);      // Фон под цифрами
        palette.setColor(QPalette::Text, Qt::red);        // Цвет самих цифр
        palette.setColor(QPalette::Button, Qt::white);    // Фон боковых кнопок
        palette.setColor(QPalette::ButtonText, Qt::red);  // Цвет стрелочек

        widget->setPalette(palette);
    }
    else if (qobject_cast<QRadioButton*>(widget) || qobject_cast<QCheckBox*>(widget))
    {
        QPalette palette = widget->palette();

        palette.setColor(QPalette::Base, Qt::white);        // Белый фон внутри кружка
        palette.setColor(QPalette::Text, Qt::red);          // Красная точка при выборе
        palette.setColor(QPalette::WindowText, Qt::red);    // Красный цвет текста подписи

        widget->setPalette(palette);
    }
    else if (qobject_cast<QComboBox*>(widget))
    {
        QPalette palette = widget->palette();

        palette.setColor(QPalette::Base, Qt::white);        // Фон поля и выпадающего списка
        palette.setColor(QPalette::Text, Qt::red);          // Цвет основного текста и пунктов списка
        palette.setColor(QPalette::Button, Qt::white);      // Фон области со стрелочкой
        palette.setColor(QPalette::ButtonText, Qt::red);    // Цвет стрелочки

        widget->setPalette(palette);
    }
    else if (qobject_cast<QLineEdit*>(widget))
    {
        QPalette palette = widget->palette();

        palette.setColor(QPalette::Base, Qt::white);               // Белый фон поля ввода
        palette.setColor(QPalette::Text, Qt::red);                 // Красный цвет вводимых букв
        palette.setColor(QPalette::PlaceholderText, QColor(255, 128, 128)); // Бледно-красный текст подсказки

        widget->setPalette(palette);
    }
    else if (qobject_cast<QTextEdit*>(widget) || qobject_cast<QPlainTextEdit*>(widget))
    {
        QPalette palette = widget->palette();

        palette.setColor(QPalette::Base, Qt::white);               // Белый фон текстового поля
        palette.setColor(QPalette::Text, Qt::red);                 // Красный цвет многострочного текста
        palette.setColor(QPalette::PlaceholderText, QColor(255, 128, 128)); // Бледно-красная подсказка

        widget->setPalette(palette);
    }
}
//--------------------------------------------------------------------------------
