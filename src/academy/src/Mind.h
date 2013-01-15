/*! \file Mind.h
    \brief 
    \author Kiselev Kirill
    \date 15.01.2013
*/

#ifndef Mind_H
#define Mind_H

#include "ImageProc.h"
#include "Solution.h"

/*!
\class Mind
\brief 
*/
class Mind
{
public:
   //! �����������
   Mind(const CardProcessing * proc);
   //! ����������
   virtual ~Mind() {}
   //! 
   virtual Solution think() = 0;
protected:
   //!
   const CardProcessing * proc_;
};

#endif

