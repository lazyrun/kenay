#include "StackParser.h"

bool StackParser::isSixProfile(const QVector<ProfileItem> & profile) const
{
   //�������� ������� 6
   //     1) ���� �� ������ ������ ������ ��������� 
   //���  2) ����� ���� ����� � ��� ����� ����� ���� ����� �������
/*
profile = [2](
{item=false value=0.77777777777777779 },
{item=true value=0.22222222222222221 })
*/
   bool isSix = true;
   if (profile.count() > 1)
   {
      //����� ������������ �����
      int max_idx = 0;
      qreal maxval = 0.;
      for (int i = 0; i < profile.count(); ++i)
      {
         if (profile.at(i).value > maxval)
         {
            maxval = profile.at(i).value;
            max_idx = i;
         }
      }
      
      for (int i = 0; i < profile.count(); ++i)
      {
         if (i == max_idx)
            continue;
         
         if (profile.at(i).value * 3. >= maxval)
         {
            //������� �� ���������
            isSix = false;
            break;
         }
      }
   }
   else if (profile.count() == 1)
   {
      if (profile.at(0).value < 0.75)
         isSix = false;
   }
   else
   {
      isSix = false;
   }

   return isSix;
}

bool StackParser::isRightThreeProfile(const QVector<ProfileItem> & profile) const
{
//true 0.0588235 
//false 0.352941 
//true 0.0588235 
//false 0.411765 
//true 0.0588235 
  
   if (profile.count() < 3)
      return false;

   if (profile.count() == 5)
   {
      if ((profile.at(0).item == true)    &&
          (profile.at(1).item == false) &&
          (profile.at(2).item == true) && 
          (profile.at(3).item == false) && 
          (profile.at(4).item == true))
          return true;

   }
      
   return false;
}

bool StackParser::isLeftThreeProfile(const QVector<ProfileItem> & profile) const
{
/*
   profile = [5](
   {item=false value=0.22222222222222221 },
   {item=true value=0.11111111111111110 },
   {item=false value=0.11111111111111110 },
   {item=true value=0.33333333333333331 },
   {item=false value=0.22222222222222221 })
*/
/*
profile = [5](
{item=false value=0.22222222222222221 },
{item=true value=0.22222222222222221 },
{item=false value=0.11111111111111110 },
{item=true value=0.22222222222222221 },
{item=false value=0.22222222222222221 })
*/
   if (profile.count() < 3)
      return false;
   if (profile.count() == 5)
   {
      if ((profile.at(0).item == false)     &&
          (profile.at(1).item == true)  &&
          (profile.at(2).item == false) && 
          (profile.at(3).item == true) && 
          (profile.at(4).item == false))
      {
         if (profile.at(0).value > profile.at(2).value &&
             profile.at(4).value > profile.at(2).value)
             return true;
      }
   }
   return false;

//false 0.222222 
//true 0.444444 
//false 0.222222 Z
//true 0.0555556 
//------ 
//true 0.0555556 
//false 0.833333 
//true 0.0555556 
//"3c" "" 
//
//false 0.222222 
//true 0.444444 
//false 0.277778 
//------ 
//true 0.0555556 
//false 0.833333 
//true 0.0555556 
//"3d" "" 
//
//false 0.235294 
//true 0.470588 
//false 0.235294 
//------ 
//false 0.941176 
//"3h" "" 
//
//false 0.235294 
//true 0.411765 
//false 0.294118 
//------ 
//true 0.0588235 
//false 0.823529 
//true 0.0588235 
//"3s" "" 

   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx + 1).value > 0.35) && 
       (profile.at(idx).value < 0.30) &&
       (profile.at(idx + 2).value < 0.30))
       return true;
      
   return false;
}


bool StackParser::isLeftFiveProfile(const QVector<ProfileItem> & profile) const
{
//"2c" "" 
//false 0.555556 
//true 0.222222 
//false 0.166667 
// or
//true 0.0769231 
//false 0.384615 
//true 0.384615 
//false 0.0769231 
   if (profile.count() == 4)
   {
      if ((profile.at(0).item == true)    &&
          (profile.at(1).item == false) &&
          (profile.at(2).item == true) && 
          (profile.at(3).item == false))
      {
         if ((profile.at(1).value > profile.at(0).value) && 
             (profile.at(2).value > profile.at(3).value))
             return true;
      }
   }

   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx).value >= 0.30) && 
       (profile.at(idx + 1).value < 0.35) &&
       (profile.at(idx + 2).value < 0.30))
       return true;
      
   return false;
}

bool StackParser::isRightFiveProfile(const QVector<ProfileItem> & profile) const
{
//false 0.111111 
//true 0.277778 
//false 0.555556 
//"5c" "" 
//false 0.111111 
//true 0.277778 
//false 0.555556 
//"5d" "" 
//false 0.117647 
//true 0.235294 
//false 0.588235 
//"5h" "" 
//false 0.117647 
//true 0.235294 
//false 0.588235 
//"5s" "" 
   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx + 2).value >= 0.40) && 
       (profile.at(idx).value < 0.30) &&
       (profile.at(idx + 1).value < 0.33))
       return true;
      
   return false;

}

bool StackParser::isLeftTwoProfile(const QVector<ProfileItem> & profile) const
{
//"2h" "" 
//false 0.294118 
//true 0.294118 
//false 0.352941 
   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx + 2).value > profile.at(idx).value) && 
       (profile.at(idx + 2).value > profile.at(idx + 1).value) &&
       qAbs<qreal>(profile.at(idx).value - profile.at(idx + 1).value < 0.1))
       return true;
   
   //������������ ���������� ���� ������������ ����� �� ����� 15%
   //qreal diff1 = qAbs(profile.at(idx).value - profile.at(idx + 1).value);
   //qreal diff2 = qAbs(profile.at(idx).value - profile.at(idx + 2).value);
   //qreal diff3 = qAbs(profile.at(idx + 1).value - profile.at(idx + 2).value);
   //qreal max_off = diff1;
   //if (diff2 > max_off)
   //   max_off = diff2;
   //if (diff3 > max_off)
   //   max_off = diff3;

   //if (max_off > 0.18)
   //   return false;
   
   return true;
}

bool StackParser::isRightTwoProfile(const QVector<ProfileItem> & profile) const
{
//"2c" "" 
//false 0.555556 
//true 0.222222 
//false 0.166667 
// or
//true 0.0769231 
//false 0.384615 
//true 0.384615 
//false 0.0769231 
   if (profile.count() == 4)
   {
      if ((profile.at(0).item == true)    &&
          (profile.at(1).item == false) &&
          (profile.at(2).item == true) && 
          (profile.at(3).item == false))
      {
         if ((profile.at(1).value > profile.at(0).value) && 
             (profile.at(2).value > profile.at(3).value))
             return true;
      }
   }

   if (profile.count() < 3)
      return false;

   int idx = -1;
   if (profile.count() >= 3)
   {
      for (int i = 0; i < profile.count() - 2; i++)
      {
         if ((profile.at(i).item == false)    &&
             (profile.at(i + 1).item == true) &&
             (profile.at(i + 2).item == false))
         {
            idx = i;
            break;
         }
      }
   }
       
   if (idx == -1)
      return false;

   if ((profile.at(idx).value >= 0.40) && 
       (profile.at(idx + 1).value < 0.35) &&
       (profile.at(idx + 2).value < 0.20))
       return true;
      
   return false;
}

bool StackParser::isLeftOneProfile(const QVector<ProfileItem> & profile) const
{
/*
profile = [3](
{item=true value=0.11111111111111110 },
{item=false value=0.33333333333333331 },
{item=true value=0.55555555555555558 })
*/
/*
profile = [4](
{item=true value=0.14285714285714285 },
{item=false value=0.14285714285714285 },
{item=true value=0.57142857142857140 },
{item=false value=0.14285714285714285 })
*/
   if (profile.count() < 3)
      return false;
   if (profile.count() >= 3)
   {
      if ((profile.at(0).item == true)     &&
          (profile.at(1).item == false)  &&
          (profile.at(2).item == true))
      {
         //����� ������������ �����. ��� ������ ���� > 50%
         foreach (ProfileItem item, profile)
         {
            if (item.value > 0.5)
               return true;
         }
      }
   }
   return false;
}

bool StackParser::isRightOneProfile(const QVector<ProfileItem> & profile) const
{
/*
profile = [1](
{item=false value=1.0000000000000000 })
*/
/*
profile = [2](
{item=true value=0.85714285714285710 },
{item=false value=0.14285714285714285 })
*/
   foreach (ProfileItem item, profile)
   {
      if (item.value >= 0.80)
         return true;
   }
   return false;
}

bool StackParser::isLeftSevenProfile(const QVector<ProfileItem> & profile) const
{
/*
profile = [3](
{item=false value=0.11111111111111110 },
{item=true value=0.33333333333333331 },
{item=false value=0.55555555555555558 })
*/
/*
profile = [3](
{item=false value=0.33333333333333331 },
{item=true value=0.33333333333333331 },
{item=false value=0.33333333333333331 })
*/
   if (profile.count() < 3)
      return false;
   if (profile.count() == 3)
   {
      if ((profile.at(0).item == false)     &&
          (profile.at(1).item == true)  &&
          (profile.at(2).item == false))
      {
         if (profile.at(0).value > 0.3 && profile.at(1).value > 0.3 &&
             profile.at(2).value > 0.3)
             return true;
      }
   }
   return false;
}

bool StackParser::isRightSevenProfile(const QVector<ProfileItem> & profile) const
{
/*
profile = [2](
{item=false value=0.44444444444444442 },
{item=true value=0.55555555555555558 })
*/
   if (profile.count() != 2)
      return false;
   if ((profile.at(0).item == false)     &&
       (profile.at(1).item == true))
   {
      if (profile.at(0).value > 0.4 &&
          profile.at(1).value > 0.5)
          return true;
   }
   return false;
}

