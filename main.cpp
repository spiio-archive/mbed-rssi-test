/* mbed Microcontroller Library
 * Copyright (c) 2017 u-blox
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "mbed.h"
#include "ATCmdParser.h"
#include "FileHandle.h"
#include "onboard_modem_api.h"

/* Definitions */
#define OUTPUT_ENTER_KEY "\r"
#define AT_PARSER_BUFFER_SIZE 256
#define AT_PARSER_TIMEOUT 8 * 1000 // Milliseconds


/* File handler */
FileHandle *fh;

/* AT Command  Parser handle */
ATCmdParser *at;

// Setup the modem
bool setup_modem()
{

  bool success = false;

  /* Initialize GPIO lines */
  onboard_modem_init();

  /* Give modem a little time to settle down */
  wait_ms(250);

  for (int retry_count = 0; !success && (retry_count < 20); retry_count++)
  {
    printf("...wait\r\n");
    onboard_modem_power_up();
    wait_ms(500);
    // The modem tends to sends out some garbage during power up. Needs to clean up.
    at->flush();

    // Set AT parser timeout to 1sec for AT OK check
    at->set_timeout(1000);

    // AT OK talk to the modem
    if (at->send("AT"))
    {
      wait_ms(100);
      if (at->recv("OK"))
      {
        success = true;
      }
    }
    // Increase the parser time to 8sec
    at->set_timeout(8000);
  }
  if (success)
  {
    // Set the final baud rate
    if (at->send("AT+IPR=%d", 115200) && at->recv("OK"))
    {
      // Need to wait for things to be sorted out on the modem side
      wait_ms(100);
      ((UARTSerial *)fh)->set_baud(115200);
    }

    // Turn off modem echoing and turn on verbose responses
    success = at->send("ATE0;+CMEE=2") && at->recv("OK") &&
              // The following commands are best sent separately

              // Turn off RTC/CTS handshaking
              at->send("AT&K0") && at->recv("OK") &&
              // Set DCD circuit(109), changes in accordance with
              // the carrier detect status
              at->send("AT&C1") && at->recv("OK") &&
              // Set DTR circuit, we ignore the state change of DTR
              at->send("AT&D0") && at->recv("OK");
  }
  return success;
}
/*
** Reading the modem IMEI and eUICC/SIM IMSI and RSSI
** 
*/

int main()
{
  printf("\n\r\n\rSpiio Sensor VC2.5 Reading the modem IMEI and eUICC/SIM IMSI\n\r");
  printf("Initialising UART for modem communication");
  fh = new UARTSerial(MDMTXD, MDMRXD, 115200);
  printf("...DONE\r\n");

  printf("Initialising the Modem AT Command Parser");
  at = new ATCmdParser(fh, OUTPUT_ENTER_KEY, AT_PARSER_BUFFER_SIZE,
                       AT_PARSER_TIMEOUT, false);
  printf("...DONE\r\n");

  printf("Initializing the modem\r\n");
  if (setup_modem())
  {
    printf("...DONE\r\nThe modem powered up\r\n");
    char imei[15 + 1];
    char imsi[15 + 1];
    char rssi[15 + 1];
    printf("Reading IMEI and IMSI\r\n");
    if (at->send("AT+CGSN") && at->recv("%15[^\n]\nOK\n", imei))
    {
      printf("IMEI: %s\r\n", imei);
    }
    if (at->send("AT+CIMI") && at->recv("%15[^\n]\nOK\n", imsi))
    {
      printf("IMSI: %s\r\n", imsi);
    }
    printf("Reading RSSI\r\n");

    while (true)
    {
      if (at->send("AT+CSQ") && at->recv("+CSQ: %6[^\n]\nOK\n", rssi))
      {
        printf("RSSI: %s\r\n", rssi);
      }
      printf("...wait\r\n");
      wait(20);
    }
  }
  else
  {
    printf("Unable To intialize modem\r\n");
  }

  printf("FINISHED...\n\r");
}
