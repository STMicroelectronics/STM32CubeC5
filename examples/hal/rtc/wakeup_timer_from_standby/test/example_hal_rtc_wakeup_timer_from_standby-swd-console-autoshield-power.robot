*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_rtc_wakeup_timer_from_standby.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             Collections
Library             struct.py
Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Console
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_rtc    profile:printf        robot:skip


*** Test Cases ***
${TEST_ID} RTC wakeup timer from standby Scenario Adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README for the RTC wakeup timer from standby example.

    Comment    Execute a first power cycle to start the example
    Reset Target    mode=POWERDOWN

    Example Step 1
    FOR    ${i}    IN RANGE    1    6
        Example Step 2
        Sleep    3.5s    # Wait for RTC wakeup and system restart
    END

    Example Step 3
    End Of Example

*** Keywords ***
Example Step 1
    [Documentation]    On first start, initialize RTC and backup domain.
    Comment    The console message for Step 1 appears only once and cannot be reliably tested, so there is nothing to check.

Example Step 2
    [Documentation]    Enter standby and restart from standby.
    Read Console Until    [INFO] Step 2.1: Enter STANDBY mode - After 3s RTC will generate a wakeup interrupt.
    Read Console Until    [INFO] Step 2.2: Resumed from STANDBY mode and count the wakeups.

Example Step 3
    [Documentation]    Check for 5 RTC wake-ups.
    Read Console Until    [INFO] Step 3: Target wakeup count reached.

End Of Example
    [Documentation]    Verify successful completion and RTC deinit.
    Read Console Until    [INFO] Step 4: RTC deinitialization COMPLETED.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
