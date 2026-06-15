*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_led_blinking_gpio.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:parts_led


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1-2


*** Keywords ***
Example Step 1-2
    [Documentation]    Turns on, then off, the MX_LED_EXAMPLE
    ...    Toggles twice the MX_LED_EXAMPLE
    Comment    These two steps together must produce a blinking frequency of 1Hz, 50%DTC.

    # improve the check by using the "get table GPIO" Autoshield feature...
    Check Frequency    pin=MX_STATUS_PIN    freq=1Hz    duty_cycle=0.5    precision=0.1
    ...    timeout=10s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
