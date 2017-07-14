//--------------------------------------------------------------------------------
#ifndef PROTO_NMEA_0183_H
#define PROTO_NMEA_0183_H
//--------------------------------------------------------------------------------
#include <QObject>
#include <QString>
#include <QWidget>
//--------------------------------------------------------------------------------
#define GGA "GGA"   // время, положение и годность навигационного решения;
#define GSA "GSA"   // спутники в решении;
#define GSV "GSV"   // видимые спутники;
#define RNC "RMC"   // рекомендованный минимум навигационных данных;
#define VTG "VTG"   // наземный курс и скорость;
#define GLL "GLL"   // GLL - местоположение, время и годность навигационного решения;
#define ZDA "ZDA"   // время, дата и временная зона.
//---
enum {
    E_NO_ERROR    = 0,
    E_ERROR_FORMAT_MESSAGE,
    E_ERROR_UNKNOWN_MESSAGE,
    E_ERROR_UNKNOWN_PORT,
    E_ERROR_UNKNOWN_BAULDRATE,
    E_ERROR_TYPE_PROTO,
    E_ERROR_SYSTEM_COORD,
    E_ERROR_HOUR,
    E_ERROR_MIN,
    E_ERROR_SEC,
    E_ERROR_CHECKSUM
};
enum {
    MESSAGE_UNKNOWN = 0,
    MESSAGE_GP_GGA,
    MESSAGE_GL_GGA,
    MESSAGE_GN_GGA,

    MESSAGE_GP_GSA,
    MESSAGE_GL_GSA,
    MESSAGE_GN_GSA,

    MESSAGE_GP_GSV,
    MESSAGE_GL_GSV,
    MESSAGE_GN_GSV,

    MESSAGE_GP_RMC,
    MESSAGE_GL_RMC,
    MESSAGE_GN_RMC,

    MESSAGE_GP_VTG,
    MESSAGE_GL_VTG,
    MESSAGE_GN_VTG,

    MESSAGE_GP_GLL,
    MESSAGE_GL_GLL,
    MESSAGE_GN_GLL,

    MESSAGE_GP_ZDA,
    MESSAGE_GL_ZDA,
    MESSAGE_GN_ZDA,

    MESSAGE_PIREA,
    MESSAGE_PIRFV,
    MESSAGE_PIRGK,
    MESSAGE_PIRRA
};
//---
union USHORT
{
    unsigned short value;
    struct
    {
        unsigned GxGGA:1;
        unsigned GxGSA:1;
        unsigned GxGSV:1;
        unsigned GxRMC:1;
        unsigned GxVTG:1;
        unsigned GxGLL:1;
        unsigned GxZDA:1;
        unsigned reserv:1;
        unsigned PIREA:1;
        unsigned PIRFV:1;
        unsigned PIRGK:1;
        unsigned PIRRA:1;
        unsigned reserved:4;
    } bits;
};
//--------------------------------------------------------------------------------
class Proto_NMEA_0183 : public QWidget
{
    Q_OBJECT
public:
    explicit Proto_NMEA_0183(QWidget *parent = 0);
    QString get_checksum(const QString &data);
    int set_port(QString *result,
                 uchar number_port,
                 uint bauldrate,
                 uchar type_proto,
                 ushort mask);
    int check_message(const QString &data);
    void run_self_tests(void);
    void test(void);

    void print_error(int err);
    void test_cheksum(void);

    void print_variable(void);

    float get_latitude_grad(void);
    float get_latitude_min(void);
    QString get_latitude_dir(void);
    QString get_latitude_string(void);

    float get_longitude_grad(void);
    float get_longitude_min(void);
    QString get_longitude_dir(void);
    QString get_longitude_string(void);

    int get_observation_hour(void);
    int get_observation_min(void);
    int get_observation_sec(void);
    QString get_observation_string(void);

    float get_height(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);

    void output_latitude_string(const QString &data);
    void output_longitude_string(const QString &data);
    void output_observation(const QString &data);

    
private:
    QByteArray message;

    int time_observation_hour;
    int time_observation_min;
    float time_observation_sec;
    int latitude_grad;
    float latitude_min;
    int longitude_grad;
    float longitude_min;
    uint day_utc;
    uint month_utc;
    uint year_utc;
    uchar local_hour;
    uchar local_min;
    QString latitude_dir;
    QString longitude_dir;
    uchar quality;
    uchar nka;
    uchar m_nka[15];
    float horizont_faktor;
    float f_height;
    float f_geoid;
    float time_in_sec;
    int id_station;
    QString type_management;
    uchar state_work;
    float PDOP;
    float HDOP;
    float VDOP;
    QString state;
    float speed_in_uzlah;
    float speed_in_km;
    float kurs_in_grad;
    float true_kurs_in_grad;
    float magnet_kurs_in_grad;
    QString data_str;
    float magnet_sklon_in_grad;
    QString reg_location;
    float X;
    float Y;
    float Z;
    float V;
    float K;

    int parse_message_GGA(const QString &data);
    int parse_message_GSA(const QString &data);
    int parse_message_GSV(const QString &data);
    int parse_message_RMC(const QString &data);
    int parse_message_VTG(const QString &data);
    int parse_message_GLL(const QString &data);
    int parse_message_ZDA(const QString &data);

    int parse_message_PIREA(const QString &data);
    int parse_message_PIRFV(const QString &data);
    int parse_message_PIRGK(const QString &data);
    int parse_message_PIRRA(const QString &data);

    void init(void);
    int calc_time_UTC(const QString &data,
                      int *hour,
                      int *min,
                      float *sec);
    int calc_latitude(const QString &data,
                      int *grad,
                      float *min);
    int calc_longitude(const QString &data,
                       int *grad,
                       float *min);

    void test_GGA(void);
    void test_GSA(void);
    void test_GSV(void);
    void test_RMC(void);
    void test_VTG(void);
    void test_GLL(void);
    void test_ZDA(void);

    void test_PIREA(void);
    void test_PIRFV(void);
    void test_PIRGK(void);
    void test_PIRRA(void);

    int check_id_message(const QString &data);
    int change_param_coord_and_time(QString *result,
                                    uchar s_coord,
                                    uchar hour,
                                    uchar min);
    int select_satellite(QString *result,
                         uint satellite_grs,
                         uint satellite_glonass);

    bool string_to_float(const QString &data, float *value);
    bool string_to_int(const QString &data, int *value);
    bool string_to_int(const QString &data, uint *value);
    bool string_to_int(const QString &data, unsigned char *value);
};
//--------------------------------------------------------------------------------
#endif
