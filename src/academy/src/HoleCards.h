/*! 
    \file HoleCards
    \brief карманные карты
*/

#ifndef HOLECARDS_H
#define HOLECARDS_H

class HoleCards
{
public:
   //!
   HoleCards();
   //!
   HoleCards(const QString & first, const QString & second);
   //!
   void setCards(const QString & first, const QString & second);
   //! полное название
   QString fullName()    const;
   //! с меткой одномастности
   QString suitedName()  const;
   //! номиналы
   QString nominalName() const;
   //!
   QString first() const { return first_; }
   //!
   QString second() const { return second_; }
   //! Признак одномастности
   bool isSuited() const;
protected:
   //! первая карта
   QString first_;
   //! вторая карта
   QString second_;
};

#endif