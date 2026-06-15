*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_sequencer_gpio_toggle.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:utilities_sequencer


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.

    Example Step 1-2
    Example Step 3
    Example Step 4


*** Keywords ***
Example Step 1-2
    [Documentation]    The GPIO and sequencer are initialized
    ...    Checks that initialization (app_init) runs without error
    Comment    ExecStatus value is then EXEC_STATUS_INIT_OK
    ...    but becomes immediately EXEC_STATUS_OK as there is no delay after.

Example Step 3
    [Documentation]    task function gpio_toggle().
    Comment    Nothing to check in this step.

Example Step 4
    [Documentation]    infinite loop calling (SEQ_RUN()) to toggle the GPIO state with a 1 second period.
    Comment    Checks the final ExecStatus
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=1s
    Comment    Checks that the example run without problem multiple times
    # The boundaries are set arbitrary to 5 and 10 to check that the example run at least 5 times.
    # But due to CubeProgrammer speed limitations, we have to check on an interval.
    Check Integer Variable    ToggleCounter    bound_low=${5}    bound_high=${10}    timeout=7s
