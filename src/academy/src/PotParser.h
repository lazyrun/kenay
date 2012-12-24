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
   //! ������� ��������
   virtual bool isSixProfile(const QVector<ProfileItem> & profile) const;
   //! ������ ������� ������
   virtual bool isRightThreeProfile(const QVector<ProfileItem> & profile) const;
   //! ����� ������� ������
   virtual bool isLeftThreeProfile(const QVector<ProfileItem> & profile) const;
   //! ����� ������� �������
   virtual bool isLeftFiveProfile(const QVector<ProfileItem> & profile) const;
   //! ������ ������� �������
   virtual bool isRightFiveProfile(const QVector<ProfileItem> & profile) const;
   //! ����� ������� ������
   virtual bool isLeftTwoProfile(const QVector<ProfileItem> & profile) const;
   //! ������ ������� ������
   virtual bool isRightTwoProfile(const QVector<ProfileItem> & profile) const;
   //! ����� ������� ��������
   virtual bool isLeftOneProfile(const QVector<ProfileItem> & profile) const;
   //! ������ ������� ��������
   virtual bool isRightOneProfile(const QVector<ProfileItem> & profile) const;
   //! ����� ������� �������
   virtual bool isLeftSevenProfile(const QVector<ProfileItem> & profile) const;
   //! ������ ������� �������
   virtual bool isRightSevenProfile(const QVector<ProfileItem> & profile) const;
};

#endif

