// Copyright (c) 2017 nyorain
// Distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://www.boost.org/LICENSE_1_0.txt

#pragma once

#include <dlg/dlg.hpp>

namespace vpp {

#define vpp_trace(...)  dlg_trace(dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_debug(...)  dlg_debug(dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_info(...) dlg_info(dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_warn(...) dlg_warn(dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_error(...) dlg_error(dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_critical(...) dlg_critical(dlg::Tag{"vpp"}, __VA_ARGS__)

#define vpp_assert_debug(expr, ...) dlg_assert_debug(expr, dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_assert_warn(expr, ...) dlg_assert_warn(expr, dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_assert_error(expr, ...)  dlg_assert_error(expr, dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_assert_critical(expr, ...) dlg_assert_critical(expr, dlg::Tag{"vpp"}, __VA_ARGS__)

#define vpp_log(...) dlg_log(dlg::Tag{"vpp"}, __VA_ARGS__)
#define vpp_assert(expr, ...) dlg_assert(expr, dlg::Tag{"vpp"}, __VA_ARGS__)

} // namespace ny
