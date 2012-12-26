#ifndef PROCACAD_H
#define PROCACAD_H

#include "ImageProc.h"
#include "CardBase.h"
#include "Config.h"
#include "ImgUtils.h"

class Opp;

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
   Opp opp(const QString & num);
protected:
   //! База карт
   CardBase cardBase_;
   //!
   QMap<QString, QDomNode> oppMap_;
};

struct OppNick
{
   bool operator==(const OppNick & rhs)
   {
      if (&rhs == this)
         return true;
      if (letters.count() == rhs.letters.count())
      {
         return letters == rhs.letters;
      }
      return false;
   }
   struct OppLetter
   {
      bool operator==(const OppLetter & rhs)
      {
         if (&rhs == this)
            return true;
         if (size == rhs.size && closed == rhs.closed)
            return true;
         return false;
      }      
      //размеры буквы
      QSize size;
      //количество замкнутых областей
      int closed;
   };
   //! Последовательность букв
   QList<OppLetter> letters;
};

#if 1
class Opp
{
public:
   Opp() {}
   void setCard(bool v) {v;}
   bool hasCard() const {return false;}

   void setDealer(bool v) {v;}
   bool isDealer() const {return false;}
   
   void setStack(qreal v) {v;}
   qreal stack() const {return 0.;}
   
   void setNick(const OppNick & nick) {nick_ = nick;}
   OppNick nick() const {return nick_;}
   
   //!http://propokerpro.ru/shkola/cash-statistika.htm
   qreal vpip() const {return 0.;}
   //! Preflop raiser
   qreal pfr() const {return 0.;}
protected:
   OppNick nick_;
};
#endif

#endif
