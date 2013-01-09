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
   virtual bool isDealer() const;
   const QRect foldRect() const;
   const QRect checkRect() const;
   const QRect callRect() const;
   const QRect raiseRect() const;
   const QRect betRect() const;
   QString cardFromImage(QImage & img) const;
   Opp opp(const QString & num);
protected:
   void parseOppNick(const QDomNode & dnOpp, Opp & opp);
   void parseOppStack(const QDomNode & dnOpp, Opp & opp);
   void parseOppBet(const QDomNode & dnOpp, Opp & opp);
   void parseOppInGame(const QDomNode & dnOpp, Opp & opp);
   void parseOppIsDealer(const QDomNode & dnOpp, Opp & opp);

   const QRect buttonRect(const QString & btn) const;
   //! База карт
   CardBase cardBase_;
   //!
   QMap<QString, QDomNode> oppMap_;
};

struct OppNick
{
   OppNick();
   OppNick(const QString & hash);
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
      
      QString hash() const;
   };
   //! Последовательность букв
   QList<OppLetter> letters;
   QString hash() const;
};

#if 1
class Opp
{
public:
   enum Action
   {
      Nope,//заглушка
      SmallBlind,
      BigBlind,
      Fold,
      Check,
      Call,
      Bet,
      Raise
   };
   
   Opp() {bet_ = 0.; stack_ = 0.; hasCards_ = false; isDealer_ = false; action_ = Nope;}
   
   void setCards(bool v) {hasCards_ = v;}
   bool hasCards() const {return hasCards_;}

   void setDealer(bool v) {isDealer_ = v;}
   bool isDealer() const {return isDealer_;}
   
   void setStack(qreal v) {stack_ = v;}
   qreal stack() const {return stack_;}
   
   void setNick(const OppNick & nick) {nick_ = nick;}
   OppNick nick() const {return nick_;}
   
   void setBet(qreal v) {bet_ = v;}
   qreal bet() const {return bet_;}

   void setAction(Action a) {action_ = a;}
   Action action() const {return action_;}

   //!http://propokerpro.ru/shkola/cash-statistika.htm
   qreal vpip() const {return 0.;}
   //! Preflop raiser
   qreal pfr() const {return 0.;}
protected:
   OppNick nick_;
   qreal stack_;
   qreal bet_;
   bool hasCards_;
   bool isDealer_;
   Action action_;
};
#endif

#endif
