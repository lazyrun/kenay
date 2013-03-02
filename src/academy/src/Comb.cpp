#include "Comb.h"

Comb::Comb()
{
}

Comb::Combs Comb::getComb(const QStringList & cards) const
{
   if (isStraightFlash(cards))
   {
      return Comb::StraightFlash;
   }
   if (isCare(cards))
   {
      return Comb::Care;
   }
   if (isFullHouse(cards))
   {
      return Comb::FullHouse;
   }
   if (isFlash(cards))
   {
      return Comb::Flash;
   }
   if (isStraight(cards))
   {
      return Comb::Straight;
   }


   return Comb::Trash;
}

Comb::Draws Comb::getDraw(const QStringList & cards) const
{
   return Comb::NoDraws;
}

bool Comb::isStraightFlash(const QStringList & cards) const
{
}

bool Comb::isCare(const QStringList & cards) const
{
   bool res = false;
   QMap<QString, int> careMap;
   
   foreach (QString card, cards)
   {
      QString nom = card.left(1);
      careMap[nom]++;
      if (careMap[nom] == 4)
      {
         res = true;
         break;
      }
   }
   return res;
}

bool Comb::isFullHouse(const QStringList & cards) const
{
   bool res = false;
   QMap<QString, int> fhMap;
   for (int i = 0; i < cards.count(); i++)
   {
      QString nom = cards.at(i).left(1);
      fhMap[nom]++;
   }
   bool has3 = false, has2 = false;
   foreach (QString nom, fhMap.keys())
   {
      if (fhMap[nom] == 3)
      {
         has3 = true;
         continue;
      }
      if (fhMap[nom] >= 2)
         has2 = true;
   }
   if (has3 && has2)
   {
      res = true;
   }
   return res;
}

bool Comb::isFlash(const QStringList & cards) const
{
   bool res = false;
   QMap<QString, int> flashMap;
   
   for (int i = 0; i < cards.count(); i++)
   {
      QString nom = cards.at(i).right(1);
      flashMap[nom]++;
      if (flashMap[nom] == 5)
      {
         res = true;
         break;
      }
   }

   return res;
}

bool Comb::isStraight(const QStringList & cards) const
{
   bool res = false;
   QString order1("23456789TJQKA");
   QString order2("A23456789TJQK");

   QMap<int, int> order1Map, order2Map;
   for (int i = 0; i < cards.count(); i++)
   {
      QString nom = cards.at(i).left(1);
      int pos = order1.indexOf(nom);
      order1Map[pos] = 1;
      pos = order2.indexOf(nom);
      order2Map[pos] = 1;
   }
   //проверяем оба мапа на предмет 5 карт подряд
   QString first = order1Map.keys().first();
   QString last = order1Map.keys().last();
   int pFirst = order1.indexOf(first);
   int pLast = order1.indexOf(last);
   if (pLast - pFirst == 4)
   {
      res = true;
   }
   else
   {
      first = order2Map.keys().first();
      last = order2Map.keys().last();
      pFirst = order2.indexOf(first);
      pLast = order2.indexOf(last);
      if (pLast - pFirst == 4)
      {
         res = true;
      }
   }

   return res;
}

