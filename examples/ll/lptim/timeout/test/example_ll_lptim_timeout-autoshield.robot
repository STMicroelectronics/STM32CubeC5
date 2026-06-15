*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_ll_lptim_timeout.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_lptim


*** Variables ***
${TIMEOUT_S}=               1
${toggle_freq}=             ${{1/(2*${TIMEOUT_S})}}
${toggle_freq_greater}=     ${{ 110*${toggle_freq} / 100}}
${toggle_freq_lower}=       ${{ 90*${toggle_freq} / 100}}


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the RADME.

    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    The LPTIM is initialized
    ...    Checks that initialization (app_init) runs without error
    Check Pin    STATUS_LED_PIN    ${LOW}    duration=2s

Example Step 2
    [Documentation]    Start the LPTIM in interrupt mode.
    Comment    Nothing to check

Example Step 3
    [Documentation]    Switch to LOW POWER mode
    Comment    Nothing to be tested here

Example Step 4
    [Documentation]    Trigger LPTIM_ETR and check the timeout toggle the pin
    ${clk_freq_hz}=    Read Test Variable    CLK_FREQ_HZ
    ${clk_max_hz}=    Read Test Variable    CLK_MAX_HZ
    ${clk_min_hz}=    Read Test Variable    CLK_MIN_HZ
    ${compare_value}=    Set Variable    ${{ ${TIMEOUT_S} * ${clk_freq_hz} }}

    Comment    Check the timeout leads to a toggle of the led at each period of the timer
    ${precision}=    Set Variable
    ...    ${{max( (${clk_max_hz} - ${clk_freq_hz}) / ${clk_freq_hz}, (${clk_freq_hz} - ${clk_min_hz}) / ${clk_freq_hz} )}}
    Write Pin    LPTIM_ETR    ${LOW}
    Check Low Pwm    STATUS_LED_PIN    ${TIMEOUT_S}    ${TIMEOUT_S}    precision=${precision}
    Release Gpio Pin    LPTIM_ETR

    Comment    Checks the timeout is as expected by applying a lower trigger than the one needed to trigger the timeout
    ${timeout_min}=    Set Variable    ${{round( ${compare_value} / ${clk_max_hz}, 2)}}
    ${timeout_max}=    Set Variable    ${{round( ${compare_value} / ${clk_min_hz}, 2)}}
    Write Frequency    LPTIM_ETR    freq=${{ round(${toggle_freq_lower}, 2) }}
    Check Time Between Events
    ...    evt1_pin=LPTIM_ETR
    ...    evt1_edge=any edge
    ...    evt2_pin=STATUS_LED_PIN
    ...    evt2_edge=any edge
    ...    t_min=${timeout_min}
    ...    t_max=${timeout_max}
    ...    time_window=${{10*${TIMEOUT_S}}}

    Comment    Check a greater trigger does not trigger the timeout
    Write Frequency    LPTIM_ETR    freq=${{ round(${toggle_freq_greater}, 2) }}
    Sleep    ${{2 * ${TIMEOUT_S}}}s
    Check Pin    STATUS_LED_PIN    ${ANY}    duration=2s
