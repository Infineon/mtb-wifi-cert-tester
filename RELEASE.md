# Wi-Fi Cert Tester tool for ModusToolbox SDK

## What's Included?
See the [README.md](./README.md) for a complete description of the WiFi Cert Tester tool using ModusToolbox SDK.

## Changelog

### v4.1.0
* Enlarge CAPI command length up to 1024 for supporting WPA3 192bit
* Added support for CYW943907AEVAL1F kit

### v4.0.0
* Add dependency on wifi-core-freertos-lwip-mbedtls to pull wifi-connection-manager release-v3.0.0

### v3.4.0
* Added support for WPA3 External Supplicant for CY8CKIT-062S2-43012

### v3.3.0
* Added support for CY8CEVAL-062S2-MUR-43439M2
* WPA3-R3 support for CY8CEVAL-062S2-LAI-4373M2 kit and CY8CEVAL-062S2-MUR-43439M2

### v3.2.0
* Added support for CY8CEVAL-062S2-LAI-4373M2 kit
* Update Copyright sections and add Infineon logo
* Throughput improvements

### v3.1.0
* Added support for WPA2 Enterprise security with following protocols
	* EAP-TLS
	* EAP-TTLS
	* PEAP0

### v3.0.0
* Initial release
* 11n PSK Certification Passed

### Supported Software and Tools
This version of the library was validated for compatibility with the following Software and Tools:

| Software and Tools                                      | Version |
| :---                                                    | :----:  |
| ModusToolbox Software Environment                       | 3.0.0   |
| - ModusToolbox Device Configurator                      | 3.10.0  |
| PSoC 6 Peripheral Driver Library (PDL)                  | 2.4.0   |
| GCC Compiler                                            | 10.3.1  |
| IAR Compiler                                            | 8.42    |
| ARMC6 Compiler                                          | 6.14    |