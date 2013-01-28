/*! \file Opp.h
    \brief 
    \author Kiselev Kirill
    \date 25.01.2013
*/

#ifndef Opp_H
#define Opp_H

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
   //! ѕоследовательность букв
   QList<OppLetter> letters;
   QString hash() const;
};

/*!
\class Opp
\brief 
*/
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
   
   Opp() {bet_ = 0.; stack_ = 0.; hasCards_ = false; isDealer_ = false; action_ = Nope;
          vpip_ = 0.; pfr_ = 0.; fold_ = 0.; limp_ = 0.;}
   
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
   qreal vpip() const      {return vpip_;}
   void setVpip(qreal vpip){vpip_ = vpip; }
   //! Preflop raiser
   qreal pfr() const       {return pfr_;}
   void setPfr(qreal pfr)  {pfr_ = pfr;}
   //! Fold
   qreal fold() const      {return fold_;}
   void setFold(qreal fold){fold_ = fold;}
   //! Limp
   qreal limp() const      {return limp_;}
   void setLimp(qreal limp){limp_ = limp;}
protected:
   OppNick nick_;
   qreal stack_;
   qreal bet_;
   bool hasCards_;
   bool isDealer_;
   Action action_;
   qreal vpip_;
   qreal pfr_;
   qreal fold_;
   qreal limp_;
};

#endif

