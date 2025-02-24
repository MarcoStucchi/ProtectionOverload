// Unit tests

#include "unity.h"
#include <assert.h>
#include "protection_overload.h"

#define TEST_MAX_TIME   3600.0  // [s] max time for test execution (default = 1 hour)

typedef struct {
    unsigned int id;
    float current;
    ProtectionOverloadState expected_state;
    float expected_time;
    const char* description;
} t_test_case;

float mock_sensor_values[] = {3.0, 3.0, 3.0, 3.0, 3.0};
int mock_index = 0;

// Test current value (mocked sensor value)
float test_current = 0.0f;



/* ------------------------------------------------ 
        Unity setup and teardown functions
   ------------------------------------------------ */

void setUp(void) { 
    // Called before every test
}

void tearDown(void) { 
    // Called after every test
}

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

    // Check expected state using Unity
    TEST_ASSERT_EQUAL_MESSAGE(expected_state, ProtectionOverload_SM_GetState(), "Protection state mismatch.");

    // Get actual time
    float actual_time = iterations * ProtectionOverload_SM_GetCallRate();

    // Check expected timing using Unity, only in case of tripped protection
    if (expected_state == ST_OVERLOAD_TRIGGERED) {
        TEST_ASSERT_FLOAT_WITHIN_MESSAGE(expected_time * protectionTolerance, expected_time, actual_time, "Protection time mismatch.");
    }
}

/* ------------------------------------------------ 
        Test Cases - Fixed Current Values
   ------------------------------------------------ */

// Protection parameters
// ! These parameters are shared among all test cases
ProtectionOverloadParams protectionParams = {
    .overload_threshold = 1.0f,     // ! Normalized to 1.0 (current passed in the test is actually I/Ithreshold)
    .k_factor = 1.0f,               // IEC 60947-2 protection k
    .cooling_rate = 0.98f,          
    .max_energy = 1.0f              // 1.0 is the trip threshold
};

t_test_case test_cases_fixed_current[] = {
    {.id = 100, .current = 0.2f, .expected_state = ST_IDLE, .description = "Low current"},
    {.id = 101, .current = 0.8f, .expected_state = ST_IDLE, .description = "Normal current"},
    {.id = 102, .current = 1.0f, .expected_state = ST_OVERLOAD_TRIGGERED, .description = "Nominal current"},
    {.id = 103, .current = 1.2f, .expected_state = ST_OVERLOAD_TRIGGERED, .expected_time = 2.27f, .description = "Overload current 1,2 x Itrip"},
    {.id = 104, .current = 1.4f, .expected_state = ST_OVERLOAD_TRIGGERED, .expected_time = 13.04f, .description = "Overload current 1,4 x Itrip"},
    {.id = 105, .current = 1.6f, .expected_state = ST_OVERLOAD_TRIGGERED, .expected_time = 0.64f, .description = "Overload current 1,6 x Itrip"},
    {.id = 106, .current = 2.0f, .expected_state = ST_OVERLOAD_TRIGGERED, .expected_time = 0.33f, .description = "Overload current 2,0 x Itrip"},
    {.id = 107, .current = 3.0f, .expected_state = ST_OVERLOAD_TRIGGERED, .expected_time = 0.12f, .description = "Overload current 3,0 x Itrip"}
 };

void test_case_launch(unsigned int test_id) {
    printf("Running test case %d: %s\n", test_cases_fixed_current[test_id].id, test_cases_fixed_current[test_id].description);
    test_ProtectionOverload_Generic(&protectionParams, test_cases_fixed_current[test_id].current, test_cases_fixed_current[test_id].expected_state, test_cases_fixed_current[test_id].expected_time);
}   

void test_fixed_current_0(void) {test_case_launch(0);}
void test_fixed_current_1(void) {test_case_launch(1);}
void test_fixed_current_2(void) {test_case_launch(2);}
void test_fixed_current_3(void) {test_case_launch(3);}
void test_fixed_current_4(void) {test_case_launch(4);}
void test_fixed_current_5(void) {test_case_launch(5);}
void test_fixed_current_6(void) {test_case_launch(6);}
void test_fixed_current_7(void) {test_case_launch(7);}

/* ------------------------------------------------ 
        Main Function
   ------------------------------------------------ */  

int main() {

    UNITY_BEGIN();

    // Test cases with fixed current values
    printf("\n--------------------------\n");
    printf("Protection Overload Test with fixed currents\n\n");
    RUN_TEST(test_fixed_current_0);
    RUN_TEST(test_fixed_current_1);
    RUN_TEST(test_fixed_current_2);
    RUN_TEST(test_fixed_current_3);
    RUN_TEST(test_fixed_current_4);
    RUN_TEST(test_fixed_current_5);
    RUN_TEST(test_fixed_current_6);
    RUN_TEST(test_fixed_current_7);

    return UNITY_END();    

}
