// Unit tests

#include <assert.h>
#include "protection_overload.h"

#define TEST_MAX_TIME   3600.0  // [s] max time for test execution (default = 1 hour)

float mock_sensor_values[] = {3.0, 3.0, 3.0, 3.0, 3.0};
int mock_index = 0;

// Test current value (mocked sensor value)
float test_current = 0.0f;

float Sensor_Read() {
    // return mock_sensor_values[mock_index++ % (sizeof(mock_sensor_values)/sizeof(float))];
    return test_current;
}

void test_ProtectionOverload_Generic(
    ProtectionOverloadParams *params,
    float simulated_current,    
    ProtectionOverloadState expected_state,
    float expected_time) {

    // Init state machine and parameters
    ProtectionOverload_SM_Init(params);

    // Max test time
    float max_test_time = TEST_MAX_TIME;
    if (max_test_time < 2*expected_time) {
        max_test_time = 2*expected_time;
    }

    // Set test current value
    test_current = simulated_current;

    // Init test iterations
    int iterations = 0;
    int max_iterations = max_test_time / ProtectionOverload_SM_GetCallRate();
    
    printf("Max number of iterations = %i\n", max_iterations);

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
    float lower_bound = expected_time * (1.0 - protectionTolerance);
    float upper_bound = expected_time * (1.0 + protectionTolerance);

    printf("exit state = %s, expected state = %s \n", ProtectionOverload_SM_GetStateDescriptor(ProtectionOverload_SM_GetState()), ProtectionOverload_SM_GetStateDescriptor(expected_state));

    // Check the expected state is verified
    assert(ProtectionOverload_SM_GetState() == expected_state);

    printf("actual time = %.2f s, expected time = %.2f s +/- %2.0f%%\n", actual_time, expected_time, 100.0f*protectionTolerance);

    // In case of trigger: check expected timing
    if (expected_state == ST_OVERLOAD_TRIGGERED) {
        assert(actual_time >= lower_bound && actual_time <= upper_bound);
    }
    printf("Test Passed!\n");
}

int main() {

    ProtectionOverloadParams protectionParams = {
        .overload_threshold = 1.0f,     // Normalized to 1.0 (current passed in the test is actually I/Ithreshold)
        .k_factor = 1.0f,               // IEC 60947 protection k
        .cooling_rate = 0.98f,          
        .max_energy = 1.0f              // 1.0 is the trip threshold
    };

    // Test cases
    test_ProtectionOverload_Generic(&protectionParams, 1.0f, ST_IDLE, 0.0f);
    test_ProtectionOverload_Generic(&protectionParams, 1.2f, ST_OVERLOAD_TRIGGERED, 2.27f);
    test_ProtectionOverload_Generic(&protectionParams, 1.4f, ST_OVERLOAD_TRIGGERED, 1.04f);
    test_ProtectionOverload_Generic(&protectionParams, 0.2f, ST_IDLE, 0.0f);
    return 0;
}
