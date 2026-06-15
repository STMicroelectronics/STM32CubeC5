*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_iwdg_reset.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_iwdg


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
    [Documentation]    The IWDG is initialized
    ...    Checks that initialization (app_init) runs without error
    Comment    Verify ExecStatus value is EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after

Example Step 2
    [Documentation]    The purpose is to refresh the watchdog PROCESS_LOOP_NUMBER times to prevent watchdog reset.
    Comment    Nothing to verify because the reset cannot be caught.

Example Step 3
    [Documentation]    The watchdog is not refreshed on purpose to trigger the IWDG reset.
    Comment    Nothing to verify because this step do not interact with hardware.

Example Step 4
    [Documentation]    The device is restarted due to IWDG.
    ...    Checks that uninitialization (app_deinit) runs without error
    Comment    The watchdog is refreshed for 3x900ms = 2700ms (with the example's defaults)
    ...        Add a duration lower than this value to be sure the watchdog is not refreshed
    # Due to STM32CubeProgrammer low speed, the duration is drastically decreased
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}    duration=1000ms

    Comment    After 1s, the watchdog fires and resets the example.
    # the timeout here is arbitrarily higher, as the example state is stable afterwards,
    # and we want to give the example enough time to restart (go through system_init(), app_init(), ...)
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=3s
