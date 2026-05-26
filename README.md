<!--
  README.md
  SimpleMtkWlan

  Created by laobamac (王孝慈) on 2026/5/5.
  Copyright © 2026 laobamac (王孝慈). All rights reserved.
-->

# SimpleMtkWlan

SimpleMtkWlan is an experimental macOS kernel extension for MediaTek MT792x PCIe
wireless adapters.

Author: laobamac (王孝慈)

## Status

This project is under active hardware bring-up. Current development focuses on:

- MT7921/MT7922 PCIe probe and attach
- Firmware loading
- DMA queue setup
- Hardware scan support
- Association/authentication TX path debugging

The driver is not release-stable yet.

## Supported Hardware

Currently targeted device:

- MediaTek MT7921, PCI ID `14c3:7961`
- MediaTek MT7921K/RZ608, PCI ID `14c3:0608`
- MediaTek MT7922, PCI ID `14c3:7922`

Other MediaTek devices are not currently supported.

## Building

Requirements:

- macOS with Xcode
- Kernel SDK headers matching the local development setup
- The project-local firmware generation script and embedded firmware data

Build:

```sh
xcodebuild -project SimpleMtkWlan.xcodeproj -scheme SimpleMtkWlan -configuration Debug build
```

The debug kext will be produced by Xcode as `SimpleMtkWlan.kext`.

## Repository Layout

- `SimpleMtkWlan/` - kernel extension entry point and MediaTek HAL
- `SimpleMtkWlan/hal_mwx/` - MT7921/MT7922 driver implementation
- `mtk80211/` - compatibility, OpenBSD net80211, and support code
- `include/` - shared driver interfaces and generated firmware table
- `scripts/` - firmware generation helpers

## License

SimpleMtkWlan is distributed under GPLv2. See `LICENSE`.

Some source files include additional or more specific license notices inherited
from upstream GPL/BSD/OpenBSD/NetBSD/FreeBSD components. Those notices are
preserved in place and apply to their respective files.

## Credits

See `CREDITS.md`.
