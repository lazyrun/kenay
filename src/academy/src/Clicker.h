/*! \file Clicker.h
    \brief 
    \author Kiselev Kirill
    \date 15.01.2013
*/

#ifndef Clicker_H
#define Clicker_H

#include <qt_windows.h>
#include "ImageProc.h"
#include "Solution.h"

/*!
\class Clicker
\brief 
*/
class Clicker
{
public:
   //! Конструктор
   Clicker(CardProcessing * const proc);
   //! Деструктор
   virtual ~Clicker() {}
   //!
   void setWinId(WId winId);
   //!
   void click(const Solution & sol);
   //!
   static void HwndToTop(WId hwnd);
protected:
   void clickTo(WId hwnd, int x, int y);
   void clickTo(WId hwnd, const QRect & rect);
   void clickFold();
   void clickCall();
   void clickCheck();
   void clickRaise();
   void clickBet();
   void clickSitOut();

   CardProcessing * const proc_;
   WId winId_;
   int screen_res_x_;
   int screen_res_y_;
};

#endif

