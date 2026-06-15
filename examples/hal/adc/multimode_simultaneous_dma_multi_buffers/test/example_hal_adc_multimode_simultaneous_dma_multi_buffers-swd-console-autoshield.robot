*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_adc_multimode_simultaneous_dma_multi_buffers.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             Collections
Library             struct.py
Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_adc    profile:printf


*** Variables ***
${LITTLE_ENDIAN}    <    # Indicates the little-endian format
${INT32_FMT}        i    # Indicates the 32-bit integer format
${FORMAT_STRING}    ${LITTLE_ENDIAN}3${INT32_FMT}


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
    [Documentation]    The ADC is configured to convert a single channel, from regular group, triggered by software.
    Comment    Check initialization runs without error
    Read Console Until    [INFO] Step 1: ADC initialization COMPLETED    timeout=10s

Example Step 2
    [Documentation]    The ADC auto calibration is performed to ensure accurate measurements
    Comment    Check ADC calibration runs without error
    Read Console Until    [INFO] Step 2: ADC start and calibration COMPLETED    timeout=10s

Example Step 3
    [Documentation]    The ADC conversion is started in DMA mode.
    Comment    Check ADC is started without error
    Read Console Until    [INFO] Step 4: DMA transfer COMPLETED for both ADCs.

Example Step 4
    [Documentation]    Wait for the interrupt service routine to be called indicating that the data buffer is filled.
    Comment    Check ADC is started without error
    Read Console Until    [INFO] Step 3: ADC conversion STARTED.

Example Step 5
    [Documentation]    Retrieve ADC raw value, and compute corresponding voltage.
    Comment    Check ADC conversion completes without error

    Read Console Until    [INFO] Step 5: Print voltage buffer for both ADCs.

    Comment    Check several voltage conversion thanks to DAC
    Write Voltage    ANALOG_PRIMARY_PIN    0V
    Sleep    1s
    ${BufferAdcPrimaryVoltage}=    Read Raw Variable    BufferAdcPrimaryVoltage
    ${unpacked_data}=    struct.Unpack    ${LITTLE_ENDIAN} 3${INT32_FMT}    ${BufferAdcPrimaryVoltage}
    ${adc_Primary0}=    Get From List    ${unpacked_data}    0
    ${adc_Primary1}=    Get From List    ${unpacked_data}    1
    ${adc_Primary2}=    Get From List    ${unpacked_data}    2
    Should Be True    ${adc_Primary0} >= 0 and ${adc_Primary0} < 100
    Should Be True    ${adc_Primary1} >= 0 and ${adc_Primary1} < 100
    Should Be True    ${adc_Primary2} >= 0 and ${adc_Primary2} < 100

    Write Pin    ANALOG_SECONDARY_PIN    0
    Sleep    1s
    ${BufferAdcSecondaryVoltage}=    Read Raw Variable    BufferAdcSecondaryVoltage
    ${unpacked_data}=    struct.Unpack    ${LITTLE_ENDIAN} 3${INT32_FMT}    ${BufferAdcSecondaryVoltage}
    ${adc_Secondary0}=    Get From List    ${unpacked_data}    0
    ${adc_Secondary1}=    Get From List    ${unpacked_data}    1
    ${adc_Secondary2}=    Get From List    ${unpacked_data}    2
    Should Be True    ${adc_Secondary0} >= 0 and ${adc_Secondary0} < 100
    Should Be True    ${adc_Secondary1} >= 0 and ${adc_Secondary1} < 100
    Should Be True    ${adc_Secondary2} >= 0 and ${adc_Secondary2} < 100

    Read Console Until    [INFO] Step 3: ADC conversion STARTED.
    Read Console Until    [INFO] Step 4: DMA transfer COMPLETED for both ADCs.
    Read Console Until    [INFO] Step 5: Print voltage buffer for both ADCs.

    Comment    Check several voltage conversion thanks to DAC
    Write Voltage    ANALOG_PRIMARY_PIN    3.2V
    Sleep    1s
    ${BufferAdcPrimaryVoltage}=    Read Raw Variable    BufferAdcPrimaryVoltage
    ${unpacked_data}=    struct.Unpack    ${LITTLE_ENDIAN} 3${INT32_FMT}    ${BufferAdcPrimaryVoltage}
    ${adc_Primary0}=    Get From List    ${unpacked_data}    0
    ${adc_Primary1}=    Get From List    ${unpacked_data}    1
    ${adc_Primary2}=    Get From List    ${unpacked_data}    2
    Should Be True    ${adc_Primary0} >= 3100 and ${adc_Primary0} < 3300
    Should Be True    ${adc_Primary1} >= 3100 and ${adc_Primary1} < 3300
    Should Be True    ${adc_Primary2} >= 3100 and ${adc_Primary2} < 3300

    Write Pin    ANALOG_SECONDARY_PIN    1
    Sleep    1s
    ${BufferAdcSecondaryVoltage}=    Read Raw Variable    BufferAdcSecondaryVoltage
    ${unpacked_data}=    struct.Unpack    ${LITTLE_ENDIAN} 3${INT32_FMT}    ${BufferAdcSecondaryVoltage}
    ${adc_Secondary0}=    Get From List    ${unpacked_data}    0
    ${adc_Secondary1}=    Get From List    ${unpacked_data}    1
    ${adc_Secondary2}=    Get From List    ${unpacked_data}    2
    Should Be True    ${adc_Secondary0} >= 3260 and ${adc_Secondary0} < 3340
    Should Be True    ${adc_Secondary1} >= 3260 and ${adc_Secondary1} < 3340
    Should Be True    ${adc_Secondary2} >= 3260 and ${adc_Secondary2} < 3340

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
