/*! \file Comb.h
    \brief 
    \author Kiselev Kirill
    \date 02.03.2013
*/

#ifndef Comb_H
#define Comb_H

/*!
\class Comb
\brief 
*/
class Comb
{
public:
   //! Возможные комбинации, без учета их силы
   enum Combs
   {
      Trash,         //мусор
      Card,          //старшая карта
      Pair,          //пара    
      TwoPair,       //две пары
      ThreeOfKind,   //тройка
      Straight,      //стрит
      Flash,         //флеш
      FullHouse,     //фулл
      Care,          //каре
      StraightFlash  //стрит флеш
   };

   //! Варианты имеющие шансы на усиление
   // предполагается, что часть карт от этих дро есть на руках
   enum Draws
   {
      NoDraws,    //похвастаться нечем
      OverCard,   // A2
      OverCards,  // AK
      BDStraight, // Бэкдорный стрит
      BDFlash,    // Бэкдорный флеш
      Gutshot,    // Гатшот
      OESD,       // двусторонний или двойной белли бастер
      FlashDraw1, //флеш дро с 1й картой
      FlashDraw2, //флеш дро с 2мя картами
   };
   
   //! Конструктор
   Comb();
   //! Деструктор
   virtual ~Comb() {}
   //!
   Combs getComb(const QStringList & cards) const;
   //!
   Draws getDraw(const QStringList & cards) const;
protected:
   //!
   bool isStraightFlash(const QStringList & cards) const;
   //!
   bool isCare(const QStringList & cards) const;
   //!
   bool isFullHouse(const QStringList & cards) const;
   //!
   bool isFlash(const QStringList & cards) const;
   //!
   bool isStraight(const QStringList & cards) const;
};

#endif

