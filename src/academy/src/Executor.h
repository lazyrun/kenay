#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <windows.h>
#include "SettingsData.h"
#include "acadlib.h"
#include "ProcAcad.h"

class CardBase;
class AlarmWidget;
class MindFL6max;
class Clicker;

//class DBManager;

class Executor : public QObject
{
Q_OBJECT
public:
   Executor(showMessageCBFunc fun, QObject * parent = 0);
   Executor(QObject * parent = 0);
   ~Executor();
   
   typedef QList<Opp::Action> ActionList;
   struct Session
   {
      QString sessionID_;
      QMap<CardProcessing::Street, QMap<int, ActionList> > history_;
   };

public slots:
   void start();
   void stop();
   void exit();
protected:
   int timer_id_;
   int interval_;
   int screen_res_x_;
   int screen_res_y_;

   QString cache_;
   ProcAcad * cardProc_;
   MindFL6max * mind_;
   Clicker * clicker_;

   //DBManager * dbManager_;
   CardBase * cardBase_;
   QStringList playingCard_;
   SettingsData data_;
   AlarmWidget * alarm_;
   bool lastIsFold_;
protected:
   void init();
   //таймер
   void timerEvent(QTimerEvent * te);
   HWND findTables(const QString & tClass, HWND BeginHandle);
   void HwndToTop(WId hwnd);
   void clickCheck(WId hwnd);
   void clickFold(WId hwnd);
   void clickTo(WId hwnd, const QRect & rect);
   void clickTo(WId hwnd, int x, int y);
   void foldOrCheck(WId hwnd, bool realClick = true);
   //QString cardFromHash(unsigned long long hash);
   QString cardFromImage(QImage & img);
   QString cardString(int nom, int suit);
   QString cardRangeFromHoles(const QString & first, const QString & second);
   
   void saveStats(const QString & session);
   
   Session currentSession_;
   //ActionList oppAction(const QString & sid, 
   //   CardProcessing::Street street, 
   //   int oppPos);
   //void appendOppAction(const QString & sid, CardProcessing::Street street,
   //   int oppPos, Action act);
   
   // активность оппа5 на префлопе
   //история
   /*
   сессия AsQd
     Префлоп
        опп1
           рейз, рейз
        опп2 
           фолд
        опп3
           фолд
        круг1
           опп1 - сб
           опп2 - бб
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
        круг2
           опп1 - фолд
           опп2 - фолд
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
        круг3
           опп1 - фолд
           опп2 - фолд
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
     Флоп
        круг1
           опп1 - сб
           опп2 - бб
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
        круг2
           опп1 - фолд
           опп2 - фолд
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз
        круг3
           опп1 - фолд
           опп2 - фолд
           опп3 - фолд
           опп4 - фолд
           опп5 - рейз


   */
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
