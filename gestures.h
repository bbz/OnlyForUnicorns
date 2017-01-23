#define ORTH_ACCEL (((SENSORS_GRAVITY_EARTH) - 0.00) * 0.15) //0.20

bool IsOrthToX(float x, float y, float z);
bool IsOrthToY(float x, float y, float z);
bool IsOrthToZ(float x, float y, float z);

bool IsXUp(float x, float y, float z);
bool IsXDown(float x, float y, float z);

bool IsYUp(float x, float y, float z);
bool IsYDown(float x, float y, float z);

bool IsZUp(float x, float y, float z);
bool IsZDown(float x, float y, float z);


