# solar-control-arduino

## 1.0.0

### Major Changes

- [#29](https://github.com/AndriiKitsun/solar-control-arduino/pull/29) [`c663fad`](https://github.com/AndriiKitsun/solar-control-arduino/commit/c663fad4febc838b5315c5ea0301509d7c382892) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - ## What's Changed

  - SC-1: WiFi integration, HTTP Server by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/1
  - SC-2: Split code to separate files by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/3
  - SC-2: PZEM-004T-V3 sensor integration by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/2
  - SC-3: Network Time Protocol (NTP) integration by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/4
  - SC-4: Implement POST request with PZEM data every n interval time by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/5
  - SC-5: Add endpoint to reset PZEM energy counter by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/6
  - SC-25: Calculate T1, T2 Energy values by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/7
  - SC-9: Multiple AC PZEM sensors support by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/8
  - SC-31, SC-6: Migrate from Arduino IDE to PlatformIO by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/10
  - SC-22: Implement WebSocket for broadcasting PZEM values by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/11
  - SC-33: Send single createdAt field shared between PZEMs by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/12
  - SC-36: PZEM values are not cleared when PZEM is disconnected by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/13
  - SC-27: PZEM-017 DC sensor integration by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/14
  - SC-32: The createdAtGmt field was sent with the current timezone instead of UTC in ISO format by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/15
  - SC-19: Backup Zone data when the WiFi connection dropped by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/17
  - SC-37: Multiply PZEM-017 sensors on one bus by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/18
  - SC-43: Add WiFi details to the status endpoint by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/19
  - SC-13: Add OpenAPI specification for the created API by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/20
  - SC-24: Merge PZEM models into one model by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/21
  - SC-48: Delete DC Sun PZEM logic by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/22
  - SC-60: Rename PZEM `id` field to the `name` for consistency by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/23
  - SC-63: Slow ESP IP resolution during HTTP requests by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/25
  - Revert "SC-63: Slow ESP IP resolution during HTTP requests" by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/26
  - SC-74: Semver release by @AndriiKitsun in https://github.com/AndriiKitsun/solar-control-arduino/pull/27

  ## New Contributors

  - @AndriiKitsun made their first contribution in https://github.com/AndriiKitsun/solar-control-arduino/pull/1
