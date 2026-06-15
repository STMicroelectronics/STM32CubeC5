*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_led_fading_pwm.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:parts_led


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    ${led_pwm_command_medium_speed_frequency}=    Read Test Variable    LED_PWM_COMMAND_MEDIUM_SPEED_FREQUENCY
    Example Step 1
    Example Step 2    ${led_pwm_command_medium_speed_frequency}
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Device initialization
    Comment    Check initialization (app_init) runs without error
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after

Example Step 2
    [Documentation]    Starts fading the LED indefinitely.
    [Arguments]    ${led_pwm_command_medium_speed_frequency}
    Comment    Check that the led fading.
    Comment    Check that the pwm command signal of led fading frequency is 512Hz
    Check Frequency    pin=MX_STATUS_LED    freq=${led_pwm_command_medium_speed_frequency}    precision=0.1
    ...    timeout=1s

End Of Example
    [Documentation]    Device uninitialization.
    Comment    The led is still fading indefinitely
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=1s
