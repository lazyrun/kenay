/*! \file StackParser.h
    \brief 
    \author Kiselev Kirill
    \date 24.12.2012
*/

#ifndef StackParser_H
#define StackParser_H

#include "ImgUtils.h"

/*!
\class StackParser
\brief 
*/
class StackParser : public Parser
{
public:
   StackParser() {}
   virtual ~StackParser() {}
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

