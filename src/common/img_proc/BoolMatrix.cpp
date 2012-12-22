#include "BoolMatrix.h"

BoolMatrix::BoolMatrix()
{
   matrix_ = 0;
   bits_ = 0;
}

BoolMatrix::BoolMatrix(const BoolMatrix & rhs)
{
   width_  = rhs.width();
   height_ = rhs.height();
   matrix_ = new bool* [width_];
   bits_ = new bool[width_ * height_];
   for (int i = 0; i < width_; ++i)
   {
      matrix_[i] = bits_ + i * height_;
   }
   
   for (int x = 0; x < width_; ++x)
   {
      for (int y = 0; y < height_; ++y)
      {
         matrix_[x][y] = rhs.matrix_[x][y];
      }
   }
}

BoolMatrix::BoolMatrix(const BoolMatrix & rhs, const QRect & rect)
{
   width_  = rect.width();
   height_ = rect.height();
   matrix_ = new bool* [width_];
   bits_ = new bool[width_ * height_];
   for (int i = 0; i < width_; ++i)
   {
      matrix_[i] = bits_ + i * height_;
   }

   for (int x = 0; x < width_; ++x)
   {
      for (int y = 0; y < height_; ++y)
      {
         matrix_[x][y] = rhs.matrix_[rect.x() + x][rect.y() + y];
      }
   }

}

BoolMatrix & BoolMatrix::operator=(const BoolMatrix & rhs)
{
   delete [] matrix_;
   delete [] bits_;

   width_  = rhs.width();
   height_ = rhs.height();
   matrix_ = new bool* [width_];
   bits_ = new bool[width_ * height_];
   for (int i = 0; i < width_; ++i)
   {
      matrix_[i] = bits_ + i * height_;
   }
   
   for (int x = 0; x < width_; ++x)
   {
      for (int y = 0; y < height_; ++y)
      {
         matrix_[x][y] = rhs.matrix_[x][y];
      }
   }

   return (*this);
}

BoolMatrix::BoolMatrix(const QImage & img)
{
   width_  = img.width();
   height_ = img.height();

   matrix_ = new bool* [width_];
   bits_ = new bool[width_ * height_];
   for (int i = 0; i < width_; ++i)
   {
      matrix_[i] = bits_ + i * height_;
   }
}

BoolMatrix::BoolMatrix(const QString & str)
{
   width_  = 0;
   height_ = 0;

   QStringList lst = str.split(":");
   if (lst.count() == 3)
   {
      bool ok = false;
      width_ = lst.at(0).toInt(&ok);
      if (!ok)
         return;
      height_ = lst.at(1).toInt(&ok);
      if (!ok)
         return;
      
      QString data = lst.at(2);
      int cnt = data.count();
      if (width_ * height_ != cnt)
         return;

	   matrix_ = new bool* [width_];
      bits_ = new bool[width_ * height_];
      for (int i = 0; i < width_; ++i)
      {
         matrix_[i] = bits_ + i * height_;
      }

      for (int y = 0; y < height_; ++y)
      {
         for (int x = 0; x < width_; ++x)
         {
            bool val = data.at(x + y * width_).digitValue();
            if (ok)
               matrix_[x][y] = val;
         }
      }
   }
}

void BoolMatrix::set(int x, int y, bool value)
{
   if (x < width_ && y < height_)
      matrix_[x][y] = value;
}

BoolMatrix::BoolMatrix(const QImage * img)
{
   width_  = img->width();
   height_ = img->height();

	matrix_ = new bool* [width_];
   bits_ = new bool[width_ * height_];
   for (int i = 0; i < width_; ++i)
   {
      matrix_[i] = bits_ + i * height_;
   }

   for (int w = 0; w < width_; ++w)
   {
      for (int h = 0; h < height_; ++h)
      {
         if (img->pixel(w, h) == qRgb(0, 0, 0))
         {
            matrix_[w][h] = 1;//черный
         }
         else
         {
            matrix_[w][h] = 0;//белый
         }
      }
   }
}

BoolMatrix::BoolMatrix(const QImage & img, uchar threshold)
{
   width_  = img.width();
   height_ = img.height();

	matrix_ = new bool* [width_];
   bits_ = new bool[width_ * height_];
   for (int i = 0; i < width_; ++i)
   {
      matrix_[i] = bits_ + i * height_;
   }

   for (int w = 0; w < width_; ++w)
   {
      for (int h = 0; h < height_; ++h)
      {
         QRgb rgb = img.pixel(w, h);
         if (qGray(rgb) < threshold)
         {
            matrix_[w][h] = 1;//черный
         }
         else
         {
            matrix_[w][h] = 0;//белый
         }
      }
   }
}

BoolMatrix::BoolMatrix(const QImage & img, const QColor & color)
{
   width_  = img.width();
   height_ = img.height();

	matrix_ = new bool* [width_];
   bits_ = new bool[width_ * height_];
   for (int i = 0; i < width_; ++i)
   {
      matrix_[i] = bits_ + i * height_;
   }

   for (int w = 0; w < width_; ++w)
   {
      for (int h = 0; h < height_; ++h)
      {
         QRgb rgb = img.pixel(w, h);
         if (rgb == color.rgb())
         {
            matrix_[w][h] = 1;//черный
         }
         else
         {
            matrix_[w][h] = 0;//белый
         }
      }
   }
}

BoolMatrix::BoolMatrix(const QImage & img, uchar grayThresh, 
                       uint minBright, uint maxBright)
{
   width_  = img.width();
   height_ = img.height();

	matrix_ = new bool* [width_];
   bits_ = new bool[width_ * height_];
   for (int i = 0; i < width_; ++i)
   {
      matrix_[i] = bits_ + i * height_;
   }

   for (int w = 0; w < width_; ++w)
   {
      for (int h = 0; h < height_; ++h)
      {
         QRgb rgb = img.pixel(w, h);
         QColor cl(rgb);

         if (qGray(rgb) < grayThresh)
         {
            matrix_[w][h] = 1;//черный
         }
         else
         {
            if (cl.value() >= (int)minBright && cl.value() <= (int)maxBright) 
            {
               matrix_[w][h] = 0;//белый
            }
            else
            {
               matrix_[w][h] = 1;
            }
         }
      }
   }
}

BoolMatrix::~BoolMatrix()
{
   delete [] matrix_;
   delete [] bits_;
}

bool BoolMatrix::at(int x, int y) const
{
   if ((x >= width_ || y >= height_) ||
      (x < 0 || y < 0))
      return false;
   return matrix_[x][y];
}

bool BoolMatrix::isWhite(int x, int y) const
{
   if ((x >= width_ || y >= height_) ||
      (x < 0 || y < 0))
      return false;
   return matrix_[x][y] == 0;
}

bool BoolMatrix::isBlack(int x, int y) const
{
   if ((x >= width_ || y >= height_) ||
      (x < 0 || y < 0))
      return false;
   return matrix_[x][y] == 1;
}

void BoolMatrix::fillRect(const QRect & rect, bool color)
{
   for (int x = rect.left(); x <= rect.right(); x++)
   {
      for (int y = rect.top(); y <= rect.bottom(); y++)
      {
         matrix_[x][y] = color;
      }
   }
}

int BoolMatrix::operator-(const BoolMatrix & rhs)
{
   int w = width_  <= rhs.width_  ? width_  : rhs.width_;
   int h = height_ <= rhs.height_ ? height_ : rhs.height_;

   int count = 0;
   for (int x = 0; x < w; ++x)
   {
      for (int y = 0; y < h; ++y)
      {
         if (matrix_[x][y] != rhs.matrix_[x][y])
         {
            count++;
         }
      }
   }
   
   return count;
}

QString BoolMatrix::toString() const
{
   QString res;
   res = QString("%1:%2:").arg(width_).arg(height_);
   for (int y = 0; y < height_; ++y)
   {
      for (int x = 0; x < width_; ++x)
      {
         res += QString::number(at(x, y));
      }
   }
   
   return res;
}

bool BoolMatrix::save(const QString & filename) const
{
   bool res = false;
   QImage img(width_, height_, QImage::Format_Mono);
   QVector<QRgb> palette;
   palette.append(qRgb(255, 255, 255));//белый
   palette.append(qRgb(0, 0, 0));//черный
   img.setColorTable(palette);

   for (int x = 0; x < width_; ++x)
   {
      for (int y = 0; y < height_; ++y)
      {
         img.setPixel(x, y, matrix_[x][y]);
      }
   }
   img.save(filename);
   return res;
}
