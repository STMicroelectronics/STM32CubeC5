*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_flash_erase_program.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_flash    profile:printf


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
    [Documentation]    initializes the FLASH instance and unlocks the FLASH control register access.
    ...    Checks that initialization (app_init) runs without error
    Read Console Until    [INFO] Step 1: Device initialization COMPLETED and the Flash access is unlocked.

Example Step 2
    [Documentation]    the flash memory is erased by specifying the address and the size of the memory that needs to be erased.
    Read Console Until    [INFO] Step 2: The selected Flash area is erased

Example Step 3
    [Documentation]    reprograms the flash memory by specifying the address, the data to be programmed, and the size of this data in bytes.
    ${DataBuffer}=    Read Raw Variable    DataBuffer
    ${FlashUserStartAddr}=    Read Integer Variable    FlashUserStartAddr
    ${read_data}=    Read Memory    ${FlashUserStartAddr}    ${BUFFER_SIZE}
    Should Be Equal    ${DataBuffer}    ${read_data}
    Read Console Until    [INFO] Step 3: The memory Flash is programmed

Example Step 4
    [Documentation]    locks the FLASH control register access and deinitializes the FLASH instance.
    ...    Checks that uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 4: Device de-init COMPLETED
    Comment    Verify MX_STATUS_LED is turned ON an not flashing
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
