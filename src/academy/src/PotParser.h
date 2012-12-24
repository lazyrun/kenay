/*! \file PotParser.h
    \brief 
    \author Kiselev Kirill
    \date 24.12.2012
*/

#ifndef PotParser_H
#define PotParser_H

#include "ImgUtils.h"
/*!
\class PotParser
\brief 
*/
class PotParser : public Parser
{
public:
   PotParser() {}
   virtual ~PotParser() {}
protected:
   //! Профиль шестерки
   virtual bool isSixProfile(const QVector<ProfileItem> & profile) const;
   //! Правый профиль тройки
   virtual bool isRightThreeProfile(const QVector<ProfileItem> & profile) const;
   //! Левый профиль тройки
   virtual bool isLeftThreeProfile(const QVector<ProfileItem> & profile) const;
   //! Левый профиль пятерки
   virtual bool isLeftFiveProfile(const QVector<ProfileItem> & profile) const;
   //! Правый профиль пятерки
   virtual bool isRightFiveProfile(const QVector<ProfileItem> & profile) const;
   //! Левый профиль двойки
   virtual bool isLeftTwoProfile(const QVector<ProfileItem> & profile) const;
   //! Правый профиль двойки
   virtual bool isRightTwoProfile(const QVector<ProfileItem> & profile) const;
   //! Левый профиль единички
   virtual bool isLeftOneProfile(const QVector<ProfileItem> & profile) const;
   //! Правый профиль единички
   virtual bool isRightOneProfile(const QVector<ProfileItem> & profile) const;
   //! Левый профиль семерки
   virtual bool isLeftSevenProfile(const QVector<ProfileItem> & profile) const;
   //! Правый профиль семерки
   virtual bool isRightSevenProfile(const QVector<ProfileItem> & profile) const;
};

#endif

