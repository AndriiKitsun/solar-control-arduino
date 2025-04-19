# solar-control-arduino

## 1.5.0

### Minor Changes

- [#56](https://github.com/AndriiKitsun/solar-control-arduino/pull/56) [`37d678e`](https://github.com/AndriiKitsun/solar-control-arduino/commit/37d678e7d07fb6845e6b8d209233a66adc138fde) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-148: Calculate average frequency and use it in protection

## 1.4.1

### Patch Changes

- [#54](https://github.com/AndriiKitsun/solar-control-arduino/pull/54) [`0d1f525`](https://github.com/AndriiKitsun/solar-control-arduino/commit/0d1f525437f2e67e0af38678ab2dea5c84a08fc8) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-135: Protection rule is not saved when existing rule is null

## 1.4.0

### Minor Changes

- [#51](https://github.com/AndriiKitsun/solar-control-arduino/pull/51) [`0569753`](https://github.com/AndriiKitsun/solar-control-arduino/commit/0569753bb2386e804e4ec3cecb8dabada74b30fd) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-133: Add AC Output Avg Voltage protection rule

- [#52](https://github.com/AndriiKitsun/solar-control-arduino/pull/52) [`8d15a53`](https://github.com/AndriiKitsun/solar-control-arduino/commit/8d15a538ed0c842ad71871408f538e5a71db1b80) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-134: Manual power off should be independent of protection rules

- [#50](https://github.com/AndriiKitsun/solar-control-arduino/pull/50) [`90117e0`](https://github.com/AndriiKitsun/solar-control-arduino/commit/90117e010f3dd8853402f9e6737ee84d1d7ab0cd) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-122: Dc battery voltage protection rule should take into account average voltage

- [#49](https://github.com/AndriiKitsun/solar-control-arduino/pull/49) [`a0c60f9`](https://github.com/AndriiKitsun/solar-control-arduino/commit/a0c60f91d57b3a9b700565ed7c86c681f7352f83) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-132: Add power relay status to the sensors websocket response

- [#48](https://github.com/AndriiKitsun/solar-control-arduino/pull/48) [`4439594`](https://github.com/AndriiKitsun/solar-control-arduino/commit/4439594a6cb67a33c3b476bb247633ebe6817cee) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-124: Turn on power relay when protection is not triggered

## 1.3.0

### Minor Changes

- [#45](https://github.com/AndriiKitsun/solar-control-arduino/pull/45) [`0d19f21`](https://github.com/AndriiKitsun/solar-control-arduino/commit/0d19f2140fec13d238e8cbbfe5cb40ff3bdb1ea9) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-104: Implement protection rules

### Patch Changes

- [#46](https://github.com/AndriiKitsun/solar-control-arduino/pull/46) [`0d79abf`](https://github.com/AndriiKitsun/solar-control-arduino/commit/0d79abffc0c3237ff41a4d7d4957f893adfc2084) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-115: Do not rewrite protection rule when it is equal with passed DTO

- [#44](https://github.com/AndriiKitsun/solar-control-arduino/pull/44) [`a2c66fc`](https://github.com/AndriiKitsun/solar-control-arduino/commit/a2c66fc55a2dab1fb55e815e37428ebbb0dd98b8) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-108: Move divider constants to the env config

## 1.2.0

### Minor Changes

- [#38](https://github.com/AndriiKitsun/solar-control-arduino/pull/38) [`08c7dd3`](https://github.com/AndriiKitsun/solar-control-arduino/commit/08c7dd3f8966fa66b53499294f875af8c23eef32) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-89: Add voltage divider support

- [#37](https://github.com/AndriiKitsun/solar-control-arduino/pull/37) [`fa5257e`](https://github.com/AndriiKitsun/solar-control-arduino/commit/fa5257e8afcc7b51b471539abb0cae0a99b1ae6b) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-98: Add endpoint to return current relay state

- [#41](https://github.com/AndriiKitsun/solar-control-arduino/pull/41) [`7b524dc`](https://github.com/AndriiKitsun/solar-control-arduino/commit/7b524dc685979bf561154d79af1d52065cc387af) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-99: Calculate full energy instead of active

- [#39](https://github.com/AndriiKitsun/solar-control-arduino/pull/39) [`c70834e`](https://github.com/AndriiKitsun/solar-control-arduino/commit/c70834e9a9382fea910f7b018fabb8cd2816711d) Thanks [@AndriiKitsun](https://github.com/AndriiKitsun)! - SC-90: Delete DC PZEM logic

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
