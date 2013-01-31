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

   //!
   Solution loosePreflop();
protected:
   //!
   PreflopPosition preflopPos_;
   //!
   int limpers_;
   //!
   int raisers_;
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
   QStringList parseRange(const QString & range) {return mind_->parseRange(range);}
protected:
   MindFLfull * mind_;
};

#endif

