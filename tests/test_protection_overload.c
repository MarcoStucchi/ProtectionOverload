// Unit tests

#include "unity.h"
#include <assert.h>
#include "protection_overload.h"

#define USE_UNITY

#define TEST_MAX_TIME   3600.0  // [s] max time for test execution (default = 1 hour)

float mock_sensor_values[] = {3.0, 3.0, 3.0, 3.0, 3.0};
int mock_index = 0;

// Test current value (mocked sensor value)
float test_current = 0.0f;

/* ------------------------------------------------ 
        Unity setup and teardown functions
   ------------------------------------------------ */
#ifdef USE_UNITY

void setUp(void) { 
    // Called before every test
}

void tearDown(void) { 
    // Called after every test
}

#endif  // USE_UNITY

/* ------------------------------------------------ 
        Mocked Sensor Read Function
   ------------------------------------------------ */

float Sensor_Read() {
    // return mock_sensor_values[mock_index++ % (sizeof(mock_sensor_values)/sizeof(float))];
    return test_current;
}

/* ------------------------------------------------ 
        Test Functions
   ------------------------------------------------ */

void test_ProtectionOverload_Generic(
    unsigned int test_id,
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

    // Actual testing loop
    // ! Exit conditions are only a tripped protection or maximum test time reached
    while (ProtectionOverload_SM_GetState() != ST_OVERLOAD_TRIGGERED && iterations < max_iterations) {

        // Run State Machine
        ProtectionOverload_SM_Run();

        // Increase test iterations
        iterations++;
    }

    // printf("exit state = %s, expected state = %s \n", ProtectionOverload_SM_GetStateDescriptor(ProtectionOverload_SM_GetState()), ProtectionOverload_SM_GetStateDescriptor(expected_state));

#ifdef USE_UNITY
    // Prepare test message
    char test_message[100];
    sprintf(test_message, "Test case failed: %d - Unexpected protection state %s", test_id, ProtectionOverload_SM_GetStateDescriptor(expected_state));
    // Check expected state using Unity
    TEST_ASSERT_EQUAL_MESSAGE(expected_state, ProtectionOverload_SM_GetState(), test_message);
#else
    // Check the expected state is verified
    assert(ProtectionOverload_SM_GetState() == expected_state);
#endif

    // Get actual time
    float actual_time = iterations * ProtectionOverload_SM_GetCallRate();

#ifdef USE_UNITY
    // Check expected timing using Unity, only in case of tripped protection
    if (expected_state == ST_OVERLOAD_TRIGGERED) {
        TEST_ASSERT_FLOAT_WITHIN(expected_time * protectionTolerance, expected_time, actual_time);
    }
#else
    // Print actual and expected time
    printf("actual time = %.2f s, expected time = %.2f s +/- %2.0f%%\n", actual_time, expected_time, 100.0f*protectionTolerance);

    // Calculate SM timings and expected time range
    float lower_bound = expected_time * (1.0 - protectionTolerance);
    float upper_bound = expected_time * (1.0 + protectionTolerance);
    
    // Check the expected timing is verified
    if (expected_state == ST_OVERLOAD_TRIGGERED) {
        assert(actual_time >= lower_bound && actual_time <= upper_bound);
    }
    printf("Test Passed!\n");
#endif

}

/* ------------------------------------------------ 
        Test Cases
   ------------------------------------------------ */
void test_ProtectionOverload_Parameterized(void) {
    struct {
        unsigned int test_id;
        float simulated_current;
        ProtectionOverloadState expected_state;
        float expected_time;
        const char* description;
    } test_cases[] = {
        {1, 0.2f, ST_IDLE, 0.0f, "Low current"},
        {2, 0.8f, ST_IDLE, 0.0f, "Normal current"},
        {3, 1.0f, ST_IDLE, 0.0f, "Nominal current"},
        {4, 1.2f, ST_IDLE, 2.27f, "Overload current 1,2 x Itrip"},
        {5, 1.4f, ST_OVERLOAD_TRIGGERED, 1.04f, "Overload current 1,4 x Itrip"},
        {6, 1.6f, ST_OVERLOAD_TRIGGERED, 0.64f, "Overload current 1,6 x Itrip"},
        {7, 2.0f, ST_OVERLOAD_TRIGGERED, 0.33f, "Overload current 2,0 x Itrip"},
        {8, 3.0f, ST_OVERLOAD_TRIGGERED, 0.12f, "Overload current 3,0 x Itrip"}
    };

    // Protection parameters
    ProtectionOverloadParams protectionParams = {
        .overload_threshold = 1.0f,     // Normalized to 1.0 (current passed in the test is actually I/Ithreshold)
        .k_factor = 1.0f,               // IEC 60947 protection k
        .cooling_rate = 0.98f,          
        .max_energy = 1.0f              // 1.0 is the trip threshold
    };

    // Calls all the test cases
    for (unsigned int i = 0; i < sizeof(test_cases) / sizeof(test_cases[0]); i++) {
        printf("Running test case %d: %s\n", test_cases[i].test_id, test_cases[i].description);
        test_ProtectionOverload_Generic(i+1, 
            &protectionParams, 
            test_cases[i].simulated_current, 
            test_cases[i].expected_state, 
            test_cases[i].expected_time);
    }
}


int main() {

#ifdef USE_UNITY

    UNITY_BEGIN();
    RUN_TEST(test_ProtectionOverload_Parameterized);
    return UNITY_END();    

#else  // USE_UNITY        
    printf("Starting Protection Overload Test\n");

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

#endif

}
