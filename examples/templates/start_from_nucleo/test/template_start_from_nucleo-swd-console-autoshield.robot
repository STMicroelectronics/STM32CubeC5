*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for template_start_from_nucleo.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:template    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example scenario


*** Keywords ***
Example scenario
    [Documentation]    initializes the button instance and turns on the LED by default.
    ...    whenever a button press event is detected toggles the LED and prints a console message.
    Comment    Check if data on console are printed correctly
    Read Console Until    Button initialization COMPLETED.
    Read Console Until    LED is turned on.
    ${led_active_state}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${led_active_state}    duration=1s
    Comment    Press the Button and check that one event is detected
    ${button_active_state}=    Read Test Variable    MX_STATUS_BUTTON_ACTIVE_STATE
    Write Pin    BUTTON_USER_PIN    ${button_active_state}
    Comment    Check if data on console are printed correctly
    Read Console Until    LED toggled.
    ${led_inactive_state}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${led_inactive_state}    duration=1s
