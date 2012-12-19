#include "ProcAcad.h"

//
//ProcAcad
//
ProcAcad::ProcAcad(const QString & mapfile)
: CardProcessing()
{
   ConfigGlobal<MainConfig>::Instance(mapfile, "root");
}

ProcAcad::~ProcAcad()
{
}

bool ProcAcad::hasFold() const 
{
   //контрольная точка фолда
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("buttons", "fold", "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "buttons", "fold", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "fold", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::hasCall() const
{
   //контрольная точка кола
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("buttons", "call", "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "buttons", "call", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "call", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::hasCheck() const
{
   //контрольная точка чека
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("buttons", "check", "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "buttons", "check", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "check", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::hasRaise() const
{
   //контрольная точка рейза
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("buttons", "raise", "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "buttons", "raise", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "raise", "control", "");

   return controlPixel(sx, sy, scl);
}

QString ProcAcad::holeFirst() const
{
   return QString();
}

QString ProcAcad::holeSecond() const
{
   return QString();
}

qreal ProcAcad::pot() const
{
   return 0.;
}

qreal ProcAcad::stack() const
{
   return 0.;
}

