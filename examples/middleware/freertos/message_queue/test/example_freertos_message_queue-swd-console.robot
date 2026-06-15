*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_freertos_message_queue.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_freertos    profile:printf


*** Variables ***
${TIMEOUT_S}=       1


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6


*** Keywords ***
Example Step 1
    [Documentation]    Create a queue and 4 tasks.
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    starts the FreeRTOS scheduler.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] Starting FreeRTOS Scheduler

Example Step 3
    [Documentation]    'Producer Task' sends message (incrementing value "ProducerValue") to the queue.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] [Producer Task]: Sending ProducerValue msg to the queue

Example Step 4
    [Documentation]    'Consumer Task' gets message from the queue, checks if it's the correct message (ProducerValue == ConsumerValue) then prints message to the serial port
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] [Consumer Task]: Getting the msg from queue & checking if it's the correct msg
    Read Console Until    [INFO] [Consumer Task]: Correct msg verified

Example Step 5
    [Documentation]    'Gen Task' waits for the message on the queue to reach a specific "GenerationValue" then sends a notification to Receive Task
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] [Receive Task]: Wait until being notified
    Read Console Until    [INFO] [Producer Task]: Sending ProducerValue msg to the queue

Example Step 6
    [Documentation]    'Receive Task' wait until being notified, then toggles the green led.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=10s
    Read Console Until    [INFO] [Gen Task]: Getting the msg from queue & checking if the GEN_VALUE is reached
    Read Console Until    [INFO] [Gen Task]: GenerationValue reached -> Notify Receive Task
    Read Console Until    [INFO] [Receive Task]: Notification received -> Toggle MX_STATUS_LED
