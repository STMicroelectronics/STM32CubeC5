*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_adc_calibration.
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


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5
    Example Step 6
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The ADC is configured to convert a single channel, from regular group, triggered by software.
    Comment    No check possible
    Read Console Until    [INFO] Step 1: ADC initialization COMPLETED.

Example Step 2
    [Documentation]    The ADC auto calibration is performed to ensure accurate measurements
    Comment    No check possible
    Read Console Until    [INFO] Step 2: ADC start COMPLETED.

Example Step 3
    [Documentation]    The ADC autocalibration is performed to ensure accurate measurements. The calibration is a prerequisite to any ADC operation. It removes the systematic errors that may vary from chip to chip.
    Comment    No check possible
    Read Console Until    [INFO] Step 3: ADC calibration COMPLETED.

Example Step 4
    [Documentation]    Retrieves the ADC calibration factors to reinject them in next loop.
    Comment    No check possible
    Read Console Until    [INFO] Step 4: ADC get calibration factor COMPLETED.

Example Step 5
    [Documentation]    Resets the ADC calibration factors to emulate low-power mode.
    Comment    No check possible
    Read Console Until    [INFO] Step 5:ADC emulate calibration factor loss by loading a dummy calibration COMPLETED.

Example Step 6
    [Documentation]    Reinjects copy of the ADC calibration factors from previous loop.
    Comment    No check possible
    Read Console Until    [INFO] Step 6: ADC set calibration factor COMPLETED.

End Of Example
    [Documentation]    Check ExecStatus and Led Status
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    timeout=10s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
