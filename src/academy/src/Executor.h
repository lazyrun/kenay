#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <qt_windows.h>
#include "acadlib.h"
#include "ProcAcad.h"

class CardBase;
class MindFL6max;
class MindFLfull;
class Clicker;
class DBManager;
class Session;

class Executor : public QObject
{
Q_OBJECT
public:
   Executor(showMessageCBFunc fun, QObject * parent = 0);
   Executor(QObject * parent = 0);
   ~Executor();
   
public slots:
   void start();
   void stop();
   void exit();
   void startRoom();
protected:
   int timer_id_;
   int interval_;

   ProcAcad * cardProc_;
   MindFLfull * mind_;
   Clicker * clicker_;
   DBManager * dbManager_;
   Session * session_;
protected:
   void init();
   //таймер
   void timerEvent(QTimerEvent * te);
   //HWND findTables(const QString & tClass, HWND BeginHandle);
};

//
class TimerKiller
{
public:
   TimerKiller(QObject * obj, int & tid, int interval)
      : interval_(interval), timer_id_(&tid), obj_(obj)
   {
      obj_->killTimer(tid);
      resp_ = true;
   }
   ~TimerKiller()
   {
      if (resp_)
         (*timer_id_) = obj_->startTimer(interval_);
   }
   void kill() { resp_ = false; }
protected:
   int interval_;
   int * timer_id_;
   QObject * obj_;
   bool resp_;
};

#endif
