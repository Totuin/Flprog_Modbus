# FIX MODBUS TCP ETHERNET 2026-09-20

FLProg_Modbus 1.0.0 — diagnostic fix for Modbus TCP over Ethernet/Wiznet
Date: 2026-09-20

Observed:
- Modbus TCP over Wi-Fi: works.
- RTU over TCP over Ethernet: works.
- Modbus TCP over Ethernet: client receives only 6-byte MBAP header,
  then reports "Insufficient bytes received".

Root cause localized to sendTxBuffer(): Modbus TCP response was sent as two
immediate server.write() calls (6-byte MBAP, then UnitId+PDU). The tested
Ethernet transport path delivered the first write but not the second.

Change:
- For FLPROG_TCP_MODBUS, MBAP + UnitId/PDU are assembled into one contiguous
  ADU and sent using a single _server.write().
- RTU over TCP path is unchanged.
- KaScada Cloud path is unchanged.

Expected FC03 response for one register is 11 bytes total, e.g.:
TT TT 00 00 00 05 01 03 02 VV VV
