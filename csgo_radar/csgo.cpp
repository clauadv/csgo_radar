#include "utils/utils.h"
#include "utils/memory/memory.h"
#include "csgo/csgo.h"
#include "csgo/entity.h"

NTSTATUS DriverEntry(PDRIVER_OBJECT driver_object, PUNICODE_STRING registry_path) {
	UNREFERENCED_PARAMETER(driver_object);
	UNREFERENCED_PARAMETER(registry_path);

	csgo::process = utils::get_process_id(_(L"csgo.exe"));
	if (csgo::process != NULL) {
		csgo::client = utils::get_module_base(csgo::process, _(L"client.dll"));
	}

	utils::print(_("found csgo -> 0x%x \n", csgo::process));
	utils::print(_("found client -> 0x%x \n", csgo::client));

	while (!csgo::unload) {
		utils::sleep(5000);

		if (utils::is_process_terminating(csgo::process)) {
			csgo::process = NULL;
			csgo::client = NULL;
			utils::print(_("unloaded i think :p \n"));

			break;
		}

		const auto local_player = reinterpret_cast<csgo::entity_t*>(memory::read<unsigned int>(csgo::client + csgo::offsets::dw_local_player));
		if (!local_player) continue;

		for (auto i = 1; i < 64; i++) {
			const auto player = reinterpret_cast<csgo::entity_t*>(memory::read<unsigned int>(csgo::client + csgo::offsets::dw_entity_list + (static_cast<unsigned long long>(i) * 0x10)));
			if (player == NULL) continue;
			if (player->is_dormant()) continue;
			if (player->team_id() == local_player->team_id()) continue;
			// if (player->spotted()) continue;

			memory::write<bool>(reinterpret_cast<unsigned int>(player) + csgo::offsets::m_spotted, true);
		}
	}

	return STATUS_SUCCESS;
}