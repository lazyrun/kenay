#include "Agent.h"
#include "qxtglobalshortcut.h"
//#include "SettingsData.h"

static QSystemTrayIcon * trayIcon_;

void CALLBACK showMessageCallbackFunc(const char * hand)
{
   trayIcon_->showMessage("Kenay", 
      QString(hand),
      QSystemTrayIcon::Information, 1000);
}

Agent::Agent(QObject * parent)
: QObject(parent)
{
   setupUi();
}

void Agent::setupUi()
{
   startShortcut_ = new QxtGlobalShortcut(this);
   startShortcut_->setShortcut(QKeySequence("Ctrl+F5"));
   startShortcut_->setEnabled(true);
   connect(startShortcut_, SIGNAL(activated()), this, SLOT(start()));

   stopShortcut_ = new QxtGlobalShortcut(this);
   stopShortcut_->setShortcut(QKeySequence("Ctrl+F6"));
   connect(stopShortcut_, SIGNAL(activated()), this, SLOT(stop()));
   stopShortcut_->setEnabled(false);

   restartShortcut_ = new QxtGlobalShortcut(this);
   restartShortcut_->setShortcut(QKeySequence("Ctrl+F7"));
   restartShortcut_->setEnabled(true);
   connect(restartShortcut_, SIGNAL(activated()), this, SLOT(restart()));
   
   QxtGlobalShortcut * exitShortcut_ = new QxtGlobalShortcut(this);
   exitShortcut_->setShortcut(QKeySequence("Ctrl+F10"));
   connect(exitShortcut_, SIGNAL(activated()), this, SLOT(exit()));

   createTrayIcon();
   trayIcon_->show();
}

void Agent::createTrayIcon()
{
   startAction_ = new QAction(tr("&Start"), this);
   connect(startAction_, SIGNAL(triggered()), this, SLOT(start()));
   startAction_->setShortcut(QKeySequence("Ctrl+F5"));
   
   stopAction_ = new QAction(tr("S&top"), this);
   connect(stopAction_, SIGNAL(triggered()), this, SLOT(stop()));
   stopAction_->setShortcut(QKeySequence("Ctrl+F6"));
   stopAction_->setEnabled(false);

   restartAction_ = new QAction(tr("&Restart"), this);
   connect(restartAction_, SIGNAL(triggered()), this, SLOT(restart()));
   restartAction_->setShortcut(QKeySequence("Ctrl+F7"));
   restartAction_->setEnabled(false);

   QAction * quitAction = new QAction(tr("&Quit"), this);
   connect(quitAction, SIGNAL(triggered()), this, SLOT(exit()));
   quitAction->setShortcut(QKeySequence("Ctrl+F10"));

   trayIconMenu_ = new QMenu(0);
   trayIconMenu_->addAction(startAction_);
   trayIconMenu_->addAction(stopAction_);
   trayIconMenu_->addAction(restartAction_);
   trayIconMenu_->addSeparator();
   trayIconMenu_->addAction(quitAction);

   trayIcon_ = new QSystemTrayIcon(this);
   trayIcon_->setContextMenu(trayIconMenu_);
   trayIcon_->setIcon(QIcon(":/icon/poker.png"));
   connect(trayIcon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)),
             this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));
}

void Agent::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
   if (reason == QSystemTrayIcon::DoubleClick)
   {
      //setWindowState(windowState() & ~Qt::WindowMinimized | Qt::WindowActive);
      //show();
      //activateWindow();
   }
}

void Agent::load(const QString & module)
{
   QString regKey = QDir::current().dirName();
   QSettings settings(regKey, "Config");
   
   QVariant sett = settings.value("Settings");
   //SettingsData data = qvariant_cast<SettingsData>(sett);

   if (module.isEmpty())
   {
      trayIcon_->showMessage("Kenay", tr("dll not specified."),
         QSystemTrayIcon::Information, 5000);
   
      //QTimer::singleShot(5000, this, SLOT(exit()));
      return;
   }
   
   if (loadedModules_.contains(module))
   {
      trayIcon_->showMessage("Kenay", 
         tr("Kenay for %1 is already loaded.").arg(module),
         QSystemTrayIcon::Information, 5000);
   }
   else
   {
      //проверка наличия библиотеки
      ExecStart f_start = (ExecStart) QLibrary::resolve(module, "ExecStart");
      ExecStop f_stop   = (ExecStop)  QLibrary::resolve(module, "ExecStop");
      ExecKill f_kill   = (ExecKill)  QLibrary::resolve(module, "ExecKill");
      ExecName f_name   = (ExecName)  QLibrary::resolve(module, "ExecName");
      f_room            = (ExecStartRoom)  QLibrary::resolve(module, "ExecStartRoom");
      if (f_start && f_stop && f_kill && f_name && f_room)
      {
         loadedModules_.append(module);
         
         f_startList_.append(f_start);
         f_stopList_.append(f_stop);
         f_killList_.append(f_kill);
         f_nameList_.append(f_name);

         char * name = new char[100];
         f_name(name, 100);
         trayIcon_->showMessage("Kenay",
            tr("Kenay for %1 has been loaded.").arg(name),
            QSystemTrayIcon::Information, 5000);
         trayIcon_->setToolTip(tr("Kenay for %1.").arg(name));

         //запуск рума по рассписанию
         int n = -1;
         n = 5000;
         QTimer::singleShot(n, this, SLOT(startRoom()));
         //if (f_start)
         //{
         //   f_start(showMessageCallbackFunc);
         //   stopShortcut_->setEnabled(true);
         //   stopAction_->setEnabled(true);

         //   restartShortcut_->setEnabled(true);
         //   restartAction_->setEnabled(true);

         //   startShortcut_->setEnabled(false);
         //   startAction_->setEnabled(false);
         //}
      }
      else
      {
         trayIcon_->showMessage("Kenay", 
            tr("Kenay cannot load module %1. Wrong format of dll.").arg(module),
            QSystemTrayIcon::Information, 500);
         return;
      }
   }
}

void Agent::handleMessage(const QString & command)
{
   if (command.contains("Wake up!"))
   {
      QStringList mods = command.split("!");
      if (mods.count() > 1)
      {
         QString mod = mods.at(1);
         load(mod);
      }
   }
}

void Agent::start()
{
   //запуск всех функций
   foreach (ExecStart f_start, f_startList_)
   {
      if (f_start)
         f_start(showMessageCallbackFunc);
   }
   startAction_->setEnabled(false);
   stopAction_->setEnabled(true);
   restartAction_->setEnabled(true);

   startShortcut_->setEnabled(false);
   stopShortcut_->setEnabled(true);
   restartShortcut_->setEnabled(true);
}

void Agent::stop()
{
   //останов всех функций
   foreach (ExecStop f_stop, f_stopList_)
   {
      if (f_stop)
         f_stop();
   }
   startAction_->setEnabled(true);
   stopAction_->setEnabled(false);
   restartAction_->setEnabled(false);

   startShortcut_->setEnabled(true);
   stopShortcut_->setEnabled(false);
   restartShortcut_->setEnabled(false);
}

void Agent::restart()
{
   stop();
   start();
}

void Agent::exit()
{
   //уничтожение всех dll
   foreach (ExecKill f_kill, f_killList_)
   {
      if (f_kill)
         f_kill();
   }
   qApp->quit();
}

void Agent::startRoom()
{
   if (f_room)
      f_room();
}
