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
Library             robot_for_stm32.Console
Variables           robot_for_stm32.cube_examples_constants
Resource            example.resource

Test Setup          Default Setup    ${TEST_CONFIG_FILE}    do_reset=False
Test Teardown       Default Cleanup

Test Tags           ip:hal_spi    profile:printf


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
    ...    nss_pin=MX_EXAMPLE_SPI_NSS_PIN
    ...    loop_mode=True
    ...    cpol=1
    ...    cpha=1

    # Autoshield will transmit the sensor value, in loop mode, when target as controller will trigger communication
    Write Spi As Target    ${SENSOR_VALUE_HEXA}

    Reset Target

    Example Step 1

    # Autoshield will automatically answer the previous messages, need to check Target request Analog measurement
    FOR     ${i}     IN RANGE    5
        Example Step 2
        Example Step 3
    END

    End Of Example


*** Keywords ***
Example Step 1
    [Documentation]    checks that the example end status is ok
    Read Console Until   [INFO] Step 1: Device initialization COMPLETED.

Example Step 2
    [Documentation]    checks the target request
    Check Spi As Target    ${SENSOR_REQUEST}    timeout=100ms

Example Step 3
    [Documentation]    checks the input voltage conversion
    Read Console Until    [INFO] Controller - Transfer COMPLETED. Input voltage is ${SENSOR_VALUE_MV} mV
    Check Integer Variable    AdcValueMv    ${SENSOR_VALUE_MV}    timeout=1s

End Of Example
    [Documentation]    checks that the example end status is ok.
    Check Integer Variable    ExecStatus    ${EXEC_STATUS_OK}    timeout=1s
