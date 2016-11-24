![](http://static.creatordev.io/logo.png)
# Ci40 Info Daemon

Visit us at [forum.creatordev.io](http://forum.creatordev.io) for support and discussion
---
Info Daemon is a helper project which complements [Awa client daemon](https://github.com/FlowM2M/AwaLWM2M). The main purpose of having this process running is to fill IPSO Object Id 3 with data. Most of the resources in the specification are handled, however due to platform limitations or project requirements, some adjustments may be needed.


## Prerequisites

The only needed requirements is to have up and running Awa Client Daemon which will be filled with data by Info Daemon.

## How to install

Easiest way of installing the **Info Daemon** is using opkg package manager.

    opkg install info-daemon

Alternatively you can compile it yourself.

## Resources handled by daemon.

The following resources in IPSO object 3 are managed:

| Resource Id | Resource Name| Value |
|:-----------:|:------------:|:-----:|
| 0  | MANUFACTURER | "Imagination" |
| 1  | MODEL_NUMBER | "Ci40" |
| 2  | SERIAL_NUMBER | _MAC address is used_ |
| 3  | FIRMWARE_VERSION | Not supported |
| 4  | REBOOT | **Executes system "reboot" command** |
| 5  | FACTORY_RESET | **Same as REBOOT** |
| 6  | AVAILABLE_POWER_SOURCES | Not supported |
| 7  | POWER_SOURCE_VOLTAGE | Not supported |
| 8  | POWER_SOURCE_CURRENT | Not supported |
| 9  | BATTERY_LEVEL |Not supported |
| 10 | MEMORY_FREE | _Returns disk free space on file system_ |
| 11 | ERROR_CODE | _Always return 0_ |
| 12 | RESET_ERROR_CODE | Not supported. |
| 13 | CURRENT_TIME | _Seconds from Unix Epoch_ **Sets new time on write** |
| 14 | UTCOFFSET | _Returns current UTC offset_ **Write is not supported.** |
| 15 | TIMEZONE | _Returns current time zone._ **Write is not supported.** |
| 16 | SUPPORTED_BINGING_AND_MODES| "U" |
| 17 | DEVICE_TYPE | "Creator Ci40" |
| 18 | HARDWARE_VERSION | Not supported. |
| 19 | SOFTWARE_VERSION | _Version of Creator Image_ |

## Contributing

If you have a contribution to make please follow the processes laid out in [contributor guide](CONTRIBUTING.md).
