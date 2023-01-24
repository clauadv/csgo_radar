#pragma once

namespace csgo {
	class entity_t {
	public:
		unsigned int is_dormant() {
			return memory::read<unsigned int>(reinterpret_cast<unsigned int>(this) + csgo::offsets::m_dormant);
		}

		unsigned int team_id() {
			return memory::read<unsigned int>(reinterpret_cast<unsigned int>(this) + csgo::offsets::m_team_num);
		}

		unsigned int spotted() {
			return memory::read<unsigned int>(reinterpret_cast<unsigned int>(this) + csgo::offsets::m_spotted);
		}
	};
}