*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_i3c_sensor_private_it_controller.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             Dialogs
Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_i3c   using:mmi


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to
    ...    the scenario described in the README.
    Comment    This manual test exists because robot framework currently does not support autoshield I3C services
    Test Setup
    Communication Between Boards
    End Of Example


*** Keywords ***
Test Setup
    [Documentation]
    Comment
    ...
    ...
    Comment
    ...
    ...
    Pause Execution

Communication Between Boards
    [Documentation]
    Comment
    ...
    ...
    Pause Execution

End Of Example
    [Documentation]
    Comment
