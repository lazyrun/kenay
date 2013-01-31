/*! 
    \file HoleCards
    \brief ��������� �����
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
   //! ������ ��������
   QString fullName()    const;
   //! � ������ �������������
   QString suitedName()  const;
   //! ��������
   QString nominalName() const;

   //! ������� �������������
   bool isSuited() const;
protected:
   //! ������ �����
   QString first_;
   //! ������ �����
   QString second_;
};

#endif