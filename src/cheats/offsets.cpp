#include "offsets.hpp"
#include <spdlog/spdlog.h>

void apex::cheats::offsets_t::init()
{
    // 0F B7 05 ? ? ? ? 39 42 08 75 11 48 8B 0A
    // lea     rax, qword_7FF7E52A7AF8
    entity_list = 0x1a1e3b8;

    // 0F 84 ? ? ? ? 49 89 5B 10 0F
    // mov     eax, cs:dword_7FF7E4B8BF5C
    local_player_index = 0x1dcf5e8;

    clientstate = 0x137e1c0;
    name_list = 0x84067a0;
    global_vars = 0x137de20;
}