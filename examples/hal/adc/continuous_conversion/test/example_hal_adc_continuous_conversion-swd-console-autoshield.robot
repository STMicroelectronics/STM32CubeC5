*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_adc_continuous_conversion.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_adc    profile:printf


*** Variables ***
${VOLTAGE_MARGIN_MV}    100
${VOLTAGE_STEP}         1000


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The ADC is configured to convert a single channel, from regular group, triggered by software.
    Comment    Check initialization runs without error
    Read Console Until    [INFO] Step 1: ADC initialization COMPLETED    timeout=10s

Example Step 2
    [Documentation]    The ADC auto calibration is performed to ensure accurate measurements
    Comment    Check ADC calibration runs without error
    Read Console Until    [INFO] Step 2: ADC start and calibration COMPLETED    timeout=10s

Example Step 3
    [Documentation]    The ADC conversion is started in polling mode.
    Comment    Check ADC is started without error
    Read Console Until    [INFO] Step 3: ADC start conversion COMPLETED.

Example Step 4
    [Documentation]    Retrieve ADC raw value, and compute corresponding voltage.
    Log    Check several voltage conversion thanks to DAC with ${VOLTAGE_MARGIN_MV} mV margin
    FOR    ${i}    IN RANGE    0    4
        ${voltage} =    Set Variable    ${${i} * ${VOLTAGE_STEP}}
        Write Voltage    ANALOG_PIN    ${voltage}mV
        Check Integer Variable
        ...    AdcMilliVolt
        ...    ${${voltage} - ${VOLTAGE_MARGIN_MV}}
        ...    ${${voltage} + ${VOLTAGE_MARGIN_MV}}
        ...    timeout=2s
        Read Console Until    [INFO] Step 4: ADC conversion DONE.
    END

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level} =    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
