#include "Executor.h"
#include "ProcAcad.h"
#include "Hooker.h"
#include "AlarmWidget.h"
#include "MindFL6max.h"
#include "MindFLfull.h"
#include "Clicker.h"
#include "DBManager.h"
#include "Session.h"

#ifdef EXECUTOR_EXE
#include "qxtglobalshortcut.h"
#endif

#define MAX_WND_TEXT 1000
#define ABSOLUTE_COORD 65535

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
   qRegisterMetaType<SettingsData>("SettingsData");
   qRegisterMetaTypeStreamOperators<SettingsData>("SettingsData");

#ifdef EXECUTOR_EXE
   //��������� ���������� �������
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

   screen_res_x_ = GetSystemMetrics(SM_CXSCREEN);//�������� ������ ������
   screen_res_y_ = GetSystemMetrics(SM_CYSCREEN);//�������� ������ ������

   cardBase_ = new CardBase();
   cardProc_ = new ProcAcad("map/acad_fr.xml");
   alarm_ = new AlarmWidget();
   clicker_ = new Clicker();
   dbManager_ = new DBManager("stat/stat_acad6max.db");
   session_ = new Session(cardProc_, 10);
   mind_ = new MindFLfull(cardProc_, session_);
}

Executor::~Executor()
{
   delete cardBase_;
   delete cardProc_;
   delete alarm_;
   delete mind_;
   delete clicker_;
   delete dbManager_;
   delete session_;
}

void Executor::start()
{
   //������ �������� �� �������������
   //QSettings settings("Holdem Folder", "Config");
   QString regKey = QDir::current().dirName();
   QSettings settings(regKey, "Config");

   QString range = settings.value("CardRange").toString();
   playingCard_ = range.split(", ", QString::SkipEmptyParts);
   QVariant sett = settings.value("Settings");
   data_ = qvariant_cast<SettingsData>(sett);

   interval_ = 1000;//data_.interval;

   lastIsFold_ = false;
   //��������� ������
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
   
   //���� ������� ���� ��������� ������
   static const QString psClass = "SunAwtFrame";

   //������� ������� ����
   WId FgWnd = GetForegroundWindow();
   char clName[MAX_WND_TEXT];
   GetClassNameA(FgWnd, clName, MAX_WND_TEXT);
   QString sClass(clName);
   if (sClass != psClass)
   {
      return;
   }

   HwndToTop(FgWnd);

   QPixmap pixTable = QPixmap::grabWindow(FgWnd);
   QImage imgTable  = pixTable.toImage();

   cardProc_->setImage(imgTable);
   if (cardProc_->hasFold())
   {
      //��� ���
      //imgTable.save("table.bmp");
      Solution sol = mind_->think();

      //QString card1 = cardProc_->holeCard("first");
      //QString card2 = cardProc_->holeCard("second");
      //session_->saveStats(card1 + card2);
      clickFold(FgWnd);
   }
#if 0
   //��� ���
   // ������������� ����������� ��� ���������
   //�������� �����������
   HwndToTop(FgWnd);
   RECT roomRect;
   GetClientRect(FgWnd, &roomRect);
   
   int start_x = 0;
   int start_y = 0;
   const int width = roomRect.right - roomRect.left/* - 210*/;
   const int height = roomRect.bottom - roomRect.top/* - 60*/;
   
   QPixmap pixRoom = QPixmap::grabWindow(FgWnd, start_x, start_y,
      width, height);
   QImage imgRoom  = pixRoom.toImage();
   
   imgRoom.save("1.bmp");

   cardProc_->setImage(imgRoom);
   //CardProcessing::HoldemLevel hl = cardProc_->holdemLevel();
   // ���� ������ �������� 
   if (cardProc_->isPreflop())
   {
      // �������� ��������� �����
      bool ok = false;
      QPair<QRect, QRect> holeCards = cardProc_->getHoleCards(&ok);
      if (!ok)
         return;
      
      QImage firstImg  = imgRoom.copy(holeCards.first);
      //firstImg.save("1.bmp");
      //QImage firstImg("Jd_blue.bmp");
      QImage secondImg = imgRoom.copy(holeCards.second);
      //secondImg.save("2.bmp");
      //QImage secondImg("Jd_red.bmp");
      //toBlackWhite(firstImg, 200);
      //toBlackWhite(secondImg, 200);

      //debugging{
      //uint num = 0;
      //num = QDateTime::currentDateTime().toTime_t();
      //QString path1 = QString("test/f%1.bmp").arg(num);
      //QString path2 = QString("test/s%1.bmp").arg(num);
      //firstImg.save(path1);
      //secondImg.save(path2);
      //}debugging

      QString card1 = cardFromImage(firstImg);
      QString card2 = cardFromImage(secondImg);
      //��������� ����� � ���
      QString joined = card1 + card2;
      if (joined == cache_ && !lastIsFold_)
      {
         //������� ����� - ������� ������, �� ������
         return;
      } 
      else
      {
         cache_ = joined;
      }
      //qDebug() << card1 << card2;

      /// Debugging {
      //uint num = 0;
      //num = QDateTime::currentDateTime().toTime_t();
      //imgRoom.save(QString("test/table_%1.bmp").arg(num));
      /// } Debugging

      /// Debugging {
      //imgRoom.copy(holeCards.first).save(QString("test/f%1.bmp").arg(num));
      //imgRoom.copy(holeCards.second).save(QString("test/s%1.bmp").arg(num));
      /// } Debugging
      //return;
      
      //������������� �����
      QString range = cardRangeFromHoles(card1, card2);
      //qDebug() << range;
      if (playingCard_.contains(range))
      {
         //panic!
         lastIsFold_ = false;
         if (data_.visualAlert)
         {
            alarm_->highlight(joined, FgWnd);
         }

         if (data_.turnBeep)
         {
            QString appExe = qApp->applicationDirPath();
            QSound::play(appExe + "/sounds/turn.wav");
         }
      }
      else
      {
         if (data_.advisorMode)
         {
            foldOrCheck(FgWnd, false);
            lastIsFold_ = false;
            cache_ = joined;

            QString advice = tr("Fold/Check this hand: %1")
               .arg(card1 + " " + card2);
            cbFun(advice.toStdString().c_str());
         }
         else
         {
            foldOrCheck(FgWnd);

            if (data_.showFolded)
            {
               QString sAction = tr("This hand has been folded: ");
               if (!lastIsFold_)
                  sAction = tr("This hand has been checked: ");
               cbFun(QString(sAction + card1 + " " + card2).toStdString().c_str());
            }
         }
      }
   }
#endif
}

HWND Executor::findTables(const QString & tClass, HWND BeginHandle)
{
   //����� ����
   HWND CurrWnd = NULL;
   CurrWnd = GetWindow(BeginHandle, GW_HWNDFIRST);
   
   while (CurrWnd) 
   {
      char clName[MAX_WND_TEXT];
      GetClassNameA(CurrWnd, clName, MAX_WND_TEXT);

      QString sClass(clName);
      if (sClass == tClass)
      {
         //������� ������ 
         //��� ������ � z �������
         return CurrWnd;
      }
      CurrWnd = GetWindow(CurrWnd, GW_HWNDNEXT);
   }
   return NULL;
}

void Executor::HwndToTop(WId hwnd)
{
   BringWindowToTop(hwnd);
   SetActiveWindow(hwnd);
   SetFocus(hwnd);
   Sleep(100);
}

void Executor::clickCheck(WId hwnd)
{
   const QRect checkRect = cardProc_->checkRect();
   clickTo(hwnd, checkRect);
}

void Executor::clickFold(WId hwnd)
{
   const QRect foldRect = cardProc_->foldRect();
   clickTo(hwnd, foldRect);
}

void Executor::clickTo(WId hwnd, const QRect & rect)
{
   int x = rect.left(), y = rect.top();
   x += qrand() % rect.width();
   y += qrand() % rect.height();
   clickTo(hwnd, x, y);
}

void Executor::clickTo(WId hwnd, int x, int y)
{
   RECT rect;
   GetWindowRect(hwnd, &rect);

   static int x_border = 0;
   static int y_border = 0;
   if (x_border == 0)
   {
      QRect wndRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
      RECT clrect;
      GetClientRect(hwnd, &clrect);
      QRect clntRect(clrect.left, clrect.top, clrect.right - clrect.left, 
         clrect.bottom - clrect.top);
      x_border = (int)(wndRect.width() - clntRect.width()) / 2.;
      y_border = wndRect.height() - clntRect.height() - x_border;
   }

   x += rect.left + x_border;
   y += rect.top + y_border;
   HwndToTop(hwnd);

   long X = (long)(x * ABSOLUTE_COORD)/screen_res_x_;
   long Y = (long)(y * ABSOLUTE_COORD)/screen_res_y_;

   Hooker hook(Hooker::Mouse);
   //��������� ������� ���������� �����
   POINT pt;
   GetCursorPos(&pt);
   long cX = (long)(pt.x * ABSOLUTE_COORD)/screen_res_x_;
   long cY = (long)(pt.y * ABSOLUTE_COORD)/screen_res_y_;
   
   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, X, Y, 0, 0);
   Sleep(100);
   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, X, Y, 0, 0);
   Sleep(100);
   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, X, Y, 0, 0);

   //������� ���������� ����� ������ �����
   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, cX, cY, 0, 0);

}

void Executor::foldOrCheck(WId hwnd, bool realClick)
{
#if 0
   //������, ���� ����� - ������
   //�������� ����� � ������ ���
   //� ������� �� ����������
   static int checkRect_x = 354;
   static int checkRect_y = 656;
   static int checkRect_w = 100;
   static int checkRect_h = 20;
   HwndToTop(hwnd);
   QPixmap pixCheck = QPixmap::grabWindow(hwnd, checkRect_x, checkRect_y, checkRect_w, checkRect_h);
   QImage imgCheck  = pixCheck.toImage();
   
   int checkLetters = ProcAcad::countCheckLetters(imgCheck);
   QString appExe = qApp->applicationDirPath();
   if ((checkLetters == 5) || (checkLetters == 3))//check / ���
   {
      if (data_.checkBeep)
      {
         QSound::play(appExe + "/sounds/check.wav");
      }
      if (realClick)
      {
         //������ ���
         clickCheck(hwnd);
      }
      //�������� ���
      cache_.clear();
      lastIsFold_ = false;

   }
   else
   {
      lastIsFold_ = true;
      if (data_.foldBeep)
      {
         QSound::play(appExe + "/sounds/fold.wav");
      }

      if (realClick)
      {
         //������ ����
         clickFold(hwnd);
      }
   }
#endif
}
