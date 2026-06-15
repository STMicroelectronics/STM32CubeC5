*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_adc_trigger_timer_dma.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             Collections
Library             struct.py
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
    [Documentation]    Computes the corresponding voltage for both ADCs and prints the ADC raw value and the computed voltage.
    Comment    Check several voltage conversion thanks to DAC
    Write Voltage    ANALOG_PIN    0V
    Sleep    1s
    ${AdcMilliVolt}=    Read Raw Variable    AdcMilliVolt
    Comment    Using ${AdcMilliVolt} Set Variable ${AdcMilliVolt}[:6] to avoid the problem in IAR.
    ...    unpack requires a buffer of 6 bytes but IAR read 8.
    ...    The other compilers GCC and AC6 do not generate this error.
    ${AdcMilliVolt}=    Set Variable    ${AdcMilliVolt}[:6]
    ${unpacked_data}=    struct.Unpack    ${LITTLE_ENDIAN} 3${INT16_FMT}    ${AdcMilliVolt}
    ${AdcMilliVolt0}=    Get From List    ${unpacked_data}    0
    ${AdcMilliVolt1}=    Get From List    ${unpacked_data}    1
    ${AdcMilliVolt2}=    Get From List    ${unpacked_data}    2
    Should Be True    ${AdcMilliVolt0} >= 0 and ${AdcMilliVolt0} < 100
    Should Be True    ${AdcMilliVolt1} >= 0 and ${AdcMilliVolt1} < 100
    Should Be True    ${AdcMilliVolt2} >= 0 and ${AdcMilliVolt2} < 100

    Write Voltage    ANALOG_PIN    3.2V
    Sleep    1s
    ${AdcMilliVolt}=    Read Raw Variable    AdcMilliVolt
    Comment    Using ${AdcMilliVolt} Set Variable ${AdcMilliVolt}[:6] to avoid the problem in IAR.
    ...    unpack requires a buffer of 6 bytes but IAR read 8.
    ...    The other compilers GCC and AC6 do not generate this error.
    ${AdcMilliVolt}=    Set Variable    ${AdcMilliVolt}[:6]
    ${unpacked_data}=    struct.Unpack    ${LITTLE_ENDIAN} 3${INT16_FMT}    ${AdcMilliVolt}
    ${AdcMilliVolt0}=    Get From List    ${unpacked_data}    0
    ${AdcMilliVolt1}=    Get From List    ${unpacked_data}    1
    ${AdcMilliVolt2}=    Get From List    ${unpacked_data}    2
    Should Be True    ${AdcMilliVolt0} >= 3100 and ${AdcMilliVolt0} < 3300
    Should Be True    ${AdcMilliVolt1} >= 3100 and ${AdcMilliVolt1} < 3300
    Should Be True    ${AdcMilliVolt2} >= 3100 and ${AdcMilliVolt2} < 3300

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
