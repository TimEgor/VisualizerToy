#pragma once

#include <cassert>

#define VT_CHECK_FUNCT(check_val, function) { if (!(check_val)) { function; return false; }}
#define VT_CHECK_INITIALIZATION(check_val) { VT_CHECK_FUNCT(check_val, release()) }
#define VT_CHECK_RETURN_FALSE(check_val) { if (!(check_val)) { return false; }}
#define VT_CHECK_RETURN_FALSE_ASSERT(check_val) { if (!(check_val)) { assert(false); return false; }}
#define VT_CHECK_RETURN_ASSERT(check_val) { if (!(check_val)) { assert(false); return; }}

//tmp assert macro
//TODO: need implement assert with custom dynamic message
#define VT_CHECK_RETURN_FALSE_ASSERT_MSG(check_val, msg) { if (!(check_val)) { assert(false); return false; }}
#define VT_CHECK_RETURN_ASSERT_MSG(check_val, msg) { if (!(check_val)) { assert(false); return; }}

#define VT_SAFE_DESTROY(obj) { if (obj) { delete obj; obj = nullptr; }}
#define VT_SAFE_DESTROY_ARRAY(obj) { if (obj) { delete[] obj; obj = nullptr; }}
#define VT_SAFE_DESTROY_WITH_RELEASING(obj) { if (obj) { obj->release(); delete obj; obj = nullptr; }}

#define VT_SAFE_CALL(ft) { if(ft) { ft(); } }
#define VT_SAFE_CALL_ARGS(ft, ...) { if(ft) { ft(__VA_ARGS__); } }