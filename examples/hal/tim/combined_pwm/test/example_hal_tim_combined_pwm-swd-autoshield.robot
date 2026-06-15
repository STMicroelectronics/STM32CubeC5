*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_tim_combined_pwm.
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
${DELAY_X_BETWEEN_CHANNELS}=    0.000018
${DELAY_Y_BETWEEN_CHANNELS}=    0.000013
${DELAY_Z_BETWEEN_CHANNELS}=    0.000008
${TIME_MEASURE_MARGIN}=         0.000004
${TIME_WINDOW}=                 0.001


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
    [Documentation]    Start Timer PWM generation for both channels.

    Comment    Check signal TON timing
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_USER_PWM_X_PIN
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_USER_PWM_X_PIN
    ...    evt2_edge=falling edge
    ...    t_min=${{ ${DELAY_X_BETWEEN_CHANNELS} - ${TIME_MEASURE_MARGIN} }}
    ...    t_max=${{ ${DELAY_X_BETWEEN_CHANNELS} + ${TIME_MEASURE_MARGIN} }}
    ...    time_window=${TIME_WINDOW}
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_USER_PWM_Y_PIN
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_USER_PWM_Y_PIN
    ...    evt2_edge=falling edge
    ...    t_min=${{ ${DELAY_Y_BETWEEN_CHANNELS} - ${TIME_MEASURE_MARGIN} }}
    ...    t_max=${{ ${DELAY_Y_BETWEEN_CHANNELS} + ${TIME_MEASURE_MARGIN} }}
    ...    time_window=${TIME_WINDOW}
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_USER_PWM_Z_PIN
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_USER_PWM_Z_PIN
    ...    evt2_edge=falling edge
    ...    t_min=${{ ${DELAY_Z_BETWEEN_CHANNELS} - ${TIME_MEASURE_MARGIN} }}
    ...    t_max=${{ ${DELAY_Z_BETWEEN_CHANNELS} + ${TIME_MEASURE_MARGIN} }}
    ...    time_window=${TIME_WINDOW}

    Comment    Check delay between signals
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_USER_PWM_X_PIN
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_USER_PWM_Y_PIN
    ...    evt2_edge=rising edge
    ...    t_min=${{ ${DELAY_X_BETWEEN_CHANNELS} - ${DELAY_Y_BETWEEN_CHANNELS} - ${TIME_MEASURE_MARGIN} }}
    ...    t_max=${{ ${DELAY_X_BETWEEN_CHANNELS} - ${DELAY_Y_BETWEEN_CHANNELS} + ${TIME_MEASURE_MARGIN} }}
    ...    time_window=${TIME_WINDOW}
    ...    time_filtering=10
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_USER_PWM_X_PIN
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_USER_PWM_Z_PIN
    ...    evt2_edge=rising edge
    ...    t_min=${{ ${DELAY_X_BETWEEN_CHANNELS} - ${DELAY_Z_BETWEEN_CHANNELS} - ${TIME_MEASURE_MARGIN} }}
    ...    t_max=${{ ${DELAY_X_BETWEEN_CHANNELS} - ${DELAY_Z_BETWEEN_CHANNELS} + ${TIME_MEASURE_MARGIN} }}
    ...    time_window=${TIME_WINDOW}
    ...    time_filtering=10

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
