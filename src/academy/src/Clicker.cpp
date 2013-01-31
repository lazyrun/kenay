#include "Hooker.h"
#include "Clicker.h"

#define ABSOLUTE_COORD 65535

Clicker::Clicker(CardProcessing * const proc): proc_(proc)
{
   screen_res_x_ = GetSystemMetrics(SM_CXSCREEN);//Получить ширину экрана
   screen_res_y_ = GetSystemMetrics(SM_CYSCREEN);//Получить высоту экрана
}

void Clicker::setWinId(WId winId)
{
   winId_ = winId;
}

void Clicker::click(const Solution & sol)
{
   switch (sol.action())
   {
      case Solution::Fold:
      {
         clickFold();
         break;
      }
      case Solution::Call:
      {
         clickCall();
         break;
      }
      case Solution::Check:
      {
         clickCheck();
         break;
      }
      case Solution::Bet:
      {
         clickBet();
         break;
      }
      case Solution::Raise:
      {
         clickRaise();
         break;
      }
      default:
         clickSitOut();
   }
}

void Clicker::clickSitOut()
{
   
}

void Clicker::clickCall()
{
   if (proc_->hasCall())
   {
      const QRect callRect = proc_->callRect();
      clickTo(winId_, callRect);
   }
}

void Clicker::clickCheck()
{
   if (proc_->hasCheck())
   {
      const QRect checkRect = proc_->checkRect();
      clickTo(winId_, checkRect);
   }
}

void Clicker::clickFold()
{
   if (proc_->hasFold())
   {
      const QRect foldRect = proc_->foldRect();
      clickTo(winId_, foldRect);
   }
}

void Clicker::clickRaise()
{
   if (proc_->hasRaise())
   {
      const QRect raiseRect = proc_->raiseRect();
      clickTo(winId_, raiseRect);
   }
   else
   {
      clickCall();
   }
}

void Clicker::clickBet()
{
   if (proc_->hasRaise())
   {
      const QRect betRect = proc_->betRect();
      clickTo(winId_, betRect);
   }
   else
   {
      clickCall();
   }
}

void Clicker::clickTo(WId hwnd, const QRect & rect)
{
   int x = rect.left(), y = rect.top();
   x += qrand() % rect.width();
   y += qrand() % rect.height();
   clickTo(hwnd, x, y);
}

void Clicker::clickTo(WId hwnd, int x, int y)
{
   RECT rect;
   GetWindowRect(hwnd, &rect);

   static int x_border = 0;
   static int y_border = 0;
   if (x_border == 0)
   {
      QRect wndRect(rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top);
      RECT clrect;
      GetClientRect(hwnd, &clrect);
      QRect clntRect(clrect.left, clrect.top, clrect.right - clrect.left, 
         clrect.bottom - clrect.top);
      x_border = (int)(wndRect.width() - clntRect.width()) / 2.;
      y_border = wndRect.height() - clntRect.height() - x_border;
   }

   x += rect.left + x_border;
   y += rect.top + y_border;
   HwndToTop(hwnd);

   long X = (long)(x * ABSOLUTE_COORD)/screen_res_x_;
   long Y = (long)(y * ABSOLUTE_COORD)/screen_res_y_;

   Hooker hook(Hooker::Mouse);
   //запомнить текущие координаты мышки
   POINT pt;
   GetCursorPos(&pt);
   long cX = (long)(pt.x * ABSOLUTE_COORD)/screen_res_x_;
   long cY = (long)(pt.y * ABSOLUTE_COORD)/screen_res_y_;
   
   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, X, Y, 0, 0);
   Sleep(100);
   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTDOWN, X, Y, 0, 0);
   Sleep(100);
   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_LEFTUP, X, Y, 0, 0);

   //вернуть координаты мышки откуда взяли
   mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, cX + 1, cY + 1, 0, 0);

}

void Clicker::HwndToTop(WId hwnd)
{
   BringWindowToTop(hwnd);
   SetActiveWindow(hwnd);
   SetFocus(hwnd);
   Sleep(100);
}
