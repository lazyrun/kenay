/*! \file Session.h
    \brief 
    \author Kiselev Kirill
    \date 15.01.2013
*/

#ifndef Session_H
#define Session_H

#include "ImageProc.h"
#include "Opp.h"

/*!
\class Session
\brief 
*/
typedef QList<Opp::Action> ActionList;

class Session
{
public:   
   Session(CardProcessing * const proc, int cnt);
   //! ��������� ���������� �� ������� �������
   void saveStats(const QString & sessionId);
   //! ��������� ���������� ����
   void stat(Opp & opp);
   //!
   int tableFormat() const {return cnt_; };
protected:   
   void saveToDB();
   CardProcessing * const proc_;
   QString sessionID_;
   QMap<CardProcessing::Street, QMap<int, ActionList> > history_;
   QMap<int, Opp> opps_;
   int cnt_;
};

#endif

