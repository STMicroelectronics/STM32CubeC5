*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_freertos_mutex.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_freertos    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    FOR    ${i}    IN RANGE    2
        Example Step 3
        Example Step 4
    END


*** Keywords ***
Example Step 1
    [Documentation]    Creates a mutex and 2 tasks with the same priority.
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    Read Console Until    [INFO] Starting FreeRTOS Schedule

Example Step 2
    [Documentation]    Starts the FreeRTOS scheduler.
    Comment    No time to do a check before the next step is executed.

Example Step 3
    [Documentation]    'Task Two' takes the mutex, toggles the green led 10 times at 100ms pace then releases the mutex.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] Task Two - Mutex Taken!
    Read Console Until    [INFO] Task Two - Toggling the status LED 10 times (100 ms)
    Check Time Between Events
    ...    evt1_pin=MX_STATUS_LED
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_STATUS_LED
    ...    evt2_edge=rising edge
    ...    t_min= 0.15
    ...    t_max=0.25
    ...    time_window=500ms
    Read Console Until    [INFO] Task Two - Mutex Given!

Example Step 4
    [Documentation]    'Task One' takes the mutex, toggles the green led 10 times at 500ms pace then releases the mutex.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] Task One - Mutex Taken!
    Read Console Until    [INFO] Task One - Toggling the status LED 10 times (500 ms)
    Check Time Between Events
    ...    evt1_pin=MX_STATUS_LED
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_STATUS_LED
    ...    evt2_edge=rising edge
    ...    t_min= 0.9
    ...    t_max=1.1
    ...    time_window=2.5s
    Read Console Until    [INFO] Task One - Mutex Given!
