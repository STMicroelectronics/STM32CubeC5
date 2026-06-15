*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_flash_write_protection.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_flash


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


*** Keywords ***
Example Step 1
    [Documentation]    Initializes the flash instance, unlocks the flash programming interface, erase the desired flash sector,
    ...                then unlocks the option byte.
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 2
    [Documentation]    Enables page-level write protection for the desired pages,
    ...                then program the option bytes.
    Comment    Checks Flash erase COMPLETED and write protection mechanism is ENABLED.

Example Step 3
    [Documentation]    Programs the flash memory by specifying the address of the flash,
    ...                the data to be programmed, and the size of this data in bytes and check that write operation is not allowed in those pages.
    Comment    Checks write protection is ENABLED and all write operation is not permitted.

Example Step 4
    [Documentation]    Disables the flash write protection mechanism for the previous flash pages.
    Comment    Checks WRP are DISABLED for all pages in Bank 2, and option bytes are programmed.

Example Step 5
    [Documentation]    Reprograms the flash memory, and lock option bytes and the flash interface programming.
    ...                Then check that data is written correctly in the chosen memory address.
    Comment    BUFFER_SIZE = BUFFER_SIZE * 512 ==> 64 bits systems
    ...        BUFFER_SIZE = BUFFER_SIZE * 1024 ==> 32 bits systems
    ${TxBuffer}=    Read Raw Variable    WriteBuffer
    ${pFlashAddress}=    Read Integer Variable    pFlashAddress
    ${BUFFER_SIZE}=    Evaluate    ${BUFFER_SIZE} * 512
    ${read_data}=    Read Memory    ${pFlashAddress}    ${BUFFER_SIZE}
    Should Be Equal    ${TxBuffer}    ${read_data}
    Comment    ExecStatus value is then EXEC_STATUS_OK.

Example Step 6
    [Documentation]    FLASH peripheral deinitialization.
    ...    Checks that uninitialization (app_deinit) runs without error
    ...    Assuming the test scenario lasts under 5s.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=5s
