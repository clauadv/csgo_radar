#pragma once
#include "../includes.h"

namespace utils {
    void print(PCCH format, ...) {
        CHAR message[512];
        va_list _valist;
        va_start(_valist, format);
        const ULONG N = _vsnprintf_s(message, sizeof(message) - 1, format, _valist);
        message[N] = L'\0';

        vDbgPrintExWithPrefix(_("[csgo_radar] "), DPFLTR_IHVDRIVER_ID, DPFLTR_ERROR_LEVEL, message, _valist);

        va_end(_valist);
    }

    void sleep(int ms) {
        LARGE_INTEGER time{};

        time.QuadPart = -(ms) * 10 * 1000;
        KeDelayExecutionThread(KernelMode, TRUE, &time);
    }

    unsigned char is_process_terminating(PEPROCESS process) {
        LARGE_INTEGER zero_time{ 0 };
        return KeWaitForSingleObject(process, Executive, KernelMode, FALSE, &zero_time) == STATUS_WAIT_0;
    }

    template <typename str_type, typename str_type_2>
    __forceinline bool crt_strcmp(str_type str, str_type_2 in_str, bool two) {
        if (!str || !in_str) return false;

        wchar_t c1, c2;
    #define to_lower(c_char) ((c_char >= 'A' && c_char <= 'Z') ? (c_char + 32) : c_char)

        do {
            c1 = *str++; c2 = *in_str++;
            c1 = to_lower(c1); c2 = to_lower(c2);

            if (!c1 && (two ? !c2 : 1))
                return true;

        } while (c1 == c2);

        return false;
    }

    PEPROCESS get_process_id(const wchar_t* process_name) {
        char image_name[15]{};
        PEPROCESS sys_process = PsInitialSystemProcess;
        PEPROCESS cur_entry = sys_process;

        do {
            RtlCopyMemory(reinterpret_cast<void*>(&image_name), reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(cur_entry) + kernel_offsets::image_file_name), sizeof(image_name));

            if (crt_strcmp(image_name, process_name, true)) {
                void* active_threads{};
                RtlCopyMemory(reinterpret_cast<void*>(&active_threads), reinterpret_cast<void*>(reinterpret_cast<unsigned long long>(cur_entry) + kernel_offsets::active_threads), sizeof(active_threads));

                if (active_threads) {
                    return cur_entry;
                }
            }

            const auto list = reinterpret_cast<PLIST_ENTRY>(reinterpret_cast<unsigned long long>(cur_entry) + kernel_offsets::active_process_links);
            cur_entry = reinterpret_cast<PEPROCESS>(reinterpret_cast<unsigned long long>(list->Flink) - kernel_offsets::active_process_links);

        } while (cur_entry != sys_process);

        return NULL;
    }

    unsigned long long get_module_base(PEPROCESS process, const wchar_t* module_name) {
        KAPC_STATE apc;
        KeStackAttachProcess(process, &apc);

        if (!process) {
            KeUnstackDetachProcess(&apc);
            return NULL;
        }

        const auto peb32 = reinterpret_cast<PPEB32>(PsGetProcessWow64Process(process));
        if (!peb32 || !peb32->Ldr) {
            KeUnstackDetachProcess(&apc);
            return NULL;
        }

        for (auto plist_entry = reinterpret_cast<PLIST_ENTRY32>(reinterpret_cast<PPEB_LDR_DATA32>(peb32->Ldr)->InLoadOrderModuleList.Flink);
             plist_entry != &(reinterpret_cast<PPEB_LDR_DATA32>(peb32->Ldr))->InLoadOrderModuleList;
             plist_entry = reinterpret_cast<PLIST_ENTRY32>(plist_entry->Flink))
        {
            const auto p_entry = CONTAINING_RECORD(plist_entry, LDR_DATA_TABLE_ENTRY32, InLoadOrderLinks);

            if (wcscmp(reinterpret_cast<wchar_t*>(p_entry->BaseDllName.Buffer), module_name) == 0) {
                unsigned long long module_base = p_entry->DllBase;
                KeUnstackDetachProcess(&apc);

                return module_base;
            }
        }

        KeUnstackDetachProcess(&apc);
        return 0;
    }
}