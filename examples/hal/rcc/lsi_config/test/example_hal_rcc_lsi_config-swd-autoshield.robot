*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_rcc_lsi_config.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_rcc


*** Variables ***
${LSI_DISABLE_DELAY_MS}=     2s


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
    [Documentation]    IPs and LSI initialized
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after
    Comment    Nothing to check

Example Step 2
    [Documentation]    The LSI is disabled during LSI_DISABLE_DELAY_MS ms and then re-enabled.
    Comment    Check application (app_process) runs without error
    Comment     We cannot check this step due to synchronization limitations.

Example Step 3
    [Documentation]    LSI restored and outputted on MCO pin.
    Comment     We check the frequency of the LSI.
    ${mco_divider}=    Read Test Variable    mco_divider
    ${lsi_frequency}=    Read Test Variable    lsi_frequency
    ${lsi_precision}=    Read Test Variable    lsi_precision
    ${mco_divider}=    Convert To Integer    ${mco_divider}
    ${lsi_frequency}=    Convert To Integer    ${lsi_frequency}
    ${lsi_precision}=    Convert To Number    ${lsi_precision}

    Sleep      ${LSI_DISABLE_DELAY_MS}

    IF    ${mco_divider} == 0
        Comment    MCO output pin is not accessible
    ELSE
        Check Frequency    mco_pin    ${{$lsi_frequency/$mco_divider}}    ${lsi_precision}    timeout=2s    duty_cycle=0.5
    END

End Of Example
    [Documentation]    End of example reached.
    Comment    Verify ExecStatus value is EXEC_STATUS_OK
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
