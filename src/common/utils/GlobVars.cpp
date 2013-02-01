/*! \file GlobVars.cpp
    \brief Класс, содержащий глобальные переменные
*/

#include "GlobVars.h"

void CGlobal::Init()
{
   const QString protocolPath   = "protocol";
   const QString protocolFile   = "protocol.log";
   proto_size_ = 5 * 1024 * 1024;

   // 0 - 9
   dbg_level_ = 5;
   
   QDir home(QDir::current());
   // Каталоги с файлами протоколов
   protoDir_ = QString("%1/%2").arg(home.path()).arg(protocolPath);
   // Имя файла протокола
   protoFile_ = QString("%1/%2").arg(protoDir_).arg(protocolFile);

   ap5(QString("protocol file = %1").arg(protoFile_));
}


const QString & CGlobal::ProtoDir(void) const
{
   return protoDir_;
}

CGlobal::~CGlobal()
{
   ap5(QString("CGlobal::~CGlobal"));
}

void CGlobal::SetDbg(int new_level)
{
   ap9(QString("Debug Level set to %1").arg(new_level));
   QMutexLocker locker(&proto_mtx_);
   dbg_level_ = new_level;
}

void CGlobal::SetProtoSize(int new_size)
{
   QMutexLocker locker(&proto_mtx_);
   proto_size_ = new_size;
}
void CGlobal::ap(const QString& msg)
{
   if(protoFile_.length()==0)
   {
      return;
   }
   QMutexLocker locker(&proto_mtx_);
   QFile proto_file(protoFile_);
   if(proto_file.size()>proto_size_)
   {
      QString f_name = proto_file.fileName();
      QString d_time = QDateTime::currentDateTime().toString(Qt::ISODate);
      d_time.replace(QString("-"), QString("_"));
      d_time.replace(QString(":"), QString("_"));
      QString copy_name = QString("%1/%2.log").arg(protoDir_).arg(d_time);
      bool check = proto_file.copy(copy_name);
      QFile::FileError err = proto_file.error();
      check = proto_file.resize(0);
      err = proto_file.error();
   }
   proto_file.open(QIODevice::Append|QIODevice::Text);
   long written = proto_file.write(QDateTime::currentDateTime()
      .toString(Qt::LocalDate).toLocal8Bit()+" ");
   written = proto_file.write(msg.toLocal8Bit());
   proto_file.write("\n");
   proto_file.close();
}

void CGlobal::ap0(const QString& msg)
{
   if(dbg_level_ >= 0)
   {
      ap(msg);
   }
}

void CGlobal::ap1(const QString& msg)
{
   if(dbg_level_ >= 1)
   {
      ap(msg);
   }
}

void CGlobal::ap2(const QString& msg)
{
   if(dbg_level_ >= 2)
   {
      ap(msg);
   }
}

void CGlobal::ap3(const QString& msg)
{
   if(dbg_level_ >= 3)
   {
      ap(msg);
   }
}

void CGlobal::ap4(const QString& msg)
{
   if(dbg_level_ >= 4)
   {
      ap(msg);
   }
}

void CGlobal::ap5(const QString& msg)
{
   if(dbg_level_ >= 5)
   {
      ap(msg);
   }
}

void CGlobal::ap6(const QString& msg)
{
   if(dbg_level_ >= 6)
   {
      ap(msg);
   }
}

void CGlobal::ap7(const QString& msg)
{
   if(dbg_level_ >= 7)
   {
      ap(msg);
   }
}

void CGlobal::ap8(const QString& msg)
{
   if(dbg_level_ >= 8)
   {
      ap(msg);
   }
}

void CGlobal::ap9(const QString & msg)
{
   if (dbg_level_ >= 9)
   {
      ap(msg);
   }
}

