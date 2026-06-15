*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_lptim_pwm_external_clock.
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
${INPUT_FREQUENCY}=     1000
${AUTORELOAD}=          99
${DUTY_CYCLE}=          0.25


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The applicative code initializes the LPTIM instance and start the LPTIM peripheral.
    Comment    Nothing to check.

Example Step 2
    [Documentation]    The device goes in stop mode. The PWM output signal is generated while in STOP mode thanks to the external clock.

    ${output_frequency}=    Set Variable    ${{ ${INPUT_FREQUENCY} / (${AUTORELOAD} + 1) }}

    Comment    Generate external clock of ${INPUT_FREQUENCY} Hz and 50% duty cycle
    Write Frequency    MX_EXAMPLE_LPTIM_IN    ${INPUT_FREQUENCY}
    Comment    Check output clock
    Check Frequency    MX_EXAMPLE_LPTIM_CH    ${output_frequency}    duty_cycle=${DUTY_CYCLE}

    Comment    Generate external clock of 2*${INPUT_FREQUENCY} Hz and 33% duty cycle
    Write Frequency    MX_EXAMPLE_LPTIM_IN    ${{2 * ${INPUT_FREQUENCY}}}    duty_cycle=0.33
    Comment    Check output clock
    Check Frequency    MX_EXAMPLE_LPTIM_CH    ${{2 * ${output_frequency}}}    duty_cycle=${DUTY_CYCLE}

End Of Example
    Comment    Nothing to check.
