/*! \file Solution.h
    \brief 
    \author Kiselev Kirill
    \date 15.01.2013
*/

#ifndef Solution_H
#define Solution_H

/*!
\class Solution
\brief 
*/
class Solution
{
public:
   enum Action
   {
      Fold,
      Call,
      Raise,
      Bet
   };
   //! Конструктор
   Solution(){}
   //! Деструктор
   virtual ~Solution() {}
   Action action() const {return action_;}
   void setAction(Action a) {action_ = a;}
protected:
   Action action_;
};

#endif

