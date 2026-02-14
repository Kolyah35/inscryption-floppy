#ifndef PAK_H
#define PAK_H

#include <stdint.h>

typedef struct {
    int32_t  hash;
    uint32_t width;
    uint32_t height;
    uint32_t channels;
    uint8_t* rgba;
} sprite_t;

typedef struct {
    sprite_t* textures;
    uint32_t count;
} pak_file_t;

int pak_load(const char* filename, pak_file_t* pak);
sprite_t* pak_get(pak_file_t* pak, const char* resource_name);
void pak_free(pak_file_t* pak);

extern pak_file_t pak_areas;
extern pak_file_t pak_cards;
extern pak_file_t pak_map;
extern pak_file_t pak_objects;
extern pak_file_t pak_portraits;
extern pak_file_t pak_sigils;
extern pak_file_t pak_ui;

#endif