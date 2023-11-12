#include <stdio.h>
#include <stdlib.h>
#include "../../include/csgo/definitions.h"
#include "../../include/definitions.h"
#include "../../include/linux/ioctl.h"
#include "../../include/util.h"
#include "../../include/math.h"

uintptr_t find_client_state(n1_process_t* proc, n1_region_t* region) {
    uintptr_t address;
    n1_signature_t signature = {
        .signature = "48 8b 05 ? ? ? ? 55 48 ? ? 5d 48 ? ? 08 c3",
        .relative = 1,
        .extra = 0,
        .offsets = {2},
        .offsets_len = 1,
        .size = 0
    };

    if (read_memory(proc, get_offset(proc, region, &signature), sizeof(uintptr_t), &address) != sizeof(uintptr_t)) {
        return 0;
    }
    address += 8;

    return address;
}

uintptr_t find_client_state_state(n1_process_t* proc, n1_region_t* region) {
    n1_signature_t signature = {
        .signature = "44 89 B3 ? ? ? ? 41 BC 01 00 00 00",
        .relative = 0,
        .extra = 0,
        .offsets = {3},
        .offsets_len = 1,
        .size = sizeof(uint32_t)
    };

    return get_offset(proc, region, &signature);
}

uintptr_t find_client_state_maxclients(n1_process_t* proc, n1_region_t* region) {
    n1_signature_t signature = {
        .signature = "41 8B ? ? ? ? ? 83 C0 01 41 89 ? ? ? ? ? 8D 46 FF",
        .relative = 0,
        .extra = 0,
        .offsets = {3},
        .offsets_len = 1,
        .size = sizeof(uint32_t)
    };

    return get_offset(proc, region, &signature);
}

uintptr_t find_client_state_viewangles(n1_process_t* proc, n1_region_t* region) {
    n1_signature_t signature = {
        .signature = "F3 0F 10 02 F3 0F 11 80 ? ? ? ? F3 0F 10 42 04",
        .relative = 0,
        .extra = 0,
        .offsets = {8},
        .offsets_len = 1,
        .size = sizeof(uint32_t)
    };

    return get_offset(proc, region, &signature);
}

uintptr_t find_client_state_ishltv(n1_process_t* proc, n1_region_t* region) {
    n1_signature_t signature = {
        .signature = "E8 ? ? ? ? 80 B8 ? ? ? ? 00 75 5B E8",
        .relative = 0,
        .extra = 0,
        .offsets = {7},
        .offsets_len = 1,
        .size = sizeof(uint32_t)
    };

    return get_offset(proc, region, &signature);
}

uintptr_t find_local_player(n1_process_t* proc, n1_region_t* region) {
    n1_signature_t signature = {
        .signature = "48 89 e5 74 0e 48 8d 05",
        .relative = 1,
        .extra = 0,
        .offsets = {7},
        .offsets_len = 1,
        .size = 0
    };

    return get_offset(proc, region, &signature);
}

uintptr_t find_nv_entity_health(n1_process_t* proc, n1_region_t* region) {
    n1_signature_t signature = {
        .signature = "C7 87 ? ? ? ? 00 00 00 00 48 8D ? ? ? ? ? BF 02 00 00 00",
        .relative = 0,
        .extra = 0,
        .offsets = {2},
        .offsets_len = 1,
        .size = sizeof(int32_t)
    };

    return get_offset(proc, region, &signature);
}

uintptr_t find_nv_player_crossid(n1_process_t* proc, n1_region_t* region) {
    n1_signature_t signature = {
        .signature = "C7 83 ? ? ? ? 00 00 00 00 E8 ? ? ? ? 48 8D ? ? ? ? ? C7 83 ? ? ? ? 00 00 00 00 E8 ? ? ? ? 80 ? ? ? ? ? ? C7 83 ? ? ? ? 00 00 00 00 C7 83 ? ? ? ? 00 00 00 00 C7 83 ? ? ? ? 00 00 00 00 C7 83 ? ? ? ? 00 00 00 00",
        .relative = 0,
        .extra = 0,
        .offsets = {2},
        .offsets_len = 1,
        .size = sizeof(int32_t)
    };

    return get_offset(proc, region, &signature);
}

// uintptr_t find_entity_list(n1_process_t* proc, n1_region_t* region) {
//     uintptr_t address;
//     n1_signature_t signature = {
//         .signature = "48 8D ? ? ? ? ? F3 0F ? ? 31 ? F3 0F ? ? ? ? ? ? F3 0F ? ? ? ? ? ? 48 8B",
//         .relative = 1,
//         .extra = 0,
//         .offsets = {2},
//         .offsets_len = 1,
//         .size = 0
//     };

//     if (read_memory(proc, get_offset(proc, region, &signature), sizeof(uintptr_t), &address) != sizeof(uintptr_t)) {
//         return 0;
//     }

//     return address;
// }

/**
 * @brief Initializes offsets for the game
 * @param process Game process (In)
 * @param offsets Offsets structure (Out)
 * @return 0 on success, 1 on error
 */
int init_offsets(n1_process_t* process, csgo_offsets_t* offsets) {
    n1_region_t client;
    n1_region_t engine;

    if (get_region(process, "client_client.so", &client) != 0) {
        return 1;
    }
    DBG_PRINTF("debug: client_client.so=%p\n", (void*)client.start);

    if (get_region(process, "engine_client.so", &engine) != 0) {
        return 1;
    }
    DBG_PRINTF("debug: engine_client.so=%p\n", (void*)engine.start);

    offsets->client_state = find_client_state(process, &engine);
    offsets->client_state_state = find_client_state_state(process, &engine);
    offsets->client_state_maxclients = find_client_state_maxclients(process, &engine);
    offsets->client_state_viewangles = find_client_state_viewangles(process, &engine);
    offsets->client_state_ishltv = find_client_state_ishltv(process, &engine);
    offsets->local_player = find_local_player(process, &client);
    offsets->nv_entity_health = find_nv_entity_health(process, &client);
    offsets->nv_entity_flags = 0x13C;
    offsets->nv_player_crossid = find_nv_player_crossid(process, &client);

    DBG_PRINTF("debug: offsets->client_state=%p\n", (void*)offsets->client_state);
    DBG_PRINTF("debug: offsets->client_state_state=%p\n", (void*)offsets->client_state_state);
    DBG_PRINTF("debug: offsets->client_state_maxclients=%p\n", (void*)offsets->client_state_maxclients);
    DBG_PRINTF("debug: offsets->client_state_viewangles=%p\n", (void*)offsets->client_state_viewangles);
    DBG_PRINTF("debug: offsets->client_state_ishltv=%p\n", (void*)offsets->client_state_ishltv);
    DBG_PRINTF("debug: offsets->local_player=%p\n", (void*)offsets->local_player);
    DBG_PRINTF("debug: offsets->nv_entity_health=%p\n", (void*)offsets->nv_entity_health);
    DBG_PRINTF("debug: offsets->nv_entity_flags=%p\n", (void*)offsets->nv_entity_flags);
    DBG_PRINTF("debug: offsets->nv_player_crossid=%p\n", (void*)offsets->nv_player_crossid);

    return 0;
}