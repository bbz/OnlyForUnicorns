#include <Adafruit_LSM9DS0.h>
#include "Arduino.h"
#include "gestures.h"

#define ORTH_ACCEL (((SENSORS_GRAVITY_EARTH) - 0.00) * 0.15) //0.20

bool IsOrthToX(float x, float y, float z)
{

  if(abs(x) < ORTH_ACCEL)
  {
    return true;
  }

  return false;
}

bool IsOrthToY(float x, float y, float z)
{

  if(abs(y) < ORTH_ACCEL)
  {
    return true;
  }

  return false;
}

bool IsOrthToZ(float x, float y, float z)
{

  if(abs(z) < ORTH_ACCEL)
  {
    return true;
  }

  return false;
}


//determine if an axis has an accel greater or equal to gravity
bool IsAxisDown(float a)
{

  return(a < (ORTH_ACCEL - SENSORS_GRAVITY_EARTH));
}

//determine if an axis has an accel greater or equal to gravity.
bool IsAxisUp(float a)
{

  return(a > (SENSORS_GRAVITY_EARTH - ORTH_ACCEL));
}


bool IsXUp(float x, float y, float z)
{
  
  if(IsOrthToY(x, y, z) && IsOrthToZ(x, y, z) && IsAxisUp(x))
  {
    return true;
  }

  return false;
}

bool IsXDown(float x, float y, float z)
{
  
  if(IsOrthToY(x, y, z) && IsOrthToZ(x, y, z) && IsAxisDown(x))
  {
    return true;
  }

  return false;
}


bool IsYUp(float x, float y, float z)
{
  
  if(IsOrthToX(x, y, z) && IsOrthToY(x, y, z) && IsAxisUp(y))
  {
    return true;
  }

  return false;
}

bool IsYDown(float x, float y, float z)
{
  
  if(IsOrthToX(x, y, z) && IsOrthToY(x, y, z) && IsAxisDown(y))
  {
    return true;
  }

  return false;
}


bool IsZUp(float x, float y, float z)
{
  
  if(IsOrthToX(x, y, z) && IsOrthToY(x, y, z) && IsAxisUp(z))
  {
    return true;
  }

  return false;
}

bool IsZDown(float x, float y, float z)
{
  
  if(IsOrthToX(x, y, z) && IsOrthToY(x, y, z) && IsAxisDown(z))
  {
    return true;
  }

  return false;
}


