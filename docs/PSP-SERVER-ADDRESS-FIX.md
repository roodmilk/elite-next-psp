# PSP server-address startup fix — 10 October 2026

Physical PSP evidence: `online.cfg` correctly contained `1 192.168.1.105 25926 6`.
The PSP associated as 192.168.1.171/24, gateway 192.168.1.254. Its diagnostic
EBOOT sent 30 valid-size HELLO datagrams to **58.1.2.128:25926**, receiving none.
The PC server also received none. EBOOT used for that observation:
`AABFBA6F2DC1E3DB29E8DD7246FBF36A028DC25B70CADD50EFD735E8AF8C3B8B`.

`online_config_load()` runs before `online_start()` loads PSP_NET_MODULE_INET.
The SDK aliases `inet_addr` to the module import `sceNetInetInetAddr`. The
SDK's `pspkerror.h` defines 0x8002013A as LIBRARY_NOT_YET_LINKED. The bytes of
that error on the PSP are 58.1.2.128; the startup code stored it as the server IP.
PPSSPP's successful earlier connections did not expose this hardware failure.

Fix: `src/online-address.h` parses four decimal IPv4 octets locally and copies
their network-order bytes into the destination, with no PSP network imports.
`online_config_load()` uses it for both default and configured addresses.
Malformed addresses disable automatic connection and fall back to loopback;
the existing configuration format, valid endpoints, profiles and protocol stay
unchanged. Invalid parsing never partially overwrites an address.

The minimal source fix is present in the shared source tree. The playable
hardware-verification build is isolated in `work/psp-network-fixed` relative to
the workspace root, retaining the previous diagnostic strip/log. It uses the
same frozen local 2.5.361 source as the diagnostic EBOOT, plus this fix; it does
not overwrite the main thread's evolving EBOOT, numbered release or VERSION.
No GitHub publication has been performed. Test/installation evidence is recorded
in that isolated folder's VERIFICATION.txt.

**Physical PSP connection confirmed, 10 October 2026 at 12:28 BST.** The player
reported CONNECTED / ONLINE. The PC log independently records HELLO ACCEPTED
from 192.168.1.171:54334 at 11:28:26.903 UTC, slot 1, system 7, planet -1,
followed by sustained traffic (322 received / 69 sent by 11:29:01 UTC), no
rejections and no send errors. This confirms this PSP-to-PC connection fix;
two-device play, all planetary contexts and public internet hosting are separate
verification tasks. Keep the source fix in the next normal release.
