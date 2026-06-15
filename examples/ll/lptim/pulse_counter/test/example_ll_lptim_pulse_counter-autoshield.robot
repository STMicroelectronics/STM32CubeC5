*** Comments ***
Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_lptim_pulse_counter.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_lptim


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    Example Step 3


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the LPTIM instance.
    Check Pin    MX_STATUS_LED    ${LOW}    duration=1s

Example Step 2
    [Documentation]    The device goes in stop mode and wait for an interrupt.
    Comment    Nothing to check

Example Step 3
    [Documentation]    Each time the LPTIM counter reaches the number of pulses to be counted, an interrupt is generated and wakes the MCU up.

    Comment    First trigger the maximum number of times without triggering the interrupt

    Comment
    ...    To fasten the test, pulses are grouped by 16 to limit communication overhead. Then compute the remaining pulses.
    ${quotient}=    Set Variable    ${{${PULSE_NUMBER}//16}}
    ${reminder}=    Set Variable    ${{${PULSE_NUMBER}%16}}
    FOR    ${index}    IN RANGE    ${quotient}
        Check Pin    MX_STATUS_LED    ${LOW}
        Write Pulse    pin=MX_EXAMPLE_LPTIM1_IN1    pattern=\xAA\xAA\xAA\xAA    duration=2ms
    END
    FOR    ${index}    IN RANGE    ${reminder}
        Check Pin    MX_STATUS_LED    ${LOW}
        Write Pulse    pin=MX_EXAMPLE_LPTIM1_IN1    pattern=\x10    duration=2ms
    END

    Comment    Trigger once more to get the interrupt that should toggle the LED on
    Check Pin    MX_STATUS_LED    ${LOW}    duration=1s
    Write Pulse    pin=MX_EXAMPLE_LPTIM1_IN1    pattern=\x10    duration=2ms
    Sleep    50ms
    Check Pin    MX_STATUS_LED    ${HIGH}    duration=1s

    Comment    Check that a PWM signal on input leads to the LED toggling
    ${toggle_frequency}=    Set Variable    1
    ${pwm_frequency}=    Set Variable    ${{2*${toggle_frequency}*${PULSE_NUMBER}}}
    Write Frequency    pin=MX_EXAMPLE_LPTIM1_IN1    freq=${pwm_frequency}Hz
    Check Frequency    pin=MX_STATUS_LED    freq=${toggle_frequency}Hz
