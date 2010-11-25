#include <vcl.h>
#pragma hdrstop

#include "PIDClass.h"
#include <Math.h>


#pragma package(smart_init)
__fastcall PIDClass::PIDClass()
{
 w = 1;
 r = 1;
 e_old = 0;
 Ue = 0;
 Xy = 1;
 Xe = 1;
 Xp = 1;
 Xi = 0;
 Xd = 0;
}

__fastcall PIDClass::~PIDClass()
{

}

void PIDClass::reset(void)
{
 e = 0;
 e_old = 0;
 KP = 0;
 KI = 0;
 KD = 0;
 y = 0;
}

void PIDClass::calc(void)
{
 e = w - r;

 if (fabs(e * Xe) <= Ue) {
  e_old = 0;
  KP = 0;
  KI = 0;
  KD = 0;
  y = 0;
 } else {

  KP = e * Xe;

  KI += e * Xe;

  KD = (e * Xe) - e_old;

  e_old = e * Xe;

  y = Xy * (KP * Xp + KI * Xi + KD * Xd);
 }

 if (fabs(y) > My) {
  if (y < 0) {
   y = -My;
  } else {
   y = My;
  }
 }


}
double PIDClass::round(double x) {
 return(floor(x + 0.5));
}
