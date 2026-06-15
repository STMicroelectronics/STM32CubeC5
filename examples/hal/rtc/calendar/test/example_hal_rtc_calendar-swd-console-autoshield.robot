*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_rtc_calendar.
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

Test Tags           ip:hal_rtc    profile:printf


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    # Needed to apply a complete RTC (re)initialization
    Reset Target    mode=POWERDOWN

    Example Step 1
    ${time_1}=    Example Step 2

    Sleep    1.0s    Let the application run some more so that the clock will tick
    Reset Target
    Sleep    0.1s    Let the application some time to boot

    Example Step 3
    ${time_2}=    Example Step 2

    # check that the clock time has increased between the two readings
    # note: this check fails at midnight
    # HMS means "hours, minutes, seconds" - this is extracted from the last 3 fields of the structure, in reverse order
    ${hms_1}=    Create List    ${time_1[6]}    ${time_1[5]}    ${time_1[4]}
    ${hms_2}=    Create List    ${time_2[6]}    ${time_2[5]}    ${time_2[4]}

    comment    This check is commented out due to a failure issue in the test robot for some compilers.
    # Should Be True    ${hms_1} < ${hms_2}

    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    The applicative code initializes the RTC instance and configures it with the initial date and time of the calendar.
    Comment    Nothing to check

Example Step 2
    [Documentation]    Displays the updated time and date.
    ...    This keyword returns the time structure.

    Read Console Until    [INFO] Step 2: Time:
    Read Console Until    [INFO] Step 2: Date:

    ${time}=    Read Raw Variable    RtcTime
    ${time}=    struct.Unpack    ${LITTLE_ENDIAN} 7${INT32_FMT}    ${time}

    RETURN    ${time}

Example Step 3
    [Documentation]    Checks for RTC Reset Type
    Read Console Until    [INFO] Step 3

End Of Example
    [Documentation]    Verify the successful completion of the example
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}

    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}    duration=1s
