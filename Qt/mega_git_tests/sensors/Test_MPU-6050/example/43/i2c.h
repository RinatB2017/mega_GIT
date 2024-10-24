//===============================================================================================
//    		 Подключение к выводам МК, задержки
//===============================================================================================
#define SDA			4		// ножка данных, SDA
#define SCL			5		// ножка тактирования, SCL
#define I2C_PORT	PORTC	// Тут указать порт, например PORTB или PORTC
#define I2C_PIN		PINC	// Тут указать порт, например PINB или PINC
#define I2C_DDR		DDRC	// Тут указать порт, например DDRB или DDRC
// все задержки при работе шины i2c, изменяйте это число только если знаете, что делаете!!!
#define I2C_DELAY	_delay_us(4);	 // по умолчанию задержки 10 микросекунд (частота работы 1/10us = 100кГц)


//===============================================================
//               Служебные переменные и библиотеки
//===============================================================
#include <util/delay.h>
volatile unsigned char i2c_frame_error=0; // если > 0, значит произошла фрейм-ошибка (после STOP-команды, линии SDA и SCL не перешли в состояние HIGH, не подтянулись к + питания)


//*************************************************************
//ПРОТОТИПЫ ФУНКЦИЙ
void i2c_init (void);									// настраивает выводы микроконтроллера
void i2c_start_cond (void);								// генерирует условие старта
void i2c_restart_cond (void);							// генерирует условие повторного старта
void i2c_stop_cond (void);								// генерирует условие стоп
unsigned char i2c_send_byte (unsigned char data);		// возвращает ACK, если = 1 значит данные переданы неуспешно, если = 0 значит данные переданы успешно, data это передаваемый байт
unsigned char i2c_get_byte (unsigned char last_byte);	// возвращает считанный байт, last_byte = 1 означает что считываем последний байт, last_byte = 0 означает что будем считывать ещё байт


//===============================================================
//                   Генерация условия стоп
//===============================================================
void i2c_stop_cond (void)	{
	I2C_DDR |= _BV(SCL); // притянуть SCL (лог.0)
	I2C_DELAY
	I2C_DDR |= _BV(SDA); // притянуть SDA (лог.0)
	I2C_DELAY
		
	I2C_DDR &= ~_BV(SCL); // отпустить SCL (лог.1)
	I2C_DELAY
	I2C_DDR &= ~_BV(SDA); // отпустить SDA (лог.1)
	I2C_DELAY
	
	// проверка фрейм-ошибки
	i2c_frame_error=0;									// сброс счётчика фрейм-ошибок
	if ((I2C_PIN & _BV(SDA)) == 0) i2c_frame_error++;	// проберяем, чтобы на ноге SDA была лог.1, иначе выдаём ошибку фрейма
	if ((I2C_PIN & _BV(SCL)) == 0) i2c_frame_error++;	// проберяем, чтобы на ноге SCL была лог.1, иначе выдаём ошибку фрейма
	I2C_DELAY
	I2C_DELAY
	I2C_DELAY
	I2C_DELAY
}


//===============================================================
//                    инициализация шины I2C
//===============================================================
void i2c_init (void) {
	////////для I2C/////////
	I2C_DDR &= ~_BV(SDA);		// когда DDR=0, вывод SDA висит в воздухе, когда DDR=1 то SDA притянута к земле (регистром DDR мы и будем манипулировать)
	I2C_DDR &= ~_BV(SCL);		// когда DDR=0, вывод SCL висит в воздухе, когда DDR=1 то SCL притянута к земле (регистром DDR мы и будем манипулировать)
	I2C_PORT &= ~_BV(SDA);      // PORT=0 означает, что когда DDR=1 то SDA притянута к земле, когда DDR=0, то SDA висит в воздухе
	I2C_PORT &= ~_BV(SCL);      // PORT=0 означает, что когда DDR=1 то SCL притянута к земле, когда DDR=0, то SCL висит в воздухе
	i2c_stop_cond();   // стоп шины на случай сбоев
	i2c_stop_cond();   // стоп шины на случай сбоев
}



//===============================================================
//                    Генерация условия старт
//===============================================================
void i2c_start_cond (void)	{	
	I2C_DDR |= _BV(SDA); // притянуть SDA (лог.0)
	I2C_DELAY
	I2C_DDR |= _BV(SCL); // притянуть SCL (лог.0)
	I2C_DELAY
}



//===============================================================
//      Генерация условия рестарт
//===============================================================
void i2c_restart_cond (void)	{	
	I2C_DDR &= ~_BV(SDA); // отпустить SDA (лог.1)
	I2C_DELAY
	I2C_DDR &= ~_BV(SCL); // отпустить SCL (лог.1)
	I2C_DELAY
	
	I2C_DDR |= _BV(SDA); // притянуть SDA (лог.0)
	I2C_DELAY
	I2C_DDR |= _BV(SCL); // притянуть SCL (лог.0)
	I2C_DELAY
}


                   
//===============================================================
//                    Отправка байта по I2С
//===============================================================
unsigned char i2c_send_byte (unsigned char data)	{	
 unsigned char i;
 unsigned char ack=1;			//АСК, если АСК=1 – произошла ошибка
	for (i=0;i<8;i++)
	{
		if ((data&0x80)==0x00) I2C_DDR |= _BV(SDA);	// Выставить бит на SDA (лог.0)
		else I2C_DDR &= ~_BV(SDA); // лог.1
		I2C_DELAY
		I2C_DDR &= ~_BV(SCL);	// Записать его импульсом на SCL       // отпустить SCL (лог.1)
		I2C_DELAY
		I2C_DDR |= _BV(SCL); // притянуть SCL (лог.0)
		data=data<<1;
	}
	I2C_DDR &= ~_BV(SDA); // отпустить SDA (лог.1), чтобы ведомое устройство смогло сгенерировать ACK
	I2C_DELAY
	I2C_DDR &= ~_BV(SCL); // отпустить SCL (лог.1), чтобы ведомое устройство передало ACK
	I2C_DELAY
	if ((I2C_PIN&_BV(SDA))==_BV(SDA)) ack=1; else ack=0;	// Считать ACK

	I2C_DDR |= _BV(SCL); // притянуть SCL (лог.0)  // приём ACK завершён
	
	return ack; // вернуть ACK (0) или NACK (1)
}






//===============================================================
//                   Считывание байта по I2C
//===============================================================
unsigned char i2c_get_byte (unsigned char last_byte)	{	
 unsigned char i, res=0;
	I2C_DDR &= ~_BV(SDA); // отпустить SDA (лог.1)

	for (i=0;i<8;i++)
	{
		res=res<<1;
		I2C_DDR &= ~_BV(SCL); // отпустить SCL (лог.1)		//Импульс на SCL
		I2C_DELAY
		if ((I2C_PIN&_BV(SDA))==_BV(SDA)) res=res|0x01; // Чтение SDA в переменную
		I2C_DDR |= _BV(SCL); // притянуть SCL (лог.0)
		I2C_DELAY
	}

	if (last_byte==0) I2C_DDR |= _BV(SDA); // притянуть SDA (лог.0)		// Подтверждение, ACK, будем считывать ещё один байт
	else I2C_DDR &= ~_BV(SDA); // отпустить SDA (лог.1)					// Без подтверждения, NACK, это последний считанный байт
	I2C_DELAY
	I2C_DDR &= ~_BV(SCL); // отпустить SCL (лог.1)
	I2C_DELAY
	I2C_DDR |= _BV(SCL); // притянуть SCL (лог.0)
	I2C_DELAY
	I2C_DDR &= ~_BV(SDA); // отпустить SDA (лог.1)

	return res; // вернуть считанное значение
}









