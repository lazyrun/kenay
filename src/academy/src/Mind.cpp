#include "Mind.h"

Mind::Mind(const CardProcessing * proc)
{
   proc_ = proc;
}

void Mind::setHole(const QString & f, const QString & s)
{
   hole_.setCards(f, s);;
}

void Mind::setBoard(const QStringList & board)
{
   board_ = board;
}


