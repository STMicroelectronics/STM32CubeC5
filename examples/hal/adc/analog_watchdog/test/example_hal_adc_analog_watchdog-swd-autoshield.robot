*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_adc_analog_watchdog.
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
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The ADC is configured to convert a single channel, from regular group, triggered by software.
    Comment    No check possible

Example Step 2
    [Documentation]    The ADC auto calibration is performed to ensure accurate measurements
    Comment    No check possible

Example Step 3
    [Documentation]    The ADC conversion is started in interrupt mode.
    Comment    No check possible

Example Step 4
    [Documentation]    Wait for Conversion data is out of analog watchdog window: conversion complete callback. Retrieve ADC raw value, and compute corresponding voltage.
    Comment    Check several voltage conversion thanks to DAC
    Write Voltage    ANALOG_PIN    0V
    Check Integer Variable    AnalogWatchdogStatus    0    timeout=2s
    Write Voltage    ANALOG_PIN    1.500V
    Check Integer Variable    AnalogWatchdogStatus    0    timeout=2s
    Write Voltage    ANALOG_PIN    1.800V
    Check Integer Variable    AnalogWatchdogStatus    1    timeout=2s
    Check Integer Variable    AdcMilliVolt    1700    1900    timeout=2s
    Write Voltage    ANALOG_PIN    1.500V
    Check Integer Variable    AnalogWatchdogStatus    0    timeout=2s
    Write Voltage    ANALOG_PIN    1.800V
    Check Integer Variable    AnalogWatchdogStatus    1    timeout=2s
    Check Integer Variable    AdcMilliVolt    1650    1950    timeout=2s

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
