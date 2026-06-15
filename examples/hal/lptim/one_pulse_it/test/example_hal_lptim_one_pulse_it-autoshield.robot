*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_lptim_one_pulse_it.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_lptim


*** Variables ***
${RESET_TIME}=              2.5    # 2.5s time window to let time to CubeProgrammer to reset the Target
${EXPECTED_FREQUENCY}=      1000
${EXPECTED_DUTY_CYCLE}=     50


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    # Need to check signal just after Target reset
    Start Pin Events    ${RESET_TIME}    0    rising edge    LPTIM_CH
    Reset Target

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
    # Get the frequency and duty cycle of the PWM signal in an array
    @{duty_cycles}=    Get Low Pwm In Array Asynchrone    LPTIM_CH    ${RESET_TIME}
    ${duty_cycles_size}=    Get Length    ${duty_cycles}

    IF    ${duty_cycles_size} == 2
        FOR    ${i}    IN RANGE    0    ${duty_cycles_size}
            ${frequency}=    Set Variable    ${duty_cycles}[${i}][0]
            ${duty_cycle}=    Set Variable    ${duty_cycles}[${i}][1]
            IF    ${frequency} < ${EXPECTED_FREQUENCY} * 0.95 or ${frequency} > ${EXPECTED_FREQUENCY} * 1.05
                FAIL    Frequency is out of the range: ${frequency}
            END
            IF    ${duty_cycle} < ${EXPECTED_DUTY_CYCLE} - 1 or ${duty_cycle} > ${EXPECTED_DUTY_CYCLE} + 1
                FAIL    Duty cycle is out of the range: ${duty_cycle}
            END
        END
    ELSE
        Fail    Expected to get 2 different duty cycles, but got ${duty_cycles_size}
    END

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
