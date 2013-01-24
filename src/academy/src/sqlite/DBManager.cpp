/*! \file DBManager.cpp
    \brief Менеджер БД
    \author Kiselev Kirill
    \date 08.07.2011    
*/

#include "DBManager.h"

DBManager::DBManager()
{
   connectDB();
}

DBManager::~DBManager()
{
   db_.close();
}

bool DBManager::connectDB()
{
   db_is_good_ = false;

   //извлечь из DBSettingsGlobal
   std::string driver;
   std::string connect;
   std::string user;
   std::string passwd;
   
   driver  = "QSQLITE";
   connect = "stat.db";
   user    = "";
   passwd  = "";
      
   if (!QSqlDatabase::isDriverAvailable(driver.c_str()))
   {
      QString err_msg = tr("Database driver not installed");
      lastError_ = QString("\"%1\" : %2").arg(QString::fromStdString(driver)).arg(err_msg);
      return false;
   }
   else
   {
      db_ = QSqlDatabase::addDatabase(QString::fromStdString(driver));
      db_.setDatabaseName(QString::fromStdString(connect));
      
      if (user.length())
      {
         db_.setUserName(QString::fromStdString(user));
      }
      if (passwd.length())
      {
         db_.setPassword(QString::fromStdString(passwd));
      }
      if (!db_.open())
      {
         QString err_msg = tr("Can't establish connection to");
         lastError_ = QString("%1 : %2").arg(err_msg).arg(QString::fromStdString(connect));
         return false;
      }
      fprintf(stdout, "%s %s %s %s\n", "Database connected: driver", driver.c_str(), "connection", connect.c_str());

      db_is_good_ = (CheckDBStruct() == 1);
   
      //получение хэндла для чисто сишных функций
      QVariant v = db_.driver()->handle();
      if (v.isValid() && qstrcmp(v.typeName(), "sqlite3*")==0)
      {
         // v.data() возвращает указатель на хэндл
         sqlite3 *handle = *static_cast<sqlite3 **>(v.data());
         if (handle != 0) 
         {
            //здесь нужно переопределить функцию upper
            //ибо она некорректно работает с регистром русских букв
            sqlite3_initialize();

            sqlite3_create_function(handle, "UPPER", 1, SQLITE_UTF8, NULL,
                                    upper_replace, NULL, NULL);
            
            //добавляем пользовательскую функцию IF
            sqlite3_create_function(handle, "IF", 2, SQLITE_UTF8, NULL,
                                    if_replace, NULL, NULL);

         }
      }
   }
   return db_is_good_;
}

bool DBManager::CheckDBStruct()
{
   QStringList tables = db_.tables();
   //проверка таблицы Статистика
   if (tables.contains("VRF_STATISTICS"))
   {
      //проверить структуру таблицы - все ли нужные столбцы присутствуют
      QSqlRecord rec = db_.record("VRF_STATISTICS");
      if (!(rec.contains("ID")          &&
            rec.contains("REQ_START")   &&
            rec.contains("LOCAL_DB")    &&
            rec.contains("REMOTE_DB")   &&
            rec.contains("REQ_USR")     &&
            rec.contains("LOCAL_IP")    &&
            rec.contains("CLIENT_ID")   &&
            rec.contains("REMOTE_HOST") &&
            rec.contains("REQ_TXT")     &&
            rec.contains("REQ_FINISH")  &&
            rec.contains("N_REPLY")     &&
            rec.contains("REPLY_TXT")   &&
            rec.contains("ERR")         &&
            rec.contains("STRERR")      &&
            rec.contains("RMASK")       &&
            rec.contains("QMASK")       &&
            rec.contains("CMASK")       &&
            rec.contains("DBSIZE")      &&
            rec.contains("FPOINTS")     &&
            rec.contains("QUALITY")     &&
            rec.contains("STATION_NAME")&&
            rec.contains("FTXT110")     &&
            rec.contains("FTXT111")     &&
            rec.contains("FTXT112")     &&
            rec.contains("FTXT108")     &&
            rec.contains("FTXT107")     &&
            rec.contains("FTXT113")     &&
            rec.contains("FTXT114")     &&
            rec.contains("FTXT116")     &&
            rec.contains("FTXT117")     &&
            rec.contains("FTXT118")     &&
            rec.contains("FTXT119")     &&
            rec.contains("FTXT120")     &&
            rec.contains("FTXT121")     &&
            rec.contains("FTXT459")     ))
      {
         //таблица есть но ее структура не соответствует текущему положению дел
         //расширить таблицу недостающими столбцами
         bool resAlter = false;   
         if (!rec.contains("RMASK"))
            resAlter = alterTable("VRF_STATISTICS", "RMASK", "TEXT");
         if (!rec.contains("QMASK"))
            resAlter = alterTable("VRF_STATISTICS", "QMASK", "TEXT");
         if (!rec.contains("CMASK"))
            resAlter = alterTable("VRF_STATISTICS", "CMASK", "INTEGER");
         if (!rec.contains("DBSIZE"))
            resAlter = alterTable("VRF_STATISTICS", "DBSIZE", "INTEGER");
         if (!rec.contains("FPOINTS"))
            resAlter = alterTable("VRF_STATISTICS", "FPOINTS", "INTEGER");
         if (!rec.contains("QUALITY"))
            resAlter = alterTable("VRF_STATISTICS", "QUALITY", "INTEGER");
         if (!rec.contains("STATION_NAME"))
            resAlter = alterTable("VRF_STATISTICS", "STATION_NAME", "VARCHAR(256)");
         if (!rec.contains("FTXT110"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT110", "VARCHAR(256)");
         if (!rec.contains("FTXT111"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT111", "VARCHAR(256)");
         if (!rec.contains("FTXT112"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT112", "VARCHAR(256)");
         if (!rec.contains("FTXT108"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT108", "VARCHAR(256)");
         if (!rec.contains("FTXT107"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT107", "VARCHAR(256)");
         if (!rec.contains("FTXT113"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT113", "VARCHAR(256)");
         if (!rec.contains("FTXT114"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT114", "VARCHAR(256)");
         if (!rec.contains("FTXT116"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT116", "VARCHAR(256)");
         if (!rec.contains("FTXT117"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT117", "VARCHAR(256)");
         if (!rec.contains("FTXT118"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT118", "VARCHAR(256)");
         if (!rec.contains("FTXT119"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT119", "VARCHAR(256)");
         if (!rec.contains("FTXT120"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT120", "VARCHAR(256)");
         if (!rec.contains("FTXT121"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT121", "VARCHAR(256)");
         if (!rec.contains("FTXT459"))
            resAlter = alterTable("VRF_STATISTICS", "FTXT459", "VARCHAR(256)");

         if (!resAlter)
         {
            QString err_msg = tr("The table %1 is obsolete").arg("VRF_STATISTICS");
            lastError_ = QString("%1").arg(err_msg);
            return false;
         }
      }
   }
   else//таблицы не существует (БД новая)
   {
      QString create_sql = QString("CREATE TABLE VRF_STATISTICS (ID VARCHAR(256) PRIMARY KEY, REQ_START TIMESTAMP, LOCAL_DB VARCHAR(256), REMOTE_DB VARCHAR(2048), REQ_USR VARCHAR(256), LOCAL_IP VARCHAR(256), CLIENT_ID VARCHAR(256), REMOTE_HOST VARCHAR(256), REQ_TXT VARCHAR(2048), REQ_FINISH TIMESTAMP, N_REPLY INTEGER, REPLY_TXT VARCHAR(6400), ERR VARCHAR(256), STRERR VARCHAR(1024), RMASK TEXT, QMASK TEXT, CMASK INTEGER, DBSIZE INTEGER, FPOINTS INTEGER, QUALITY INTEGER, STATION_NAME VARCHAR(256), FTXT110 VARCHAR(256), FTXT111 VARCHAR(256), FTXT112 VARCHAR(256), FTXT108 VARCHAR(256), FTXT107 VARCHAR(256), FTXT113 VARCHAR(256), FTXT114 VARCHAR(256), FTXT116 VARCHAR(256), FTXT117 VARCHAR(256), FTXT118 VARCHAR(256), FTXT119 VARCHAR(256), FTXT120 VARCHAR(256), FTXT121 VARCHAR(256), FTXT459 VARCHAR(256))");
      db_.exec(create_sql);
      if (db_.lastError().type() != QSqlError::NoError)
      {
         QString err_msg = tr("Can't create table %1").arg("VRF_STATISTICS");
         lastError_ = QString("%1 : %2").arg(err_msg).arg(QSqlDatabase::database().lastError().text());
         return false;
      }
   }

   //проверка таблицы Ошибки
   if (tables.contains("VRF_FAILURES"))
   {
      //проверить структуру таблицы - все ли нужные столбцы присутствуют
      QSqlRecord rec = db_.record("VRF_FAILURES");

      if (!(rec.contains("ID")          &&
            rec.contains("REQ_START")   &&
            rec.contains("LOCAL_DB")    &&
            rec.contains("REMOTE_DB")   &&
            rec.contains("REQ_USR")     &&
            rec.contains("LOCAL_IP")    &&
            rec.contains("CLIENT_ID")   &&
            rec.contains("REMOTE_HOST") &&
            rec.contains("ERR")         &&
            rec.contains("STRERR")      ))
      {
         //таблица есть но ее структура не соответствует текущему положению дел
         QString err_msg = tr("The table %1 is obsolete").arg("VRF_FAILURES");
         lastError_ = QString("%1").arg(err_msg);
         return false;
      }
   }
   else
   {
      //Таблицы не существует (БД новая)
      QString create_sql = QString("CREATE TABLE VRF_FAILURES (ID VARCHAR(256) PRIMARY KEY, REQ_START TIMESTAMP, LOCAL_DB VARCHAR(256), REMOTE_DB VARCHAR(2048), REQ_USR VARCHAR(256), LOCAL_IP VARCHAR(256), CLIENT_ID VARCHAR(256), REMOTE_HOST VARCHAR(256), ERR VARCHAR(1024), STRERR VARCHAR(256))");
      db_.exec(create_sql);
      if (db_.lastError().type() != QSqlError::NoError)
      {
         QString err_msg = tr("Can't create table %1").arg("VRF_FAILURES");
         lastError_ = QString("%1 : %2").arg(err_msg).arg(QSqlDatabase::database().lastError().text());
         return false;
      }
   }
   //проверка таблицы Кандидаты
   if (tables.contains("VRF_CANDIDATES"))
   {
      //проверить структуру таблицы - все ли нужные столбцы присутствуют
      QSqlRecord rec = db_.record("VRF_CANDIDATES");

      if (!(rec.contains("ID")      &&
            rec.contains("ID_STAT") &&
            rec.contains("DB_ID")   &&
            rec.contains("SEG_ID")  &&
            rec.contains("FILE_ID") &&
            rec.contains("FTXT103") &&
            rec.contains("FTXT105") &&
            rec.contains("FTXT107") &&
            rec.contains("FTXT108") &&
            rec.contains("FTXT110") &&
            rec.contains("FTXT111") && 
            rec.contains("FTXT112") &&
            rec.contains("FTXT113") &&
            rec.contains("FTXT116") && 
            rec.contains("FTXT117") && 
            rec.contains("FTXT118") && 
            rec.contains("FTXT121") &&
            rec.contains("PHOTO")   ))
      {
         //таблица есть но ее структура не соответствует текущему положению дел
         bool resAlter = false;
         if (!rec.contains("PHOTO"))
            resAlter = alterTable("VRF_CANDIDATES", "PHOTO", "BLOB");
         
         if (!resAlter)
         {
            QString err_msg = tr("The table %1 is obsolete").arg("VRF_CANDIDATES");
            lastError_ = QString("%1").arg(err_msg);
            return false;
         }
      }
   }
   else
   {
      //Таблицы не существует (БД новая)
      //ID_STAT - ID внешней таблицы
      QString create_sql = QString("CREATE TABLE VRF_CANDIDATES (ID VARCHAR(256) PRIMARY KEY, ID_STAT VARCHAR(256), DB_ID VARCHAR(256), SEG_ID VARCHAR(256), FILE_ID VARCHAR(256), FTXT103 VARCHAR(256), FTXT105 VARCHAR(256), FTXT107 VARCHAR(256), FTXT108 VARCHAR(256), FTXT110 VARCHAR(256), FTXT111 VARCHAR(256), FTXT112 VARCHAR(256), FTXT113 VARCHAR(256),FTXT116 VARCHAR(256), FTXT117 VARCHAR(256), FTXT118 VARCHAR(256), FTXT121 VARCHAR(6400), PHOTO BLOB, FOREIGN KEY (ID_STAT) REFERENCES VRF_STATISTICS (ID))");
      
      db_.exec(create_sql);
      if (db_.lastError().type() != QSqlError::NoError)
      {
         QString err_msg = tr("Can't create table %1").arg("VRF_CANDIDATES");
         lastError_ = QString("%1 : %2").arg(err_msg).arg(QSqlDatabase::database().lastError().text());
         return false;
      }
   }
   
   //проверка таблицы Файлы
   if (tables.contains("VRF_FILES"))
   {
      //проверить структуру таблицы - все ли нужные столбцы присутствуют
      QSqlRecord rec = db_.record("VRF_FILES");
      if (!(rec.contains("ID")       &&
            rec.contains("MD5_HASH") &&
            rec.contains("FILE")     &&
            rec.contains("PATH")  ))
      {
         //таблица есть но ее структура не соответствует текущему положению дел
         QString err_msg = tr("The table %1 is obsolete").arg("VRF_FILES");
         lastError_ = QString("%1").arg(err_msg);
         return false;
      }
   }
   else
   {
      //Таблицы не существует (БД новая)
      QString create_sql = QString("CREATE TABLE VRF_FILES (ID INTEGER PRIMARY KEY, MD5_HASH VARCHAR(64), FILE VARCHAR(256), PATH VARCHAR(256))");
      
      db_.exec(create_sql);
      if (db_.lastError().type() != QSqlError::NoError)
      {
         QString err_msg = tr("Can't create table %1").arg("VRF_FILES");
         lastError_ = QString("%1 : %2").arg(err_msg).arg(QSqlDatabase::database().lastError().text());
         return false;
      }
   }

   QString create_idx = QString("CREATE INDEX IDX_STATISTICS_ID ON VRF_STATISTICS (ID)");
   db_.exec(create_idx);

   create_idx = QString("CREATE INDEX IDX_STATISTICS_REQSTART ON VRF_STATISTICS (REQ_START)");
   db_.exec(create_idx);

   create_idx = QString("CREATE INDEX IDX_CANDIDATES_IDSTAT ON VRF_CANDIDATES (ID_STAT)");
   db_.exec(create_idx);

   create_idx = QString("CREATE INDEX IDX_FILES ON VRF_FILES (ID)");
   db_.exec(create_idx);

   return true;
}

bool DBManager::alterTable(const QString & tableName,
                           const QString & columnName,
                           const QString & columnType)
{
   QString alter_sql = QString("ALTER TABLE %1 ADD %2 %3").arg(tableName).arg(columnName).arg(columnType);
      
   db_.exec(alter_sql);
   QString se = db_.lastError().text();
   return se.trimmed().isEmpty();
}

