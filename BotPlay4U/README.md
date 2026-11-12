# BotPlay4U

BotPlay4U is an iOS-oriented Geode project based on an existing Geometry Dash timing-sequence mod. The source supplied for this adaptation identified its upstream project as [whxkk1/Autoclicker](https://github.com/whxkk1/Autoclicker).

## Current controls
- **Draggable HUD**: drag the small control on the gameplay screen to reposition it. Tap **BOT ON/OFF** to toggle the configured sequence, or tap **SET** to open the settings panel.
- **Bot**: replays a configured hold/release timing sequence. It is not yet a path-solving bot and does not automatically learn arbitrary levels.
- **Safe Mode**: enabled by default. During a Bot run it masks the reported progress to 0 and blocks standard-level completion near 99%, then pauses the level with a visible barrier. Turn Safe Mode off to allow normal completion.
- **Hitboxes**: toggles Geometry Dash debug hitbox drawing.
- **Timing editor**: configure the hold ticks, release ticks, and repeat count for each input, then press **Save**.

## Build
The GitHub Actions workflow in `.github/workflows/multi-platform.yml` builds iOS and other supported targets using the Geode build action.

**Build status:** this source has not been compiled in this environment because the Geode SDK/Xcode toolchain is unavailable here. Run the workflow on GitHub and check its logs before installing the resulting `.geode` file. In-game behavior, especially Safe Mode on platformer levels and end-trigger-heavy levels, still needs device testing.
