#ifndef PROCACAD_H
#define PROCACAD_H

#include "ImageProc.h"
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
   virtual QString holeFirst() const;
   virtual QString holeSecond() const;
   virtual qreal pot() const;
   virtual qreal stack() const;

};

#endif
