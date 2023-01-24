#include "includes.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);

	csgo::process = utils::get_process_id(_(L"csgo.exe"));
	if (csgo::process != NULL) {
		utils::print("csgo -> %x \n", csgo::process);

		const auto client = utils::get_module_base(csgo::process, _(L"client.dll"));
		utils::print("client -> %x \n", client);
	}

	return STATUS_SUCCESS;
}