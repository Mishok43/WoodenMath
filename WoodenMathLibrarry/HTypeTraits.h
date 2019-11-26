#pragma once

#define TTTrue(Value) bool _TMP = Value, typename std::enable_if_t<_TMP, bool> = true

#define TTNumbrEqual(Numbr, Value) uint32_t _TMP = Numbr, typename std::enable_if_t<_TMP == Value, bool> = true

#define TTTypeEqual(Type, OtherT)  typename _TMP2 = Type, typename std::enable_if_t<std::is_same_v<_TMP2, OtherT>, bool> = true

#define TTNumbrNotEqual(Numbr, Value) uint32_t _TMP = Numbr, typename std::enable_if_t<_TMP != Value, bool> = true

#define TTTypeNotEqual(Type, OtherT)  typename _TMP2 = Type, typename std::enable_if_t<!std::is_same_v<_TMP2, OtherT>, bool> = true

#define TTIsFloating(Type)  typename _TMP2 = Type, typename std::enable_if_t<std::is_same_v<_TMP2, float> || std::is_same_v<_TMP2, double>, bool> = true

#define TTIsInteger(Type)  typename _TMP2 = Type, typename std::enable_if_t<std::is_same_v<_TMP2, int32_t> || std::is_same_v<_TMP2, uint32_t>, bool> = true
