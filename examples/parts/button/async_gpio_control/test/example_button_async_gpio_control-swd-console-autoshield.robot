*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_button_async_gpio_control.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:parts_button    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Part Button initialization

    Read Console Until    [INFO] Step 1: Button and LED initialization COMPLETED.
    Comment    Check button pull resistor configuration
    ${active_button_level}=    Read Test Variable    MX_STATUS_BUTTON_ACTIVE_STATE
    Check Button Configuration    BUTTON_USER_PIN    ${active_button_level}

Example Step 2
    [Documentation]    Enable Part Button interrupt
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Read Console Until    [INFO] Step 2: Press the Button:
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}    timeout=2s
    Check Integer Variable    NbButtonEvent    0
    Check Pin    MX_STATUS_LED    ${inactive_level}

Example Step 3
    [Documentation]    Log the number of Button event and toggle the led
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE

    Comment    Press the Button and check that one event is detected
    Write Pin    BUTTON_USER_PIN    ${HIGH}
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=1s
    Read Console Until    [INFO] Step 3: Button event nr. 1: button pressed.
    Check Integer Variable    NbButtonEvent    1

    Comment    Release the Button and check that one event is detected
    Write Pin    BUTTON_USER_PIN    ${LOW}
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=1s
    Read Console Until    [INFO] Step 3: Button event nr. 2: button released.
    Check Integer Variable    NbButtonEvent    2

    Comment    Press the button multiple times and check that all the events keep being detected
    FOR    ${index}    IN RANGE    4    20    2
        Write Pulse    BUTTON_USER_PIN
        Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=1s
        Read Console Until    [INFO] Step 3: Button event nr. ${index-1}: button pressed.
        Read Console Until    [INFO] Step 3: Button event nr. ${index}: button released.
        Check Integer Variable    NbButtonEvent    ${index}
    END

End Of Example
    [Documentation]    Check that the example is still running with no error
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
