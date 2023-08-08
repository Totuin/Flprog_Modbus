//Подключаем необходимую библиотеку
#include "flprogModbusRTU.h"

//FLProgUart RtuDevice(1);
//ModbusSlaveRTU SlaveRTU1(&RtuDevice);

//Создаем объект непосредстредственно Модбас слейва на первом уарте

ModbusSlaveRTU SlaveRTU1(1);

int _DiscreteInputAddreses[] = {8, 9, 10, 11, 12, 14, 15, 16, 17, 18};

int counter = -10;
bool blinkVar = 0;
uint32_t startTimer;
int lastError;

void setup()
{

  //SlaveRTU1.setUart(&RtuDevice);


/*
    Задаем последовательность байтов для хранения различных типов данных
    Возможные знаыения:

    Для Integer:
    FLPROG_AB_ORDER
    FLPROG_BA_ORDER

    Для остальных типов:
    FLPROG_ABCD_ORDER
    FLPROG_CDAB_ORDER
    FLPROG_BADC_ORDER
    FLPROG_DCBA_ORDER

    Вызов данных функций не обязателен.
    По умолчанию заданны  значения  FLPROG_AB_ORDER  и FLPROG_ABCD_ORDER
    Вызов данных функций возможен в любое время если необходимо изменить эти значения в режиме выполнения программы
    SlaveRTU1.setLongOrder(FLPROG_DCBA_ORDER);
    SlaveRTU1.setFloatOrder(FLPROG_BADC_ORDER);
    SlaveRTU1.setUnsignedlongOrder(FLPROG_CDAB_ORDER);
    SlaveRTU1.setIntOrder(FLPROG_AB_ORDER);

    Если необходимо - задаём номер пина управления потоком приёма - передачи
     SlaveRTU1.setPinPeDe(13);


    Параметрирузируем порт:
    Вызовы не обязательны по умолчанию параметры порта 9600 8N1
    Параметры порта можно менять во время исполнения программы

    Задаём скорость порта допустимые константы:
    FLPROG_SPEED_300
    FLPROG_SPEED_600
    FLPROG_SPEED_1200
    FLPROG_SPEED_2400
    FLPROG_SPEED_4800
    FLPROG_SPEED_9600
    FLPROG_SPEED_14400
    FLPROG_SPEED_19200
    FLPROG_SPEED_28800
    FLPROG_SPEED_38400
    FLPROG_PEED_57600
    FLPROG_SPEED_115200

    SlaveRTU1.setPortSpeed(FLPROG_SPEED_14400);

  
  Задаем количество дата бит
  SlaveRTU1.setPortDataBits(8);

  //Задаем количество стоп бит
   SlaveRTU1.setPortStopBits(1);


    Задаем четность
    Значения 0 (NONE), 1 (EVEN), 2 (ODD)
 
  SlaveRTU1.setPortParity(0);
 */
  
  SlaveRTU1.begin(1);


// задаем области памяти
  SlaveRTU1.setDataTable(FLPROG_HOLDING_REGISTR, 10);
  SlaveRTU1.setDataTable(FLPROG_INPUT_REGISTR, 10);
  SlaveRTU1.setDataTable(FLPROG_COIL, 10, 5);
  SlaveRTU1.setDataTable(FLPROG_DISCRETE_INPUT, 10, _DiscreteInputAddreses);
  
//Инициализируем слейв (Первый параметр номера слейва)
  SlaveRTU1.begin(1);
}

void loop()
{
 //Цикл работы слейва
  SlaveRTU1.pool();

/*
    Играемся с регистрами

    Доступные функции:
       void saveLong(long value, byte table, int startAddres);
       void saveUnsignedLong(unsigned long value, byte table, int startAddres);
       void saveFloat(float value, byte table, int startAddres);
       void saveInteger(int value, byte table, int startAddres);
       void saveByte(byte value, byte table, int startAddres);
       void saveBool(bool value, byte table, int startAddres);
       byte readByte(byte table, int startAddres);
       int readInteger(byte table, int startAddres);
       float readFloat(byte table, int startAddres);
       long readLong(byte table, int startAddres);
       unsigned long readUnsignedLong(byte table, int startAddres);
       bool readBool(byte table, int startAddres);
  */



  SlaveRTU1.saveInteger((SlaveRTU1.readInteger(FLPROG_HOLDING_REGISTR, 0)), FLPROG_INPUT_REGISTR, 9);
  SlaveRTU1.saveLong((SlaveRTU1.readLong(FLPROG_HOLDING_REGISTR, 6)), FLPROG_INPUT_REGISTR, 5);
  SlaveRTU1.saveInteger((SlaveRTU1.readFloat(FLPROG_HOLDING_REGISTR, 4) * 100), FLPROG_INPUT_REGISTR, 0);
  SlaveRTU1.saveUnsignedLong((SlaveRTU1.readUnsignedLong(FLPROG_HOLDING_REGISTR, 8)), FLPROG_INPUT_REGISTR, 7);
  SlaveRTU1.saveBool(blinkVar, FLPROG_COIL, 5);
  SlaveRTU1.saveBool(blinkVar, FLPROG_DISCRETE_INPUT, 8);
  SlaveRTU1.saveBool((SlaveRTU1.readBool(FLPROG_COIL, 6)), FLPROG_DISCRETE_INPUT, 9);

  if (flprog::isTimer(startTimer, 1000))
  {
    counter++;
    if (counter > 10)
    {
      counter = -10;
    }
    blinkVar = !blinkVar;
    SlaveRTU1.saveInteger(counter, FLPROG_INPUT_REGISTR, 6);
    SlaveRTU1.saveInteger(counter, FLPROG_HOLDING_REGISTR, 1);
    SlaveRTU1.saveLong((counter*1000), FLPROG_INPUT_REGISTR, 1);
    SlaveRTU1.saveFloat((counter / 10.0), FLPROG_HOLDING_REGISTR, 2);
    SlaveRTU1.saveFloat((counter / 100.0), FLPROG_INPUT_REGISTR, 3);
    startTimer = millis();
  }
  // получение последней ошибки слейва
  lastError = SlaveRTU1.getLastError();
}