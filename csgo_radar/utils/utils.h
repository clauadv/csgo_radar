#pragma once
#include "../includes.h"

namespace utils {
	void print(PCCH format, ...);
	template<typename str_type, typename str_type_2>
	bool crt_strcmp(str_type str, str_type_2 in_str, bool two);
	PEPROCESS get_process_id(const wchar_t* process_name);
	unsigned long long get_module_base(PEPROCESS process, const wchar_t* module_name);
}