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
   Session(CardProcessing * proc);
   void saveStats(const QString & sessionId);
protected:   
   void saveToDB();
   CardProcessing * proc_;
   QString sessionID_;
   QMap<CardProcessing::Street, QMap<int, ActionList> > history_;
   QMap<int, Opp> opps_;
};

#endif

