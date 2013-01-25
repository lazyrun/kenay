#include "Opp.h"

//
// OppNick
//
OppNick::OppNick()
{
}

OppNick::OppNick(const QString & hash)
{
   QStringList letts = hash.split(";");
   foreach (QString let, letts)
   {
      bool ok = false;
      int ilet = let.toInt(&ok, 16);
      if (!ok)
         continue;
      int tmp = ilet >> 5;
      tmp = tmp << 5;
      int c = ilet - tmp;
      ilet = tmp >> 5;
      tmp = ilet >> 5;
      tmp = tmp << 5;

      int h = ilet - tmp;
      ilet = tmp >> 5;
      tmp = ilet >> 5;
      tmp = tmp << 5;
      int w = ilet - tmp;
      
      OppLetter letter;
      letter.closed = c;
      letter.size = QSize(w, h);
      letters << letter;
   }
}

QString OppNick::hash() const
{
   QStringList lst;
   foreach (OppLetter letter, letters)
   {
      lst << letter.hash();
   }
   return lst.join(";");
}

QString OppNick::OppLetter::hash() const
{
   int w = size.width();
   int h = size.height();
   int c = closed;
   int r = w;
   r = r << 5;
   r += h;
   r = r << 5;
   r += c;
   return QString::number(r, 16);
}

