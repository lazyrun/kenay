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
   const QRect foldRect() const;
   const QRect checkRect() const;
   const QRect callRect() const;
   const QRect raiseRect() const;
   const QRect betRect() const;
   QString cardFromImage(QImage & img) const;
   
   virtual Opp opp(const QString & num);
protected:
   void parseOppNick(const QDomNode & dnOpp, Opp & opp);
   void parseOppStack(const QDomNode & dnOpp, Opp & opp);
   void parseOppBet(const QDomNode & dnOpp, Opp & opp);
   void parseOppInGame(const QDomNode & dnOpp, Opp & opp);
   void parseOppIsDealer(const QDomNode & dnOpp, Opp & opp);

   const QRect buttonRect(const char * btn) const;
   //! База карт
   CardBase cardBase_;
   //!
   QMap<QString, QDomNode> oppMap_;
};

#endif
