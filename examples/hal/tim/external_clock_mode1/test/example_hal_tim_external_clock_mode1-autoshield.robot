*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_tim_external_clock_mode1.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_tim


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Follow Pin To Pin    MX_TIM_EXTERNAL_CLOCK_MODE1_EMULATOR_PIN    MX_TIM_EXTERNAL_CLOCK_MODE1_INPUT_PIN
    Reset Target
    Example Step 1
    Example Step 2
    End Of Example

*** Keywords ***
Example Step 1
    [Documentation]    Configure the GPIO INPUT to enable the EXTI interrupt
    ...    Checks that initialization (app_init) runs without error
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}

Example Step 2
    [Documentation]    Waiting for the EXTI interrupt
    Comment    Check button pull resistor configuration
    ${active_button_level}=    Read Test Variable    MX_STATUS_BUTTON_ACTIVE_STATE
    Check Button Configuration    MX_EXAMPLE_EXTI_TRIGGER_PIN    ${active_button_level}

    FOR    ${i}    IN RANGE    0    5
        Comment    Active EXTI interruption on the board pin
        Write Pulse    MX_EXAMPLE_EXTI_TRIGGER_PIN
        Sleep    200ms
    END

End Of Example
    [Documentation]    Unconfigure the GPIO INPUT and leave the example
    ...    Checks that uninitialization (app_deinit) runs without error
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=1s
    Check Pin    MX_STATUS_LED    ${active_level}    duration=250ms
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
