#pragma once

#define TTNumbrEqual(Numbr, Value) uint32_t _TMP = Numbr, typename std::enable_if_t<_TMP == Value, bool> = true

