# Cargo exchange appointments

Implemented in the shared post-2.5.416 development tree on 12 October 2026.

## Player-visible flow

- A roaming trader first gives a complete cargo quote. Nothing is saved or transferred yet.
- Explicit acceptance creates a `CARGO EXCHANGE` side contract in the five-slot Mission Log.
- The appointment records the exact trader, home system, requested commodity, offered commodity and quantity.
- The Mission Log marks the trader and states the full exchange. The reward field reads `BARTER`, never a false credit amount.
- The player returns to the named system and hails that trader again to exchange goods.
- Missing requested cargo or insufficient hold space leaves the appointment active and produces a contextual reply.
- Completion transfers the exact goods once and queues a no-units completion card.
- Trader destruction or manual abandonment closes the appointment with a no-payment failure card. Leaving or docking does not fail it.
- Quiet Watch cargo offers use the same saved appointment flow.

## Compatibility

The existing V37 `Job` record stores appointments as `MISSION_PROCUREMENT` with its previously unused stage-one state. No save-version change or new stream was added. Ordinary Mission Board procurement contracts retain stage zero and their existing behavior.

## Verification

Focused PSP-emulator checks pass for quote-only behavior, explicit acceptance, exact Mission Log terms, missing cargo, one-time transfer, V37 save/load, trader-loss failure and zero-credit outcomes. The wider inherited game suite still reports its existing 17 unrelated baseline failures.

Latest isolated retail candidate built from the shared source:

`outputs/conversation-trade-final/EBOOT.PBP`
