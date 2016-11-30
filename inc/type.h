#pragma once

#include <cstdint>

using u8 = uint8_t;
using i8 = int8_t;

using u16 = uint16_t;
using i16 = int16_t;

using u32 = uint32_t;
using i32 = int32_t;

using u64 = uint64_t;
using i64 = int64_t;

using f32 = float;
using f64 = double;

//Choose arbitrarily a type for variant ptr
// struct Variant
// {
//     using Var = unsigned long;
//     Var ptr;
//     template<typename T>
//     void set(T *newPtr)
//     {
//         ptr = reinterpreter_cast<Var>(newPtr);
//     }
//
//     template<typename T>
//     T* get()
//     {
//         return reinterpreter_cast<T*>(ptr);
//     }
//
//     template<typename T>
//     T& get()
//     {
//         return *reinterpreter_cast<T*>(ptr);
//     }
// };

using Variant = unsigned long;
//using VoidRet = i32;
