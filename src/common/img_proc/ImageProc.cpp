#include "ImageProc.h"

//
//CardProcessing
//
CardProcessing::CardProcessing(const QString & mapfile)
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
   original_ = img;
   delete matrix_;
   matrix_ = new BoolMatrix(img, threshold_);
   
   holdemLevel_ = Undefined;
}

