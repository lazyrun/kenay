#include "ImageProc.h"

//
//CardProcessing
//
CardProcessing::CardProcessing()
{
   threshold_ = 240;
   matrix_ = 0;
}

CardProcessing::~CardProcessing()
{
   delete matrix_;
}

void CardProcessing::setImage(const QImage & img)
{
   img_ = img;
   delete matrix_;
   matrix_ = new BoolMatrix(img, threshold_);
   
   street_ = Undefined;
}

void CardProcessing::save(const QString & fname)
{
   img_.save(fname);
}

bool CardProcessing::controlPixel(const QString & sx, const QString & sy, 
                            const QString & scl) const
{
   bool okx = false, oky = false;
   int x = sx.toInt(&okx);
   int y = sy.toInt(&oky);
   if (okx && oky && x > 0 && x < img_.width() && y > 0 && y < img_.height())
   {
      QColor cl(scl);
      if (cl.isValid())
      {
         QRgb pcl = img_.pixel(x, y);
         return pcl == cl.rgb();
      }
   }
   
   return false;
}
