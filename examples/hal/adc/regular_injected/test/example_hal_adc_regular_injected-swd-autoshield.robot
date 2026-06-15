*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_adc_regular_injected.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_adc


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Configures the ADC to convert multiple channels.
    Comment    No check possible

Example Step 2
    [Documentation]    The ADC auto calibration is performed to ensure accurate measurements
    Comment    No check possible

Example Step 3
    [Documentation]    The ADC conversion is started in polling mode for regular channels.
    Comment    No check possible

Example Step 4
    [Documentation]    The ADC conversion is started in it mode for injected channels.
    Comment    No check possible

Example Step 5
    [Documentation]    Retrieves ADC channels conversion data of GPIO, VrefInt voltages in mV and sensor temperature in DegC.
    Comment    Check several voltage conversion thanks to DAC.
    ${Vref_internal}=    Read Test Variable    Vref_internal
    ${voltage_below_vref}=    Evaluate    ${Vref_internal} - 100
    ${voltage_over_vref}=    Evaluate    ${Vref_internal} + 100

    ${Temp_DegreeCelsius}=    Read Test Variable    Temp_DegreeCelsius
    ${Temp_DegreeCelsius_below}=    Evaluate    ${Temp_DegreeCelsius} - 10
    ${Temp_DegreeCelsius_over}=    Evaluate    ${Temp_DegreeCelsius} + 10

    # ADC group regular conversions
    Write Voltage    ANALOG_PIN    0V
    Check Integer Variable    VoltageGpioVoltage    0    100    timeout=3s
    Write Voltage    ANALOG_PIN    1.200V
    Check Integer Variable    VoltageGpioVoltage    1100    1300    timeout=3s
    Write Voltage    ANALOG_PIN    3.200V
    Check Integer Variable    VoltageGpioVoltage    3100    3300    timeout=3s
    # ADC group injected conversions
    Check Integer Variable    VrefIntVoltage    ${voltage_below_vref}    ${voltage_over_vref}    timeout=2s
    Check Integer Variable    TempDegreeCelsius    ${Temp_DegreeCelsius_below}    ${Temp_DegreeCelsius_over}    timeout=2s


End Of Example
    [Documentation]    Check ExecStatus and Led Status
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=30s
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
