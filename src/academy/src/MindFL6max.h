/*! \file MindFL6max.h
    \brief 
    \author Kiselev Kirill
    \date 15.01.2013
*/

#ifndef MindFL6max_H
#define MindFL6max_H

#include "Mind.h"

/*!
\class MindFL6max
\brief 
*/
class MindFL6max //: public Mind
{
public:
   //! Конструктор
   MindFL6max(CardProcessing * const proc, Session * const session);
   //! Деструктор
   virtual ~MindFL6max() {}
   //!
   virtual Solution think();
protected:
};

#endif

