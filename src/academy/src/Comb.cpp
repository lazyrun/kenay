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
   if (isThreeOfKind(cards))
   {
      return Comb::ThreeOfKind;
   }
   if (isTwoPair(cards))
   {
      return Comb::TwoPair;
   }
   if (isPair(cards))
   {
      return Comb::Pair;
   }

   return Comb::Trash;
}

Comb::Draws Comb::getDraw(const QStringList & cards) const
{
   return Comb::NoDraws;
}

bool Comb::isStraightFlash(const QStringList & cards) const
{
   return (isFlash(cards) && isStraight(cards));
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
   QString order("23456789TJQKA");
   QString aorder("A2345");

   if (cards.count() < 5)
      return false;

   QMap<int, QString> orderMap, aorderMap;
   for (int i = 0; i < cards.count(); i++)
   {
      QString nom = cards.at(i).left(1);
      int pos = order.indexOf(nom);
      orderMap[pos] = nom;
      pos = aorder.indexOf(nom);
      aorderMap[pos] = nom;
   }
   QString cmb;
   cmb = QStringList(orderMap.values()).join("");
   if (cmb.length() < 5)
      return false;
   if (order.contains(cmb))
      return true;

   cmb = QStringList(aorderMap.values()).join("");
   if (cmb.length() < 5)
      return false;
   if (aorder.contains(cmb))
      return true;

   return false;
}

bool Comb::isThreeOfKind(const QStringList & cards) const
{
   bool res = false;
   QMap<QString, int> threeMap;
   
   foreach (QString card, cards)
   {
      QString nom = card.left(1);
      threeMap[nom]++;
      if (threeMap[nom] == 3)
      {
         res = true;
         break;
      }
   }
   return res;
}

bool Comb::isTwoPair(const QStringList & cards) const
{
   QMap<QString, int> twoMap;
   
   int pairs = 0;
   foreach (QString card, cards)
   {
      QString nom = card.left(1);
      twoMap[nom]++;
      if (twoMap[nom] == 2)
      {
         pairs++;
      }
   }
   
   return pairs >= 2;
}

bool Comb::isPair(const QStringList & cards) const
{
   bool res = false;
   QMap<QString, int> pairMap;
   
   foreach (QString card, cards)
   {
      QString nom = card.left(1);
      pairMap[nom]++;
      if (pairMap[nom] == 2)
      {
         res = true;
         break;
      }
   }
   
   return res;
}

