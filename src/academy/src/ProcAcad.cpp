#include "ProcAcad.h"

//
//ProcAcad
//
ProcAcad::ProcAcad(const QString & mapfile)
: CardProcessing(mapfile)
{
   ConfigGlobal<MainConfig>::Instance(mapfile, "root");
}

ProcAcad::~ProcAcad()
{
}

bool ProcAcad::hasFold() const 
{
   return false;
}

bool ProcAcad::hasCall() const
{
   return false;
}

bool ProcAcad::hasCheck() const
{
   return false;
}

bool ProcAcad::hasRaise() const
{
   return false;
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

