//--------------------------------------------------------------------------------
#ifndef PROTO_NMEA_0183_HPP
#define PROTO_NMEA_0183_HPP
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
//--------------------------------------------------------------------------------
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
union U_USHORT
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

    explicit Proto_NMEA_0183(QWidget *parent = nullptr);
    QString get_checksum(const QString &data);
    int set_port(QString *result,
                 uchar number_port,
                 uint bauldrate,
                 uchar type_proto,
                 ushort mask);
    int check_message(QString data);
    void run_self_tests(void);
    void test(void);

    void print_error(QByteArray data, int err);
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

    int test_GGA(void);
    int test_GSA(void);
    int test_GSV(void);
    int test_RMC(void);
    int test_VTG(void);
    int test_GLL(void);
    int test_ZDA(void);

    int test_PIREA(void);
    int test_PIRFV(void);
    int test_PIRGK(void);
    int test_PIRRA(void);

signals:
    void info(const QString &);
    void debug(const QString &);
    void error(const QString &);
    void trace(const QString &);

    void output_latitude_string(const QString &data);
    void output_longitude_string(const QString &data);
    void output_observation(const QString &data);

    
private:
    int time_observation_hour = 0;
    int time_observation_min = 0;
    float time_observation_sec = 0;
    int latitude_grad = 0;
    float latitude_min = 0;
    int longitude_grad = 0;
    float longitude_min = 0;
    uint day_utc = 0;
    uint month_utc = 0;
    uint year_utc = 0;
    uchar local_hour = 0;
    uchar local_min = 0;
    QString latitude_dir;
    QString longitude_dir;
    uchar quality = 0;
    uchar nka = 0;
    uchar m_nka[15] = { 0 };
    float horizont_faktor = 0;
    float f_height = 0;
    float f_geoid = 0;
    float time_in_sec = 0;
    int id_station = 0;
    QString type_management;
    uchar state_work = 0;
    float PDOP = 0;
    float HDOP = 0;
    float VDOP = 0;
    QString state;
    float speed_in_uzlah = 0;
    float speed_in_km = 0;
    float kurs_in_grad = 0;
    float true_kurs_in_grad = 0;
    float magnet_kurs_in_grad = 0;
    QString data_str;
    float magnet_sklon_in_grad = 0;
    QString reg_location;
    float X = 0;
    float Y = 0;
    float Z = 0;
    float V = 0;
    float K = 0;

    int parse_message_GGA(const QString &data);
    int parse_message_GSA(const QString &data);
    int parse_message_GSV(const QString &data);
    int parse_message_RMC(const QString &data);
    int parse_message_VTG(const QString &data);
    int parse_message_GLL(const QString &data);
    int parse_message_ZDA(QString &data);

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
