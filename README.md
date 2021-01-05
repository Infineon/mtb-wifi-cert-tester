# WiFi Cert Tester tool for AnyCloud SDK
The WiFi cert tester tool is used for WiFi Certification of 11n, PMF, WPA3 and 11AC. The WiFi cert tester tool uses the command console asset to initialize and invokes wifi-cert middleware init function.

The WiFi cert tester tool waits for commands from UCC test agent on serial (UART interface) and executes the UCC Test commands and responds to the UCC test agent by sending response in serial console.

The Wifi Certification using WiFi cert tester tool needs knowledge of the WiFi Alliance Test beds, associated S/W tools and  H/W.

The instructions are as per certification requirements of Wifi Alliance orginization.
Please refer to https://www.wi-fi.org/download.php?file=/sites/default/files/private/Certification_Overview_v5.2_0.pdf


## Requirements

- [ModusToolbox™ IDE](https://www.cypress.com/products/modustoolbox-software-environment) v2.2
- Programming Language: C
- Associated Parts: All [PSoC 6 MCU](http://www.cypress.com/PSoC6) parts

## Dependent assets
- [Wifi Connection Manager](https://github.com/cypresssemiconductorco/wifi-connection-manager)
- [Command Console](https://github.com/cypresssemiconductorco/command-console)
- [Wifi Cert Library](https://cypresssemiconductorco.github.io/wifi-cert)

## Supported Kits

- [PSoC 6 Wi-Fi BT Prototyping Kit](https://www.cypress.com/CY8CPROTO-062-4343W) (CY8CPROTO-062-4343W) 
- [PSoC 62S2 Wi-Fi BT Pioneer Kit](https://www.cypress.com/CY8CKIT-062S2-43012) (CY8CKIT-062S2-43012)
- [PSoC 6 Wi-Fi BT Pioneer Kit](https://www.cypress.com/CY8CKIT-062-WiFi-BT) (CY8CKIT-062-WIFI-BT)
- [PSoc 62S1 Wi-Fi BT Pioneer Kit](https://www.cypress.com/part/cyw9p62s1-43012evb-01)(CYW9P62S1-43012EVB-01)
- [PSoC 62S1 Wi-Fi BT Pioneer Kit](https://www.cypress.com/file/298076/download) (CYW9P62S1-43438EVB-01)

## Test Setup

### Hardware Setup
#### Device Setup
This example uses the board's default configuration. See the kit user guide to ensure that the board is configured correctly

### WiFi Cert Test bed Setup
The simplified WiFi Cert test bed setup is the shown in the image below

1. TEST AP  is the AP which is configured as per the test pre-requisites
2. DUT Control agent is the Linux Laptop connected the PSoC6 Kit (DUT) ( see Supported Kits)
3. UCC linux laptop triggers the test to be run and sends the test commands to DUT(Device Under Test) Control agent laptop, the DUT control agent laptop relays the command to the PSoC6 kit.
4. PSoC6 kit is the DUT ( Device Under Test) running WiFi cert tester tool

![Simplied WiFi Cert Test bed](images/simplified-wifi-cert-test-bed.png)


### Software Setup

This example requires Tera-term or putty or MiniCom terminal to observe the output, the wifi cert test bed uses the python scripts take control of the tera-term or putty or minicom.


## Using the wifi cert tester tool


### In Command-line Interface (CLI):

#### Code checkout and build 
On Linux and macOS, you can use any terminal application. On Windows, open the modus-shell app from the Start menu.

``` 
- `git clone https://github.com/cypresssemiconductorco/mtb-anycloud-wifi-cert-tester`
- `cd mtb-anycloud-wifi-cert-tester`
- `make getlibs`
- `make program TARGET=CY8CKIT-062S2-43012 TOOLCHAIN=GCC_ARM`
```

### WiFi Cert Test Operation

1. copy the `../mtb_shared/wifi-cert/latest-v3.X/scripts` folder into DUT control agent Laptop.
2. set the correct vendor and product in `init_dut_ca.sh`
```
vi ./scripts/init_dut_ca.sh
# update the line below with correct vendor and product id for the Cypress PSoC6 Kit
# which can be retrieved by typing dmesg once the kit is connected to PC via USB cable.
sudo modprobe ftdi_sio vendor=0xa5c product=0x43fa
```
3. Save and close the file

4. Run the shell script and make sure the FTDI driver is displayed with tty/ACM*
```
`./scripts/init_dut_ca.sh`
```
5. Run minicom -s  and select serial port setup,

6. set the serial port to /dev/ttyACM0

7. set the baud rate appropriately 115200

8. Turn off Hardware Flow Control.

9. Exit the Minicom

10.  Run the script
```
`./dut_ca.py -t /dev/ttyACM$x` ( where x can 0 to ..n)
```
10. Now the Kit is ready for commands from UCC test agent

11. The UCC laptop runs the test as batch command (i.e seris of tests) or individual tests

12. The Test script configures Test AP per the test setup pre-requisties and after that actual test runs

13. The Test results are collected in the UCC Laptop under `/logs` folder for the particular test.

14. List of wifi cert test tool commands can be retrieved by typing list
#example 
		> list
			0. sta_get_ip_config

			1. sta_set_ip_config

			2. sta_get_info

			3. sta_get_mac_address

			4. sta_is_connected

			5. sta_verify_ip_connection

			6. sta_get_bssid

			7. ca_get_version

			8. device_get_info

			9. device_list_interfaces

			10. sta_set_encryption

			11. sta_set_psk

			12. sta_set_security

			13. sta_associate

			14. sta_preset_testparameters

			15. traffic_send_ping

			16. traffic_stop_ping

			17. traffic_agent_config

			18. traffic_agent_reset

			19. traffic_agent_send

			20. traffic_agent_receive_start

			21. traffic_agent_receive_stop

			22. sta_set_11n

			23. sta_disconnect

			24. sta_scan

			25. sta_reassoc

			26. sta_reset_default

			27. sta_set_wireless

			28. whdlog

			29. wicedlog

			30. reboot


All other trademarks or registered trademarks referenced herein are the property of their respective owners.

![Banner](images/Banner.png)

-------------------------------------------------------------------------------

© Cypress Semiconductor Corporation, 2019. This document is the property of Cypress Semiconductor Corporation and its subsidiaries ("Cypress"). This document, including any software or firmware included or referenced in this document ("Software"), is owned by Cypress under the intellectual property laws and treaties of the United States and other countries worldwide. Cypress reserves all rights under such laws and treaties and does not, except as specifically stated in this paragraph, grant any license under its patents, copyrights, trademarks, or other intellectual property rights. If the Software is not accompanied by a license agreement and you do not otherwise have a written agreement with Cypress governing the use of the Software, then Cypress hereby grants you a personal, non-exclusive, nontransferable license (without the right to sublicense) (1) under its copyright rights in the Software (a) for Software provided in source code form, to modify and reproduce the Software solely for use with Cypress hardware products, only internally within your organization, and (b) to distribute the Software in binary code form externally to end users (either directly or indirectly through resellers and distributors), solely for use on Cypress hardware product units, and (2) under those claims of Cypress's patents that are infringed by the Software (as provided by Cypress, unmodified) to make, use, distribute, and import the Software solely for use with Cypress hardware products. Any other use, reproduction, modification, translation, or compilation of the Software is prohibited.  
TO THE EXTENT PERMITTED BY APPLICABLE LAW, CYPRESS MAKES NO WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, WITH REGARD TO THIS DOCUMENT OR ANY SOFTWARE OR ACCOMPANYING HARDWARE, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. No computing device can be absolutely secure. Therefore, despite security measures implemented in Cypress hardware or software products, Cypress shall have no liability arising out of any security breach, such as unauthorized access to or use of a Cypress product. CYPRESS DOES NOT REPRESENT, WARRANT, OR GUARANTEE THAT CYPRESS PRODUCTS, OR SYSTEMS CREATED USING CYPRESS PRODUCTS, WILL BE FREE FROM CORRUPTION, ATTACK, VIRUSES, INTERFERENCE, HACKING, DATA LOSS OR THEFT, OR OTHER SECURITY INTRUSION (collectively, "Security Breach"). Cypress disclaims any liability relating to any Security Breach, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any Security Breach. In addition, the products described in these materials may contain design defects or errors known as errata which may cause the product to deviate from published specifications. To the extent permitted by applicable law, Cypress reserves the right to make changes to this document without further notice. Cypress does not assume any liability arising out of the application or use of any product or circuit described in this document. Any information provided in this document, including any sample design information or programming code, is provided only for reference purposes. It is the responsibility of the user of this document to properly design, program, and test the functionality and safety of any application made of this information and any resulting product. "High-Risk Device" means any device or system whose failure could cause personal injury, death, or property damage. Examples of High-Risk Devices are weapons, nuclear installations, surgical implants, and other medical devices. "Critical Component" means any component of a High-Risk Device whose failure to perform can be reasonably expected to cause, directly or indirectly, the failure of the High-Risk Device, or to affect its safety or effectiveness. Cypress is not liable, in whole or in part, and you shall and hereby do release Cypress from any claim, damage, or other liability arising from any use of a Cypress product as a Critical Component in a High-Risk Device. You shall indemnify and hold Cypress, its directors, officers, employees, agents, affiliates, distributors, and assigns harmless from and against all claims, costs, damages, and expenses, arising out of any claim, including claims for product liability, personal injury or death, or property damage arising from any use of a Cypress product as a Critical Component in a High-Risk Device. Cypress products are not intended or authorized for use as a Critical Component in any High-Risk Device except to the limited extent that (i) Cypress's published data sheet for the product explicitly states Cypress has qualified the product for use in a specific High-Risk Device, or (ii) Cypress has given you advance written authorization to use the product as a Critical Component in the specific High-Risk Device and you have signed a separate indemnification agreement.  
Cypress, the Cypress logo, Spansion, the Spansion logo, and combinations thereof, WICED, PSoC, CapSense, EZ-USB, F-RAM, and Traveo are trademarks or registered trademarks of Cypress in the United States and other countries. For a more complete list of Cypress trademarks, visit cypress.com. Other names and brands may be claimed as property of their respective owners.
