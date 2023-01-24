#pragma once

#include <ntdef.h>
#include <ntifs.h>
#include <stdio.h>
#include <stdarg.h> 
#include <ntdef.h>
#include <minwindef.h>

#include "utils/libraries/sk_crypter.h"
#include "utils/utils.h"

#include "nt/structs.h"
#include "nt/api.h"

namespace kernel_offsets {
	constexpr unsigned int image_file_name = 0x5a8;
	constexpr unsigned int active_threads = 0x5f0;
	constexpr unsigned int active_process_links = 0x448;
}

namespace csgo {
	static PEPROCESS process{ NULL };
}