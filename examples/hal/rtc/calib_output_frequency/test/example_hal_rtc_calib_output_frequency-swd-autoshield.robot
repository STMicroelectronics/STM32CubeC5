*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_rtc_calib_output_frequency.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             Collections
Library             struct.py
Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_rtc


*** Test Cases ***
${TEST_ID} RTC calib output frequency Scenario Adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README for the RTC calib output frequency example.
    # Needed to apply a complete RTC (re)initialization
    Reset Target    mode=POWERDOWN

    Example Step 1
    
    End Of Example

*** Keywords ***
Example Step 1
    [Documentation]    Initialize RTC and enable 1 Hz calibration output.
    Comment    Check that the RTC Output pin produces a 1 Hz square wave.
    Check Frequency    pin=RTC_OUT1    freq=1Hz    precision=0.1

End Of Example
    [Documentation]    Verify successful completion.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s

