# BotPlay4U

BotPlay4U is an iOS-oriented Geode project based on an existing Geometry Dash timing-sequence mod. The source supplied for this adaptation identified its upstream project as [whxkk1/Autoclicker](https://github.com/whxkk1/Autoclicker).

## Current controls

- **Draggable HUD**: drag the small control on the gameplay screen to reposition it. Tap **BOT ON/OFF** to toggle the configured sequence, or tap **SET** to open the settings panel.
- **Bot**: replays a configured hold/release timing sequence. It is not yet a path-solving bot and does not automatically learn arbitrary levels.
- **Safe Mode**: enabled by default. During a Bot run it masks the reported progress to 0 and blocks standard-level completion near 99%, then pauses the level with a visible barrier. Turn Safe Mode off to allow normal completion.
- **Hitboxes**: toggles Geometry Dash debug hitbox drawing.
- **Timing editor**: configure the hold ticks, release ticks, and repeat count for each input, then press **Save**.

## Build

The GitHub Actions workflow at .github/workflows/ios-build.yml builds the iOS Geode package using Geode SDK 5.11.2. The current package metadata targets Geometry Dash 2.2081 on iOS.

The iOS build completed successfully in GitHub Actions for commit b0e7b9c. Download the BotPlay4U-iOS artifact from that workflow run. A successful build confirms compilation and packaging only; installation and gameplay behavior still require testing on a compatible iPhone and Geode installation. Safe Mode behavior on platformer levels and levels with unusual end triggers also remains unverified.

This source does not yet implement the full adaptive learning, memory, replay, prediction, and menu feature specification.
