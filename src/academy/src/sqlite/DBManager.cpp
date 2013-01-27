/*! \file DBManager.cpp
    \brief Менеджер БД
    \author Kiselev Kirill
    \date 08.07.2011    
*/

#include "DBManager.h"

DBManager::DBManager(const QString & file)
{
   connectDB(file);
}

DBManager::~DBManager()
{
   db_.close();
}

bool DBManager::connectDB(const QString & connect)
{
   db_is_good_ = false;

   //извлечь из DBSettingsGlobal
   QString driver;
   
   driver  = "QSQLITE";
   if (!QSqlDatabase::isDriverAvailable(driver))
   {
      QString err_msg = tr("Database driver not installed");
      lastError_ = QString("\"%1\" : %2").arg(driver).arg(err_msg);
      return false;
   }
   else
   {
      db_ = QSqlDatabase::addDatabase(driver, "STAT_DB");
      db_.setDatabaseName(connect);
      
      if (!db_.open())
      {
         QString err_msg = tr("Can't establish connection to");
         lastError_ = QString("%1 : %2").arg(err_msg).arg(connect);
         return false;
      }

      db_is_good_ = (CheckDBStruct() == 1);
  
   }
   return db_is_good_;
}

bool DBManager::CheckDBStruct()
{
   QStringList tables = db_.tables();
   //проверка таблицы Статистика
   if (tables.contains("PREFLOP"))
   {
      //проверить структуру таблицы - все ли нужные столбцы присутствуют
      QSqlRecord rec = db_.record("PREFLOP");
      if (!(rec.contains("ID")     &&
            rec.contains("NICK")   && // ник игрока
            rec.contains("CNT")    && // количество сыгранных им раздач
            rec.contains("FOLD")   && // количество фолдов
            rec.contains("VPIP")   && // участие в префлопе (без учета блайндов)
            rec.contains("PFR")    && // вход рейзом
            rec.contains("LIMP")   )) // вход лимпом
      {
         //таблица есть но ее структура не соответствует текущему положению дел
         //расширить таблицу недостающими столбцами
         bool resAlter = false;   
         if (!rec.contains("NICK"))
            resAlter = alterTable("PREFLOP", "NICK", "TEXT");
         if (!rec.contains("CNT"))
            resAlter = alterTable("PREFLOP", "CNT", "INTEGER");
         if (!rec.contains("FOLD"))
            resAlter = alterTable("PREFLOP", "FOLD", "INTEGER");
         if (!rec.contains("VPIP"))
            resAlter = alterTable("PREFLOP", "VPIP", "INTEGER");
         if (!rec.contains("PFR"))
            resAlter = alterTable("PREFLOP", "PFR", "INTEGER");
         if (!rec.contains("LIMP"))
            resAlter = alterTable("PREFLOP", "LIMP", "INTEGER");

         if (!resAlter)
         {
            QString err_msg = tr("The table %1 is obsolete").arg("PREFLOP");
            lastError_ = QString("%1").arg(err_msg);
            return false;
         }
      }
   }
   else//таблицы не существует (БД новая)
   {
      QString create_sql = QString("CREATE TABLE PREFLOP (ID INTEGER PRIMARY KEY, NICK TEXT, CNT INTEGER, FOLD INTEGER, VPIP INTEGER, PFR INTEGER, LIMP INTEGER)");
      db_.exec(create_sql);
      if (db_.lastError().type() != QSqlError::NoError)
      {
         QString err_msg = tr("Can't create table %1").arg("PREFLOP");
         lastError_ = QString("%1 : %2").arg(err_msg).arg(QSqlDatabase::database().lastError().text());
         return false;
      }
   }

   QString create_idx = QString("CREATE INDEX IDX_PREFLOP_ID ON PREFLOP (ID)");
   db_.exec(create_idx);

   create_idx = QString("CREATE INDEX IDX_NICK ON PREFLOP (NICK)");
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

bool DBManager::clearTable(const QString & tableName)
{
   QString del_sql = QString("DELETE FROM %1").arg(tableName);
   QString vac_sql = QString("VACUUM %1").arg(tableName);

   db_.exec(del_sql);
   db_.exec(vac_sql);
   
   return !db_.lastError().isValid();
}
