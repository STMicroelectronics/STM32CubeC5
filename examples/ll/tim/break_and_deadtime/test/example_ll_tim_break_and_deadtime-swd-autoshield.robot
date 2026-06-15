*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_tim_break_and_deadtime.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_tim


*** Variables ***
${DEADTIME_DELAY}=              0.0000037
${TIME_WINDOW}=                 0.001
${DEADTIME_DELAY_MARGIN}=       0.000002


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Configure the GPIO and the TIM for PWM generation.
    Comment    Checking that initialization (app_init) runs without error is not possible:
    ...    ExecStatus is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.
    ...    MX_STATUS_LED is OFF at the startup but becomes immediately HIGH.

Example Step 2
    [Documentation]    Starts the timer in Output Capture mode for each channel.
    Comment    Set t_min and t_max with 2 microseconds margin over DEADTIME_DELAY.
    ${MIN_DELAY_MS}=    Set Variable    ${{ ${DEADTIME_DELAY} - ${DEADTIME_DELAY_MARGIN} }}
    ${MAX_DELAY_MS}=    Set Variable    ${{ ${DEADTIME_DELAY} + ${DEADTIME_DELAY_MARGIN} }}

    Comment    Check output PWM frequency and duty cycle for channel CHy
    Check Frequency    pin=MX_EXAMPLE_USER_PWM_CHy_PIN    freq=8000Hz    duty_cycle=0.47    precision=0.1

    Comment    Check output PWM frequency and duty cycle for channel CHyN
    Check Frequency    pin=MX_EXAMPLE_USER_PWM_CHyN_PIN    freq=8000Hz    duty_cycle=0.47    precision=0.1

    Comment    Checks the deadtime between channel CHy and its complementary channel CHyN.
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_USER_PWM_CHy_PIN
    ...    evt1_edge=falling edge
    ...    evt2_pin=MX_EXAMPLE_USER_PWM_CHyN_PIN
    ...    evt2_edge=rising edge
    ...    t_min= ${MIN_DELAY_MS}
    ...    t_max=${MAX_DELAY_MS}
    ...    time_window=${TIME_WINDOW}

    Comment    Checks the deadtime between channel CHyN and channel CHy
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_USER_PWM_CHyN_PIN
    ...    evt1_edge=falling edge
    ...    evt2_pin=MX_EXAMPLE_USER_PWM_CHy_PIN
    ...    evt2_edge=rising edge
    ...    t_min= ${MIN_DELAY_MS}
    ...    t_max=${MAX_DELAY_MS}
    ...    time_window=${TIME_WINDOW}

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
