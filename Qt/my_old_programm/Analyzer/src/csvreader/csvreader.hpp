#ifndef CSVREADER_HPP
#define CSVREADER_HPP

#include <QObject>
#include <QString>
#include <QStringList>
#include <QFile>


class QTextStream;

// Класс чтения входных CSV-файлов
class CsvReader: public QObject
{
    Q_OBJECT

private:
    QFile _file;                        // ucxoдный CSV-фaйл
    QChar _separator;                   // разделитель, по которому определяется разбивка на колонки
    QList<QStringList> _lines_list;     // массив полученный в результате разбора CSV-файла

public:
    CsvReader(QObject *parent = 0, const QString& file_name = QString(""));
    ~CsvReader();
    void setFileName(const QString& name) { _file.setFileName(name); }  // ycmaнoвka имени файла
    bool Open();                                                        // omkpыmue файла
    QList<QStringList> CSVRead();                                       // разбор файла
    void close() { if(isOpen()) _file.close(); }                        // зakpыmue файла
    bool isOpen() const { return _file.isOpen(); }                      // npoвepka открытия файла

};
#endif // CSVREADER_HPP
