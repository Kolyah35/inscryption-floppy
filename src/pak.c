#include <stdio.h>
#include <stdlib.h>
#include "pak.h"

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ONLY_PNG
#define STBI_NO_STDIO
#define STBI_NO_SIMD
#include "stb_image.h"

int32_t calc_hash(const char* s) {
    uint32_t h = 0;
    while (*s) {
        h = (31 * h + (unsigned char)(*s)) & 0xFFFFFFFF;
        s++;
    }

    if (h & 0x80000000)
        return (int32_t)(h - 0x100000000);
    else
        return (int32_t)h;
}

int pak_load(const char* filename, pak_file_t* pak) {
    FILE* f = fopen(filename, "rb");
    if (!f) return 0;

    fread(&pak->count, sizeof(pak->count), 1, f);
    pak->textures = malloc(pak->count * sizeof(sprite_t));

    int loaded = 0;
    
    for (int i = 0; i < pak->count; i++) {
        int32_t  hash;
        uint32_t size;

        if (fread(&hash, sizeof(hash), 1, f) != 1)
            break;

        if (fread(&size, sizeof(size), 1, f) != 1)
            break;

        uint8_t* data = (uint8_t*)malloc(size);
        if (!data) {
            fclose(f);
            return 0;
        }

        if (fread(data, 1, size, f) != size) {
            free(data);
            break;
        }

        int w,h,c;
        uint8_t* rgba = stbi_load_from_memory(data, size, &w, &h, &c, 4);
        if (!rgba) {
            printf("Failed to decode PNG at index %zu\n", i);
            rgba = NULL;
            w = h = 0;
        }

        free(data);

        pak->textures[i].hash = hash;
        pak->textures[i].width = w;
        pak->textures[i].height = h;
        pak->textures[i].channels = 4;
        pak->textures[i].rgba = rgba;

        loaded++;
    }

    fclose(f);

    return loaded;
}

sprite_t* pak_get(pak_file_t* pak, const char* resource_name) {
    int32_t hash = calc_hash(resource_name);

    for (uint32_t i = 0; i < pak->count; i++) {
        if (pak->textures[i].hash == hash) {
            return &pak->textures[i];
        }
    }

    return NULL;
}

void pak_free(pak_file_t* pak) {
    for (uint32_t i = 0; i < pak->count; i++)
        free(pak->textures[i].rgba);

    free(pak->textures);
}