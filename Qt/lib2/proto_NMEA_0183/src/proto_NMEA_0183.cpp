//--------------------------------------------------------------------------------
#include <QStringList>
//--------------------------------------------------------------------------------
#include "proto_NMEA_0183.hpp"
//--------------------------------------------------------------------------------
Proto_NMEA_0183::Proto_NMEA_0183(QWidget *parent) :
    QWidget(parent)
{
    if(parent)
    {
        connect(this, SIGNAL(info(QString)),    parent, SIGNAL(info(QString)));
        connect(this, SIGNAL(debug(QString)),   parent, SIGNAL(debug(QString)));
        connect(this, SIGNAL(error(QString)),   parent, SIGNAL(error(QString)));
        connect(this, SIGNAL(trace(QString)),   parent, SIGNAL(trace(QString)));
        connect(this, SIGNAL(message(QString)), parent, SIGNAL(message(QString)));
    }

    init();
}
//--------------------------------------------------------------------------------
void Proto_NMEA_0183::init(void)
{
    time_observation_hour = 0;
    time_observation_min = 0;
    time_observation_sec = 0;
    latitude_grad = 0;
    latitude_min = 0;
    latitude_dir.clear();
    longitude_dir.clear();
    day_utc = 0;
    month_utc = 0;
    year_utc = 0;
    local_hour = 0;
    local_min = 0;
    quality = 0;
    nka = 0;
    for(int n=0; n<15; n++)
        m_nka[n] = 0;
    horizont_faktor = 0;
    f_height = 0.0f;
    f_geoid = 0.0f;
    time_in_sec = 0.0f;
    id_station = 0;
    type_management.clear();
    state_work = 0;
    PDOP = 0.0f;
    HDOP = 0.0f;
    VDOP = 0.0f;
    state.clear();
    speed_in_uzlah = 0.0f;
    speed_in_km = 0.0f;
    kurs_in_grad = 0.0f;
    true_kurs_in_grad = 0.0f;
    magnet_kurs_in_grad = 0.0f;
    data_str.clear();
    magnet_sklon_in_grad = 0.0f;
    reg_location.clear();
    X = 0.0f;
    Y = 0.0f;
    Z = 0.0f;
    V = 0.0f;
    K = 0.0f;
}
//--------------------------------------------------------------------------------
QString Proto_NMEA_0183::get_checksum(const QString &data)
{
    QString temp;
    uchar sum;
    int n = 0;

    if(data.length()<3)
    {
        emit error(QString("len = %1").arg(data.length()));
        return "";
    }
    temp = data.mid(0, data.length());

    sum = 0;
    for(n=0; n<temp.length(); n++)
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
        sum ^= temp.at(n).toAscii();
#else
        sum ^= temp.at(n).toLatin1();
#endif

    return QString("%1").arg(sum, 2, 16, QLatin1Char('0')).toUpper();
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::set_port(QString *result,
                              uchar number_port,
                              uint bauldrate,
                              uchar type_proto,
                              ushort mask)
{
    QString str_port;
    QString str_bauldrate;
    QString str_checksum;
    QString temp;
    QString message;

    str_port.clear();
    switch(number_port)
    {
    case 0:
    case 1:
        str_port = QString("%1").arg(number_port);
        break;
    case 0xFF:
        str_port = "";
        break;
    default:
        emit error(QString("unknown port %1").arg(number_port));
        return E_ERROR_UNKNOWN_PORT;
        break;
    }

    str_bauldrate.clear();
    switch(bauldrate)
    {
    case 1200:
    case 1800:
    case 2400:
    case 4800:
    case 9600:
    case 19200:
    case 38400:
    case 57600:
    case 115200:
        str_bauldrate = QString("%1").arg(bauldrate);
        break;
    default:
        emit error(QString("unknown bauldrate %1").arg(bauldrate));
        return E_ERROR_UNKNOWN_BAULDRATE;
        break;
    }

    if(type_proto > 4)
    {
        emit error(QString("unknown type_proto %1").arg(type_proto));
        return E_ERROR_TYPE_PROTO;
    }

    message = QString("$PIRPR,%1,%2,%3,%4*")
            .arg(str_port)
            .arg(str_bauldrate)
            .arg(QString("%1").arg(type_proto))
            .arg(QString("%1").arg(mask, 4, 16, QLatin1Char('0')).toUpper());

    str_checksum = get_checksum(message);

    temp = QString("%1%2\r\n")
            .arg(message)
            .arg(str_checksum);

    *result = temp;

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::change_param_coord_and_time(QString *result,
                                                 uchar s_coord,
                                                 uchar hour,
                                                 uchar min)
{
    QString message;
    QString temp;

    if(s_coord > 4)
    {
        emit error(QString("unknown s_coord %1").arg(s_coord));
        return E_ERROR_SYSTEM_COORD;
    }

    if(hour > 23)
    {
        emit error(QString("hour = %1").arg(hour));
        return E_ERROR_HOUR;
    }

    if(min > 59)
    {
        emit error(QString("min = %1").arg(min));
        return E_ERROR_MIN;
    }

    message = QString("$PIRTR,%1,%2%3*")
            .arg(QString("%1").arg(s_coord))
            .arg(hour, 2, 10, QLatin1Char('0')).toUpper()
            .arg(min, 2, 10, QLatin1Char('0')).toUpper();

    temp = QString("%1%2\r\n")
            .arg(message)
            .arg(get_checksum(message));

    *result = temp;

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::select_satellite(QString *result,
                                      uint satellite_grs,
                                      uint satellite_glonass)
{
    QString message;
    QString temp;

    message = QString("$PIRSR,%1,%2,*")
            .arg(satellite_grs, 8, 16, QLatin1Char('0')).toUpper()
            .arg(satellite_glonass, 6, 16, QLatin1Char('0')).toUpper();

    temp = QString("%1%2\r\n")
            .arg(message)
            .arg(get_checksum(message));

    *result = temp;

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::check_id_message(const QString &data)
{
    if(data == "GPGGA") return MESSAGE_GP_GGA;
    if(data == "GLGGA") return MESSAGE_GL_GGA;
    if(data == "GNGGA") return MESSAGE_GN_GGA;

    if(data == "GPGSA") return MESSAGE_GP_GSA;
    if(data == "GLGSA") return MESSAGE_GL_GSA;
    if(data == "GNGSA") return MESSAGE_GN_GSA;

    if(data == "GPGSV") return MESSAGE_GP_GSV;
    if(data == "GLGSV") return MESSAGE_GL_GSV;
    if(data == "GNGSV") return MESSAGE_GN_GSV;

    if(data == "GPRMC") return MESSAGE_GP_RMC;
    if(data == "GLRMC") return MESSAGE_GL_RMC;
    if(data == "GNRMC") return MESSAGE_GN_RMC;

    if(data == "GPVTG") return MESSAGE_GP_VTG;
    if(data == "GLVTG") return MESSAGE_GL_VTG;
    if(data == "GNVTG") return MESSAGE_GN_VTG;

    if(data == "GPGLL") return MESSAGE_GP_GLL;
    if(data == "GLGLL") return MESSAGE_GL_GLL;
    if(data == "GNGLL") return MESSAGE_GN_GLL;

    if(data == "GPZDA") return MESSAGE_GP_ZDA;
    if(data == "GLZDA") return MESSAGE_GL_ZDA;
    if(data == "GNZDA") return MESSAGE_GN_ZDA;

    if(data == "PIREA") return MESSAGE_PIREA;
    if(data == "PIRFV") return MESSAGE_PIRFV;
    if(data == "PIRGK") return MESSAGE_PIRGK;
    if(data == "PIRRA") return MESSAGE_PIRRA;

    return MESSAGE_UNKNOWN;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::check_message(QString data)
{
    QStringList sl;
    int id_message;

    if(data.length() < 6)
    {
        emit error("data.length() < 6");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("no delimiter");
        return E_ERROR_FORMAT_MESSAGE;
    }

    int index = -1;

    index = data.indexOf('*');
    if(index > 0)
    {
        data = data.left(index);
    }

    id_message = check_id_message(sl.at(0));
    switch(id_message)
    {
    case MESSAGE_GP_GGA:
    case MESSAGE_GL_GGA:
    case MESSAGE_GN_GGA:
        return parse_message_GGA(data);

    case MESSAGE_GP_GSA:
    case MESSAGE_GL_GSA:
    case MESSAGE_GN_GSA:
        return parse_message_GSA(data);

    case MESSAGE_GP_GSV:
    case MESSAGE_GL_GSV:
    case MESSAGE_GN_GSV:
        return parse_message_GSV(data);

    case MESSAGE_GP_RMC:
    case MESSAGE_GL_RMC:
    case MESSAGE_GN_RMC:
        return parse_message_RMC(data);

    case MESSAGE_GP_VTG:
    case MESSAGE_GL_VTG:
    case MESSAGE_GN_VTG:
        return parse_message_VTG(data);

    case MESSAGE_GP_GLL:
    case MESSAGE_GL_GLL:
    case MESSAGE_GN_GLL:
        return parse_message_GLL(data);

    case MESSAGE_GP_ZDA:
    case MESSAGE_GL_ZDA:
    case MESSAGE_GN_ZDA:
        return parse_message_ZDA(data);

    case MESSAGE_PIREA:
        return parse_message_PIREA(data);

    case MESSAGE_PIRFV:
        return parse_message_PIRFV(data);

    case MESSAGE_PIRGK:
        return parse_message_PIRGK(data);

    case MESSAGE_PIRRA:
        return parse_message_PIRRA(data);

    default:
        emit error(QString("unknown message %1").arg(id_message));
        return E_ERROR_UNKNOWN_MESSAGE;
        break;
    }

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::calc_time_UTC(const QString &data,
                                   int *hour,
                                   int *min,
                                   float *sec)
{
    bool ok = false;
    uint temp;
    float f_temp = 0;

    emit debug(QString("calc_time_UTC: data [%1]").arg(data));

    float time_utc = data.toFloat(&ok);
    if(!ok)
        return E_ERROR_FORMAT_MESSAGE;

    emit debug(QString("time_utc %1").arg(time_utc));

    ok = string_to_int(data.mid(0, 2), &temp);
    if(ok)
    {
        if(temp < 24) *hour = temp;
    }
    ok = string_to_int(data.mid(2, 2), &temp);
    if(ok)
    {
        if(temp < 59) *min = temp;
    }
    ok = string_to_float(data.mid(4, 5), &f_temp);
    if(ok)
    {
        *sec = f_temp;
    }

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::calc_latitude(const QString &data,
                                   int *grad,
                                   float *min)
{
    bool ok = false;

    emit debug(QString("calc_latitude: data [%1]").arg(data));

    if(data.isEmpty())
    {
        emit debug("calc_latitude: нет данных");
        return E_NO_ERROR;
    }

    if(data.length() < 9)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    // «GGMM.MM» — широта. 2 цифры градусов(«GG»), 2 цифры целых минут, точка и дробная часть минут переменной длины. Лидирующие нули не опускаются.
    ok = string_to_int(data.mid(0, 2), grad);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(data.mid(2), min);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::calc_longitude(const QString &data,
                                    int *grad,
                                    float *min)
{
    bool ok = false;

    emit debug(QString("calc_longitude: data [%1]").arg(data));

    if(data.isEmpty())
    {
        emit debug("calc_latitude: нет данных");
        return E_NO_ERROR;
    }

    if(data.length() < 9)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    // «gggmm.mm» — долгота. 3 цифры градусов(«ggg»), 2 цифры целых минут, точка и дробная часть минут переменной длины. Лидирующие нули не опускаются.
    ok = string_to_int(data.mid(0, 3), grad);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(data.mid(3), min);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void Proto_NMEA_0183::print_variable(void)
{
    emit info(QString("time_observation_hour %1").arg(time_observation_hour));
    emit info(QString("time_observation_min %1").arg(time_observation_min));
    emit info(QString("time_observation_sec %1").arg(time_observation_sec));
    emit info(QString("latitude_grad %1").arg(latitude_grad));
    emit info(QString("day_utc %1").arg(day_utc));
    emit info(QString("month_utc %1").arg(month_utc));
    emit info(QString("year_utc %1").arg(year_utc));
    emit info(QString("local_hour %1").arg(local_hour));
    emit info(QString("local_min %1").arg(local_min));
    emit info(QString("latitude_min %1").arg(latitude_min));
    emit info(QString("latitude_dir %1").arg(latitude_dir));
    emit info(QString("longitude_dir %1").arg(longitude_dir));
    emit info(QString("quality %1").arg(quality));
    emit info(QString("nka %1").arg(nka));
    emit info(QString("horizont_faktor %1").arg(horizont_faktor));
    emit info(QString("f_height %1").arg(f_height));
    emit info(QString("f_geoid %1").arg(f_geoid));
    emit info(QString("time_in_sec %1").arg(time_in_sec));
    emit info(QString("id_station %1").arg(id_station));
    emit info("---");
    for(int n=1; n<13; n++)
        emit info(QString("nka_%1 %2").arg(n).arg(m_nka[n]));
    emit info("---");
    emit info(QString("type_management %1").arg(type_management));
    emit info(QString("state_work %1").arg(state_work));
    emit info(QString("state %1").arg(state));
    emit info(QString("speed_in_uzlah %1").arg(speed_in_uzlah));
    emit info(QString("speed_in_km %1").arg(speed_in_km));
    emit info(QString("kurs_in_grad %1").arg(kurs_in_grad));
    emit info(QString("true_kurs_in_grad %1").arg(true_kurs_in_grad));
    emit info(QString("magnet_kurs_in_grad %1").arg(magnet_kurs_in_grad));
    emit info(QString("data_str %1").arg(data_str));
    emit info(QString("magnet_sklon_in_grad %1").arg(magnet_sklon_in_grad));
    emit info(QString("reg_location %1").arg(reg_location));
    emit info(QString("X %1").arg(X));
    emit info(QString("Y %1").arg(Y));
    emit info(QString("Z %1").arg(Z));
    emit info(QString("V %1").arg(V));
    emit info(QString("K %1").arg(K));
}
//--------------------------------------------------------------------------------
bool Proto_NMEA_0183::string_to_float(const QString &data, float *value)
{
    bool ok = true;
    if(data.isEmpty())
        return true;
    *value = data.toFloat(&ok);

    return ok;
}
//--------------------------------------------------------------------------------
bool Proto_NMEA_0183::string_to_int(const QString &data, int *value)
{
    bool ok = true;
    if(data.isEmpty())
        return true;
    *value = data.toInt(&ok);

    return ok;
}
//--------------------------------------------------------------------------------
bool Proto_NMEA_0183::string_to_int(const QString &data, uint *value)
{
    bool ok = true;
    if(data.isEmpty())
        return true;
    *value = data.toInt(&ok);

    return ok;
}
//--------------------------------------------------------------------------------
bool Proto_NMEA_0183::string_to_int(const QString &data, unsigned char *value)
{
    int temp_value;
    bool ok = true;
    if(data.isEmpty())
        return true;
    temp_value = data.toInt(&ok);
    if(ok)
        *value = temp_value;

    return ok;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_GGA(const QString &data)
{
    bool ok = false;
    QString temp;
    QStringList sl;
    int err = E_NO_ERROR;

    emit debug("parse_message_GGA");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    err = calc_time_UTC(sl.at(1),
                        &time_observation_hour,
                        &time_observation_min,
                        &time_observation_sec);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    err = calc_latitude(sl.at(2),
                        &latitude_grad,
                        &latitude_min);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    latitude_dir = sl.at(3);

    err = calc_longitude(sl.at(4),
                         &longitude_grad,
                         &longitude_min);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    longitude_dir = sl.at(5);

    ok = string_to_int(sl.at(6), &quality);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_int(sl.at(7), &nka);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(8), &horizont_faktor);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(9), &f_height);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(11), &f_geoid);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(13), &time_in_sec);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    temp = sl[14].remove('*');
    ok = string_to_int(temp, &id_station);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    // print_variable();

    emit output_latitude_string(get_latitude_string());
    emit output_longitude_string(get_longitude_string());
    emit output_observation(get_observation_string());

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_GSA(const QString &data)
{
    QStringList sl;
    int n = 0;
    bool ok = false;

    emit debug("parse_message_GSA");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    type_management = sl.at(1);

    ok = string_to_int(sl.at(2), &state_work);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    for(n=3; n<15; n++)
    {
        ok = string_to_int(sl.at(n), &m_nka[n-3]);
        if(!ok)
        {
            return E_ERROR_FORMAT_MESSAGE;
        }
    }

    ok = string_to_float(sl.at(15), &PDOP);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(16), &HDOP);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(17), &VDOP);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    // print_variable();

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_GSV(const QString &data)
{
    Q_UNUSED(data);

    emit debug("parse_message_GSV");

    // QStringList sl;

    // sl = data.split(",");
    // emit debug(QString("count %1").arg(sl.count()));

    // ???
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_RMC(const QString &data)
{
    bool ok = false;
    QStringList sl;
    int err = E_NO_ERROR;

    emit debug("parse_message_RMC");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }
    emit debug(QString("found %1 elements").arg(sl.count()));

    err = calc_time_UTC(sl.at(1),
                        &time_observation_hour,
                        &time_observation_min,
                        &time_observation_sec);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    state = sl.at(2);
    if(state == "V")
    {
        emit error("недостоверно");
        return E_NO_ERROR;
    }

    err = calc_latitude(sl.at(3),
                        &latitude_grad,
                        &latitude_min);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    latitude_dir = sl.at(4);

    err = calc_longitude(sl.at(5),
                         &longitude_grad,
                         &longitude_min);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    longitude_dir = sl.at(6);

    ok = string_to_float(sl.at(7), &speed_in_uzlah);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(8), &kurs_in_grad);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    data_str = sl.at(9);

    ok = string_to_float(sl.at(10), &magnet_sklon_in_grad);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    // temp = sl.at(11);

    // temp = sl.at(12);

    // print_variable();

    emit output_latitude_string(get_latitude_string());
    emit output_longitude_string(get_longitude_string());
    emit output_observation(get_observation_string());

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_VTG(const QString &data)
{
    //QString temp;
    bool ok = false;
    QStringList sl;

    emit debug("parse_message_VTG");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    ok = string_to_float(sl.at(1), &true_kurs_in_grad);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(3), &magnet_kurs_in_grad);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(5), &speed_in_uzlah);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(7), &speed_in_km);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    // temp = sl.at(9);

    // print_variable();

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_GLL(const QString &data)
{
    //QString temp;
    QStringList sl;
    int err = E_NO_ERROR;

    emit debug("parse_message_GLL");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    state = sl.at(6);
    if(state == "V")
    {
        emit error("недостоверно");
        return E_NO_ERROR;
    }

    err = calc_latitude(sl.at(1),
                        &latitude_grad,
                        &latitude_min);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    latitude_dir = sl.at(2);

    err = calc_longitude(sl.at(3),
                         &longitude_grad,
                         &longitude_min);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    longitude_dir = sl.at(4);

    err = calc_time_UTC(sl.at(5),
                        &time_observation_hour,
                        &time_observation_min,
                        &time_observation_sec);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    // temp = sl.at(7);

    // print_variable();

    emit output_latitude_string(get_latitude_string());
    emit output_longitude_string(get_longitude_string());
    emit output_observation(get_observation_string());

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_ZDA(QString &data)
{
    bool ok = false;
    QStringList sl;
    int err = E_NO_ERROR;

    emit debug("parse_message_ZDA");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    err = calc_time_UTC(sl.at(1),
                        &time_observation_hour,
                        &time_observation_min,
                        &time_observation_sec);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    ok = string_to_int(sl.at(2), &day_utc);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }
    if(day_utc > 31)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_int(sl.at(3), &month_utc);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }
    if(month_utc > 12)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_int(sl.at(4), &year_utc);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_int(sl.at(5), &local_hour);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_int(sl.at(6), &local_min);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    // print_variable();

    emit output_observation(get_observation_string());

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_PIREA(const QString &data)
{
    QStringList sl;

    emit debug("parse_message_PIREA");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_PIRFV(const QString &data)
{
    QStringList sl;

    emit debug("parse_message_PIRFV");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_PIRGK(const QString &data)
{
    bool ok = false;
    QStringList sl;
    int err = E_NO_ERROR;

    emit debug("parse_message_PIRGK");

    sl = data.split(",");
    if(sl.count() == 0)
    {
        emit debug("sl.count() == 0");
        return E_ERROR_UNKNOWN_MESSAGE;
    }

    err = calc_time_UTC(sl.at(1),
                        &time_observation_hour,
                        &time_observation_min,
                        &time_observation_sec);
    if(err != E_NO_ERROR)
    {
        return err;
    }

    ok = string_to_float(sl.at(3), &X);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(4), &Y);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(5), &Z);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(6), &V);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(sl.at(7), &K);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    data_str = sl.at(8);

    ok = string_to_float(data.at(9), &HDOP);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_float(data.at(10), &VDOP);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    ok = string_to_int(sl.at(11), &nka);
    if(!ok)
    {
        return E_ERROR_FORMAT_MESSAGE;
    }

    // print_variable();

    emit output_observation(get_observation_string());

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::parse_message_PIRRA(const QString &data)
{
    QStringList sl;

    emit debug("parse_message_PIRRA");

    sl = data.split(",");
    emit debug(QString("parse_message_PIRRA: count %1").arg(sl.count()));

    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
void Proto_NMEA_0183::run_self_tests(void)
{
    test_GGA();
    test_GSA();
    test_GSV();
    test_RMC();
    test_VTG();
    test_GLL();
    test_ZDA();

    test_PIREA();
    test_PIRFV();
    test_PIRGK();
    test_PIRRA();
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_GGA(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("GL");
    message.append("GGA,");
    message.append("123456.12,");  // время обсервации UTC (часы, минуты, целая и дробная часть секунд)
    message.append("1234.1234,");  // широта  ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("N,");          // север/юг
    message.append("12345.1234,"); // долгота ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("E,");          // восток/запад
    message.append("0,");          // показатель качества обсервации 0-1-2
    message.append("12,");         // число НКА в решении
    message.append("0.1,");        // величина горизонтального геометрического фактора
    message.append("2.3,");        // высота над средним уровнем моря
    message.append("M,");          // единица измерения высоты - метры
    message.append("0.1,");        // превышение геоида на эллепсоидом WGS-84
    message.append("M,");          // единица измерения - метры
    message.append("0.1,");        // возраст дифференциальных поправок (время в секундах)
    message.append("1234");        // идентификатор дифферинциальной станции 0000-1024

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_GGA: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_GSA(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("GL");
    message.append("GSA,");
    message.append("A,");      // тип управления   A-M
    message.append("2,");      // режим работы     1-2-3
    message.append("1,2,3,4,5,6,7,8,9,10,11,12,"); // номера НКА в решении
    message.append("1.2,");    // пространственный геометрический фактор ухудшения точности (PDOP)
    message.append("3.4,");    // геометрический фактор ухудшения точности в плане (HDOP)
    message.append("5.6");     // геометрический фактор ухудшения точности в плане (VDOP)

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_GSA: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_GSV(void)
{
    emit debug("test_GSV: пока не сделано");
    return E_NO_ERROR;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_RMC(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("GL");
    message.append("RMC,");
    message.append("112233.44,");  // время UTC (часы, минуты, целая и дробная часть секунд)
    message.append("A,");          // статус     A-D-V
    message.append("1234.1234,");  // широта  ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("N,");          // север/юг
    message.append("12345.1234,"); // долгота ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("E,");          // восток/запад
    message.append("1.2,");        // наземная скорость, в узлах
    message.append("3.4,");        // наземный курс, в градусах
    message.append("01022012,");   // дата день-месяц-год
    message.append("5.6,");        // магнитное склонение в градусах
    message.append("E,");          // восток/запад
    message.append("A");           // режим местоопределения     A-D-E-M-S-N

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_RMC: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_VTG(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("GL");
    message.append("VTG,");
    message.append("1.2,");    // наземный курс в градусах
    message.append("T,");      // истинный T
    message.append("3.4,");    // наземный курс в градусах
    message.append("M,");      // магнитный M
    message.append("5.6,");    // наземная скорость
    message.append("Т,");      // в узлах
    message.append("7.8,");    // наземная скорость
    message.append("K,");      // в км/ч
    message.append("A");       // режим местоопределения   A-D-E-M-S-N

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_VTG: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_GLL(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("GL");
    message.append("GLL,");
    message.append("1234.1234,");  // широта  ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("N,");          // север/юг
    message.append("12345.1234,"); // долгота ХХ - градусы, ХХ.ХХХХ - целая и дробная часть минут
    message.append("E,");          // восток/запад
    message.append("123456.12,");  // время обсервации UTC (часы, минуты, целая и дробная часть секунд)
    message.append("A,");          // статус A-D-V
    message.append("A");           // режим местоопределения     A-D-E-M-S-N

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_GLL: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_ZDA(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("GL");
    message.append("ZDA,");
    message.append("123456.12,");  // время обсервации UTC (часы, минуты, целая и дробная часть секунд)
    message.append("01,");         // день UTC  (от 01 до 31)
    message.append("02,");         // месяц UTC (от 01 до 12)
    message.append("2012,");       // год UTC
    message.append("01,");         // часы локальной временной зоны (от 00 до +- 13)
    message.append("02");          // минуты локальной временной зоны (от 00 до 59)

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_ZDA: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_PIREA(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("PIREA,");
    message.append("5");  // результат теста (код ошибки)

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_PIREA: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_PIRFV(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("PIRFV,");
    message.append("12.34");  // номер версии встроенного ПО приемника

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_PIRFV: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_PIRGK(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("PIRGK,");
    message.append("123456.12,");      // время UTC (часы, минуты, целая и дробная часть секунд)
    message.append("1,");              // индикатор качества GNSS 0-1
    message.append("1.2,");            // координата X, м
    message.append("500000.1000000,"); // координата Y, м
    message.append("3.4,");            // высота, м
    message.append("5.6,");            // скорость, м/с
    message.append("7.8,");            // курс, в градусах
    message.append("01022012,");       // дата день-месяц-год
    message.append("3.4,");            // геометрический фактор ухудшения точности в плане (HDOP)
    message.append("5.6,");            // геометрический фактор ухудшения точности в плане (VDOP)
    message.append("7");               // количество НКА в решении

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_PIRGK: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::test_PIRRA(void)
{
    QString data;
    QString message;
    QString checksum;
    QString temp;
    int res;

    message.clear();
    message.append("PIRRA,");
    message.append("1,");
    message.append("2,");
    message.append("3,");
    message.append("4,");
    message.append("5");

    checksum = get_checksum(message);

    data.clear();
    //data.append("$");
    data.append(message);
    data.append("*");
    data.append(checksum);
    data.append((char)0x0D);
    data.append((char)0x0A);

    res = check_message(data);
    temp = QString("test_PIRRA: %1").arg((res == E_NO_ERROR) ? "пройден" : "НЕ пройден");
    emit debug(temp);
    return res;
}
//--------------------------------------------------------------------------------
void Proto_NMEA_0183::test(void)
{
#if 1
    union U_USHORT mask;
    QString temp;

    mask.value = 0;
    mask.bits.GxGGA = 1;
    mask.bits.GxRMC = 1;
    mask.bits.GxVTG = 1;
    mask.bits.GxGLL = 1;
    mask.bits.GxZDA = 1;
    mask.bits.PIREA = 1;
    mask.bits.PIRFV = 1;
    mask.bits.PIRGK = 1;
    mask.bits.PIRRA = 1;
    set_port(&temp, 0, 9600, 4, mask.value);
    emit debug(temp);

    temp.clear();
    change_param_coord_and_time(&temp, 0, 0, 0);
    emit debug(QString("change_param_coord_and_time: %1").arg(temp));

    temp.clear();
    select_satellite(&temp, 1, 2);
    emit debug(QString("select_satellite: %1").arg(temp));

    run_self_tests();
#endif
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
float Proto_NMEA_0183::get_latitude_grad(void)
{
    return latitude_grad;
}
//--------------------------------------------------------------------------------
float Proto_NMEA_0183::get_latitude_min(void)
{
    return latitude_min;
}
//--------------------------------------------------------------------------------
QString Proto_NMEA_0183::get_latitude_dir(void)
{
    return latitude_dir;
}
//--------------------------------------------------------------------------------
QString Proto_NMEA_0183::get_latitude_string(void)
{
    QString latitude_dir;
    QString temp;

    QChar dg(0x00B0);
    latitude_dir = get_latitude_dir().toUpper();

    if(get_latitude_dir() == "N") temp = "с.ш.";
    if(get_latitude_dir() == "S") temp = "ю.ш.";
    if(get_latitude_dir() == "W") temp = "з.д.";
    if(get_latitude_dir() == "E") temp = "в.д.";

    return QString("%1 %2 %3' %4")
            .arg((int)get_latitude_grad(),  2, 10, QLatin1Char('0'))
            .arg(dg)
            .arg((int)get_latitude_min(),  2, 10, QLatin1Char('0'))
            .arg(temp);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
float Proto_NMEA_0183::get_longitude_grad(void)
{
    return longitude_grad;
}
//--------------------------------------------------------------------------------
float Proto_NMEA_0183::get_longitude_min(void)
{
    return longitude_min;
}
//--------------------------------------------------------------------------------
QString Proto_NMEA_0183::get_longitude_dir(void)
{
    return longitude_dir;
}
//--------------------------------------------------------------------------------
QString Proto_NMEA_0183::get_longitude_string(void)
{
    QString longitude_dir;
    QString temp;

    QChar dg(0x00B0);

    longitude_dir = get_latitude_dir().toUpper();

    if(get_longitude_dir() == "N") temp = "с.ш.";
    if(get_longitude_dir() == "S") temp = "ю.ш.";
    if(get_longitude_dir() == "W") temp = "з.д.";
    if(get_longitude_dir() == "E") temp = "в.д.";

    return QString("%1 %2 %3' %4")
            .arg((int)get_longitude_grad(),  2, 10, QLatin1Char('0'))
            .arg(dg)
            .arg((int)get_longitude_min(),  2, 10, QLatin1Char('0'))
            .arg(temp);
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::get_observation_hour(void)
{
    return time_observation_hour;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::get_observation_min(void)
{
    return time_observation_min;
}
//--------------------------------------------------------------------------------
int Proto_NMEA_0183::get_observation_sec(void)
{
    return time_observation_sec;
}
//--------------------------------------------------------------------------------
QString Proto_NMEA_0183::get_observation_string(void)
{
    return QString("%1:%2:%3")
            .arg(get_observation_hour(), 2, 10, QLatin1Char('0'))
            .arg(get_observation_min(),  2, 10, QLatin1Char('0'))
            .arg(get_observation_sec(),  2, 10, QLatin1Char('0'));
}
//--------------------------------------------------------------------------------
//--------------------------------------------------------------------------------
float Proto_NMEA_0183::get_height(void)
{
    return f_height;
}
//--------------------------------------------------------------------------------
void Proto_NMEA_0183::print_error(QByteArray data, int err)
{
    if(err != E_NO_ERROR)
    {
        emit debug(data);
    }
    switch (err)
    {
    case E_NO_ERROR:
        emit info("NO_ERROR");
        break;

    case E_ERROR_FORMAT_MESSAGE:
        emit error("ERROR_FORMAT_MESSAGE");
        break;

    case E_ERROR_UNKNOWN_MESSAGE:
        emit error("ERROR_UNKNOWN_MESSAGE");
        break;

    case E_ERROR_UNKNOWN_PORT:
        emit error("ERROR_UNKNOWN_PORT");
        break;

    case E_ERROR_UNKNOWN_BAULDRATE:
        emit error("ERROR_UNKNOWN_BAULDRATE");
        break;

    case E_ERROR_TYPE_PROTO:
        emit error("ERROR_TYPE_PROTO");
        break;

    case E_ERROR_SYSTEM_COORD:
        emit error("ERROR_SYSTEM_COORD");
        break;

    case E_ERROR_HOUR:
        emit error("ERROR_HOUR");
        break;

    case E_ERROR_MIN:
        emit error("ERROR_MIN");
        break;

    case E_ERROR_SEC:
        emit error("ERROR_SEC");
        break;

    case E_ERROR_CHECKSUM:
        emit error("ERROR_CHECKSUM");
        break;

    default:
        emit error(QString("unknown error %1").arg(err));
        break;
    }
}
//--------------------------------------------------------------------------------
void Proto_NMEA_0183::test_cheksum(void)
{
    QString temp = get_checksum("GNGSA,A,1,,,,,,,,,,,,,,,");
    if(temp == "00")
        emit debug("checksum is ok!");
    else
        emit error(QString("checksum 0x%1 is bad").arg(temp));
}
//--------------------------------------------------------------------------------
