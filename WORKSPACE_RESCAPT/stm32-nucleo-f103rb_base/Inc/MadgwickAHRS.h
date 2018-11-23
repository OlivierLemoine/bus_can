//=====================================================================================================
// MadgwickAHRS.h
//=====================================================================================================
//
// Implementation of Madgwick's IMU and AHRS algorithms.
// See: http://www.x-io.co.uk/node/8#open_source_ahrs_and_imu_algorithms
//
// Date         Author          Notes
// 29/09/2011   SOH Madgwick    Initial release
// 02/10/2011   SOH Madgwick    Optimised for reduced CPU load
//
//=====================================================================================================
#ifndef MadgwickAHRS_h
#define MadgwickAHRS_h
 
 
#include <math.h>
 
//----------------------------------------------------------------------------------------------------
// Variable declaration
 
extern volatile float beta;             // algorithm gain
extern volatile float q0, q1, q2, q3;   // quaternion of sensor frame relative to auxiliary frame

typedef union _quatern
{
    float f;
    char  c[4];
}Quatern;

//---------------------------------------------------------------------------------------------------
// Function declarations
 
void MadgwickAHRSupdate(float g[3], float a[3], float m[3]);
void MadgwickAHRSupdateIMU(float g[3], float a[3]);
#endif
//=====================================================================================================
// End of file
//=====================================================================================================