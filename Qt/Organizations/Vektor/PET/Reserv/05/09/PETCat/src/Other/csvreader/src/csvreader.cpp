//--------------------------------------------------------------------------------
#include <QTextStream>
//--------------------------------------------------------------------------------
#include "csvreader.hpp"
//--------------------------------------------------------------------------------
CsvReader::CsvReader(QObject *parent, const QString& file_name): QObject(parent),
    _file(file_name),
    _separator(',')
{

}
//--------------------------------------------------------------------------------
CsvReader::~CsvReader()
{
    close();
}
//--------------------------------------------------------------------------------
bool CsvReader::Open()
{
    if(!_file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return false;
    }
    else
    {
        return true;
    }
}
//--------------------------------------------------------------------------------
//чистка значения
QString trimCSV(QString item)
{
    if((!item.isEmpty())&&(item[0] == QChar(34)))
        item.remove(0,1);
    if((!item.isEmpty())&&(!item.isNull())&(item[item.count()-1] == QChar(34)))
        item.remove(item.count()-1,1);
    if(!item.isEmpty())
        item = item.replace("\"\"","\"");
    return item;

}
//--------------------------------------------------------------------------------
//Paзбop файла
//в соответствии со спецификацией, подробнее: http://ru.wikipedia.org/wiki/CSV
QList<QStringList> CsvReader::CSVRead()
{
    if (_file.isOpen())
    {
        bool Quote = false;
        QList<QString> ItemList;
        QString item = "";
        QTextStream out(&_file);
        while(!out.atEnd())
        {
            QString line(out.readLine().simplified());
            int count = line.count();
            for (int i = 0;i<count;i++)
            {
                if (line[i] == QChar(34))
                {
                    Quote = (Quote) ? false : true;
                }
                if ((Quote != true)&(line[i] == _separator))
                {
                    ItemList.append(trimCSV(item));
                    item = "";
                }
                else
                {
                    item += line[i];
                }

                if ((count-1 == i)&(Quote != true))
                {
                    item = trimCSV(item);
                    if (item != "")
                        ItemList.append(item);
                    _lines_list.append(ItemList);
                    ItemList.clear();
                    item = "";
                }
            }

        }
    }
    close();
    return _lines_list;
}
//--------------------------------------------------------------------------------
void CsvReader::set_new_separator(QChar new_separator)
{
    _separator = new_separator;
}
//--------------------------------------------------------------------------------
