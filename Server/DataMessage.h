#ifndef DATAMESSAGE_H
#define DATAMESSAGE_H
//07.09.21

#ifdef POI
typedef unsigned char quint8;
typedef char qint8;
typedef unsigned short quint16;
typedef short qint16;
typedef unsigned int quint32;
typedef int qint32;
typedef unsigned long long quint64;
typedef long long qint64;
#include "common_includes.h"
#else
#include <QObject>
//#include <sys/time.h>
//#include "stdint.h"
#endif

enum MESSAGES_ID	{

    // id message RMO 1 - 49

    RMO_VOI_TRACK_SETTING_MESSAGE			= 1,					// Сообщение о проритетности трассы
    RMO_VOI_TRACK_DELETE_MESSAGE			= 2,					// Сообщение об удалении трассы от РМО
    RMO_VOI_TARGET_POSITION_MESSAGE       	= 3,					// Сообщение с параметрами ЦУ
    RMO_VOI_TARGET_DELETE_MESSAGE         	= 4,					// Сообщение с удалением ЦУ
    RMO_REGFILE_CLOSE						= 5,					// Закрытие файла регистрации
    RMO_VOI_DRIVE_TO_ROSITION_MESSAGE       = 6,                    // Сообщение с данными куда смотреть антенне

    RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE     = 9,                    // Сообщение с сектором биологической защиты для ВОИ
    RMO_POI_COMMAND_MESSAGE					= 10,					// Сообщение с командой управления
    RMO_RARM_FINISH_CURRENT_FILE            = 11,                   // Завершение записиь текущего файла регистрации//////
    RMO_RARM_CONNECTION_CONTROL_WITH_POI    = 12,                   // Команда форсированного отключения/подключения реальной первички к РаРМУ
    RMO_RARM_RECORDING_CONTROL              = 13,                   // Команда разрешения/запрета записи файла регистрации
    RMO_OTHER_FORGET_ALL_DATA               = 14,                   // Команда общего сброса данных прошлого режима работы(режима работы РМО)
    RMO_POI_SIMULATOR_PLAY_MESSAGE          = 15,                   // Команда запуска/остановки работы имитатора ПОИ
    RMO_POI_SIMULATOR_CHANGE_FILE_MESSAGE   = 16,                   // Команда смена файла для имитатора ПОИ

    // id message POI 50 - 99

    POI_VOI_DRIVE_ANSWER_HEX_MESSAGE		= 50,					// Сообщение с ответом от привода напрямую
    POI_VOI_FINISH_RAY_MESSAGE				= 51,					// Сообщение об окончании работы в луче
    POI_KTA_MESSAGE							= 52,					// Сообщение о КТА
    POI_POSITION_SENSORS_MESSAGE			= 53,					// Сообщение с положением антенны в пространстве
    POI_DSP_MESSAGE 						= 54,					// Сообщение о ДСП
    POI_STATE_MESSAGE						= 55,					// Сообщение с состоянием ПОИ
    NAV_MESSAGE								= 56,					// Сообщение с навигационными данными
    INCLINOMETR_MESSAGE						= 57,					// Сообщение от инклинометра
    MIF380_STATE_MESSAGE					= 58,					// Сообщение с состоянием МПЧ380
    M14EP1_STATE_MESSAGE					= 59,					// Сообщение с состоянием М14ЕП1 (верхний управляющий блок питания)
    M11PS2_STATE_MESSAGE					= 60,					// Сообщение с состоянием М11ПС2 (ПС)
    M12UV1_STATE_MESSAGE					= 61, 					// Сообщение с состоянием М12УВ1 (ППМ)
    M12HL1_STATE_MESSAGE					= 62,					// Сообщение с состоянием М12ХЛ1 (плата интерфейсов)
    M14EP3_STATE_MESSAGE					= 63,					// Сообщение с состоянием М14ЕП3 (нижний управляющий блок питания, главный)
    M14EK2_STATE_MESSAGE					= 64,					// Сообщение с состоянием М14ЕК2 (блок питания М12УВ1)

    // id message VOI 100 - 149
    PC_SYNC_MESSAGE                         = 150,                  // Сообщение синхронизации

    VOI_RMO_AIM_MESSAGE						= 100,					// Сообщение о трассе цели
    VOI_RMO_MARK_MESSAGE                  	= 101,					// Сообщение о КТА от ВОИ
    VOI_POI_START_RAYS_MESSAGE				= 102,					// Сообщение с лучами на следующие 200 мс от ВОИ на ПОИ
    VOI_RMO_RAY_SET_MESSAGE               	= 103,					// Сообщение с набором лучей на следующий такт
    VOI_RMO_CREATE_SPECIAL_SECTOR_MESSAGE	= 104,					// Сообщение о создании спец-сектора
    VOI_RMO_DELETE_SPECIAL_SECTOR_MESSAGE 	= 105,					// Сообщение об удалении спец-сектора
    VOI_RMO_TRACK_DELETE_MESSAGE			= 106,					// Сообщение об удалении трассы от вторички
    VOI_POI_DRIVE_UM_COMMAND_MESSAGE		= 107,					// Сообщение с командой угломестным приводам
    VOI_POI_DRIVE_AZ_COMMAND_MESSAGE        = 108,					// Сообщение с командой азимутальному приводу
    VOI_RMO_PRIORITY_SECTOR_MESSAGE         = 109,                  // Сообщение с подтверждеием/отменой высокого приоритета у сектора
    VOI_RMO_STATE_METEO_MESSAGE             = 110,                  // Сообщение о состоянии метеостанции

    RDDS_OUT_UNIT_POINT_OBJ                 = 140,                  // Сообщение UnitPointObj в RDDS
    RDDS_OUT_UNIT_POINT_OBJ_FOR_DIST        = 141,                  // Сообщение UnitPointObjForDist в RDDS
    RDDS_OUT_UNIT_DSP                       = 142,                  // Сообщение UnitDSP в RDDS
    RDDS_OUT_UNIT_POINT_OBJ_ANS_SIGNAL      = 143,                  // Сообщение UnitPointObjAnsSignal в RDDS
    RDDS_OUT_TRAJECTORY                     = 144,                  // Сообщение Trajectory в RDDS
    RDDS_OUT_PARAM_BEARING_FOR_ANI          = 145,                  // Сообщение ParamBearingForANI в RDDS
    RDDS_IN_TARGET_FOR_RLS                  = 149,                  // Сообщение TargetForRLS из RDDS

    // free id 150 - 239

    //id message Registration 240 - 249

    REG_CHANGE_FILE                         = 240,                  // Сообщение об изменении файла для воспроизведения
    REG_RREWIND_FILE                        = 241,                  // Сообщение для перемотки файла воспроизведения
    REG_PLAY_ON                             = 242,                  // Сообщение для запуска/остановки воспроизведения
    REG_CURRENT_PLAY_TIME                   = 243,                  // Сообщение о времени воспроизведения из файла

    // id message RARM 250 - 255

    RARM_SYSTEM_CONTROL_MESSAGE				= 253,					// Сообщение с состоянием подсистем
    ALL_SYSTEM_HANDSHAKE					= 255					// Сообщение о рукопожатии с ММИРом
};

enum SYSTEMS_ID    {
    POI_SYS_ID,						// ПОИ MASTER - все управление через него
    VOI_SYS_ID,
    RMO_SYS_ID,
    RARM_SYS_ID,
    RDDS_SYS_ID,
    POI_SYS_ID_INCOMMING_CONN,
    PROGRAM_SIMULATOR_SYS_ID,
    REGISTRATION_SYS_ID,            // tcpSender
    POI_SLAVE_SYS_ID, 				// ПОИ SLAVE - только выдача КТА на РМО

    POI_SIMULATOR,                  // Имитатор первички
    PROCESS_CONTROLLER,             // Программа контроля и синхронизации всех модулей
};

typedef struct timeval64   {								// Время
    quint64 secs;                        // Cекунды
    quint64 usecs;                       // Микросекунды
}timeval64;

// ID: POI_KTA_MESSAGE
struct POIKtaMessage {								// Сообщение с обнаруженной КТА
    timeval64 sTimeMeasurement;						// Время измерения координат
    float Az_ant;									// Азимут антенны
    float Um_ant;									// Угол места антенны
    float D;										// Дальность КТА
    float Az;										// Азимут КТА
    float Um;										// Угол места КТА
    float Vr;										// Радиальная скорость КТА
    float Amp;										// Амплитуда КТА
    quint8 num_work_point;							// Номер рабочей точки
    quint16 info;
    uint8_t Reserved[1];
    float DebugKtaData[16];							// Отладочная информация
};

// ID: VOI_POI_START_RAYS_MESSAGE
// Сообщение с набором лучей на следующий такт для ПОИ
struct RayPrms { // Структура параметров работы в луче
    unsigned az: 10; // Номер луча по азимуту (луч 125 = 0гр, шаг = 90гр/250 = 0.36гр)
    unsigned um: 10; // Номер луча по углу места (луч 125 = 0гр, шаг = 90гр/250 = 0.36гр)
    unsigned mode: 8; // Номер запуска (от 0 до 127, +128 - признак ДСП)
    unsigned duration: 4; // Длительность работы в луче enum DURATION_ID
};

#define MAX_RAYS_NUM_PER_CYCLE 100 // Максимальное количество лучей в одном цикле (200 мс)
struct VOIStartRaysMessage {
    timeval64 sTimeMeasurement;
    quint32 count; // количество лучей в данной пачке
    struct RayPrms Rays[MAX_RAYS_NUM_PER_CYCLE]; // Параметры работы в лучах (количество count)
    quint16 info[MAX_RAYS_NUM_PER_CYCLE]; // байт для синхронизации с вторичкой (возвращается в POI_VOI_FINISH_RAY_MESSAGE и POI_KTA_MESSAGE)
    quint8 index_letter[MAX_RAYS_NUM_PER_CYCLE]; // 0 - 9 - индексы letter[], 255 - без letter (режим не по ответчику)
    quint32 letter[10]; // литера для работы по ответному сигналу (вместо литеры передаются две частоты: биты 0-15 - частота запросного сигнала, биты 16-31 - частота ответного сигнала)
    quint8 polarization[10]; // поляризация // 0 - вертикально, 1 - горизонтально
    quint8 num_ray_dsp[2]; // от 0 до 99 - индекс луча в режиме ДСП, который необходимо транслировать в РМО // 255 - не дсп
    int info_rmo[2]; // имя цели, которое необходимо транслировать в РМО вместе с лучом num_ray_dsp[]
    quint8 count_repeat_dist[2]; // количество повторений по дистанции // передача данных в РМО
    qint8 count_repeat_vr[2]; // количество повторений по радиальной скорости // передача данных в РМО
    quint32 reserved;
};

#define LAUNCH_MODES_TOTAL_NUM		64
#define RAYS_DURATION_TOTAL_NUM		7
enum LAUNCH_MODES	{
    // ЧЗ, полоса 3 МГц (стандартная)
    FS_3MHz_0 	= 0,
    FS_3MHz_1 	= 1,
    FS_3MHz_2 	= 2,
    FS_3MHz_3 	= 3,
    FS_3MHz_4 	= 4,
    FS_3MHz_5 	= 5,
    FS_3MHz_6 	= 6,
    FS_3MHz_7 	= 7,

    // ЧЗ, полоса 6 МГц (расширенная)
    FS_6MHz_0 	= 8,
    FS_6MHz_1 	= 9,
    FS_6MHz_2 	= 10,
    FS_6MHz_3 	= 11,
    FS_6MHz_4 	= 12,
    FS_6MHz_5 	= 13,
    FS_6MHz_6 	= 14,
    FS_6MHz_7 	= 15,

    // СЗ, полоса 3 МГц (стандартная), скважность 10
    MS_dc10_0 	= 16,
    MS_dc10_1 	= 17,
    MS_dc10_2 	= 18,
    MS_dc10_3 	= 19,
    MS_dc10_4 	= 20,
    MS_dc10_5 	= 21,
    MS_dc10_6 	= 22,
    MS_dc10_7 	= 23,
    MS_dc10_8 	= 24,
    MS_dc10_9 	= 25,
    MS_dc10_10 	= 26,
    MS_dc10_11 	= 27,
    MS_dc10_12 	= 28,
    MS_dc10_13 	= 29,
    MS_dc10_14 	= 30,
    MS_dc10_15 	= 31,

    // СЗ, полоса 3 МГц (стандартная), скважность 3
    MS_dc3_0 	= 32,
    MS_dc3_1 	= 33,
    MS_dc3_2 	= 34,
    MS_dc3_3 	= 35,
    MS_dc3_4 	= 36,
    MS_dc3_5 	= 37,
    MS_dc3_6 	= 38,
    MS_dc3_7 	= 39,
    MS_dc3_8 	= 40,
    MS_dc3_9 	= 41,
    MS_dc3_10 	= 42,
    MS_dc3_11 	= 43,
    MS_dc3_12 	= 44,
    MS_dc3_13 	= 45,
    MS_dc3_14 	= 46,
    MS_dc3_15 	= 47,

    // РЗ, полоса 3 МГц (стандартная)
    IS_0		= 48,
    IS_1		= 49,
    IS_2		= 50,

    // АО ЛЧМ, полоса 3 МГц (стандартная)
    AR_LFM_0	= 51,
    AR_LFM_1	= 52,
    AR_LFM_2	= 53,
    AR_LFM_3	= 54,

    // АО РИ #1, полоса 3 МГц (стандартная)
    AR_RI_1_0	= 55,
    AR_RI_1_1	= 56,
    AR_RI_1_2	= 57,
    AR_RI_1_3	= 58,

    // АО РИ #2, полоса 3 МГц (стандартная)
    AR_RI_2_0	= 59,
    AR_RI_2_1	= 60,
    AR_RI_2_2	= 61,
    AR_RI_2_3	= 62,
};

enum RAYS_DURATION	{								// Длительность работы в луче
    DURATION_2      = 0,
    DURATION_5      = 1,
    DURATION_10     = 2,
    DURATION_20     = 3,
    DURATION_60     = 4,
    DURATION_200    = 5,
    DURATION_2000	= 6,
};

// ID: POI_STATE_MESSAGE
struct PoiStateMessage {							// Сообщение с состоянием ПОИ
    timeval64 sTimeMeasurement;						// Время формирования сообщения
    // 32bit
    quint32 WorkPointNum;							// Номер рабочей точки
    float AzmSensorValue;							// Азимут антенны
    float UgmSensorValue;							// Угол места антенны
    quint32 FiltersState;							// Состояние фильтров (каждый бит отвечает за набор фильтров отдельного режима работы)
    quint32 RaysNum;								// Номера лучей (биты [0..15] - азимутальный, биты [16..31] - угломестный)
    quint32 Reserved_32b[3];						// Резервные 32-битные слова
    // 16bit
    quint16 UartConnections;						// Наличие связи по UART (каждый бит отвечает за отдельный порт UART)
    quint16 Reserved_16b[3];						// Резервные 16-битные слова
    // 8bit
    qint8 BoardTemperature[2];						// Температура двух вычислителей
    quint8 SRIO[2];									// Состояние интерфейсов SRIO на двух вычислителях (бит 0: 0 - нет связи, 1 - исправно)
    quint8 Ethernet[2];								// Состояние интерфейсов Ethernet на двух вычислителях (бит 0: 0 - нет линка, 1 - есть линк, биты [1..2] - скорость связи 10,100,1000 Мбит/с)
    quint8 RadiationState;							// Излучение (0 - выключено, 1 - включено)
    quint8 PhasingResult;							// Результат фазирования РЛС
    quint8 NowSendingDSP;							// Ведется передача ДСП на РМО (бит [7]: 0-нет, 1-да; биты [0..6] - текущий прогресс в %)
    quint8 WorkMode;								// Режим работы
    quint8 DrivesMotionDetector;					// Обнаружение движения приводов ОПУ (биты [0..3] - азимутальный, биты [4..7] - угломестный)
    quint8 ComputerInfo;							// Информация о вычислителе (бит 0: 0 - нижний, 1 - верхний спецвычислитель; бит 1: текущий статус 0 - в резерве, 1 - рабочий)
    quint8 AntennaMotionSpeed[2];					// Скорость движения антенны (байт 0 - азимут, байт 1 - угол места, [град/с])
    quint8 Reserved_8b[16];							// Резервные 8-битные слова
};

enum RmoCommandId  {
    CmdRadiation = 1,
    CmdWorkPoint = 2,
    CmdSendPoiData = 3,
    CmdTurnOnFullPoiReg = 4,
    CmdStartSendFullPoiReg = 5,
    CmdCtrlDriveRotation = 6,
    CmdGetBlindSectors = 7,
    CmdConfig4MainWork = 8,
    CmdRestartRls = 9,
    CmdRequestNav = 10,
    CmdRes_08 = 11,
    CmdRes_09 = 12,
    CmdRes_10 = 13,
    CmdRes_11 = 14,
    CmdRes_12 = 15,
};

// ID: RMO_POI_COMMAND_MESSAGE
struct RmoCommandMessage   {            		 // Сообщение с командой / квитанцией
    timeval64 sTimeMeasurement;          // Время приема данных
    quint8 CommandId;                     // Идентификатор команды (RLSOCommandId)
    quint8 Radiation;                     // Управление излучением (0 - выключено, 1 - включено)
    quint8 WorkPoint;                     // Номер рабочей точки (от 0 до 29, 255 - рабочая точка не установлена, фазировка не прошла)
    quint8 Res00;                         // Зарезервировано
    quint8 Res01;                         // Зарезервировано
    quint8 Res02;                         // Зарезервировано
    quint8 Res03;                         // Зарезервировано
    quint8 Res04;                         // Зарезервировано
    quint8 Res05;                         // Зарезервировано
    quint8 Res06;                         // Зарезервировано
    quint8 Res07;                         // Зарезервировано
    quint8 Res08;                         // Зарезервировано
    quint8 Res09;                         // Зарезервировано
    quint8 Res10;                         // Зарезервировано
    quint8 Res11;                         // Зарезервировано
    quint8 Res12;                         // Зарезервировано
};

// ID: POI_POSITION_SENSORS_MESSAGE
struct PoiPositionSensorMessage {								// Сообщение с данными с угломестного и азимутального датчиков
    timeval64 sTimeMeasurement;                                 // Время измерения координат
    float Azm;													// Азимут антенны
    float Ugm;													// Угол места антенны
};

// ID: POI_VOI_FINISH_RAY_MESSAGE
struct POIFinishRayMessage	{									// Сообщение об окончании работы в луче
    timeval64 sTimeMeasurement;
    quint16 info;
    quint8 count_kta;
    quint8 reserved[5];
};

// ID: POI_DSP_MESSAGE
struct POIDistSpeedPortraitHeader	{							// Заголовок дальностно-скоростного портрета
    timeval64 sTimeMeasurement;                                 // Время формирования ДСП
    int DistSamplesNum;											// Количество дальностных отсчетов
    int TimeSamplesNum;											// Количество импульсов в луче
    float Sensor_Azm;											// Значение датчика азимута
    float Sensor_Ugm;											// Значение датчика угла места
    int Value4Rmo;												// Значение для РМО
    quint32 RayCounter;											// Счетчик лучей ПОИ
    quint8 LaunchMode;											// Номер запуска
    quint8 RayNum_Azm;											// Номер луча по азимуту
    quint8 RayNum_Ugm;											// Номер луча по углу места
    quint8 TotalPartsNum;										// Общее количество передаваемых частей ДСП
    quint8 CurrentPartNum;										// Номер передаваемой части ДСП
    quint16 TimeSamplesNumInPack;								// Количество импульсов в текущей части ДСП
    quint8 Reserved[1];
};

// ID: MIF380_STATE_MESSAGE
enum MIF380_ID	{												// Перечень ID МПЧ380
    MIF380_ID_Shared = 0,										// Широковещательный ID МПЧ380 (в состоянии не используется)
    MIF380_ID_Azm = 3,											// ID МПЧ380, ответственного за привода ОПУ по азимуту
    MIF380_ID_Ugm_Left = 2,										// ID МПЧ380, ответственного за левый привод ОПУ по углу места
    MIF380_ID_Ugm_Right = 1,									// ID МПЧ380, ответственного за правый привод ОПУ по углу места
};
enum MIF380FunctionalState	{									// Перечень возможных функциональных состояний МПЧ
    MIF380_FS_NoConnection = 0x00,								// Нет связи с МПЧ
    MIF380_FS_Ready = 0x02,										// Готовность
    MIF380_FS_Run = 0x03,										// Работа
    MIF380_FS_Error = 0x04,										// Авария
};
enum MIFStateMessage	{										// Перечень возможных сообщений МПЧ
    MIF380_SM_Norm = 0x02,										// Норма (Штатная работа)
    MIF380_SM_HeatingIF = 0x0E,									// Нагрев ПЧ (Нагрев МПЧ близок к максимальному)
    MIF380_SM_Autostart = 0x27,									// Автостарт (Ожидание автоматического старта после подачи питания)
    MIF380_SM_MaxCurrent = 0x0F,								// Максимальный ток (Ток на выходе МПЧ близок к максимально допустимому)
    MIF380_SM_TorqueLimitation = 0x12,							// Ограничение момента (Работа с ограничением момента)
    MIF380_SM_Phase = 0x03,										// Фаза сети (Нарушение целостности фаз)
    MIF380_SM_HighInputVoltage = 0x04,							// Высокое напряжение на входе (Действующее значение напряжения на входе МПЧ выше допустимого)
    MIF380_SM_LowInputVoltage = 0x05,							// Низкое напряжение на входе (Действующее значение напряжение на входе МПЧ ниже допустимого)
    MIF380_SM_LowVoltageDCCircuit = 0x06,						// Низкое напряжение звена постоянного тока (ЗПТ) (Пониженного напряжение на силовых конденсаторах)
    MIF380_SM_HighVoltageDCCircuit = 0x07,						// Высокое напряжение ЗПТ (Повышенное напряжение на силовых конденсаторах)
    MIF380_SM_ShortCircuitInOutput1 = 0x0A,						// КЗ на выходе (Короткое замыкание на выходе)
    MIF380_SM_ShortCircuitInOutput2 = 0x0B,						// КЗ на выходе (Короткое замыкание на выходе)
    MIF380_SM_CurrentOverload = 0x0C,							// Перегрузка по току (Ток на выходе МПЧ выше допустимого)
    MIF380_SM_OverheatingIF = 0x0D,								// Перегрев ПЧ (Температура МПЧ выше допустимой)
    MIF380_SM_MemoryError = 0x10,								// Ошибка памяти (Неудачная процедура записи/чтения из памяти)
    MIF380_SM_LoadPhaseBreak = 0x13,							// Обрыв фазы нагрузки (Ток несимметрии нагрузки превысил допустимый уровень)
    MIF380_SM_MeasureSystemFailed = 0x14,						// Сбой системы измерения (Система измерения нагрузки сигнализирует сбой)
    MIF380_SM_DrivePhaseBreak = 0x18,							// Обрыв фазы двигателя
    MIF380_SM_ExternalBlocking = 0x20,							// Внешняя блокировка (Блокировка релейным сигналом)
    MIF380_SM_ConnectionTimeout = 0x22,							// Таймаут связи (Таймаут передачи данных по последовательному порту)
    MIF380_SM_RotorPositionSensorError1 = 0x26,					// Сбой ДПР (Ошибка обработки данных датчика положения ротора)
    MIF380_SM_RotorPositionSensorError2 = 0x28,					// Сбой ДПР (Ошибка обработки данных датчика положения ротора)
    MIF380_SM_SoftwareLimit1 = 0x29,							// Программный предел 1 (При работе контура положения достигнут программный предел)
    MIF380_SM_SoftwareLimit2 = 0x2A,							// Программный предел 2 (При работе контура положения достигнут программный предел)
    MIF380_SM_OverflowVirtualEncoder = 0x2C,					// Переполнение виртуального энкодера
};
struct MIF380StateMessage	{									// Сообщение с состоянием МПЧ
    timeval64 sTimeMeasurement;									// Время формирования сообщения
    quint8 FunctionalState;										// Функциональное состояние МПЧ. Если 0, то нет связи
    quint8 StateMessage;										// Сообщение о состоянии МПЧ
    quint16 SignalsState;										// Состояние релейных сигналов
    quint8 ID;													// ID МПЧ
    quint8 Reserved_x8[3];										// Зарезервировано
    float OutputFrequency;										// Выходная частота [Гц]
    float OutputVoltage;										// Выходное напряжение [В]
    float DC_Voltage;											// Напряжение звена постоянного тока (ЗПТ) [В]
    float IF_InputVoltage;										// Напряжение на входе МПЧ [В]
    float IF_OutputCurrent;										// Ток на выходе МПЧ [А]
    float IF_Temperature;										// Температура ПЧ [C°]
    float RotorPositionSensor;									// Значение датчика положения ротора (ДПР) [°]
    float MotorTemperature;										// Значение датчика температуры мотора [°]
    float VirtualEncoderValue;									// Значение виртуального энкодера [°]
    float Reserved[7];											// Зарезервировано
};

// ID: VOI_POI_DRIVE_UM_COMMAND_MESSAGE
struct VoiDriveUmCommandMessage {
    float Um; // от 0 до 90 градусов (90?)
    float Reserved[3];
};

// ID: VOI_POI_DRIVE_AZ_COMMAND_MESSAGE
struct VoiDriveAzCommandMessage	{                               // Сообщение с командой азимутальному приводу
    int Speed[2];												// Скорость движения ОПУ [1°/с = 1000ед.] (>0 - против часов стрелки, <0 - по часовой стрелке, 0 - остановка)
    int Acceleration[2];										// Ускорение/Торможение ОПУ при изменении скорости [1°/с^2 = 1000ед.] (>0 - ускорение, <0 - замедление, 0 - отключено)
    float Reserved[2][10];										// Зарезервировано
};

// ID: NAV_MESSAGE
struct NavMessage	{											// Сообщение с навигационными данными
    timeval64 sTimeMeasurement;                                 // Время формирования сообщения
    uint64_t timeUTC;											// Время (мс)
    quint8 state;                                           	// 0 - нет связи
    quint8 SatelliteNum_GPS;										// Количество спутников
    quint8 SatelliteNum_GLN;										// Количество спутников
    quint8 Reserved[5];
    double Latitude;												// Широта
    double Longitude;												// Долгота
    double Height;													// Высота
    double CKO;														// СКО
};

// ID: M14EP1_STATE_MESSAGE
enum AntennaDoorStateByte	{									// Байт 1 состояния контроллера створки (!!! если блок М14ЕП1 не получает ответа от контроллера створки, то бит 7 выставляется в 1)
    AntennaDoor_Ok								= 0x4F,			// Нет ошибок
    AntennaDoor_DataSizeError					= 0x02,			// Не принятое необходимое количество данных
    AntennaDoor_BootLoader						= 0x42,			// Блок находится в загрузчике, основная программа не запущена
};
struct AntennaDoorState	{										// Структура, описывающее состояние створки антенны
    quint8 Id;													// ID створки (0 - левая, 1 - правая)
    quint8 State;												// Байт состояния 1 (AntennaDoorStateByte)
    quint8 SoftwareType;										// Тип запущенного ПО (0x4C - загрузчик, 0x4B - штатное ПО)
    qint8 TemperatureBrake[2];									// Температура тормозов (0 - верхний, 1 - нижний)
    quint8 FoldingMotorState;									// Состояние двигателя складывания/раскладывания (0 - неактивен, 1 - активен: раскладывание, 2 - активен: складывание)
    quint8 FoldingBrakeState[2];								// Состояние тормозов двигателя складывания/раскладывания (байт 0 - верхний, байт 1 - нижний; 1 - неактивен, 0 - активен)
    quint8 TiltDriveBrakeState;									// Состояние тормозов привода наклона (1 - неактивен, 0 - активен)
    quint8 FoldingBrakeHeatingState;							// Состояние обогрева тормозов двигателя складывания/раскладывания (0 - неактивен, 1 - активен)
    quint8 TiltDriveBrakeHeatingState;							// Состояние обогрева тормозов привода наклона (0 - неактивен, 1 - активен)
    quint8 M14HK1_High_PowerState;								// Питание верхнего блока М14ХК1 (0 - неактивен, 1 - активен)
    quint8 M14HK1_Low_PowerState;								// Питание нижнего блока М14ХК1 (0 - неактивен, 1 - активен)
    quint8 AntennaHeatingState;									// Обогрев антенны (0 - неактивен, 1 - активен)
    quint8 MIF380_Ugm_PowerState;								// Питание МПЧ-380 (наклон) (0 - неактивен, 1 - активен)
    quint8 EmergencyTiltLimitSwitchState;						// Аварийный концевик наклона (1 - неактивен, 0 - активен)
    quint8 TrackTiltLimitSwitchState;							// Путевой концевик наклона (1 - неактивен, 0 - активен)
    quint8 FoldingLimitSwitchState;								// Концевик складывания (1 - неактивен, 0 - активен)
    quint8 UnfoldingLimitSwitchState[2];						// Концевик раскладывания (байт 0 - верхний, байт 1 - нижний; 1 - неактивен, 0 - активен)
    quint8 AntennaDoorMotionDirection;							// Направление движения створки (0 - складывание, 1 - раскладывание, 2 - створка остановлена)
    quint8 AntennaDoorMotionStep;								// Шаг движения створки (0 - отпускаем тормоза, 1 - запускаем двигатели, 2 - движение створки с контролем конечных выключателей, 4 - остановка двигателей, постановка на тормоз)
    quint16 AntennaDoorMotionCounter;							// Счетчик движения створки (от 0 до 1204, emp = 50мс, действие выполнено/отменено - счетчик останавливается)
};
enum M14EP1_State1	{											// Байт 1 состояния М14ЕП1
    M14EP1_NoConnection							= 0x00,			// Нет связи
    M14EP1_Ok									= 0x4F,			// Нет ошибок
    M14EP1_NoPrefix								= 0x01,			// Не принят префикс
    M14EP1_DataSizeError						= 0x02,			// Не принятое необходимое количество данных
    M14EP1_BootLoader							= 0x42,			// Блок находится в загрузчике, основная программа не запущена
};
struct M14EP1_StateMessage	{									// Сообщение с состоянием М14ЕП1
    timeval64 sTimeMeasurement;									// Время формирования сообщения
    quint8 State1;												// Байт 1 состояния
    quint8 State2;												// Байт 2 состояния
    qint8 Temperature;											// Температура блока
    quint8 PowerState_27V;										// Состояние питания 27В (0 - Выкл, 1 - Вкл)
    quint8 PowerState_5V;										// Состояние питания 5В (0 - Выкл, 1 - Вкл)
    quint8 PowerState_ESPbox;									// Состояние питания ESP-box (0 - Выкл, 1 - Вкл)
    quint8 HeatingState;										// Состояние подогрева блока (0 - Выкл, 1 - Вкл)
    quint8 M14HK1_Active;										// Активный блок М14ХК1 (0 - верхний, 1 - нижний)
    quint8 Restart_5V;											// Необходимость пересброса 5В
    quint8 Reserved_8bit[3];
    float Vp_27V;												// Напряжение питания 27В
    struct AntennaDoorState DoorsState[2];						// Состояние створок
};

// ID: M14EK2_STATE_MESSAGE
enum M14EK2_StateByte	{										// Байт 1 состояния M14EK2 (!!! если блок М14ЕП1 не получает ответа от M14EK2, то бит 7 выставляется в 1)
    M14EK2_Ok									= 0x4F,			// Нет ошибок
    M14EK2_DataSizeError						= 0x02,			// Не принятое необходимое количество данных
    M14EK2_BootLoader							= 0x42,			// Блок находится в загрузчике, основная программа не запущена
};
struct M14EK2_StateMessage	{									// Сообщение с состоянием М14ЕК2
    timeval64 sTimeMeasurement;									// Время формирования сообщения
    quint8 State1;												// Байт 1 состояния
    quint8 State2;												// Байт 2 состояния
    quint8 Id;													// ID блока (от 0 до 7 - левая створка, от 8 до 15 - правая створка)
    qint8 Temperature;											// Температура блока
    quint8 Power_27V_State;										// Состояние питания 27В (0 - Выкл, 1 - Вкл)
    quint8 Power_6V_State;										// Состояние питания 6В (0 - Выкл, 1 - Вкл)
    quint8 DiagInputState;										// Состояние диагностического входа
    quint8 Reserved_8bit;
};

// ID: INCLINOMETR_MESSAGE
struct InclinometerMessage	{									// Сообщение с информацией от инклинометра
    timeval64 sTimeMeasurement;									// Время формирования сообщения
    float AzimuthSensorValue;									// Значение датчика азимута антенны
    int InclinometerId;											// Адрес инклинометра
    float AngleY;												// Угол по оси Y [угловые секунды]
    float AngleX;												// Угол по оси X [угловые секунды]
    quint8 state;                                           	// 0 - нет связи
    quint8 Reserved_x8[3];
    float Reserved_float[3];									// Резерв
};

// ID: M12UV1_STATE_MESSAGE
struct M12UV1_StateMessage  {						// Сообщение с состоянием одного М12УВ1 (ППМ)
    timeval64 sTimeMeasurement;						// Время формирования сообщения
    quint16 RtmNum;									// Номер ППМ
    quint16 LimitMinAttTxValue;						// Значение ограничения минимального ослабления аттенюатора ПрД
    quint16 TxAttValue[9];							// Значение аттенюатора ПрД [дБ * 10] (0 - предварительного усилителя, 1-8 - каналов)
    quint16 Reserved_x16[1];
    quint8 FragmentNum;								// Номер фрагмента антенны
    quint8 AzmRayNum;								// Номер луча по азимуту
    quint8 UgmRayNum;								// Номер луча по углу места
    quint8 RtmState1;								// Состояние ППМ ('O' - если все исправно или код ошибки)
    quint8 RtmState2;								// Состояние ППМ
    quint8 TxSwitch;								// Состояние переключателя Tx (0b0 - ПрД выкл, ПрМ выкл; 0b01 - ПрД выкл, ПрМ вкл; 0b10 - Имитатор ИЗМ; 0b11 - ПрД по ИЗМ)
    quint8 Polarization;							// Состояние поляризации
    quint8 ResponderModeState;						// Режим с ответчиком (0 - выкл., 1 - вкл.)
    quint8 RtmAnswer[6];							// Состояние ППМ - наличие ответов на все 4 запроса состояния
    quint8 SoftwareVersion;							// Версия ПО
    qint8 Temperature;								// Температура
    quint8 WorkPointNum;							// Номер рабочей точки
    quint8 TestChannelNum;							// Номер тестового канала
    quint8 Test_Fap_Det_SwitchState;				// Состояние переключателя ТС/ФАП - Детектор (0 - ТС/ФАП; 1 - Детектор)
    quint8 TurnOffAllSwitches;						// Выключатель переключателей (0 - Выключены, 1 - Включены)
    quint8 AutoConnect2Det;							// Автоматическое подключение каналов к детектору (0 - выключено, 1 - включено)
    quint8 ResponderTxWorkPoint;					// Номер рабочей точки в режиме с ответчиком
    quint8 ChannelsState;							// Состояние каналов (0 - Выкл, 1 - Вкл)
    quint8 AutoChangePhases;						// Автопереключение фаз по ИЗМ (0 - Выкл, 1 - Вкл)
    float TxPhaseValue[8];							// Значение фазовращателя ПрД каждого из каналов
    float RxPhaseValue[8];							// Значение фазовращателя ПрМ
    float AverVoltageTxDet[8];						// Усредненное напряжение с детектора ЗС каждого из каналов [мВ]
    float TxImpulseDuration[8];						// Длительность импульса детектора ЗС каждого из каналов [нс]
    quint8 CommonSwitchState;						// Общий выключатель (0 - Выкл, 1 - Вкл)
    quint8 Reserved_x8[7];
};

// ID: M12HL1_STATE_MESSAGE
typedef enum M12HL1_State	{												// Состояние М12ХЛ1
    M12HL1_NoConnection										= 0x0,			// Нет связи
    M12HL1_Ok												= 0x4F,			// Нет ошибок
    M12HL1_NoPrefix											= 0x02,			// Не принят префикс
    M12HL1_NoCommand										= 0x03,			// Не принята команда (префиск принят)
    M12HL1_DataSizeError									= 0x04,			// Не принятое необходимое количество данных (Префикс и команда приняты)
    M12HL1_BootLoader_Error									= 0x42,			// Блок находится в загрузчике, основная программа не запущена
}M12HL1_State;
struct M12HL1_StateMessage	{
    timeval64 sTimeMeasurement;						// Время формирования сообщения
    quint32 State;									// Состояние ('O' - если все исправно или код ошибки)
    quint32 Reserved;
    quint32 UartPortSpeed[8];						// Скорость портов UART
    quint8 UartPortConfigs[8];						// Настройки портов UART
};

// ID: M11PS2_STATE_MESSAGE
typedef enum M11PS2_WorkModes	{											// Режимы работы М11ПС2
    M11PS2_RlsMode											= 0x00,			// РЛС
    M11PS2_TestMode_Fap                                     = 0x01,			// Тестовый режим ФАП
    M11PS2_TestMode_Ts										= 0x02,			// Тестовый режим с формированием сигнала ТС
    M11PS2_ResponderMode									= 0x03,			// С ответчиком
}M11PS2_WorkModes;
typedef enum M11PS2_State	{												// Состояние М11ПС2
    M11PS2_Ok												= 0x4F,			// Нет ошибок
    M11PS2_NoPrefix											= 0x02,			// Не принят префикс "ПС2"
    M11PS2_NoCommand										= 0x03,			// Не принята команда для ПС (префиск принят)
    M11PS2_DataSizeError									= 0x04,			// Не принятое необходимое количество данных (Префикс и команда приняты)
    M11PS2_InvalidMode										= 0x05,			// Ошибка выбора режима
    M11PS2_InvalidSynthesizerNum							= 0x06,			// Ошибка выбора аттенюатора
    M11PS2_FailedSynthesizer_1								= 0x07,			// Не включился первый синтезатор
    M11PS2_FailedSynthesizer_2								= 0x08,			// Не включился второй синтезатор
    M11PS2_FailedTwoSynthesizers							= 0x09,			// Не включились оба синтезатора
    M11PS2_FailedAtt_Zs										= 0x0D,			// Не установлено значение аттенюатора ЗС, ошибка аттенюатора
    M11PS2_FailedAtt_Ts										= 0x0E,			// Не установлено значение аттенюатора ТС, ошибка аттенюатора
    M11PS2_FailedAtt_Fap									= 0x0F,			// Не установлено значение аттенюатора ФАП, ошибка аттенюатора
    M11PS2_BootLoader_Error									= 0x42,			// Блок находится в загрузчике, основная программа не запущена
}M11PS2_State;
struct M11PS2_StateMessage  {						// Сообщение с состоянием одной М11ПС2 (ПС)
    timeval64 sTimeMeasurement;						// Время формирования сообщения
    quint16 Num;									// Номер блока
    quint8 State;									// Состояние (M11PS2_State)
    quint8 State2;									// Состояние
    float AttValue_Zs;								// Значение аттенюатора ЗС (от 0 до 31.5 дБ)
    float AttValue_Ts;								// Значение аттенюатора ТС (от 0 до 31.5 дБ)
    float AttValue_Fap;								// Значение аттенюатора ФАП (от 0 до 31.5 дБ)
    quint32 Frequency;								// Частота гетеродина
    quint8 SynthesizerNum;							// Номер синтезатора
    quint8 Mode;									// Режим работы (M11PS2_WorkModes)
    quint8 WorkPointNum_Zs;							// Номер рабочей точки ЗС
    quint8 WorkPointNum_Request;					// Номер рабочей точки Запрос
    quint8 WorkPointNum_Answer;						// Номер рабочей точки Ответ
    qint8 Temperature;								// Температура
    quint8 SoftwareVersion;							// Версия ПО
    quint8 Reserved[5];
};

// ID: VOI_RMO_RAY_SET_MESSAGE
// Сообщение с набором лучей на следующий такт
struct VOIRaySetMessage      {
   timeval64 sTimeMeasurement;
   quint8 count;
   float az[100];
   float ugm[100];
   int id[100]; // номера id:
   // от 0 до бесконечности - номера трасс
   // от -1 до -10 сектора (соответственно имея -(сектор+1), так как ноль занят)
   // от -11 до -20 это кандидаты, vr, и прочее
   // от -100 до -бесконечности спец-сектора (спец-сектор привязывается к родительской трассе -(трасса+100))
   quint8 mode[100];
   float R_dn[100]; // радиус диаграммы направленности каждого лучика
};

// ID: VOI_RMO_AIM_MESSAGE
// Сообщение о цели (ИРЛС-О -> КП)
struct RLSOaimMessage {
    timeval64 sTimeMeasurement; // Время измерения координат
    int trackID; // Номер трассы в системе номеров ИРЛС-О
    float D; // Дальность
    float Az; // Азимут
    float Um; // Угол места
    float h; // высота h
    float Vx; // скорость по х
    float Vy; // скорость по у
    float Vz; // скорость по z
    quint8 typeSignal; // 0 - Отраженный, 1 - Ответный, 3 - Отражённый + ответный // enum TargetWorkMode
    quint8 typeSignalNow; // 0 - Отраженный, 1 - Ответный, 10 - экстраполяция // enum TargetWorkMode
    quint32 letter; // литера
};


// ID: RMO_VOI_TARGET_POSITION_MESSAGE
// Сообщение с параметрами ЦУ
typedef struct SettingAnswerSector
{
    quint8 set_mode;
    quint8 freq_rays;
    quint8 type_letter;
    quint32 letter;
}SettingAnswerSector;

struct RMOTargetPositionMessage {
    timeval64 sTimeMeasurement;
    quint8 id;
    quint8 isUpdate; //0 - новый сектор, 1 - обновление
    float azimuth[2]; // 0-й - азимут центра ЦУ (направление), 1-й - ширина по азимуту
    float ugm[2]; // 0-й - угол места центра ЦУ (направление), 1-й - ширина по углу места
    float dist[2];
    float speed[2];
    quint8 work_mode; // 0,1,3,4,5,6
    quint8 set_mode[2]; // набор режимов // 255 - нет набора // 0 - ОТР, 1 - ДСП
    quint8 freq_rays[2]; // частота осмотра одного луча
    quint8 DSP; // 0 - 2 мсек, 1 - 20 мсек
    quint8 out_dsp; // 0 - не выводить дсп, 1 - выводить дсп
    quint8 count_answer; // количество ответчиков
    SettingAnswerSector answer[10];
    quint8 look_priority; // приоритет на попадание в луч // 1 - высокий (луч обязательно осматривается), 2 - низкий (как попадёт)
};

// ID: RMO_VOI_TARGET_DELETE_MESSAGE
// Сообщение с удалением ЦУ
struct RMOTargetDeleteMessage      {
    timeval64 sTimeMeasurement;
    quint8 id;
};

// ID: RMO_VOI_DRIVE_TO_ROSITION_MESSAGE
// Сообщение с данными куда смотреть антенне
struct RMODriveToPositionMessage      {
    timeval64 sTimeMeasurement;
    quint8 exist; // 1 - анетнна направляется в заданную позицию и удерживает её, 0 - указания отменены
    float Az;
    float Um;
};

// ID: VOI_RMO_CREATE_SPECIAL_SECTOR_MESSAGE
// Сообщение о создании спец-сектора
struct VOICreateSpecialSectorMessage {
    timeval64 sTimeMeasurement;
    int id_trass;
    quint8 work_mode; // TargetWorkMode // только ReflectedSignal = 0 и ResponseSignal = 1
    float azimuth[2];
    float ugm[2];
    float dist[2];
};

// ID: VOI_RMO_DELETE_SPECIAL_SECTOR_MESSAGE
// Сообщение об удалении спец-сектора
struct VOIDeleteSpecialSectorMessage {
    timeval64 sTimeMeasurement;
    int id_trass;
};

enum TargetWorkMode {
    ReflectedSignal = 0,
    ResponseSignal = 1,
    DSP = 2,
    ReflectedAndResponse = 3,
    ReflectedAndDSP = 4,
    ResponseAndDSP = 5,
    ReflectedAndResponseAndDSP = 6,
    Extrapolation = 10
};

// ID: RMO_VOI_TRACK_SETTING_MESSAGE
// Сообщение о проритетности трассы
struct RMOTrackSetting {
    timeval64 sTimeStamp;           // Время создания сообщение
    int aimID;                      // Номер цели в системе номеров ИРСЛ-С
    quint8 priority;                // 0 - нормальная частота осмотра, 1 - высокая частота осмотра
    quint8 work_mode;               // TargetWorkMode // без DSP = 2,
    quint8 DSP;                     // 0 - 2 мсек (хвост), 1 - 20 мсек
    quint8 out_dsp;                 // 0 - не выводить дсп, 1 - выводить дсп
    quint8 type_answer;             // тип ответчика // 0 - ЛЧМ, 1 - РИ
    quint32 letter;                 // литера
    quint8 gen_target;              // главная цель (цель за которой следит антенна) // 1 - главная цель, 0 - нет
    quint8 spec_sectors_on;         // возможно ли создавать спец-сектора у удаляемых трасс // 0 - нет, 1 - можно,
    quint8 priority_spec_sectors;   // получает ли максимальный приоритет спец-сектор на поиск потерянной трассы
                                    // 0 - приоритет как у потерянной   трассы, 1 - приоритет,
    quint8 time_searching_in_spec_sector;  // время сброса спец-сектора (сек)
    quint8 trackingZoneWidth;       // Ширина зоны слежения (10 - 45 град)
};

// ID: RMO_VOI_TRACK_DELETE_MESSAGE // ID: VOI_RMO_TRACK_DELETE_MESSAGE
// Сообщение об удалении трассы от РМО
struct DeleteTrackMessage {
    timeval64 sTimeStamp;   // Время создания сообщение
    quint32 aimID;           // Номер цели в системе номеров ИРСЛ-С
};

// ID: VOI_RMO_PRIORITY_SECTOR_MESSAGE
// Сообщение с подтверждеием/отменой высокого приоритета у сектора
struct VOIPrioritySectorMessage {
    timeval64 sTimeStamp;   // Время создания сообщение
    quint8 id;
    quint8 possible_priority; // 1 - возможен высокий приоритет, 0 - не возможен высокий приоритет (ставится низкий)
};

//ID: VOI_RMO_STATE_METEO_MESSAGE
struct VOIStateMeteoMessage { // Сообщение о состоянии метеостанции
    timeval64 sTimeMeasurement; // Время создание сообщения
    uint8_t state;  // 0 - нет связи, 1 - сообщение пришло, контрольная сумма не совпала, 2 - отказ метеостанции от исполнения команды,
                    // 3 - неверные данные, 4 - всё штатно работает, 5 - время ожидания ответа истекло
};

//ID: VOI_RMO_MARK_MESSAGE
struct VOIMarkMessage { // Сообщение с обнаруженной КТА
    timeval64 sTimeMeasurement; // Время измерения координат
    float D; // Дальность КТА
    float Azm; // Азимут КТА
    float Ugm; // Угол места КТА
    float Vr; // Радиальная скорость КТА
    float Amp; // Амплитуда КТА
    int num_target; // Номер объекта (цели)
    bool flag_target; // Признак цели // true - цель, false - только отождествлённая кта
    bool flag_answer; // Признак ответчика // true - по ответчику, false - по отражённому сигналу
};

//ID: RDDS_OUT_UNIT_POINT_OBJ
struct RDDS_UnitPointObj { // Сообщение UnitPointObj в RDDS
    timeval64 sTimeMeasurement; // Время измерения координат
    qint16 num_obj; // Номер измеряемого объекта
    qint8 mark_reli_dimension; // Признак достоверности измерения
    qint8 mark_point_obj; // Признак типа объекта – «точечный объект»
    float dist; // Дальность, [м]
    float az; // Азимут, [градус]
    float um; // Угол места, [градус]
    float vr; // Радиальная скорость, [м/с]
    float SD_dist; // СКО измерения дальности, [м]
    float SD_az; // СКО измерения азимута, [угл. минуты]
    float SD_um; // СКО измерения угла места, [угл. минуты]
    float SD_vr; // СКО измерения радиальной скорости, [м/с]
    float RCS_obj; // ЭПР объекта, [м2]
    float SD_RCS_obj; // СКО измерения ЭПР объекта, [м2]
};

//ID: RDDS_OUT_UNIT_POINT_OBJ_FOR_DIST
struct RDDS_UnitPointObjForDist { // Сообщение UnitPointObjForDist  в RDDS
    timeval64 sTimeMeasurement; // Время измерения координат
    qint16 num_obj; // Номер измеряемого объекта
    qint8 mark_reli_dimension; // Признак достоверности измерения
    float dist_center; // Дальность условного центра объекта, [м]
    float az_center; // Азимут условного центра объекта, [градус]
    float um_center; // Угол места условного центра объекта, [градус]
    float vr_center; // Радиальная скорость условного центра объекта, [м/с]
    float length_obj_dist; // Протяженность объекта по дальности, [м]
    float length_obj_az; // Протяженность объекта по азимуту, [м]
    float length_obj_um; // Протяженность объекта по углу места, [м]
    float SD_dist; // СКО измерения дальности, [м]
    float SD_az; // СКО измерения азимута, [угл. минуты]
    float SD_um; // СКО измерения угла места, [угл. минуты]
    float SD_vr; // СКО измерения радиальной скорости, [м/с]
    float RCS_obj; // ЭПР объекта, [м2]
    float radial_size_obj; // Радиальный размера объекта, [м]
};

//ID: RDDS_OUT_UNIT_POINT_OBJ_ANS_SIGNAL
struct RDDS_UnitPointObjAnsSignal { // Сообщение UnitPointObjAnsSignal  в RDDS
    timeval64 sTimeMeasurement; // Время измерения координат
    qint16 num_obj; // Номер измеряемого объекта
    float dist; // Дальность, [м]
    float az; // Азимут, [градус]
    float um; // Угол места, [градус]
    float SD_dist; // СКО измерения дальности, [м]
    float SD_az; // СКО измерения азимута, [угл. минуты]
    float SD_um; // СКО измерения угла места, [угл. минуты]
    quint16 letter; // Частотный литер ответного сигнала
};

//ID: RDDS_OUT_TRAJECTORY
struct RDDS_Trajectory { // Сообщение Trajectory в RDDS
    timeval64 sTimeMeasurement; // Время измерения координат
    qint16 num_obj; // Номер измеряемого объекта
    quint8 mark_mode; // Признак режима измерения
    quint8 mark_filter_smooth; // Признак типа фильтра сглаживания
    quint8 num_GO; // Номер ГО, в состав которого входит оцениваемый объект
    quint8 mark_type_obj; // Признак типа измеряемого объекта
    quint8 mark_priority; // Признак приоритета обслуживания объекта
    quint8 mark_type_trajectory; // Признак типа траектории
    quint8 mark_fall_trajectory; // Признак типа сброса траектории с сопровождения (по достижению минимальной высоты, по выходу из сектора, вследствие потери точности и др.)
    quint8 mark_maneuver; // Признак начала маневра
    quint8 type_maneuver; // Тип манёвра
    quint8 mark_drag; // Признак обнаружения торможения (для баллистического объекта, входящего в атмосферу)
    float X; // Координата Х в МПСК , [м]
    float Y; // Координата Y в МПСК , [м]
    float Z; // Координата Z в МПСК , [м]
    float Vx; // Скорость вдоль оси ОХ в МПСК , [м/с]
    float Vy; // Скорость вдоль оси OY в МПСК , [м/с]
    float Vz; // Скорость вдоль оси OZ в МПСК , [м/с]
    float ballistic_coef; // Баллистический коэффициент
    float SD_X; // СКО оценки координаты X в МПСК, [м]
    float SD_Y; // СКО оценки координаты Y в МПСК, [м]
    float SD_Z; // СКО оценки координаты Z в МПСК, [м]
    float SD_Vx; // СКО оценки координаты Vx в МПСК, [м]
    float SD_Vy; // СКО оценки координаты Vy в МПСК, [м]
    float SD_Vz; // СКО оценки координаты Vz в МПСК, [м]
    float SD_koef_drag; // СКО оценки коэффициента торможения
    float SD_koef_lift; // СКО оценки коэффициента подъемной силы
    float SD_side_power; // СКО оценки коэффициента боковой силы
    float height; // Высота, [м]
};

//ID: RDDS_OUT_PARAM_BEARING_FOR_ANI
struct RDDS_ParamBearingForANI { // Сообщение ParamBearingForANI в RDDS
    timeval64 sTimeMeasurement; // Время измерения координат
    quint16 num_bearing; // Номер пеленга
    quint8 mark_reli_dimension; // Признак достоверности измерения
    float az; // Азимут, [градус]
    float um; // Угол места, [градус]
    float SD_az; // СКО измерения азимута, [градус]
    float SD_um; // СКО измерения угла места, [градус]
    float width_sector; // Ширина сектора эффективного подавления, [градус]
    float up_limit_freq; // Верхняя граница частотного диапозона помехи, [МГц]
    float down_limit_freq; // Нижняя граница частотного диапозона помехи, [МГц]
    float spectral_density_power_noise; // Спектральная плотность мощности помехи, [Вт/Гц]
};

//ID: RDDS_IN_TARGET_FOR_RLS
struct RDDS_TargetForRLS { // Сообщение TargetForRLS из RDDS
    quint8 num_rls; // Номер РЛС
    timeval64 sTimeMeasurement; // Время
    quint8 type_obj; // Тип измеряемого объекта
    quint8 priority_obj; // Приоритет измеряемого объекта
    float az_target; // Азимут ТЦУ в СФСК РЛС ТИ, [градус]
    float V_az_target; // Скорость изменения азимута ТЦУ в СФСК РЛС ТИ, [градус / с]
    float um_target; // Угол места ТЦУ в СФСК РЛС ТИ, [градус]
    float V_um_target; // Скорость изменения угла места ТЦУ в СФСК РЛС ТИ, [градус / с]
    float dist_target; // Дальность ТЦУ, [м]
    float vr_target; // Радиальная сокрость объекта ТЦУ, [м / с]
    float SD_az_target; // СКО ТЦУ по азимуту, [градус]
    float SD_V_az_target; // СКО ТЦУ по скорости изменения азимута, [градус / с]
    float SD_um_target; // СКО ТЦУ по углу места, [градус]
    float SD_V_um_target; // СКО ТЦУ по скорости изменения угла места, [градус / с]
    float SD_dist_target; // СКО ТЦУ по дальности, [м]
    float SD_vr_target; // СКО ТЦУ по радиальной скорости, [м / с]
};

// ID: REG_RREWIND_FILE
// Сообщение для перемотки файла воспроизведения
struct SimMessageRewind {
    timeval64 timeStart;      // Время начала воспроизведения
};

// ID: REG_PLAY_ON                   = 127
// Сообщение для запуска/остановки воспроизведения
struct simPlayOn {
    quint8 playOn;           // Воспроизведение (0 - остановить,
    //                  1 - начать воспроизведение с реальной скоростью,
    //                  2 - ускорение в 2 раза,
    //                  3 - замедление в 2 раза
    //                  255 - перезапуск
};

// ID: REG_CURRENT_PLAY_TIME         = 128
// Сообщение о времени воспроизведения из файла
struct simTimeInFile {
    timeval64 timeStart;
};

// ID: RARM_SYSTEM_CONTROL_MESSAGE
// Сообщение с состоянием подсистем
struct RARMSysControlMessage {
    int sysControl;
};

enum REG_PLAY_CONTROL {
    CMD_PLAY            = 1,
    CMD_PAUSE           = 0,
    CMD_HALF_SPEED      = 3,
    CMD_2x              = 2,
    CMD_4x              = 4,
    CMD_8x              = 8,
    CMD_STOP            = 255,
};

// ID: RMO_VOI_BIO_DEFENCE_SECTORS_MESSAGE     = 9
// Сообщение с сектором биологической защиты для ВОИ
struct RMOBioDefenceSectorMessage {
    quint8 id;
    quint8 method; // 0 - добавить сектор, 1 - удалить сектор
    float azimuth[2];
};


// ID: RMO_RARM_FINISH_CURRENT_FILE     = 11
struct RMOFinishCurrentFileMessage {
    timeval64 sTimeMeasurement; // Время создание сообщения
};

// ID: RMO_RARM_CONNECTION_CONTROL_WITH_POI     = 12
struct RMOConnectionControlMessage {
    timeval64 sTimeMeasurement; // Время создание сообщения
    quint8 connectionFlag;      // Разрешить/запретить подключение(1/0)
};

// ID: RMO_RARM_RECORDING_CONTROL     = 13
struct RMORecordingControlMessage {
    timeval64 sTimeMeasurement; // Время создание сообщения
    quint8 recordingFlag;      // Разрешить/запретить запись(1/0)
};

// ID: RMO_OTHER_FORGET_ALL_DATA     = 14
struct RMOForgetAllDataMessage {
    timeval64 sTimeMeasurement; // Время создание сообщения
};

// ID: RMO_POI_SIMULATOR_PLAY_MESSAGE          = 15,                   // Команда запуска/остановки работы имитатора ПОИ
// struct RMORecordingControlMessage {
//     timeval64 sTimeMeasurement; // Время создание сообщения
//     quint8 recordingFlag;      // Разрешить/запретить запись(1/0)
// };

// ID: RMO_POI_SIMULATOR_CHANGE_FILE_MESSAGE   = 16,                   // Команда смена файла для имитатора ПОИ



#endif // DATAMESSAGE_H
