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
#include "mbed_config.h"
#include "UbloxATCellularInterfaceExt.h"

UbloxATCellularInterfaceExt *interface = new UbloxATCellularInterfaceExt(MDMTXD, MDMRXD, MBED_CONF_UBLOX_CELL_BAUD_RATE, true);

int main()
{
  printf("\n\r\n\rSpiio VC2.5. Reading the Antenna RSSI \n\r");

  printf("Initializing the modem\r\n");
  if (interface->init(MBED_CONF_APP_DEFAULT_PIN))
  {

    bool connected = false;

    interface->set_credentials(MBED_CONF_APP_APN, MBED_CONF_APP_USERNAME, MBED_CONF_APP_PASSWORD);
    for (int x = 0; x < 10; x++)
    {
      int connect = interface->connect();
      if (connect == 0)
      {
        connected = true;
        break;
      }
    }

    if (!connected)
    {
      printf("Connecting to the packet network failed...");
      exit;
    }

    printf("...DONE\r\nThe modem powered up\r\n");
    char rssi[15 + 1];
    printf("Reading RSSI\r\n");

    while (true)
    {
      printf("RSSI: %i\r\n", interface->rssi());
      printf("...wait\r\n");
      wait(20);
    }
  }
  else
  {
    printf("Unable To intialize modem\r\n");
  }
}
