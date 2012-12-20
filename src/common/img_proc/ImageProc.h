#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include "BoolMatrix.h"

/*!
\class CardProcessing
\brief ����������� ����� ��������� ������ �����
*/
class CardProcessing
{
public:
   //! ������� ������� ����
   enum HoldemLevel
   {
      Undefined,
      Preflop,
      Flop,
      Turn,
      River
   };
   //! ctor.
   CardProcessing();
   //! dtor.
   virtual ~CardProcessing();
   //! ������� ����� �����
   virtual void setImage(const QImage & img);
   //! ��������� �� ������ ��������
   bool isPreflop() const {return holdemLevel_ == Preflop; }
   //! ��������� �� ������ �����
   bool isFlop() const {return holdemLevel_ == Flop; }
   //! ��������� �� ������ �����
   bool isTurn() const {return holdemLevel_ == Turn; }
   //! ��������� �� ������ ������
   bool isRiver() const {return holdemLevel_ == River; }
   //! ���������� ������� ������
   HoldemLevel holdemLevel() const;
   //! ���������� ����� ����������
   uchar threshold() const {return threshold_; }

   virtual bool hasFold() const = 0;
   virtual bool hasCall() const = 0;
   virtual bool hasCheck() const = 0;
   virtual bool hasRaise() const = 0;
   virtual QString holeCard(const QString & card) const = 0;
   virtual qreal pot() const = 0;
   virtual qreal stack() const = 0;

protected:
   //! �������� ����� �������
   bool controlPixel(const QString & sx, const QString & sy, 
                            const QString & scl) const;
protected:
   //! ������������ ��������
   QImage img_;
   //! ������ �������
   BoolMatrix * matrix_;
   //! ����� �������� � �/�
   uchar threshold_;
   //! ������� �������
   HoldemLevel holdemLevel_;
};

#endif
