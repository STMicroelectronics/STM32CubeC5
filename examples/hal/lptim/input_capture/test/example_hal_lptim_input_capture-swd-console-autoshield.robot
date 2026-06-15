*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation      Automatic test for example_hal_lptim_input_capture.
...                This test suite is verifying that the application binary runs without error,
...                according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_lptim    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...                the scenario described in the README.

    Example Step 1
    Example Step 2
    Example Step 3
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the GPIO and the LPTIM for input capture measurement.
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.

Example Step 2
    [Documentation]    The device goes in stop mode and the DMA starts to transfer data.
    Comment    Wait for MCU goes in stop mode
    Read Console Until    [INFO] Step 2: The device goes in stop mode
    # Sleep additiionnal time to be sure that the MCU is in stop mode before starting to generate the input signal
    Sleep    200ms

Example Step 3
    [Documentation]    Check the wake up of the MCU.
    Write Frequency    MX_EXAMPLE_USER_INPUT_PIN    100
    Read Console Until    [INFO] Step 3: The transfer completes and the DMA generates an interrupt to wake up the MCU.
    Check Integer Variable    MeasuredInputSignalCounter    bound_low=270    bound_high=330    timeout=10s
    Read Console Until    [INFO] Step 3: The expected and measured input signal frequencies match.

End Of Example
    [Documentation]    Check that the example end with no error
    Comment    Verify ExecStatus value is still EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    duration=1s
    Comment    Verify status LED status is HIGH.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin         MX_STATUS_LED   ${active_level}    duration=2s
