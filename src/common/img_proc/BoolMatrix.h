#ifndef BOOLMATRIX_H
#define BOOLMATRIX_H

typedef QList<QPoint> Border;

class BoolMatrix
{
public:
   //!  онструктор
   BoolMatrix();
   //! копирующий конструктор
   BoolMatrix(const BoolMatrix & rhs);
   //!
   explicit BoolMatrix(const BoolMatrix & rhs, const QRect & rect);
   //! ¬ матрице остаютс€ только пиксели заданного цвета
   explicit BoolMatrix(const QImage & img, const QColor & color);

   //! ¬ матрице остаютс€ только пиксели R,G,B которых больше заданого значени€
   explicit BoolMatrix(const QImage & img, int r, int g, int b);

   //! ћатрица сразу режетс€ по серому 
   explicit BoolMatrix(const QImage & img, uchar threshold, bool inverse = false);

   //! ѕодаетс€ черно бела€ картинка
   explicit BoolMatrix(const QImage * img);
   //! 
   explicit BoolMatrix(const QImage & img, uchar grayThresh,
      uint minBright, uint maxBright);
   //! матрица создаетс€ но не заполн€етс€
   explicit BoolMatrix(const QImage & img);
   //! матрица создаетс€ из строки
   explicit BoolMatrix(const QString & str);
   //!
   ~BoolMatrix();

   QString toString() const;

   int operator-(const BoolMatrix & rhs);
   BoolMatrix & operator=(const BoolMatrix & rhs);

   int width()  const { return width_;  }
   int height() const { return height_; }

   //! «адать значение
   void set(int x, int y, bool value);
   //! ѕолучить значение
   bool at(int x, int y) const;
   //! бела€ точка
   bool isWhite(int x, int y) const;
   //! черна€ точка
   bool isBlack(int x, int y) const;
   //! залить область
   void fillRect(const QRect & rect, bool color);
   //! —охранить матрицу как ч/б картинку
   bool save(const QString & filename) const;

protected:
   bool ** matrix_;
   bool * bits_;
   int width_;
   int height_;
};

#endif
