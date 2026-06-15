*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_dac_simple_conversion.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_dac    profile:printf


*** Variables ***
@{DAC_VOLTAGE_ARRAY}=       ${0}    ${1100}    ${2200}    ${3300}    ${0}


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The DAC is initialized by app_init().
    ...    Checks that initialization (app_init) runs without error
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    The DAC convert the parameter value to the correspondent output voltage.
    ${dac_tue_mv}=    Read Test Variable    DAC_TUE_MV
    ${dac_tue_mv}=    Convert To Integer    ${dac_tue_mv}
    # This voltage correspond to the TUE of the DAC that impact the DAC accuracy.

    FOR    ${dac_voltage}    IN    @{DAC_VOLTAGE_ARRAY}
        Read Console Until    [INFO] Step 2: DAC output voltage set to ${dac_voltage} mV.
        Check Voltage
        ...    DAC_PIN
        ...    ${{$dac_voltage - $dac_tue_mv}}mV
        ...    ${{$dac_voltage + $dac_tue_mv}}mV
        ...    timeout=2s
    END

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
