#include <linux/input-event-codes.h>
#include <stdlib.h>
#include <stdio.h>
#include "../../include/util.h"
#include "../../include/linux/ioctl.h"
#include "../../include/definitions.h"
#include "../../include/csgo/definitions.h"
#include "../../include/csgo/offset_manager.h"
#include "../../include/globals.h"

/**
 * @brief Main execution function for the game hack
 * @return 0 on success, positive value on error
 */
int csgo_run(void) {
    n1_process_t process;
    csgo_offsets_t offsets;  

    if (get_process("csgo_linux64", &process) != 0) {
        return 1;
    }

    if (init_offsets(&process, &offsets) != 0) {
        return 1;
    }

    while (get_process("csgo_linux64", &process) == 0 && !exit_signal) {
        uintptr_t player;
        uint32_t player_flags = 10;
        int32_t crosshair_id = 0;
        uint32_t game_state;
        read_memory(&process, offsets.client_state + offsets.client_state_state, sizeof(uint32_t), &game_state);

        if (game_state == 6) { // player is connected to a server
            read_memory(&process, offsets.local_player, sizeof(uint32_t), &player);
            read_memory(&process, player + offsets.nv_entity_flags, sizeof(uint32_t), &player_flags);
            read_memory(&process, player + offsets.nv_player_crossid, sizeof(uint32_t), &crosshair_id);
            
            if (player_flags & (1<<0) && get_key_state(KEY_DOWN) == key_state_pressed) {
                generate_keyboard_input(KEY_SPACE, keyboard_action_actuate);
            }

            if (crosshair_id && get_key_state(KEY_LEFTALT) == key_state_pressed) {
                generate_mouse_input(BTN_LEFT, mouse_action_actuate, 0);
            }
        }
        
        msleep(5);
    }

    return 0;
}