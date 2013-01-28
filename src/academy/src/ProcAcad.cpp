#include "ProcAcad.h"
#include "PotParser.h"
#include "StackParser.h"
#include "BetParser.h"

//
//ProcAcad
//
ProcAcad::ProcAcad(const QString & mapfile)
: CardProcessing()
{
   QString fn = ConfigGlobal<MainConfig>::Instance(mapfile, "root").fileName();
   QString rn = ConfigGlobal<MainConfig>::Instance(mapfile, "root").rootName();
   if (fn != mapfile)
      ConfigGlobal<MainConfig>::Instance(mapfile, "root").init(mapfile, rn);
}

ProcAcad::~ProcAcad()
{
}

bool ProcAcad::hasFold() const 
{
   //контрольна€ точка фолда
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("buttons", "fold", "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "buttons", "fold", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "fold", "control", "");

   return controlPixel(sx, sy, scl);
}

CardProcessing::Street ProcAcad::street()
{
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("river", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "river", "");
   QString sy = config.settingAttribute("y", "river", "");

   if (controlPixel(sx, sy, scl))
   {
      return street_ = River;
   }
   
   sx = config.settingAttribute("x", "turn", "");
   sy = config.settingAttribute("y", "turn", "");
   if (controlPixel(sx, sy, scl))
   {
      return street_ = Turn;
   }
   
   sx = config.settingAttribute("x", "flop", "");
   sy = config.settingAttribute("y", "flop", "");
   if (controlPixel(sx, sy, scl))
   {
      return street_ = Flop;
   }
   
   sx = config.settingAttribute("x", "preflop", "");
   sy = config.settingAttribute("y", "preflop", "");
   if (controlPixel(sx, sy, scl))
   {
      return street_ = Preflop;
   }

   return Undefined;
}

bool ProcAcad::hasCall() const
{
   //контрольна€ точка кола
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("buttons", "call", "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "buttons", "call", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "call", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::hasCheck() const
{
   //контрольна€ точка чека
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("buttons", "check", "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "buttons", "check", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "check", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::hasRaise() const
{
   //контрольна€ точка рейза
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("buttons", "raise", "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "buttons", "raise", "control", "");
   QString sy = config.settingAttribute("y", "buttons", "raise", "control", "");

   return controlPixel(sx, sy, scl);
}

bool ProcAcad::isDealer() const
{
   //контрольна€ точка фошки дилера
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("dealer", "").toString();
   //координаты
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
   //снимаем координтаты
   //контроль карты по пикселю
   //цвет
   QString scl =
      config.settingValue("hole", card, "control", "").toString();
   //координаты
   QString sx = config.settingAttribute("x", "hole", card, "control", "");
   QString sy = config.settingAttribute("y", "hole", card, "control", "");
   if (!controlPixel(sx, sy, scl))
      return QString();
   //снимаем изображение
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
   //цвет
   QString scl =
      config.settingValue("pot", "").toString();
   //координаты
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
   
   //нарезаем по буквам
   QList<BoolMatrix> letts = 
      ImgUtils::splitByLetters(*potMatrix);
   
   delete potMatrix;
   qreal val = 0.0;
   if (letts.count() > 14)
   {
      //отбрасываем 5 первых букв слова Pots:
      QList<BoolMatrix> potLetts = letts.mid(5);

      //Pots: $1, Main: $
      //ищем M в слове Main
      int Midx = 0;
      int maxWidth = 0;
      for (int i = 0; i < potLetts.count(); i++)
      {
         int lettWidth = potLetts.at(i).width();
         if (lettWidth > maxWidth)
         {
            maxWidth = lettWidth;
            Midx = i;
         }
      }
      //отбрасываем 5 первых букв слова Pots:
      potLetts = letts.mid(5, Midx - 1);
      PotParser potParser;
      val = ImgUtils::parseRealNumber(potLetts, &potParser);

      potLetts = letts.mid(Midx + 11);
      val += ImgUtils::parseRealNumber(potLetts, &potParser);
   }
   else if (letts.count() > 4)
   {
      //отбрасываем 4 первые буквы слова Pot:
      QList<BoolMatrix> potLetts = letts.mid(4);
      
      //for (int i = 0; i < potLetts.count(); i++)
      //{
      //   potLetts.at(i).save(QString("let_%1.bmp").arg(i));
      //}
      PotParser potParser;
      val = ImgUtils::parseRealNumber(potLetts, &potParser);
   }
   return val;
}

qreal ProcAcad::stack() const
{
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   //цвет
   QString scl =
      config.settingValue("stack", "").toString();
   //координаты
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
   
   //нарезаем по буквам
   QList<BoolMatrix> letts = 
      ImgUtils::splitByLetters(*stackMatrix);
   delete stackMatrix;
   qreal val = 0.0;
   if (letts.count() > 1)
   {
      //отбрасываем 1
      QList<BoolMatrix> stackLetts = letts.mid(1);

      //for (int i = 0; i < stackLetts.count(); i++)
      //{
      //   stackLetts.at(i).save(QString("stack_%1.bmp").arg(i));
      //}

      StackParser stackParser;
      val = ImgUtils::parseRealNumber(stackLetts, &stackParser);
   }
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
      //изображение подт€гиваетс€ под эталон
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
   
   Opp opp;
   parseOppNick(dnOpp, opp);
   parseOppStack(dnOpp, opp);
   parseOppInGame(dnOpp, opp);
   parseOppIsDealer(dnOpp, opp);
   parseOppBet(dnOpp, opp);
   
   return opp;
}

void ProcAcad::parseOppNick(const QDomNode & dnOpp, Opp & opp)
{
   QDomNode dnNick = dnOpp.firstChildElement("nickname");
   if (dnNick.isNull())
      return;

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
      return;

   QImage imgOpp = img_.copy(x, y, w, h);   
   //imgOpp.save("opp.bmp");
   
   if (scl.isEmpty())
      return;

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
   
   opp.setNick(nick);
}

void ProcAcad::parseOppStack(const QDomNode & dnOpp, Opp & opp)
{
   //стек оппонента
   qreal stack = 0.;
   QDomNode dnStack = dnOpp.firstChildElement("stack");
   if (!dnStack.isNull())
   {
      QString sx = dnStack.attributes().namedItem("x").nodeValue();
      QString sy = dnStack.attributes().namedItem("y").nodeValue();
      QString sw = dnStack.attributes().namedItem("w").nodeValue();
      QString sh = dnStack.attributes().namedItem("h").nodeValue();
      QString scl = dnStack.firstChild().nodeValue();
      
      int sok = 0; bool ok = false;
      int x = sx.toInt(&ok); sok += ok;
      int y = sy.toInt(&ok); sok += ok;
      int w = sw.toInt(&ok); sok += ok;
      int h = sh.toInt(&ok); sok += ok;
      if (sok == 4)
      {
         QImage imgStack = img_.copy(x, y, w, h);
         BoolMatrix * stackMatrix = new BoolMatrix(imgStack, QColor(scl));
   
         //нарезаем по буквам
         QList<BoolMatrix> letts = 
            ImgUtils::splitByLetters(*stackMatrix);
         
         delete stackMatrix;

         //отбрасываем 1
         if (letts.count() > 1)
         {
            QList<BoolMatrix> stackLetts = letts.mid(1);
            
            //for (int i = 0; i < stackLetts.count(); i++)
            //{
            //   stackLetts.at(i).save(QString("stack_%1.bmp").arg(i));
            //}

            StackParser stackParser;
            stack = ImgUtils::parseRealNumber(stackLetts, &stackParser);
         }
      }
   }
   opp.setStack(stack);
}

void ProcAcad::parseOppInGame(const QDomNode & dnOpp, Opp & opp)
{
   //наличие карт у оппонента
   bool hasCards = false;
   QDomNode dnCards = dnOpp.firstChildElement("cards");
   if (!dnCards.isNull())
   {
      QString sx = dnCards.attributes().namedItem("x").nodeValue();
      QString sy = dnCards.attributes().namedItem("y").nodeValue();
      QString scl = dnCards.firstChild().nodeValue();
      
      hasCards = controlPixel(sx, sy, scl);
   }
   opp.setCards(hasCards);
}

void ProcAcad::parseOppIsDealer(const QDomNode & dnOpp, Opp & opp)
{
   //наличие фишки дилера
   bool isDealer = false;
   QDomNode dnDealer = dnOpp.firstChildElement("dealer");
   if (!dnDealer.isNull())
   {
      QString sx = dnDealer.attributes().namedItem("x").nodeValue();
      QString sy = dnDealer.attributes().namedItem("y").nodeValue();
      QString scl = dnDealer.firstChild().nodeValue();
      
      isDealer = controlPixel(sx, sy, scl);
   }
   opp.setDealer(isDealer);
}

void ProcAcad::parseOppBet(const QDomNode & dnOpp, Opp & opp)
{
   //ставка оппонента
   qreal bet = 0.;
   QDomNode dnBet = dnOpp.firstChildElement("bet");
   if (!dnBet.isNull())
   {
      QString sx = dnBet.attributes().namedItem("x").nodeValue();
      QString sy = dnBet.attributes().namedItem("y").nodeValue();
      QString sw = dnBet.attributes().namedItem("w").nodeValue();
      QString sh = dnBet.attributes().namedItem("h").nodeValue();
      QString scl = dnBet.firstChild().nodeValue();
      
      int sok = 0; bool ok = false;
      int x = sx.toInt(&ok); sok += ok;
      int y = sy.toInt(&ok); sok += ok;
      int w = sw.toInt(&ok); sok += ok;
      int h = sh.toInt(&ok); sok += ok;
      if (sok == 4)
      {
         QImage imgBet = img_.copy(x, y, w, h);
         QScopedPointer<BoolMatrix> betMatrix(new BoolMatrix(imgBet, QColor(scl)));
   
         //нарезаем по буквам
         QList<BoolMatrix> letts = 
            ImgUtils::splitByLetters(*betMatrix);
         if (letts.count() == 0)
            return;

         Settings & config = 
            ConfigGlobal<MainConfig>::Instance();
         //
         QString ssb =
            config.settingValue("smallblind", "").toString();
         QString sbb =
            config.settingValue("bigblind", "").toString();
         
         qreal smallBlind = ssb.toDouble(&ok);
         if (!ok)
            smallBlind = 0.;

         qreal bigBlind   = sbb.toDouble(&ok);
         if (!ok)
            bigBlind = 0.;


         //берем первую букву
         //возможные варианты
         // Blind $ - 2 дырки, Bet $
         // Call $, Check  - ни одной
         // Raise $ - 1 дырка {из за того что слитно, то перва€ буква идет как Ra = 2 дырки, но зато 4-€ с дыркой}
         // Fold
         
         //for (int i = 0; i < letts.count(); i++)
         //{
         //   letts.at(i).save(QString("bet_%1.bmp").arg(i));
         //}

         QList<PointList> areasFirst = ImgUtils::closedAreas(letts.at(0),
               ImgUtils::Four);
         QList<PointList> areasSecond = ImgUtils::closedAreas(letts.at(1),
               ImgUtils::Four);
         QList<PointList> areasFour = ImgUtils::closedAreas(letts.at(3),
               ImgUtils::Four);
         
         if (letts.count() == QString("Fold").count())
         {
            opp.setAction(Opp::Fold);
         }
         else if (letts.count() == QString("Check").count())
         {
            opp.setAction(Opp::Check);
         }
         else
         {
            int midPos = 0;
            bool isBlind = false;
            if (areasFirst.count() == 2)
            {
               if (areasFour.count() == 0)
               {
                  if (areasSecond.count() == 0)
                  {
                     midPos = QString("Blind$").count();
                     isBlind = true;
                  }
               }
               else if (areasFour.count() == 1)
               {
                  midPos = QString("Raise$").count();
                  opp.setAction(Opp::Raise);
               }
               else if (areasFour.count() == 2)
               {
                  midPos = QString("Bet$").count();
                  opp.setAction(Opp::Bet);
               }
            }
            else
            {
               midPos = QString("Call$").count();
               opp.setAction(Opp::Call);
            }
            //отбрасываем
            QList<BoolMatrix> betLetts = letts.mid(midPos);
            
            BetParser betParser;
            bet = ImgUtils::parseRealNumber(betLetts, &betParser);
            if (isBlind)
            {
               if (qFuzzyCompare(bet, smallBlind))
               {
                  opp.setAction(Opp::SmallBlind);
               }
               else if (qFuzzyCompare(bet, bigBlind))
               {
                  opp.setAction(Opp::BigBlind);
               }
               else
               {
                  opp.setAction(Opp::BigBlind);
               }
            }
         }
      }
   }
   opp.setBet(bet);
}

const QRect ProcAcad::foldRect() const
{
   return buttonRect("fold");
}

const QRect ProcAcad::checkRect() const
{
   return buttonRect("check");
}

const QRect ProcAcad::callRect() const
{
   return buttonRect("call");
}

const QRect ProcAcad::raiseRect() const
{
   return buttonRect("raise");
}

const QRect ProcAcad::betRect() const
{
   return buttonRect("bet");
}

const QRect ProcAcad::buttonRect(const char * btn) const
{
   Settings & config = 
      ConfigGlobal<MainConfig>::Instance();
   
   QString sx = config.settingAttribute("x", "buttons", btn, "");
   QString sy = config.settingAttribute("y", "buttons", btn, "");
   QString sw = config.settingAttribute("w", "buttons", btn, "");
   QString sh = config.settingAttribute("h", "buttons", btn, "");

   int sok = 0; bool ok = false;
   int x = sx.toInt(&ok); sok += ok;
   int y = sy.toInt(&ok); sok += ok;
   int w = sw.toInt(&ok); sok += ok;
   int h = sh.toInt(&ok); sok += ok;
   if (sok != 4)
      return QRect();

   return QRect(x, y, w, h);
}
