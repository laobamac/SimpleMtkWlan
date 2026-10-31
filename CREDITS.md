<!--
  CREDITS.md
  SimpleMtkWlan

  Created by laobamac (王孝慈) on 2026/5/17.
  Copyright © 2026 laobamac (王孝慈). All rights reserved.
-->

# Credits

SimpleMtkWlan is maintained by laobamac (王孝慈).

This repository contains code from several upstream projects and systems. Their
copyright notices and license headers are intentionally preserved in source
files.

## Major Sources

- Original macOS WLAN driver framework and compatibility layer by zxystd
  (钟先耀), released under the GNU General Public License terms present in the
  source files.
- OpenBSD `net80211`, PCI, crypto, and driver support code, including work by
  OpenBSD, NetBSD, FreeBSD, and individual contributors credited in the source
  headers.
- MediaTek MT792x/OpenBSD `if_mwx` related driver code and definitions,
  including credits retained in `SimpleMtkWlan/hal_mwx/MtkMwx.cpp` and
  `SimpleMtkWlan/hal_mwx/if_mwxreg.h`.
- Firmware blobs and firmware packaging support for MediaTek MT7921 and MT7922.

## Current Project Work

- Project rename, packaging, and MT7921/MT7922-focused maintenance by laobamac
  (王孝慈).
- MediaTek MT7921/MT7922 attach, firmware load, DMA, scan, and TX/authentication path
  development continues in this repository.
- Fork upkeep, build reproducibility, and stability work by ItzVyral (with
  OpenCode), https://github.com/ItzVyral/SimpleMtkWlan.

## Build-time Dependencies

- **MacKernelSDK** — https://github.com/acidanthera/MacKernelSDK
  macOS kernel headers (`libkern/`, `machine/`, kernel side of `IOKit`, ...)
  and `libkmod.a`, distributed under the Apple Public Source License 2.0
  (`LICENSE.txt` inside the submodule).
  Taken: nothing is copied into this repository; the project compiles against
  the headers through `KERNEL_EXTENSION_HEADER_SEARCH_PATHS` and links the
  `libkmod.a` it ships. Present as a git submodule at `MacKernelSDK/`, build
  only. Added 2026-10-31, pinned to commit
  `3f750085caa17ec3a7880f11c11bf4f48cd6a164`.

## License Notes

The top-level project is distributed under GPLv2. Individual files may include
more specific GPL, BSD, OpenBSD, NetBSD, FreeBSD, or permissive license notices;
those notices remain authoritative for the corresponding files.
