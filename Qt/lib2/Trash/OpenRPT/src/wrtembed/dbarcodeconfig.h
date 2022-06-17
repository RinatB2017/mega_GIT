#ifndef DBARCODECONFIG_H
#define DBARCODECONFIG_H

#include <QWidget>

namespace Ui {
    class DBarcodeConfig;
}

class DBarcodeConfig : public QWidget
{
    Q_OBJECT

public:
    explicit DBarcodeConfig(QWidget *parent = nullptr);
    ~DBarcodeConfig();

    int getSliderMaxValue();
    void setSliderMaxValue(int i);

private:
    Ui::DBarcodeConfig *ui;
};

#endif // DBARCODECONFIG_H
