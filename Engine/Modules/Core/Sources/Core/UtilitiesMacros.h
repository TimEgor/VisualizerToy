#pragma once

#define VT_CHECK_FUNCT(check_val, function) { if (!(check_val)) { function; return false; }}
#define VT_CHECK_INITIALIZATION(check_val) { VT_CHECK_FUNCT(check_val, release()) }
#define VT_CHECK_RETURN_FALSE(check_val) { if (!(check_val)) { return false; }}

#define VT_SAFE_DESTROY(obj) { if (obj) { delete obj; obj = nullptr; }}
#define VT_SAFE_DESTROY_ARRAY(obj) { if (obj) { delete[] obj; obj = nullptr; }}
#define VT_SAFE_DESTROY_WITH_RELEASING(obj) { if (obj) { obj->release(); delete obj; obj = nullptr; }}