*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_rtc_timestamp.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_rtc    profile:printf


*** Test Cases ***
${TEST_ID} RTC Timestamp Scenario Adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README for the RTC Timestamp example.
    # Needed to apply a complete RTC (re)initialization
    Reset Target    mode=POWERDOWN
    Example Step 1
    Example Step 2
    FOR    ${i}    IN RANGE    0    2
        Example Step 3
    END

    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The applicative code initializes the RTC instance, configures the calendar, and enables the timestamp interrupt.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2
    [Documentation]    Displays the current time and date.
    Read Console Until    [INFO] Step 2: Current Time :
    Read Console Until    [INFO] Step 2: Current Date :

Example Step 3
    [Documentation]    Waits for a timestamp event, then displays the timestamped time and date.
    Comment    Simulates a high level on the timestamp input (user button press).
    Write Pulse    BUTTON_USER_PIN
    Read Console Until    [INFO] Step 3: Timestamp
    Read Console Until    [INFO] Step 3: Datestamp

End Of Example
    [Documentation]    Verify the successful completion of the example
    Comment    Verify status LED is activated
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=1s
