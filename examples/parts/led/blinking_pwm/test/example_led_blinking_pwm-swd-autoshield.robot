*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_led_blinking_pwm.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:parts_led

*** Variables ***
${TIME_WINDOW_S}=                 20
${PRECISION}=                     0.1
${EXPECTED_DUTY_CYCLE}=           50
${EXPECTED_FREQ_HZ_SIGNAL1}=      1    # 1Hz
${EXPECTED_FREQ_HZ_SIGNAL2}=      2.5  # 2.5Hz
${EXPECTED_FREQ_HZ_SIGNAL3}=      4    # 4Hz


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1

    Comment    Check the LED signal behavior (frequency and duty cycle) over a full period of time,
    ...    and save the results in an array to use for the next tests.

    Reset Target

    # Get the frequency and duty cycle of the PWM signal in an array
    @{duty_cycles}=    Get Low Pwm In Array    MX_STATUS_LED    ${TIME_WINDOW_S}s

    Example Step 2    @{duty_cycles}
    Example Step 3    @{duty_cycles}
    Example Step 4    @{duty_cycles}


*** Keywords ***
Example Step 1
    [Documentation]    Device initialization
    Comment    Check initialization (app_init) runs without error
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}    timeout=1s

Example Step 2
    [Documentation]    Turns on/off the LED, 10 times, using the ``led_pwm_on``
    ...    and ``led_pwm_off`` parts LED PWM functions
    Comment    Check that the led toggling frequency is 1Hz, duty cycle of 50%
    ...    and with precision of 10%

    [Arguments]    @{duty_cycles}

    FOR    ${i}    IN RANGE    0    8
        ${frequency}=    Set Variable    ${duty_cycles}[${i}][0]
        ${duty_cycle}=    Set Variable    ${duty_cycles}[${i}][1]

        IF    ${duty_cycle} < ${EXPECTED_DUTY_CYCLE} * (1 - ${PRECISION}) or ${duty_cycle} > ${EXPECTED_DUTY_CYCLE} * (1 + ${PRECISION})

            FAIL    duty cycle = ${duty_cycle} is out of range
        END
        IF    ${frequency} < ${EXPECTED_FREQ_HZ_SIGNAL1} * (1 - ${PRECISION}) or ${frequency} > ${EXPECTED_FREQ_HZ_SIGNAL1} * (1 + ${PRECISION})

            FAIL    frequency = ${frequency} is out of range
        END
    END

Example Step 3
    [Documentation]    Toggles the LED, 10 times, using the ``led_pwm_toggle`` parts LED PWM function
    Comment    Check that the led toggling frequency is 2.5Hz, duty cycle of 50%
    ...    and with precision of 10%

    [Arguments]    @{duty_cycles}

    Comment    The 9th element in the array represents a transition step between signal 1 and signal 2 has been excluded from the check.
    FOR    ${i}    IN RANGE    10    18
        ${frequency}=    Set Variable    ${duty_cycles}[${i}][0]
        ${duty_cycle}=    Set Variable    ${duty_cycles}[${i}][1]

         IF    ${duty_cycle} < ${EXPECTED_DUTY_CYCLE} * (1 - ${PRECISION}) or ${duty_cycle} > ${EXPECTED_DUTY_CYCLE} * (1 + ${PRECISION})

            FAIL    duty cycle = ${duty_cycle} is out of range
        END
        IF    ${frequency} < ${EXPECTED_FREQ_HZ_SIGNAL2} * (1 - ${PRECISION}) or ${frequency} > ${EXPECTED_FREQ_HZ_SIGNAL2} * (1 + ${PRECISION})

            FAIL    frequency = ${frequency} is out of range
        END
    END

Example Step 4
    [Documentation]    Starts blinking the LED, during few second indefinitely.
    Comment    Check that the led blinking frequency is 4Hz, duty cycle of 50%
    ...    and with precision of 10%

    [Arguments]    @{duty_cycles}

    Comment    The 19th element in the array represents a transition step between signal 2 and signal 3 and has been excluded from the check.
    FOR    ${i}    IN RANGE    20    30
        ${frequency}=    Set Variable    ${duty_cycles}[${i}][0]
        ${duty_cycle}=    Set Variable    ${duty_cycles}[${i}][1]

         IF    ${duty_cycle} < ${EXPECTED_DUTY_CYCLE} * (1 - ${PRECISION}) or ${duty_cycle} > ${EXPECTED_DUTY_CYCLE} * (1 + ${PRECISION})

            FAIL    duty cycle = ${duty_cycle} is out of range
        END
        IF    ${frequency} < ${EXPECTED_FREQ_HZ_SIGNAL3} * (1 - ${PRECISION}) or ${frequency} > ${EXPECTED_FREQ_HZ_SIGNAL3} * (1 + ${PRECISION})

            FAIL    frequency = ${frequency} is out of range
        END
    END
