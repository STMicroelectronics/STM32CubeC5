*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_tim_6_step_pwm.
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
${SWITCHING_FREQUENCY}=     30300
${TIME_WINDOW_MICROS}=      25000
${FREQUENCY_MARGIN}=        10
${DUTY_CYCLE_MARGIN}=       7


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

    Comment    Checks the PWM generation on each channel
    Check Toggling Phase    MX_EXAMPLE_TIM_CH1    ${SWITCHING_FREQUENCY}    48
    Check Toggling Phase    MX_EXAMPLE_TIM_CH1N    ${SWITCHING_FREQUENCY}    48
    Check Toggling Phase    MX_EXAMPLE_TIM_CH2    ${SWITCHING_FREQUENCY}    24
    Check Toggling Phase    MX_EXAMPLE_TIM_CH2N    ${SWITCHING_FREQUENCY}    24
    Check Toggling Phase    MX_EXAMPLE_TIM_CH3    ${SWITCHING_FREQUENCY}    12
    Check Toggling Phase    MX_EXAMPLE_TIM_CH3N    ${SWITCHING_FREQUENCY}    12

    Comment    Checks the duration between channels
    Check Channel Delay    MX_EXAMPLE_TIM_CH1    MX_EXAMPLE_TIM_CH2    900us    1100us
    Check Channel Delay    MX_EXAMPLE_TIM_CH2    MX_EXAMPLE_TIM_CH3    900us    1100us
    Check Channel Delay    MX_EXAMPLE_TIM_CH3    MX_EXAMPLE_TIM_CH1N    900us    1100us
    Check Channel Delay    MX_EXAMPLE_TIM_CH1N    MX_EXAMPLE_TIM_CH2N    900us    1100us
    Check Channel Delay    MX_EXAMPLE_TIM_CH2N    MX_EXAMPLE_TIM_CH3N    900us    1100us
    Check Channel Delay    MX_EXAMPLE_TIM_CH3N    MX_EXAMPLE_TIM_CH1    900us    1100us

End Of Example
    [Documentation]    Check that the example end with no error.
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s

###################################################################################################
# Functions Helpers
#

Check Toggling Phase
    [Documentation]    Check Toggling timings phase
    [Arguments]    ${pin}    ${request_frequency}    ${request_duty_cycle}

    ${long_time_count}=    Set Variable    0
    ${pwm_fail}=    Set Variable    0

    # Get the frequency and duty cycle of the PWM signal in an array
    @{duty_cycles}=    Get Low Pwm In Array    ${pin}    ${TIME_WINDOW_MICROS}us
    ${duty_cycles_size}=    Get Length    ${duty_cycles}

    FOR    ${i}    IN RANGE    1    ${duty_cycles_size}
        ${frequency}=    Set Variable    ${duty_cycles}[${i}][0]
        ${duty_cycle}=    Set Variable    ${duty_cycles}[${i}][1]

        # Get Start of toggling phase
        IF    ${duty_cycle} <= 1 and ${frequency} > 240 and ${frequency} < 260
            # Count the number of stable low-level phase, only one must be detected in the TIME_WINDOW frame
            Comment    Stable low level phase detected
            ${long_time_count}=    Set Variable    ${${long_time_count} + 1}
        END

        # Check only during the toggling phase
        IF    ${long_time_count} == 1
            # Check the frequency and duty cycle values of toggling phase
            IF    ${frequency} < ${request_frequency} * (100 - ${FREQUENCY_MARGIN}) / 100 or ${frequency} > ${request_frequency} * (100 + ${FREQUENCY_MARGIN}) / 100
                ${pwm_fail}=    Set Variable    ${${pwm_fail} + 1}
            ELSE
                IF    ${duty_cycle} < ${request_duty_cycle} - ${DUTY_CYCLE_MARGIN} or ${duty_cycle} > ${request_duty_cycle} + ${DUTY_CYCLE_MARGIN}
                    FAIL    Duty cycle is out of the range (+/-${DUTY_CYCLE_MARGIN}%): ${duty_cycle}
                END
            END

            # Toggling signal before or atfer stable low level phase time may be shorter. Only 2 are allowed
            IF    ${pwm_fail} > 2
                FAIL    Too mmany frequencies out of the range (+/-${DUTY_CYCLE_MARGIN}%): ${duty_cycle}
            END
        END
    END

Check Channel Delay
    [Documentation]    Check the delay between two channels
    [Arguments]    ${pin_1}    ${pin_2}    ${t_min}    ${t_max}
    Check Time Between Events
    ...    evt1_pin=${pin_1}
    ...    evt1_edge=rising edge
    ...    evt2_pin=${pin_2}
    ...    evt2_edge=rising edge
    ...    t_min=${t_min}
    ...    t_max=${t_max}
    ...    time_window=${TIME_WINDOW_MICROS}us
    ...    time_filtering=50
