*** Comments ***
Copyright (c) 2025 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_spi_full_duplex_two_boards_com_polling_controller.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}
Test Teardown       Default Cleanup

Test Tags           ip:hal_usart


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to the scenario described
    ...    in the README.

    Init Usart Responder  MX_EXAMPLE_USART_SCLK_PIN  MX_EXAMPLE_USART_TX_PIN  MX_EXAMPLE_USART_RX_PIN  loop_mode=True

    # Autoshield will transmit the following messages as responder, in loop mode, when target as controller will trigger communication
    Write Usart As Target    ${MESSAGE_WHEN_READING}
    Write Usart As Target    ${MESSAGE_PING}
    Write Usart As Target    ${MESSAGE_WHEN_READING}
    Write Usart As Target    ${MESSAGE_PONG}

    Example Step 1
    FOR    ${i}    IN RANGE    5
        Check Usart Communications    ${MESSAGE_PING}
        Check Usart Communications    ${MESSAGE_PONG}
    END
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the USART instance and the NVIC.
    Comment    Nothing can be checked at this step for the responder.

Check Usart Communications
    [Documentation]    checks that sent and received buffers match.
    [Arguments]    ${expected_message}
    Check Usart As Target    ${expected_message}    timeout=1000ms
    Check Usart As Target    ${MESSAGE_WHEN_READING}    timeout=1000ms

End Of Example
    [Documentation]    Marks the end of the example scenario.
    ${active_level}=    Read Test Variable    MX_STATUS_LED_ACTIVE_STATE
    Check Pin    MX_STATUS_LED    ${active_level}
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=2s
