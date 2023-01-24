#pragma once

namespace csgo {
	static PEPROCESS process{ NULL };
	static unsigned long long client{ NULL };
	static bool unload{ false };

	namespace offsets {
		// auto updater?

		constexpr unsigned int dw_local_player = 0xdea964;
		constexpr unsigned int dw_entity_list = 0x4dfff14;
		constexpr unsigned int m_team_num = 0xf4;
		constexpr unsigned int m_dormant = 0xed;
		constexpr unsigned int m_spotted = 0x93d;
	}
}