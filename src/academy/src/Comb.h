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
   //! ��������� ����������, ��� ����� �� ����
   enum Combs
   {
      Trash,         //�����
      Card,          //������� �����
      Pair,          //����    
      TwoPair,       //��� ����
      ThreeOfKind,   //������
      Straight,      //�����
      Flash,         //����
      FullHouse,     //����
      Care,          //����
      StraightFlash  //����� ����
   };

   //! �������� ������� ����� �� ��������
   // ��������������, ��� ����� ���� �� ���� ��� ���� �� �����
   enum Draws
   {
      NoDraws,    //������������ �����
      OverCard,   // A2
      OverCards,  // AK
      BDStraight, // ��������� �����
      BDFlash,    // ��������� ����
      Gutshot,    // ������
      OESD,       // ������������ ��� ������� ����� ������
      FlashDraw1, //���� ��� � 1� ������
      FlashDraw2, //���� ��� � 2�� �������
   };
   
   //! �����������
   Comb();
   //! ����������
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

