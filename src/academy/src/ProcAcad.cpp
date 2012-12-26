#include "ProcAcad.h"
#include "PotParser.h"
#include "StackParser.h"

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

bool ProcAcad::isDealer() const
{
   //����������� ����� ����� ������
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //����
   QString scl =
      config.settingValue("dealer", "").toString();
   //����������
   QString sx = config.settingAttribute("x", "dealer", "");
   QString sy = config.settingAttribute("y", "dealer", "");

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
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //����
   QString scl =
      config.settingValue("pot", "").toString();
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
   
   delete potMatrix;
   //����������� 4 ������ ����� ����� Pot:
   QList<BoolMatrix> potLetts = letts.mid(4);
   
   //for (int i = 0; i < potLetts.count(); i++)
   //{
   //   potLetts.at(i).save(QString("let_%1.bmp").arg(i));
   //}
   PotParser potParser;
   qreal val = ImgUtils::parseRealNumber(potLetts, &potParser);
   return val;
}

qreal ProcAcad::stack() const
{
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //����
   QString scl =
      config.settingValue("stack", "").toString();
   //����������
   QString sx = config.settingAttribute("x", "stack", "");
   QString sy = config.settingAttribute("y", "stack", "");
   QString sw = config.settingAttribute("w", "stack", "");
   QString sh = config.settingAttribute("h", "stack", "");
   
   int sok = 0; bool ok = false;
   int x = sx.toInt(&ok); sok += ok;
   int y = sy.toInt(&ok); sok += ok;
   int w = sw.toInt(&ok); sok += ok;
   int h = sh.toInt(&ok); sok += ok;
   if (sok != 4)
      return 0.0;

   QImage imgStack = img_.copy(x, y, w, h);
   BoolMatrix * stackMatrix = new BoolMatrix(imgStack, QColor(scl));
   
   //�������� �� ������
   QList<BoolMatrix> letts = 
      ImgUtils::splitByLetters(*stackMatrix);
   //����������� 1
   QList<BoolMatrix> stackLetts = letts.mid(1);

   delete stackMatrix;

   //for (int i = 0; i < stackLetts.count(); i++)
   //{
   //   stackLetts.at(i).save(QString("stack_%1.bmp").arg(i));
   //}

   StackParser stackParser;
   qreal val = ImgUtils::parseRealNumber(stackLetts, &stackParser);
   return val;
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

Opp ProcAcad::opp(const QString & num)
{
   if (oppMap_.isEmpty())
   {
      Settings & config = 
         ConfigGlobal<MainConfig>::Instance();
      
      QDomElement & rootNode = config.root();
      QDomElement oppsNode = rootNode.firstChildElement("opps");
      QDomNodeList oppList = oppsNode.elementsByTagName("opp");
      for (int i = 0; i < oppList.count(); i++)
      {
         QDomNode dnOpp = oppList.at(i);
         QString num = dnOpp.attributes().namedItem("num").nodeValue();
         oppMap_.insert(num, dnOpp);
      }
   }
   QDomNode dnOpp = oppMap_.value(num);

   if (dnOpp.isNull())
      return Opp();
   
   QDomNode dnNick = dnOpp.firstChildElement("nickname");
   if (dnNick.isNull())
      return Opp();

   QString sx = dnNick.attributes().namedItem("x").nodeValue();
   QString sy = dnNick.attributes().namedItem("y").nodeValue();
   QString sw = dnNick.attributes().namedItem("w").nodeValue();
   QString sh = dnNick.attributes().namedItem("h").nodeValue();
   QString scl = dnNick.firstChild().nodeValue();

   int sok = 0; bool ok = false;
   int x = sx.toInt(&ok); sok += ok;
   int y = sy.toInt(&ok); sok += ok;
   int w = sw.toInt(&ok); sok += ok;
   int h = sh.toInt(&ok); sok += ok;
   if (sok != 4)
      return Opp();


   QImage imgOpp = img_.copy(x, y, w, h);   
   //imgOpp.save("opp.bmp");
   
   if (scl.isEmpty())
      return Opp();

   QColor cl(scl);
   int r = 0, g = 0, b = 0;
   cl.getRgb(&r, &g, &b);
   BoolMatrix bmOpp(imgOpp, r, g, b);
   //bmOpp.save("wb_opp.bmp");

   QList<BoolMatrix> letts = 
      ImgUtils::splitByLetters(bmOpp);
   
   OppNick nick;
   for (int i = 0; i < letts.count(); i++)
   {
      //letts.at(i).save(QString("opp_%1.bmp").arg(i));
      QList<PointList> areas = ImgUtils::closedAreas(letts.at(i),
         ImgUtils::Four);      
      OppNick::OppLetter letter;
      letter.size = QSize(letts.at(i).width(), letts.at(i).height());
      letter.closed = areas.count();
      nick.letters.append(letter);
   }
   
   Opp opp;
   opp.setNick(nick);
   
   //���� ���������
   qreal stack = 0.;
   QDomNode dnStack = dnOpp.firstChildElement("stack");
   if (!dnStack.isNull())
   {
      sx = dnStack.attributes().namedItem("x").nodeValue();
      sy = dnStack.attributes().namedItem("y").nodeValue();
      sw = dnStack.attributes().namedItem("w").nodeValue();
      sh = dnStack.attributes().namedItem("h").nodeValue();
      scl = dnStack.firstChild().nodeValue();
      
      sok = 0; ok = false;
      x = sx.toInt(&ok); sok += ok;
      y = sy.toInt(&ok); sok += ok;
      w = sw.toInt(&ok); sok += ok;
      h = sh.toInt(&ok); sok += ok;
      if (sok == 4)
      {
         QImage imgStack = img_.copy(x, y, w, h);
         BoolMatrix * stackMatrix = new BoolMatrix(imgStack, QColor(scl));
   
         //�������� �� ������
         QList<BoolMatrix> letts = 
            ImgUtils::splitByLetters(*stackMatrix);
         //����������� 1
         QList<BoolMatrix> stackLetts = letts.mid(1);
         delete stackMatrix;

         StackParser stackParser;
         stack = ImgUtils::parseRealNumber(stackLetts, &stackParser);
      }
   }
   opp.setStack(stack);
   
   //������� ���� � ���������
   bool hasCards = false;
   QDomNode dnCards = dnOpp.firstChildElement("cards");
   if (!dnCards.isNull())
   {
      sx = dnCards.attributes().namedItem("x").nodeValue();
      sy = dnCards.attributes().namedItem("y").nodeValue();
      scl = dnCards.firstChild().nodeValue();
      
      hasCards = controlPixel(sx, sy, scl);
   }
   opp.setCards(hasCards);

   //������� ����� ������
   bool isDealer = false;
   QDomNode dnDealer = dnOpp.firstChildElement("dealer");
   if (!dnDealer.isNull())
   {
      sx = dnDealer.attributes().namedItem("x").nodeValue();
      sy = dnDealer.attributes().namedItem("y").nodeValue();
      scl = dnDealer.firstChild().nodeValue();
      
      isDealer = controlPixel(sx, sy, scl);
   }
   opp.setDealer(isDealer);

   return opp;
}
