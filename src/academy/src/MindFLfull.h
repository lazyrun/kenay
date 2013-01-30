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
public:
   //! Конструктор
   MindFLfull(CardProcessing * const proc, Session * const session);
   //! Деструктор
   virtual ~MindFLfull() {}
protected:
   //!
   virtual Solution preflopSolution();
   //!
   virtual Solution flopSolution();
   //!
   virtual Solution turnSolution();
   //!
   virtual Solution riverSolution();
   //!
   void preflopPosition();
   
   PreflopPosition preflopPos_;

   int tradeInPreflop();

   //!
   Solution round0Solution();
   //!
   Solution tightPreflop();
   //!
   Solution loosePreflop();
};

#endif

