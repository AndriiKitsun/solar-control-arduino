# solar-control-arduino

## 1.1.0

### Minor Changes

- [#34](https://github.com/AndriiKitsun/solar-control-arduino/pull/34) [`0189d42`](https://github.com/AndriiKitsun/solar-control-arduino/commit/0189d42af3a10eaf4e7de9ab0b6a1d9db08ff202) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-79: Move calculation of T1/T2 energy from BasePzem class to AcPzem

- [#24](https://github.com/AndriiKitsun/solar-control-arduino/pull/24) [`8e581d9`](https://github.com/AndriiKitsun/solar-control-arduino/commit/8e581d9256e97c26c39288f72c7c473e8185e5a5) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-49: Add relay support

- [#33](https://github.com/AndriiKitsun/solar-control-arduino/pull/33) [`73c6f20`](https://github.com/AndriiKitsun/solar-control-arduino/commit/73c6f20c095434ba0b62ca1411518b9279196bc7) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - Rework RS485 transmitter logic using Modbus protocol

### Patch Changes

- [#35](https://github.com/AndriiKitsun/solar-control-arduino/pull/35) [`3182e32`](https://github.com/AndriiKitsun/solar-control-arduino/commit/3182e32ee09d80be7815ffecb71b32687d1ab2ea) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-80: Update project libraries

## 1.0.1

### Patch Changes

- [#31](https://github.com/AndriiKitsun/solar-control-arduino/pull/31) [`2e67c75`](https://github.com/AndriiKitsun/solar-control-arduino/commit/2e67c75853d216ca33b21017f2ff9791a5c1e177) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - Add publish command for release job

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
