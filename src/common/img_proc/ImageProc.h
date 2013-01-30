#ifndef IMAGEPROC_H
#define IMAGEPROC_H

#include "BoolMatrix.h"
#include "Opp.h"

/*!
\class CardProcessing
\brief ����������� ����� ��������� ������ �����
*/
class CardProcessing
{
public:
   //! ������� ������� ����
   enum Street
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
   bool isPreflop() const {return street_ == Preflop; }
   //! ��������� �� ������ �����
   bool isFlop() const {return street_ == Flop; }
   //! ��������� �� ������ �����
   bool isTurn() const {return street_ == Turn; }
   //! ��������� �� ������ ������
   bool isRiver() const {return street_ == River; }
   //! ���������� ������� ������
   virtual Street street() = 0;
   //! ���������� ����� ����������
   uchar threshold() const {return threshold_; }

   virtual bool hasFold() const = 0;
   virtual bool hasCall() const = 0;
   virtual bool hasCheck() const = 0;
   virtual bool hasRaise() const = 0;
   virtual QString holeCard(const QString & card) const = 0;
   virtual qreal pot() const = 0;
   virtual qreal stack() const = 0;
   virtual bool isDealer() const = 0;
   virtual Opp opp(const QString & num) = 0;
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
   Street street_;
};

#endif
