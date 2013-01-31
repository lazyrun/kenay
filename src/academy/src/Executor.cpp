#include "Executor.h"
#include "ProcAcad.h"
#include "MindFL6max.h"
#include "MindFLfull.h"
#include "Clicker.h"
#include "DBManager.h"
#include "Session.h"

#ifdef EXECUTOR_EXE
#include "qxtglobalshortcut.h"
#endif

#define MAX_WND_TEXT 1000

static showMessageCBFunc cbFun;

Executor::Executor(showMessageCBFunc fun, QObject * parent)
: QObject(parent)
{
   cbFun = fun;
   init();
}

Executor::Executor(QObject * parent)
: QObject(parent)
{
   init();
}

void Executor::init()
{
#ifdef EXECUTOR_EXE
   //установка глобальных хоткеев
   QxtGlobalShortcut * startShortcut = new QxtGlobalShortcut(this);
   startShortcut->setShortcut(QKeySequence("Ctrl+F5"));
   connect(startShortcut, SIGNAL(activated()), 
      this, SLOT(start()));

   QxtGlobalShortcut *stopShortcut = new QxtGlobalShortcut(this);
   stopShortcut->setShortcut(QKeySequence("Ctrl+F6"));
   connect(stopShortcut, SIGNAL(activated()), 
      this, SLOT(stop()));
   
   QxtGlobalShortcut *exitShortcut = new QxtGlobalShortcut(this);
   exitShortcut->setShortcut(QKeySequence("Ctrl+F10"));
   connect(exitShortcut, SIGNAL(activated()), 
      this, SLOT(exit()));
#endif

   cardProc_ = new ProcAcad("map/acad_fr.xml");
   clicker_ = new Clicker(cardProc_);
   dbManager_ = new DBManager("stat/stat_acadfr.db");
   session_ = new Session(cardProc_, 10);
   mind_ = new MindFLfull(cardProc_, session_);
}

Executor::~Executor()
{
   delete cardProc_;
   delete mind_;
   delete clicker_;
   delete dbManager_;
   delete session_;
}

void Executor::start()
{
   interval_ = 1000;//data_.interval;
   //запускаем таймер
   timer_id_ = startTimer(interval_);
}

void Executor::stop()
{
   if (timer_id_)
      killTimer(timer_id_);
}

void Executor::exit()
{
   QCoreApplication::quit();
}

void Executor::timerEvent(QTimerEvent *)
{
   TimerKiller killer(this, timer_id_, interval_);
   
   //ищем топовое окно заданного класса
   static const QString psClass = "SunAwtFrame";

   //смотрим топовое окно
   WId FgWnd = GetForegroundWindow();
   char clName[MAX_WND_TEXT];
   GetClassNameA(FgWnd, clName, MAX_WND_TEXT);
   QString sClass(clName);
   if (sClass != psClass)
   {
      return;
   }

   Clicker::HwndToTop(FgWnd);

   QPixmap pixTable = QPixmap::grabWindow(FgWnd);
   QImage imgTable  = pixTable.toImage();

   cardProc_->setImage(imgTable);
   if (cardProc_->hasFold())
   {
      //наш ход
      //imgTable.save("table.bmp");
      Solution sol = mind_->think();
      clicker_->setWinId(FgWnd);
      clicker_->click(sol);
      
      //clickFold(FgWnd);
      //cbFun(QString(sAction + card1 + " " + card2).toStdString().c_str());
   }
}

/*
HWND Executor::findTables(const QString & tClass, HWND BeginHandle)
{
   //найти окно
   HWND CurrWnd = NULL;
   CurrWnd = GetWindow(BeginHandle, GW_HWNDFIRST);
   
   while (CurrWnd) 
   {
      char clName[MAX_WND_TEXT];
      GetClassNameA(CurrWnd, clName, MAX_WND_TEXT);

      QString sClass(clName);
      if (sClass == tClass)
      {
         //вернуть первое 
         //оно первое в z порядке
         return CurrWnd;
      }
      CurrWnd = GetWindow(CurrWnd, GW_HWNDNEXT);
   }
   return NULL;
}
*/
