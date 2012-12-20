#ifndef PROCACAD_H
#define PROCACAD_H

#include "ImageProc.h"
#include "CardBase.h"
#include "Config.h"

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
   //! Банк из картинки
   qreal potFromImage(QImage & img) const;
   //! База карт
   CardBase cardBase_;
};

#endif
