/*
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The ASF licenses this file to You under the Apache License, Version 2.0
 * (the "License"); you may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef ROCKETMQ_CONFIG_HPP_
#define ROCKETMQ_CONFIG_HPP_

#if __cplusplus < 201103L
#define _ROCKETMQ_STD_VER 1998L
#elif __cplusplus < 201402L
#define _ROCKETMQ_STD_VER 2011L
#elif __cplusplus < 201703L
#define _ROCKETMQ_STD_VER 2014L
#elif __cplusplus < 202002L
#define _ROCKETMQ_STD_VER 2017L
#else
#define _ROCKETMQ_STD_VER 2020L
#endif

#if defined(__has_include)
#define _ROCKETMQ_HAS_INCLUDE(HEADER) __has_include(HEADER)
#else
#define _ROCKETMQ_HAS_INCLUDE(HEADER) 0
#endif  // defined(__has_include)

#if _ROCKETMQ_HAS_INCLUDE(<version>)
#include <version>
#endif  // _ROCKETMQ_HAS_INCLUDE(<version>)

#if __cpp_variable_templates >= 201304L || _ROCKETMQ_STD_VER >= 2014L
#define _ROCKETMQ_VARIABLE_TEMPLATES 201304L
#endif  // __cpp_variable_templates >= 201304L || _ROCKETMQ_STD_VER >= 2014L

#if __cpp_constexpr >= 201304L || _ROCKETMQ_STD_VER >= 2014L
#define _ROCKETMQ_CONSTEXPR constexpr
#else  // __cpp_constexpr >= 201304L || _ROCKETMQ_STD_VER >= 2014L
#define _ROCKETMQ_CONSTEXPR inline
#endif  // __cpp_constexpr >= 201304L || _ROCKETMQ_STD_VER >= 2014L

#if __cpp_inline_variables >= 201606L || _ROCKETMQ_STD_VER >= 2017L
#define _ROCKETMQ_INLINE_VARIABLES 201606L
#define _ROCKETMQ_INLINE_VAR inline
#else  // __cpp_inline_variables >= 201606L || _ROCKETMQ_STD_VER >= 2017L
#define _ROCKETMQ_INLINE_VAR
#endif  // __cpp_inline_variables >= 201606L || _ROCKETMQ_STD_VER >= 2017L

#if _ROCKETMQ_STD_VER >= 2020L
#define _ROCKETMQ_CONSTEXPR_CTOR _ROCKETMQ_CONSTEXPR
#else  // _ROCKETMQ_STD_VER >= 2020L
#define _ROCKETMQ_CONSTEXPR_CTOR
#endif  // _ROCKETMQ_STD_VER >= 2020L

#if __cpp_if_constexpr >= 201606L || _ROCKETMQ_STD_VER >= 2017L
#define _ROCKETMQ_IF_CONSTEXPR 201606L
#endif  // __cpp_if_constexpr >= 201606L || _ROCKETMQ_STD_VER >= 2017L

#if __cpp_concepts >= 201907L || _ROCKETMQ_STD_VER >= 2020L
#define _ROCKETMQ_CONCEPTS 201907L
#endif  // __cpp_concepts >= 201907L || _ROCKETMQ_STD_VER >= 2020L

#if __cpp_lib_filesystem >= 201703L
#define _ROCKETMQ_LIB_FILESYSTEM 201703L
#elif _ROCKETMQ_STD_VER >= 2017L && defined(__has_include)
#if __has_include(<filesystem>)
#define _ROCKETMQ_LIB_FILESYSTEM 201703L
#endif  // __has_include(<filesystem>)
#endif  // _ROCKETMQ_STD_VER >= 2017L && defined(__has_include)

#if __cpp_lib_bit_cast >= 201806L
#define _ROCKETMQ_LIB_BIT_CAST = 201806L
#endif

#if __cpp_lib_three_way_comparison >= 201907L
#define _ROCKETMQ_LIB_THREE_WAY_COMPARISON 201907L
#elif _ROCKETMQ_STD_VER >= 2020L && defined(__has_include)
#if __has_include(<compare>)
#define _ROCKETMQ_LIB_THREE_WAY_COMPARISON 201907L
#endif  // __has_include(<ranges>)
#endif

#if __cpp_lib_ranges >= 201911L
#define _ROCKETMQ_LIB_RANGES 201911L
#elif _ROCKETMQ_STD_VER >= 2020L && defined(__has_include)
#if __has_include(<ranges>)
#define _ROCKETMQ_LIB_RANGES 201911L
#endif  // __has_include(<ranges>)
#endif  // _ROCKETMQ_STD_VER >= 2020L && defined(__has_include)

#if __cpp_lib_concepts >= 202002L
#define _ROCKETMQ_LIB_CONCEPTS 202002L
// #elif _ROCKETMQ_STD_VER >= 2020L && defined(__has_include)
// #if __has_include(<concepts>)
// #define _ROCKETMQ_LIB_CONCEPTS 202002L
// #endif  // __has_include(<concepts>)
#endif  // _ROCKETMQ_STD_VER >= 2020L && defined(__has_include)

#endif  // ROCKETMQ_CONFIG_HPP_
