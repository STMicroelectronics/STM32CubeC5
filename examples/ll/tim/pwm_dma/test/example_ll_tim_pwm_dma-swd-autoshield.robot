*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_tim_pwm_dma.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_tim


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Configure the GPIO, DMA and the TIM for PWM generation.
    Comment    Checking that initialization (app_init) runs without error is not possible:
    ...    ExecStatus is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.
    ...    MX_STATUS_LED is OFF at the startup but becomes immediately HIGH.

Example Step 2
    [Documentation]    Start Timer PWM generation with variable duty cycle.
    Comment    Check output PWM frequency and duty cycle for channel 1

    Check Frequency
    ...    MX_EXAMPLE_USER_PWM_PIN
    ...    freq=4kHz
    ...    precision=0.1
    ...    duty_cycle=0.50
    ...    duty_cycle_precision=0.1

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
