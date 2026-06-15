*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_iwdg_early_interrupt.
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

Test Tags           ip:hal_iwdg    profile:printf


*** Variables ***
${PROCCESS_LOOP_NUMBER}     ${3}


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    Example Step 3
    Example Step 4
    Example Step 5


*** Keywords ***
Example Step 1
    [Documentation]    The IWDG is initialized
    ...    Checks that initialization (app_init) runs without error
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after
    ${inactive_level}=    Read Test Variable    MX_STATUS_LED_INACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${inactive_level}    timeout=1ms

Example Step 2
    [Documentation]    The purpose is to refresh the watchdog PROCESS_LOOP_NUMBER times to prevent watchdog reset.
    FOR    ${index}    IN RANGE    1    ${PROCCESS_LOOP_NUMBER}
        Read Console Until    [INFO] Step 2: IWDG refreshed.
    END

Example Step 3
    [Documentation]    The watchdog is not refreshed on purpose to trigger the IWDG reset.
    Read Console Until    [INFO] Step 3: IWDG not refreshed on purpose.

Example Step 4
    [Documentation]    The memory is programmed during the interrupt.
    Read Console Until    [INFO] Step 4: The memory is programmed during the interrupt.

Example Step 5
    [Documentation]    Device's restart completed due to IWDG. Data verification completed.
    ...    Checks that uninitialization (app_deinit) runs without error
    Read Console Until    [INFO] Step 5: Device's restart completed due to IWDG. Data verification completed.
    ${DataBuffer}=    Read Raw Variable    DataBuffer
    ${MemoryUserStartAdd}=    Read Integer Variable    MemoryUserStartAdd
    ${read_data}=    Read Memory    ${MemoryUserStartAdd}    ${BUFFER_SIZE}
    Comment    Using ${DataBuffer} Set Variable ${DataBuffer}[:29] to avoid the problem in GCC.
    ...    The Cube program reads the DataBuffer on 32 bits, but the buffer size is 29.
    ...    The other compilers AC6 and IAR do not generate this error.
    ${DataBuffer}=    Set Variable    ${DataBuffer}[:29]
    Should Be Equal    ${DataBuffer}    ${read_data}
    Comment
    ...    The status of the ExecStatus variable should be checked after the reset. The duration allows to check the
    ...    device has not restarted yet and is still under the app_process function. So the ExecStatus variable is
    ...    set to the value return by the app_init function.
    Comment    After having wait the appropriate amount of time before, the test checks the device has restarted and
    ...    reach the app_deinit function.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=1s
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
