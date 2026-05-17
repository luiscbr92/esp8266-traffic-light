# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## What this is

ESP8266 firmware for a physical "focus light" — a single NeoPixel LED that signals work status (red = focus, yellow = wrapping up, green = available, off). The device exposes a web UI and HTTP API over WiFi so it can be controlled from any browser on the local network.

Target hardware: SparkFun ESP8266 Thing Dev (`thingdev` board in PlatformIO).

## Build & flash

```bash
pio run                   # compile only
pio run -t upload         # compile and flash over USB
pio device monitor        # open serial monitor at 115200 baud
pio run -t clean          # clean build artifacts
```

## Development environment

The repo ships a Docker image with PlatformIO and Claude Code pre-installed, intended for USB-less / host-agnostic development sessions.

```bash
# Copy and edit .env to set WORKSPACE_PATH to your local checkout
cp .env.example .env

docker compose run --rm claude   # drops into bash inside the container
```

## WiFi credentials

Credentials are injected as build flags — never hardcoded. `include/secrets.h` provides fallback placeholder values.

Real credentials go in `platformio_local.ini` (gitignored), which overrides `platformio.ini`:

```ini
[env:esp_thing_dev]
build_flags =
    -D WIFI_SSID='"YourRealNetwork"'
    -D WIFI_PASS='"YourRealPassword"'
```

## Architecture

`main.cpp` calls `setup()` once and `loop()` continuously, following Arduino conventions.

**Module overview:**

| Module | Files | Responsibility |
|---|---|---|
| `light` | `src/light.cpp`, `include/light.h` | NeoPixel LED on pin 4. `LightState` enum: OFF/RED/YELLOW/GREEN |
| `pomodoro` | `src/pomodoro.cpp`, `include/pomodoro.h` | Timer state machine: IDLE→FOCUS(20m)→WRAP_UP(5m)→BREAK(5m)→IDLE. Calls `light_set()` on each transition |
| `web_server` | `src/web_server.cpp`, `include/web_server.h` | `ESP8266WebServer` on port 80. Serves embedded HTML UI and REST endpoints |
| `wifi_setup` | `src/wifi_setup.cpp`, `include/wifi_setup.h` | Connects to WiFi; max 30 retries. Returns `false` if connection fails (web server is skipped) |
| `onboard_led` | `src/onboard_led.cpp`, `include/onboard_led.h` | LED on pin 5 (active LOW). Blinks while connecting, solid on when connected |

**HTTP API:**

| Method | Path | Effect |
|---|---|---|
| GET | `/` | HTML control UI |
| GET | `/status` | JSON: `{state, pomodoro_running, phase, remaining_seconds}` |
| POST | `/state/{red\|yellow\|green\|off}` | Set light directly; stops any running pomodoro |
| POST | `/pomodoro/start` | Starts pomodoro from FOCUS phase |
| POST | `/pomodoro/stop` | Stops pomodoro, sets light OFF |

**Important:** `pomodoro_loop()` must be called from `main.cpp`'s `loop()` for automatic phase transitions to work — verify this is wired up when modifying either file.
