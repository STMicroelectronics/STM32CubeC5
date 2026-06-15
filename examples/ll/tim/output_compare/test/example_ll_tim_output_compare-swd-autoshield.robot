*** Comments ***
Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_tim_oc_toggle.
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
${DELAY_BETWEEN_CHANNELS}=      0.00025
${TIME_WINDOW}=                 0.005


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
    Comment    Calculate t_min as 90% of DELAY_BETWEEN_CHANNELS and t_max as 110%.
    ${90%_OF_DELAY}=    Set Variable    ${{ ${DELAY_BETWEEN_CHANNELS} * 0.9 }}
    ${110%_OF_DELAY}=    Set Variable    ${{ ${DELAY_BETWEEN_CHANNELS} * 1.1 }}

    Comment    Check output PWM frequency and duty cycle for channel y
    Check Frequency    pin=MX_EXAMPLE_STATUS_OC_TOGGLE_1_PIN    freq=500Hz    duty_cycle=0.5    precision=0.1

    Comment    Check output PWM frequency and duty cycle for channel z
    Check Frequency    pin=MX_EXAMPLE_STATUS_OC_TOGGLE_2_PIN    freq=500Hz    duty_cycle=0.5    precision=0.1

    Comment    Checks the duration between channel y and channel z
    Check Time Between Events
    ...    evt1_pin=MX_EXAMPLE_STATUS_OC_TOGGLE_1_PIN
    ...    evt1_edge=rising edge
    ...    evt2_pin=MX_EXAMPLE_STATUS_OC_TOGGLE_2_PIN
    ...    evt2_edge=rising edge
    ...    t_min= ${90%_OF_DELAY}
    ...    t_max=${110%_OF_DELAY}
    ...    time_window=${TIME_WINDOW}

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    Check Pin    MX_STATUS_LED    ${HIGH}    duration=1s
