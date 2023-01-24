#pragma once
#include "../../csgo/csgo.h"

namespace memory {
	template <typename T>
	auto read(unsigned int address) -> T {
		T ret{};
		KAPC_STATE apc{};
		KeStackAttachProcess(csgo::process, &apc);

		if (MmIsAddressValid(reinterpret_cast<void*>(address))) {
			ret = *reinterpret_cast<T*>(address);
		} else {
			utils::print("couldn't read at 0x%p \n", address);

			T free{};
			RtlZeroMemory(&free, sizeof(T));
			ret = free;
		}

		KeUnstackDetachProcess(&apc);
		return ret;
	}

	template <typename T>
	void write(unsigned int address, T value) {
		KAPC_STATE apc{};
		KeStackAttachProcess(csgo::process, &apc);

		if (MmIsAddressValid(reinterpret_cast<void*>(address)))
			*reinterpret_cast<T*>(address) = value;
		else {
			utils::print("couldn't write at 0x%p \n", address);
		}

		KeUnstackDetachProcess(&apc);
	}
}