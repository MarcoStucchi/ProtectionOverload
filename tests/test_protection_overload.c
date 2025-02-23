// Unit tests

#include <assert.h>
#include "protection_overload.h"

#define CALL_RATE_MS 10

float mock_sensor_values[] = {10.0, 15.0, 25.0, 30.0, 40.0};
int mock_index = 0;

float Sensor_Read() {
    return mock_sensor_values[mock_index++ % 5];
}

void test_ProtectionOverload_Generic(
    ProtectionOverloadParams *params,    
    float expected_time,
    float tolerance, 
    ProtectionOverloadState expected_state,
    float max_test_time) {

    // Init state machine and parameters
    ProtectionOverload_SM_Init(params);

    // Init test iterations
    int iterations = 0;
    int max_iterations = max_test_time / ProtectionOverload_SM_GetCallRate();

    // Actual testing loop
    // ! Exit conditions are only a tripped protection or maximum test time reached
    while (ProtectionOverload_SM_GetState() != ST_OVERLOAD_TRIGGERED && iterations < max_iterations) {

        // Run State Machine
        ProtectionOverload_SM_Run();

        // Increase test iterations
        iterations++;
    }

    // Calculate SM timings and expected time range
    float actual_time = iterations * ProtectionOverload_SM_GetCallRate();
    float lower_bound = expected_time * (1.0 - tolerance / 100.0);
    float upper_bound = expected_time * (1.0 + tolerance / 100.0);

    printf("exit state = %s, expected state = %s \n", ProtectionOverload_SM_GetStateDescriptor(ProtectionOverload_SM_GetState()), ProtectionOverload_SM_GetStateDescriptor(expected_state));

    // Check the expected state is verified
    assert(ProtectionOverload_SM_GetState() == expected_state);

    // In case of trigger: check expected timing
    if (expected_state == ST_OVERLOAD_TRIGGERED) {
        assert(actual_time >= lower_bound && actual_time <= upper_bound);
    }
    printf("Test Passed!\n");
}

int main() {
    ProtectionOverloadParams params = {
        .overload_threshold = 20.0f,
        .k_factor = 0.01f,
        .cooling_rate = 0.98f,
        .max_energy = 1.0f
    };
    test_ProtectionOverload_Generic(&params, 1000.0f, 10.0f, ST_OVERLOAD_TRIGGERED, 50.0f);
    return 0;
}
