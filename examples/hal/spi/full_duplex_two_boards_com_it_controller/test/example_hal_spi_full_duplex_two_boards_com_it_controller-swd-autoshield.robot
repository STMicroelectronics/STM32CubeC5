*** Comments ***
Copyright (c) 2026 STMicroelectronics.
All rights reserved.

This software is licensed under terms that can be found in the LICENSE file
in the root directory of this software component.
If no LICENSE file comes with this software, it is provided AS-IS.

Documentation on how to run    test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Running_Robot_tests
Documentation on how to write test suites: https://codex.cro.st.com/plugins/mediawiki/wiki/stm32cube/index.php?title=Developer_Journey:_Writing_a_Robot_test


*** Settings ***
Documentation       Automatic test for example_hal_spi_full_duplex_two_boards_com_it_controller.
...                 This test suite is verifying that the application binary runs without error,
...                 according to the README documentation.

Library             robot_for_stm32
Library             robot_for_stm32.STM32CubeProgrammer
Library             robot_for_stm32.Autoshield
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_spi


*** Test Cases ***
${TEST_ID} Scenario adherence
    [Documentation]    This test checks that the application binary runs according to the scenario described
    ...    in the README.

    ${baudrate}=    Read Test Variable    SPI_SCK_FREQ
    Init Spi Responder
    ...    ${baudrate}
    ...    MX_EXAMPLE_SPI_SCK_PIN
    ...    MX_EXAMPLE_SPI_MOSI_PIN
    ...    MX_EXAMPLE_SPI_MISO_PIN
    ...    loop_mode=True

    # Autoshield will transmit the following messages as responder, in loop mode, when target as controller will trigger communication
    Write Spi As Target    ${MESSAGE_PING}
    Write Spi As Target    ${MESSAGE_PONG}

    Reset Target
    # Example Step 1 too quick to be tested now -> moved in "Check Init Status" test

    # Autoshield will automatically answer the previous messages
    # Wait for end of example while checking ProcessLoops
    Check Integer Variable    ProcessLoops    0    timeout=3s

    # Example process with Autoshield - All checks will be done at the end of scenario
    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    configures and initializes the SPI instance.
    ...    Registers the user callbacks for SPI events: TX/RX transfer completed and transfer error.
    # Check Integer Variable    ExecStatus    ${EXEC_STATUS_INIT_OK}

End Of Example
    [Documentation]    checks that the example end status is ok.
    Check Spi Communications
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=1s

Check Spi Communications
    [Documentation]    checks that sent and received buffers match.
    FOR    ${i}    IN RANGE    5
        Check Spi As Target    ${MESSAGE_PING}    timeout=100ms
        Check Spi As Target    ${MESSAGE_PONG}    timeout=100ms
    END
