*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_i2c_epprom_dma.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.Console
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_i2c


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4


*** Keywords ***
Example Step 1
    [Documentation]    Configures and initializes the I2C instance.
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED.
    Comment    Checking the LED status is not currently possible,
    ...    as the example immediately starts I2C communications

Example Step 2
    [Documentation]    Waits the EEPROM    to become ready for communication.
    Comment    Nothing to check.

Example Step 3
    [Documentation]    Writes a data buffer to the EEPROM in DMA mode.
    Read Console Until    [INFO] Step 3: Write operation to the EEPROM is COMPLETED.

Example Step 4
    [Documentation]    Reads back the same data buffer from the EEPROM in DMA mode,
    ...    then checks that it matches the buffer written in Step 3
    Read Console Until    [INFO] Step 4 : Read operation from the EEPROM is COMPLETED.
    Read Console Until    READ/WRITE buffers are IDENTICAL

    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=10s

    ${TxBuffer}=    Read Raw Variable    TxBuffer
    ${RxBuffer}=    Read Raw Variable    RxBuffer
    Should Be Equal    ${TxBuffer}    ${RxBuffer}
