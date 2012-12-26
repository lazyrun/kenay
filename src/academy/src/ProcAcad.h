#ifndef PROCACAD_H
#define PROCACAD_H

#include "ImageProc.h"
#include "CardBase.h"
#include "Config.h"
#include "ImgUtils.h"

class ProcAcad : public CardProcessing
{
public:
   //! ctor.
   ProcAcad(const QString & mapfile);
   //! dtor.
   virtual ~ProcAcad();
   virtual bool hasFold() const;
   virtual bool hasCall() const;
   virtual bool hasCheck() const;
   virtual bool hasRaise() const;
   virtual QString holeCard(const QString & card) const;
   virtual qreal pot() const;
   virtual qreal stack() const;
   QString cardFromImage(QImage & img) const;
protected:
   //! ���� ����
   CardBase cardBase_;
};

#if 0
class Opp
{
public:
   bool hasCard() const;
   bool isDealer() const;
   qreal stack() const;
   //http://propokerpro.ru/shkola/cash-statistika.htm
   //! Value in pot �� ��������
   /*
VPIP ��� VP$IP ���������������� ��� "voluntary put $ in pot" � ���������� � ���������� ���������, ��� ����� ����� ����������� ������ � ����� ������ ����� ��������. � ���� ������� ���������� �� ������, ����� ����� �������� �� ��������, ��������� ��������� ��������� ��� �� ������ ��������� ������� ������, �.�. ���������� ��� �� ������, � ������� ����� ����������� �������������� ������ ��� ����, ����� ����� �� ���������� ����� ��������. ���� ����� ������ ������������ ������, �.�. ������ � ������� �������� �������, � ����� �� ���������� �� �������� ����� ���, ��� ��� ������ ����������� ��������� ���������� �� ����, �� ������ ������ �� ���������� � ���������� VPIP.
   */
   qreal vpip() const;
   //! Preflop raiser
   qreal pfr() const;
};
#endif

#endif
