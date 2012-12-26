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
   //! База карт
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
   //! Value in pot на префлопе
   /*
VPIP или VP$IP расшифровывается как "voluntary put $ in pot" и показывает в процентном выражение, как часто игрок инвестирует деньги в самом первом круге торговли. В этот процент включаются те случаи, когда игрок повышает на префлопе, коллирует повышение соперника или же просто коллирует большой блайнд, т.е. фактически все те случаи, в которых игрок инвестирует дополнительные деньги для того, чтобы дойти до следующего круга торговли. Если игрок делает обязательную ставку, т.е. играет в позиции большого блайнда, и никто из соперников не повышает перед ним, что даёт игроку возможность бесплатно посмотреть на флоп, то данные случаи не включаются в показатель VPIP.
   */
   qreal vpip() const;
   //! Preflop raiser
   qreal pfr() const;
};
#endif

#endif
