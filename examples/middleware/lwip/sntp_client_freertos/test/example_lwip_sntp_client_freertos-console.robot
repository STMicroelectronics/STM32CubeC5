*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_lwip_sntp_client_freertos.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_lwip    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    # Wait 5 seconds before ending the test to observe console history 
    Sleep    5s
    Example Step 3
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    Initialize the application by creating the example task.
    Read Console Until    [INFO] Starting LwIP SNTP client FreeRTOS example

Example Step 2
    [Documentation]    Starts the FreeRTOS™ scheduler.
    Read Console Until    [INFO] Starting FreeRTOS Scheduler

Example Step 3
    [Documentation]    Initializes the LwIP stack.
    ${a_value}=    Set Variable    ${17}
    check_console_text_history    [INFO] AppLwIP - Initializing LwIP

Example Step 4
    [Documentation]    Starts the SNTP client.
    check_console_text_history    [INFO] AppSntpClient - Waiting for IP address
