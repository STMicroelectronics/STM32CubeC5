*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_tim_encoder.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_tim    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Reset Target
    Example Step 1
    Example Step 2
    Example Step 3
    End Of Example

${TEST_ID} User Test
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README while simulating the external wiring.
    Comment   Simulate the wiring between encoder outputs and encoder interface inputs.
    Follow Pin To Pin    MX_TIM_CHANNEL_ENCODER_EMULATOR_1    MX_TIM_CHANNEL_ENCODER_INTERFACE_1
    Follow Pin To Pin    MX_TIM_CHANNEL_ENCODER_EMULATOR_2    MX_TIM_CHANNEL_ENCODER_INTERFACE_2

    # The wiring simulation must be started before the example
    Reset Target
    Example Step 1
    Example Step 2
    Read Console Until    [INFO] Step 3.1: Encoder interface is in Forward direction.
    Read Console Until    [INFO] Step 3.2: Encoder interface is in Backward direction.
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Configures both timers, one that emulate a quadrature encoder signal and the other as an encoder interface.
    Comment    Checking that initialization (app_init) runs without error is not possible:
    ...    ExecStatus is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.
    ...    MX_STATUS_LED is OFF at the startup but becomes immediately HIGH.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2
    [Documentation]    Starts both timers, the timer to emulate quadrature encoder signals and the timer encoder interface.
    Comment    Check emulated signals
    Read Console Until    [INFO] Step 2: Emulation signals and encoder interface started.

Example Step 3
    [Documentation]    Update the emulated signals to toggle every 1s, between two phases (+90° and -90°) to emulate a Forward/Backward rotation.
    Comment    Check the emulated signals and regenerate them on encoder interface inputs.

    Comment    Checks the emulated signals correspond to a forward direction and regenerates them.
    Check Frequency    MX_TIM_CHANNEL_ENCODER_EMULATOR_1    10kHz
    Check Frequency    MX_TIM_CHANNEL_ENCODER_EMULATOR_2    10kHz
    Check Time Between Events
    ...    MX_TIM_CHANNEL_ENCODER_EMULATOR_1
    ...    rising edge
    ...    MX_TIM_CHANNEL_ENCODER_EMULATOR_2
    ...    rising edge
    ...    t_min=20us
    ...    t_max=30us
    ...    time_window=100ms
    Write Dual Frequency    MX_TIM_CHANNEL_ENCODER_INTERFACE_1    MX_TIM_CHANNEL_ENCODER_INTERFACE_2    10kHz    90
    Read Console Until    [INFO] Step 3.1: Encoder interface is in Forward direction.
    # This check is ignored as STM32CubeProgrammer takes too much time.
    # Check Integer Variable    EncoderDirection    0

    Comment    Check the emulated signals correspond to a backward direction and regenerates them.
    Check Time Between Events
    ...    MX_TIM_CHANNEL_ENCODER_EMULATOR_2
    ...    rising edge
    ...    MX_TIM_CHANNEL_ENCODER_EMULATOR_1
    ...    rising edge
    ...    t_min=20us
    ...    t_max=30us
    ...    time_window=100ms
    Write Dual Frequency    MX_TIM_CHANNEL_ENCODER_INTERFACE_2    MX_TIM_CHANNEL_ENCODER_INTERFACE_1    10kHz    90
    Read Console Until    [INFO] Step 3.2: Encoder interface is in Backward direction.
    # This check is ignored as STM32CubeProgrammer takes too much time.
    # Check Integer Variable    EncoderDirection    1

End Of Example
    [Documentation]    Check that the example ends with no error.
    Comment    Verify MX_STATUS_LED is turned ON and not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Comment    Wait for the LED to be turned ON in 1s max
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=1s
    Check Pin    MX_STATUS_LED    ${active_level}    duration=100ms
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    # This check is ignored as STM32CubeProgrammer takes too much time.
    # Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
