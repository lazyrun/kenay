#ifndef BOOLMATRIX_H
#define BOOLMATRIX_H

typedef QList<QPoint> Border;

class BoolMatrix
{
public:
   //! �����������
   BoolMatrix();
   //! ���������� �����������
   BoolMatrix(const BoolMatrix & rhs);
   //!
   BoolMatrix(const BoolMatrix & rhs, const QRect & rect);
   //! � ������� �������� ������ ������� ��������� �����
   BoolMatrix(const QImage & img, const QColor & color);

   //! ������� ����� ������� �� ������ (����� �� ������)
   BoolMatrix(const QImage & img, uchar threshold, bool inverse = false);

   //! �������� ����� ����� ��������
   BoolMatrix(const QImage * img);
   //! 
   BoolMatrix(const QImage & img, uchar grayThresh, uint minBright, uint maxBright);
   //! ������� ��������� �� �� �����������
   BoolMatrix(const QImage & img);
   //! ������� ��������� �� ������
   BoolMatrix(const QString & str);
   //!
   ~BoolMatrix();

   QString toString() const;

   int operator-(const BoolMatrix & rhs);
   BoolMatrix & operator=(const BoolMatrix & rhs);

   int width()  const { return width_;  }
   int height() const { return height_; }

   //! ������ ��������
   void set(int x, int y, bool value);
   //! �������� ��������
   bool at(int x, int y) const;
   //! ����� �����
   bool isWhite(int x, int y) const;
   //! ������ �����
   bool isBlack(int x, int y) const;
   //! ������ �������
   void fillRect(const QRect & rect, bool color);
   //! ��������� ������� ��� �/� ��������
   bool save(const QString & filename) const;

protected:
   bool ** matrix_;
   bool * bits_;
   int width_;
   int height_;
};

#endif
