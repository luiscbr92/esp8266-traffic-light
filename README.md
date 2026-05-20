# Focus Light

**A physical work-status indicator for your desk.**

Focus Light is ESP8266 firmware that drives a single NeoPixel LED to signal your current work state — red for deep focus, yellow for wrapping up, green for available, off when idle. It runs a small web server so you can control it from any browser on your local network. The project is under active development.

---

## Features

- **Four light states** — off, red (focus), yellow (wrap-up), green (available)
- **Pomodoro timer** — automatic 20 min focus → 5 min wrap-up → 5 min break cycle
- **Web UI** — responsive dark-themed control panel served directly from the device
- **REST API** — set state or control the timer from any HTTP client
- **Host unit tests** — pure C++ tests run on your machine without hardware
- **Docker dev environment** — no local PlatformIO install required

---

## Hardware

| Component | Detail |
|-----------|--------|
| Microcontroller | SparkFun ESP8266 Thing Dev |
| LED | Single WS2812B NeoPixel on pin 4 |

---

## Getting Started

### Prerequisites

- [PlatformIO CLI or IDE extension](https://platformio.org/), **or** Docker + Docker Compose

### WiFi credentials

Credentials are injected as build flags and never hardcoded. Create a `platformio_local.ini` file (already gitignored) that overrides the defaults:

```ini
[env:esp_thing_dev]
build_flags =
    -D WIFI_SSID='"YourNetwork"'
    -D WIFI_PASS='"YourPassword"'
```

### Build & flash

```bash
pio run                 # compile only
pio run -t upload       # compile and flash over USB
pio device monitor      # open serial monitor at 115200 baud
pio run -t clean        # clean build artifacts
```

### Docker alternative

If you prefer not to install PlatformIO locally:

```bash
cp .env.example .env    # set WORKSPACE_PATH to your local checkout
docker compose run --rm claude
```

This drops you into a bash shell inside the container with PlatformIO and Claude Code pre-installed.

---

## Usage

Once the device boots and connects to WiFi, it prints its IP address to the serial monitor (ideally you create a DHCP reservation in your local network). Open that address in a browser to reach the control panel, where you can start or stop the pomodoro timer and set the light state manually.

The device also exposes a REST API for scripting or integration with other tools.

---

## Running Tests

Unit tests run on the host (no hardware needed):

```bash
pio test -e native
```

---

## License

MIT — see [LICENSE](LICENSE).
