// Protection Overload Header

#pragma once

#include <stdint.h>
#include <stdio.h>

// State Machine States
typedef enum {
    ST_IDLE,                            // Protection active and running
    ST_OVERLOAD_TRIGGERED               // Protection triggered Breaker opening 
} ProtectionOverloadState;

// Parameters Structure
typedef struct {
    float overload_threshold;           // Current threshold
    float k_factor;                     // IEC 60947 protection k
    float cooling_rate;
    float max_energy;
} ProtectionOverloadParams;

// State Machine parameters
typedef struct {
    ProtectionOverloadState state;
    ProtectionOverloadParams params;
    float accumulated_energy;
} ProtectionOverloadSM;

// API Functions
void ProtectionOverload_SM_Init(ProtectionOverloadParams *params);
float ProtectionOverload_SM_GetCallRate();
void ProtectionOverload_SM_Run();
ProtectionOverloadState ProtectionOverload_SM_GetState();
const char* ProtectionOverload_SM_GetStateDescriptor(ProtectionOverloadState state);

// Sensor input function (mocked in tests)
float Sensor_Read();
