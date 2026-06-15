*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_dac_signal_generation_dma_silent_tim.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             Collections
Library             struct.py
Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_dac


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The DAC, DMA and TIM are initialized by app_init().
    ...    Checks that initialization (app_init) runs without error
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Converts the data from millivolts to raw data because DAC cannot handle directly millivolts data
    ...    but only raw data between 0 and the maximale DAC resolution.
    ${theoretical_buffer}=    Read Test Variable    RAW_BUFFER
    ${embedded_buffer}=    Read Raw Variable    SineWave

    # Convert the raw block of bytes to a tuple of ints
    # Use the Python built-in struct library for that purpose:
    # The "<60H" format code translates to "60 unsigned 2-bytes integer, little-endian"
    ${embedded_buffer}=    struct.Unpack    <60H    ${embedded_buffer}
    Lists Should Be Equal    ${{eval($theoretical_buffer)}}    ${embedded_buffer}

Example Step 3
    [Documentation]    The DAC conversion is started using the DMA feature and the TIM started to trigger the DAC
    ...    conversion.
    Comment    Nothing can be checked here

End Of Example
    [Documentation]    Check ExecStatus
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
