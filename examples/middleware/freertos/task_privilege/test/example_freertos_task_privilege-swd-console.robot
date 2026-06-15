*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_freertos_task_privilege.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:mw_freertos    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Example Step 1
    Example Step 2
    FOR    ${i}    IN RANGE    4
        Example Step 3
    END
    Example Step 4
    Example Step 5
    Example Step 6
    Example Step 7
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    Creates a background task, a privileged Main task, a privileged Supervisor task,
    ...    two unprivileged tasks, and a queue used to transfer fault information.
    Comment    No console output expected.
    Comment    Checking ExecStatus is not currently possible due to the delay of CubeProgrammer.

Example Step 2
    [Documentation]    Starts the FreeRTOS scheduler.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] Starting FreeRTOS Scheduler

Example Step 3
    [Documentation]    MainTask monitors the two unprivileged tasks and reports their initial Ready state once.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] Main Privileged Task is running
    Read Console Until    [INFO] Task unprivileged Task1 is in state:${SPACE}${SPACE}Ready
    Read Console Until    [INFO] Task unprivileged Task2 is in state:${SPACE}${SPACE}Ready

Example Step 4
    [Documentation]    Task1 triggers a MemManage fault and SupervisorTask reports it.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] Supervisor Privileged Task is running
    Read Console Until    [INFO] MemManage fault occurred
    Read Console Until    [DATA] Address:

Example Step 5
    [Documentation]    SupervisorTask suspends the unprivileged tasks after receiving the fault information.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] Unprivileged Tasks were suspended.

Example Step 6
    [Documentation]    MainTask reports that Task1 is suspended and requests investigation.
    Comment    No time to do a check before the next step is executed.
    Read Console Until    [INFO] Main Privileged Task is running
    Read Console Until    [INFO] Task unprivileged Task1 is in state:${SPACE}${SPACE}Suspended
    Read Console Until    [ALERT] User must investigate the memory fault cause

Example Step 7
    [Documentation]    MainTask reports that Task2 is also suspended.
    Comment    No time to do a check before the end-of-example check is executed.
    Read Console Until    [INFO] Task unprivileged Task2 is in state:${SPACE}${SPACE}Suspended
    Read Console Until    [ALERT] User must investigate the memory fault cause

End Of Example
    [Documentation]    The scheduler keeps running and the privileged monitoring tasks continue executing
    ...    after the two unprivileged tasks have been suspended, without repeating unchanged task states.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=30s
