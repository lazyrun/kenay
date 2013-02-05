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
   virtual bool isDealer() const;
   virtual Street street();
   virtual const QRect foldRect() const;
   virtual const QRect checkRect() const;
   virtual const QRect callRect() const;
   virtual const QRect raiseRect() const;
   virtual const QRect betRect() const;
   QString cardFromImage(QImage & img) const;
   
   virtual Opp opp(const QString & num);
   virtual QStringList board() const;
protected:
   void parseOppNick(const QDomNode & dnOpp, Opp & opp);
   void parseOppStack(const QDomNode & dnOpp, Opp & opp);
   void parseOppBet(const QDomNode & dnOpp, Opp & opp);
   void parseOppHasCards(const QDomNode & dnOpp, Opp & opp);
   void parseOppIsDealer(const QDomNode & dnOpp, Opp & opp);
   void parseOppInGame(Opp & opp);

   const QRect buttonRect(const char * btn) const;
   
   QString boardCard(const QString & card) const;

   //! База карт
   CardBase cardBase_;
   //!
   QMap<QString, QDomNode> oppMap_;
};

#endif
