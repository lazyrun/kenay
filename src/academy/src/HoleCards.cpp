#include "HoleCards.h"

HoleCards::HoleCards()
{
}

HoleCards::HoleCards(const QString & first,
                     const QString & second)
{
   setCards(first, second);
}

void HoleCards::setCards(const QString & first, const QString & second)
{
   QString cards = "AKQJT98765432";
   QString suit  = "chds";

   Q_ASSERT(first.length() == 2 && second.length() == 2);
   Q_ASSERT(cards.contains(first.left(1)));
   Q_ASSERT(cards.contains(second.left(1)));
   Q_ASSERT(suit.contains(first.right(1)));
   Q_ASSERT(suit.contains(second.right(1)));
   Q_ASSERT(first != second);
   
   int idx1 = cards.indexOf(first.left(1));
   int idx2 = cards.indexOf(second.left(1));
   if (idx1 > idx2)
   {
      first_  = second;
      second_ = first;
   }
   else
   {
      first_  = first;
      second_ = second;
   }
}

QString HoleCards::fullName() const
{
   return (first_ + second_);
}

QString HoleCards::suitedName() const
{
   return nominalName() + (isSuited() ? "s" : "o");
}

bool HoleCards::isSuited() const
{
   return (first_.right(1) == second_.right(1));
}

QString HoleCards::nominalName() const
{
   return first_.left(1) + second_.left(1);
}
