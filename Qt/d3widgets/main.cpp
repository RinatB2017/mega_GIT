#include <QApplication>
#include "charts/pie/dependencywheelwidget.h"
#include <QDebug>
#include <QTextCodec>

void setDependencies(d3widgets::DependencyWheelWidget *wheelWidget);

int main(int argc, char *argv[])
{
#if QT_VERSION < 0x050000
    QTextCodec::setCodecForCStrings (QTextCodec::codecForName("utf-8"));
#endif

    QApplication a(argc, argv);

    for (int i = 0; i < 1 ; i++) //для тестирования затрат памяти
    {
        d3widgets::DependencyWheelWidget *dependencyWheelWidget = new d3widgets::DependencyWheelWidget();
        setDependencies(dependencyWheelWidget);
        dependencyWheelWidget->setMargin(200);
        dependencyWheelWidget->show();
    }
    return a.exec();
}

void setDependencies(d3widgets::DependencyWheelWidget *wheelWidget)
{
    // установка зависимостей на примере "Хаб "Программирование" может включать статьи из
    // хаба "Python"
    wheelWidget->appendDependency("Программирование", "Python");
    wheelWidget->appendDependency("Программирование", "C++");
    wheelWidget->appendDependency("Программирование", "Веб-разработка");
    wheelWidget->appendDependency("Программирование", "PHP");
    wheelWidget->appendDependency("Программирование", "JavaScript");
    wheelWidget->appendDependency("Программирование", "Java");
    wheelWidget->appendDependency("Веб-разработка", "CSS");
    wheelWidget->appendDependency("Веб-разработка", "PHP");
    wheelWidget->appendDependency("Веб-разработка", "HTML");
    wheelWidget->appendDependency("Веб-разработка", "Веб-дизайн");
    wheelWidget->appendDependency("Веб-разработка", "JavaScript");
    wheelWidget->appendDependency("JavaScript", "Веб-разработка");
    wheelWidget->appendDependency("Python", "Веб-разработка");
    wheelWidget->appendDependency("Game-development", "Java");
    wheelWidget->appendDependency("Game-development", "Mobile Development");
    wheelWidget->appendDependency("Информационная безопасность", "Криптография");
    wheelWidget->appendDependency("Криптография", "Алгоритмы");
    wheelWidget->appendDependency("Разработка", "Веб-разработка");
    wheelWidget->appendDependency("Разработка", "Game-development");
    wheelWidget->appendDependency("Разработка", "Mobile Development");
    wheelWidget->appendDependency("Разработка", "Разработка под Windows");
    wheelWidget->appendDependency("Разработка", "Программирование микроконтроллеров");
    wheelWidget->appendDependency("Программирование микроконтроллеров", "FPGA");
    wheelWidget->appendDependency("FPGA", "Программирование микроконтроллеров");
    wheelWidget->appendDependency("Программирование микроконтроллеров", "Assembler");
    wheelWidget->appendDependency("Программирование микроконтроллеров", "Электроника для начинающих");
    wheelWidget->appendDependency("Электроника для начинающих", "FPGA");
    wheelWidget->appendDependency("OpenSource", "Программирование");
    wheelWidget->appendDependency("OpenSource", "Программирование");
    wheelWidget->appendDependency("C++", "Программирование");
    wheelWidget->appendDependency("Интерфейсы", "Визуализация данных");
    wheelWidget->appendDependency("Веб-дизайн", "Визуализация данных");
    wheelWidget->appendDependency("Веб-дизайн", "Usability");
    wheelWidget->appendDependency("Usability", "Визуализация данных");
    wheelWidget->appendDependency("Алгоритмы", "Программирование");
    wheelWidget->appendDependency("Мобильный Web", "HTML");
    wheelWidget->appendDependency("Мобильный Web", "JavaScript");
    wheelWidget->appendDependency("Мобильный Web", "CSS");
    wheelWidget->appendDependency("Mobile Development", "Разработка под Android");
    wheelWidget->appendDependency("Mobile Development", "Разработка под iOS");
    wheelWidget->appendDependency("Mobile Development", "Разработка под Windows Phone");
    wheelWidget->appendDependency("Mobile Development", "Мобильный Web");
    wheelWidget->appendDependency("Mobile Development", "C#");
    wheelWidget->appendDependency("Разработка под Android", "Mobile Development");
    wheelWidget->appendDependency("CSS", "Веб-разработка");
    wheelWidget->appendDependency("PHP", "Веб-разработка");
    wheelWidget->appendDependency("Python", "Веб-разработка");
    wheelWidget->appendDependency("HTML", "Веб-разработка");
    wheelWidget->appendDependency("Настройка Linux", "*nix");
    wheelWidget->appendDependency("Java", "Программирование");
    wheelWidget->appendDependency("C#", "Программирование");
    wheelWidget->appendDependency("Разработка под iOS", "Mobile Development");
    wheelWidget->appendDependency("Разработка под Windows", "VisualStudio");
    wheelWidget->appendDependency("*nix", "Настройка Linux");
    wheelWidget->appendDependency("VisualStudio", "Разработка под Windows");
}
