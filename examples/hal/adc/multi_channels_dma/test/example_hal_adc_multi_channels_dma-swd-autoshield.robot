*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_adc_multi_channels_dma.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_adc


*** Variables ***
${VOLTAGE_MARGIN_MV}=       100
${VOLTAGE_STEP}=            1000


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
    Comment    No check possible

Example Step 2
    [Documentation]    The ADC auto calibration is performed to ensure accurate measurements
    Comment    No check possible

Example Step 3
    [Documentation]    The ADC conversion is started in DMA mode.
    Comment    No check possible

Example Step 4
    [Documentation]    Wait for End of Conversion (EOC). Retrieve ADC raw value, and compute corresponding voltage.
    Log    Check several voltage conversion thanks to DAC with ${VOLTAGE_MARGIN_MV} mV margin
    ${Vref_internal}=    Read Test Variable    Vref_internal
    ${voltage_below_vref}=    Evaluate    ${Vref_internal} - 100
    ${voltage_over_vref}=    Evaluate    ${Vref_internal} + 100

    ${Temp_DegreeCelsius_below}=    Read Test Variable    Temp_DegreeCelsius_below
    ${Temp_DegreeCelsius_over}=    Read Test Variable    Temp_DegreeCelsius_over

    ${Vref_analog}=    Read Test Variable    Vref_analog
    ${Vref_analog_below}=    Evaluate    ${Vref_analog} - 100
    ${Vref_analog_over}=    Evaluate    ${Vref_analog} + 100

    FOR    ${i}    IN RANGE    0    4
        ${voltage}=    Set Variable    ${${i} * ${VOLTAGE_STEP}}
        Write Voltage    ANALOG_PIN    ${voltage}mV
        Check Integer Variable
        ...    VoltageGpioVoltage
        ...    ${${voltage} - ${VOLTAGE_MARGIN_MV}}
        ...    ${${voltage} + ${VOLTAGE_MARGIN_MV}}
        ...    timeout=2s
        Check Integer Variable    VrefIntVoltage    ${voltage_below_vref}    ${voltage_over_vref}    timeout=2s
        Check Integer Variable
        ...    TempDegreeCelsius
        ...    ${Temp_DegreeCelsius_below}
        ...    ${Temp_DegreeCelsius_over}
        ...    timeout=2s
        Check Integer Variable    VrefAnalogVoltage    ${Vref_analog_below}    ${Vref_analog_over}    timeout=2s
    END

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
