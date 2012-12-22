#include "ProcAcad.h"
#include "ImgUtils.h"

double round (double x, int precision)
{
   int mul = 10;
   
   for (int i = 0; i < precision; i++)
      mul *= mul;
   if (x > 0)
      return floor(x * mul + .5) / mul;
   else
      return ceil(x * mul - .5) / mul;
}

//
//ProcAcad
//
ProcAcad::ProcAcad(const QString & mapfile)
: CardProcessing()
{
   ConfigGlobal<MainConfig>::Instance(mapfile, "root");
}

ProcAcad::~ProcAcad()
{
}

bool ProcAcad::hasFold() const 
{
   //����������� ����� �����
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //����
   QString scl =
      config.settingValue("buttons", "fold", "control", "").toString();
   //����������
   QString sx = config.settingAttribute("x", "buttons", "fold", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "fold", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::hasCall() const
{
   //����������� ����� ����
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //����
   QString scl =
      config.settingValue("buttons", "call", "control", "").toString();
   //����������
   QString sx = config.settingAttribute("x", "buttons", "call", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "call", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::hasCheck() const
{
   //����������� ����� ����
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //����
   QString scl =
      config.settingValue("buttons", "check", "control", "").toString();
   //����������
   QString sx = config.settingAttribute("x", "buttons", "check", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "check", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::hasRaise() const
{
   //����������� ����� �����
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //����
   QString scl =
      config.settingValue("buttons", "raise", "control", "").toString();
   //����������
   QString sx = config.settingAttribute("x", "buttons", "raise", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "raise", "control", "");

   return controlPixel(sx, sy, scl);
}

QString ProcAcad::holeCard(const QString & scard) const
{
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   QScopedArrayPointer<char> card(new char[scard.length() + 1]);
   strcpy(card.data(), scard.toStdString().c_str());
   //������� �����������
   //�������� ����� �� �������
   //����
   QString scl =
      config.settingValue("hole", card, "control", "").toString();
   //����������
   QString sx = config.settingAttribute("x", "hole", card, "control", "");
   QString sy = config.settingAttribute("y", "hole", card, "control", "");
   if (!controlPixel(sx, sy, scl))
      return QString();
   //������� �����������
   sx = config.settingAttribute("x", "hole", card, "");
   sy = config.settingAttribute("y", "hole", card, "");
   QString sw = config.settingAttribute("w", "hole", card, "");
   QString sh = config.settingAttribute("h", "hole", card, "");
   int sok = 0; bool ok = false;
   int x = sx.toInt(&ok); sok += ok;
   int y = sy.toInt(&ok); sok += ok;
   int w = sw.toInt(&ok); sok += ok;
   int h = sh.toInt(&ok); sok += ok;
   if (sok != 4)
      return QString();

   QImage imgFirst = img_.copy(x, y, w, h);

   return cardFromImage(imgFirst);
}

qreal ProcAcad::pot() const
{
   //����������� ����� �����
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //����
   QString scl =
      config.settingValue("pot", "color", "").toString();
   //����������
   QString sx = config.settingAttribute("x", "pot", "");
   QString sy = config.settingAttribute("y", "pot", "");
   QString sw = config.settingAttribute("w", "pot", "");
   QString sh = config.settingAttribute("h", "pot", "");
   
   int sok = 0; bool ok = false;
   int x = sx.toInt(&ok); sok += ok;
   int y = sy.toInt(&ok); sok += ok;
   int w = sw.toInt(&ok); sok += ok;
   int h = sh.toInt(&ok); sok += ok;
   if (sok != 4)
      return 0.0;

   QImage imgPot = img_.copy(x, y, w, h);
   BoolMatrix * potMatrix = new BoolMatrix(imgPot, QColor(scl));
   //�������� �� ������
   QList<BoolMatrix> letts = 
      ImgUtils::splitByLetters(*potMatrix);
   //����������� 4 ������ ����� ����� Pot:
   QList<BoolMatrix> potLetts = letts.mid(4);
   for (int i = 0; i < potLetts.count(); i++)
   {
      potLetts.at(i).save(QString("let_%1.bmp").arg(i));
   }

   qreal val = 0.0;
   int dot = 0;
   //������� ����� �����
   for (int i = 0; i < potLetts.count(); i++)
   {
      bool isDot = ImgUtils::isDot(potLetts[i]);
      if (!isDot)
      {
         qreal digit = ImgUtils::parseDigit(potLetts[i]);
         val = val * 10. + digit;
      }
      else
      {
         dot = i + 1;
         break;
      }
   }
   //������� ������� �����
   qreal fract = 0.0;
   for (int i = 0; i < potLetts.count() - dot; i++)
   {
      qreal digit = ImgUtils::parseDigit(potLetts[dot + i]);
      fract = fract + digit / qPow(10., i + 1);
   }

   val += fract;
   return val;
}

qreal ProcAcad::stack() const
{
   return 0.;
}

QString ProcAcad::cardFromImage(QImage & img) const
{
   int imgW = img.width();
   int imgH = img.height();

   int minDist = imgW * imgH;
   int minIdx = 0;
   
   BoolMatrix * whatMatrix = new BoolMatrix(img, 200);
   const int baseCount = cardBase_.count();
   for (int i = 0; i < baseCount; ++i)
   {
      const BoolMatrix * bm = cardBase_.matrix(i);
      //����������� ������������� ��� ������
      if ((bm->width()  != imgW) ||
          (bm->height() != imgH) )
      {
         img = img.scaled(imgW, imgH);
         delete whatMatrix;
         whatMatrix = new BoolMatrix(img, 200);
      }
      int res = *whatMatrix - *bm;

      if (res < minDist)
      {
         minDist = res;
         minIdx = i;
      }
      if (res == 0)
         break;
   }
   delete whatMatrix;
   
   return cardBase_.nominal(minIdx);
}
