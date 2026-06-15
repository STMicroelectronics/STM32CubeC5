*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_tim_pwm_input.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_tim    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    Example Step 4
    Example Step 3
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the GPIO and the TIM for PWM input measurement.
    Comment    Checking ExecStatus is not currently possible.
    # Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}    timeout=2s

Example Step 2
    [Documentation]    Starts the timer in Input Capture mode for the two channels.
    Comment    No Check

Example Step 4
    [Documentation]    Logs the value of the frequency and the duty cycle of the external PWM signal
    ...    (if USE_TRACE is enabled).
    Comment    Check console first logs with no input PWM.
    Comment    step4 comes before step3 due to asynchronoucity of step3
    Read Console Until    [INFO] PWM frequency 0 Hz - duty cycle 0 %
    Comment    Check first measurement values with no input PWM.
    Check Integer Variable    FrequencyHz    0    timeout=2s
    Check Integer Variable    DutyCyclePercent    0

Example Step 3
    [Documentation]    Measures of the frequency and the duty cycle on each rising edge of the external input signal
    Comment    Check output PWM frequency and duty cycle
    Check PWM Measurement    ${50}    ${5}
    Check PWM Measurement    ${24000}    ${40}
    Check PWM Measurement    ${50000}    ${90}

End Of Example
    [Documentation]    Check that the example end with no error
    Comment    Verify ExecStatus value is still EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    duration=1s
    Comment    Verify status LED status is HIGH.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=2s

Check PWM Measurement
    [Documentation]    Check PWM Measurement with the specified frequency and duty cycle
    [Arguments]    ${pwm_freq}    ${pwm_duty}
    Comment    Set PWM frequency and duty cycle
    Write Frequency    MX_EXAMPLE_USER_PWM_PIN    ${pwm_freq}    duty_cycle=${pwm_duty / 100}
    Comment    Check output PWM frequency
    Check Integer Variable    FrequencyHz    ${pwm_freq}    timeout=2s    precision=0.05
    Comment    Check output PWM duty cycle
    Check Integer Variable    DutyCyclePercent    bound_low=${pwm_duty - 2}    bound_high=${pwm_duty + 2}
