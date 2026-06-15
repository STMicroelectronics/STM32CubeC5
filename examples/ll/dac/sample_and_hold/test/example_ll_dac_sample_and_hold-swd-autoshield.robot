*** Comments ***
Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_dac_sample_and_hold.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_dac


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1
    Example Step 2
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The DAC is initialized and the sample and hold mode is enabled by app_init().
    ...    Checks that initialization (app_init) runs without error
    Comment    Nothing to check

Example Step 2
    [Documentation]    The DAC output a voltage and enter in STOP0 mode.
    ${LSI_FREQ}=    Read Test Variable    lsi_freq
    ${DAC_HOLD_TIME_CYCLE}=    Read Test Variable    dac_hold_time_cycle
    ${DAC_REFRESH_TIME_CYCLE}=    Read Test Variable    dac_refresh_time_cycle

    ${DAC_HOLD_TIME_CYCLE}=    Convert To Integer    ${DAC_HOLD_TIME_CYCLE}
    ${LSI_FREQ}=    Convert To Integer    ${LSI_FREQ}
    ${DAC_REFRESH_TIME_CYCLE}=    Convert To Integer    ${DAC_REFRESH_TIME_CYCLE}

    ${SIGNAL_FREQ}=    Set Variable    ${{ $LSI_FREQ / ($DAC_HOLD_TIME_CYCLE + $DAC_REFRESH_TIME_CYCLE) }}

    Check Frequency    DAC_PIN    ${SIGNAL_FREQ}    0.1    timeout=5s

End Of Example
    [Documentation]    Check the target's core is not running. When target is in stop mode, the core stays in sleep
    ...    mode. Check the LED remain on at the end of the example.
    Comment    Core Status (sleep mode) should be tested when service is working ok
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
