*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_lptim_set_once_it.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_lptim


*** Variables ***
${TIME_WINDOW}=         0.25
${EXPECTED_DELAY}=      0.0006
${SIGNAL_MARGIN}=       0.00005


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the LPTIM instance, registers the callbacks and starts LPTIM in interrupt mode.
    Comment    Checking that initialization (app_init) runs without error is not possible:
    ...    ExecStatus is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after

Example Step 2
    [Documentation]    Waits for the low-power timer's repetition counter underflow event
    Comment    Verify MX_STATUS_LED is turned OFF an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    IF    ${active_level} > 0
        ${inactive_level}=    Set Variable    ${LOW}
    ELSE
        ${inactive_level}=    Set Variable    ${HIGH}
    END
    Check Pin    MX_STATUS_LED    ${inactive_level}    duration=1s

    Start Pin Events    ${TIME_WINDOW}    0    rising edge    LPTIM_ETR    LPTIM_CH

    Comment    Apply a trigger on GPIO ETR input pin
    # Generate a short pulse on LPTIM ETR input to trigger the LPTIM counter start
    Write Pulse    LPTIM_ETR    duration=0.001s

    Check Time Between Events Asynchrone
    ...    LPTIM_ETR
    ...    rising edge
    ...    LPTIM_CH
    ...    rising edge
    ...    time_window=${TIME_WINDOW}
    ...    t_min=${{${EXPECTED_DELAY} - ${SIGNAL_MARGIN}}}
    ...    t_max=${{${EXPECTED_DELAY} + ${SIGNAL_MARGIN}}}

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
