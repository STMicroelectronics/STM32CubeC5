*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_tim_ext_trigger_synchro.
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
${MIN_TIME}=        0ms
${MAX_TIME}=        100us
${TIME_WINDOW}=     3


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Configure the GPIO and the TIM.
    Comment    Checking that initialization (app_init) runs without error is not possible:
    ...    ExecStatus is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.
    ...    MX_STATUS_LED is OFF at the startup but becomes immediately HIGH.

Example Step 2
    [Documentation]    Start all three timers to generate synchronized signals when an external trigger occurs.

    Comment    Set PIN HIGH
    Write Pin    MX_EXAMPLE_TIM_INPUT    ${HIGH}

    Comment    Check output PWM frequency and duty cycle for TIMx channel_x
    Check Frequency    pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_X    freq=24kHz    duty_cycle=0.5    precision=0.1

    Comment    Check output PWM frequency and duty cycle for TIMy channel_x
    Check Frequency    pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_Y    freq=24kHz    duty_cycle=0.5    precision=0.1

    Comment    Check output PWM frequency and duty cycle for TIMz channel_x
    Check Frequency    pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_Z    freq=24kHz    duty_cycle=0.5    precision=0.1

    Comment    Checks the duration between channels
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_X
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_Y
    ...    evt2_edge=rising edge
    ...    t_min= ${MIN_TIME}
    ...    t_max=${MAX_TIME}
    ...    time_window=${TIME_WINDOW}
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_X
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_Z
    ...    evt2_edge=rising edge
    ...    t_min= ${MIN_TIME}
    ...    t_max=${MAX_TIME}
    ...    time_window=${TIME_WINDOW}
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_Y
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_TIM_OUTPUT_SIGNAL_Z
    ...    evt2_edge=rising edge
    ...    t_min= ${MIN_TIME}
    ...    t_max=${MAX_TIME}
    ...    time_window=${TIME_WINDOW}

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=3s
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
