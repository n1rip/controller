#pragma once

#include <sys/types.h>
#include <stdint.h>

/**
 * @brief Contains offsets about game
 */
typedef struct cs_offsets_s {
    uintptr_t client_state;
    uintptr_t client_state_state;
    uintptr_t client_state_maxclients;
    uintptr_t client_state_viewangles;
    uintptr_t client_state_ishltv;
    uintptr_t nv_entity_health;
    uintptr_t nv_entity_flags;
    uintptr_t local_player;
    uintptr_t nv_player_crossid;
} cs_offsets_t;