/*! \file MindFLfull.h
    \brief 
    \author Kiselev Kirill
    \date 15.01.2013
*/

#ifndef MindFLfull_H
#define MindFLfull_H

#include "Mind.h"

/*!
\class MindFLfull
\brief 
*/
class MindFLfull : public Mind
{
friend class MindFLfullPrivate;
public:
   //! Конструктор
   MindFLfull(CardProcessing * const proc, Session * const session);
   //! Деструктор
   virtual ~MindFLfull() {}
protected:
   //!
   virtual Solution flopSolution();
   //!
   virtual Solution turnSolution();
   //!
   virtual Solution riverSolution();
   //!
   int tradeInPreflop();
   //! 
   void preflopPosition();

   virtual Solution preflopSolution();
      Solution round0Solution();
         Solution tightPreflop();
            Solution sbTight();
               Solution sbTightNoRaise();
               Solution sbTightOneRaise();
               Solution sbTightMoreRaise();
            Solution bbTight();
               Solution bbTightNoRaise();
               Solution bbTightOneRaise();
               Solution bbTightMoreRaise();
            Solution utgTight();
               Solution utgTightNoRaise();
               Solution utgTightOneRaise();
               Solution utgTightMoreRaise();
            Solution mTight();
               Solution mTightNoRaise();
               Solution mTightOneRaise();
               Solution mTightMoreRaise();
            Solution buTight();
               Solution buTightNoRaise();
               Solution buTightOneRaise();
               Solution buTightMoreRaise();

            Solution sbLoose();
               Solution sbLooseNoRaise();
               Solution sbLooseOneRaise();
               Solution sbLooseMoreRaise();
            Solution bbLoose();
               Solution bbLooseNoRaise();
               Solution bbLooseOneRaise();
               Solution bbLooseMoreRaise();
            Solution utgLoose();
               Solution utgLooseNoRaise();
               Solution utgLooseOneRaise();
               Solution utgLooseMoreRaise();
            Solution mLoose();
               Solution mLooseNoRaise();
               Solution mLooseOneRaise();
               Solution mLooseMoreRaise();
            Solution buLoose();
               Solution buLooseNoRaise();
               Solution buLooseOneRaise();
               Solution buLooseMoreRaise();
      
      Solution roundASolution();
   
   Solution pfrVpipSolution();
   //!
   Solution loosePreflop();
protected:
   //!
   const qreal tightThreshold_;
   QPair<qreal, qreal> pCareful_;
   QPair<qreal, qreal> pTight_;
   QPair<qreal, qreal> pModerate_;
   QPair<qreal, qreal> pLoose_;
   QPair<qreal, qreal> pManiak_;

   QString ranCareful_;
   QString ranTight_;
   QString ranModerate_;
   QString ranLoose_;
   QString ranManiak_;

   bool useTight_;
};

/*!
\class MindFLfullPrivate
\brief 
*/
class MindFLfullPrivate
{
public:
   MindFLfullPrivate(MindFLfull * const mind) { mind_ = mind;}
   Mind::PreflopPosition preflopPos() {return mind_->preflopPos_;}
   int limpers() {return mind_->limpers_;}
   int raisers() {return mind_->raisers_;}
   CardProcessing::Street street() {return mind_->street_;}
   QStringList board() {return mind_->board_;}
   QStringList parseRange(const QString & range) {return mind_->parseRange(range);}
   void setHole(const HoleCards & hole) { mind_->hole_ = hole; }
   void setBoard(const QStringList & board) { mind_->board_ = board; }
   Comb::Combs comb() const {return mind_->comb();}
protected:
   MindFLfull * mind_;
};

#endif

