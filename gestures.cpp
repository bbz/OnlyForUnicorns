#include <Adafruit_LSM9DS0.h>
//#include "Arduino.h"
#include "gestures.h"

bool IsOrthToX(float x, float y, float z) { return (abs(x) < ORTH_ACCEL); }
bool IsOrthToY(float x, float y, float z) { return (abs(y) < ORTH_ACCEL); }
bool IsOrthToZ(float x, float y, float z) { return (abs(z) < ORTH_ACCEL); }

//determine if an axis has an accel greater or equal to gravity
bool IsAxisDown(float a) { return (a < (ORTH_ACCEL - SENSORS_GRAVITY_EARTH)); }
//determine if an axis has an accel greater or equal to gravity.
bool IsAxisUp(float a) { return (a > (SENSORS_GRAVITY_EARTH - ORTH_ACCEL)); }

bool IsXUp  (float x, float y, float z) { return (IsOrthToY(x, y, z) && IsOrthToZ(x, y, z) && IsAxisUp  (x)); }
bool IsXDown(float x, float y, float z) { return (IsOrthToY(x, y, z) && IsOrthToZ(x, y, z) && IsAxisDown(x)); }
bool IsYUp  (float x, float y, float z) { return (IsOrthToX(x, y, z) && IsOrthToY(x, y, z) && IsAxisUp  (y)); }
bool IsYDown(float x, float y, float z) { return (IsOrthToX(x, y, z) && IsOrthToY(x, y, z) && IsAxisDown(y)); }
bool IsZUp  (float x, float y, float z) { return (IsOrthToX(x, y, z) && IsOrthToY(x, y, z) && IsAxisUp  (z)); }
bool IsZDown(float x, float y, float z) { return (IsOrthToX(x, y, z) && IsOrthToY(x, y, z) && IsAxisDown(z)); }
