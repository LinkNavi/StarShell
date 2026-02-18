/*
 * matugen_colors.h - Header-only parser for matugen colors.json
 * 
 * Usage:
 *   MatugenColors colors;
 *   if (matugen_load_colors(NULL, &colors)) {
 *       // Use colors.primary, colors.surface, etc.
 *   }
 */

#ifndef MATUGEN_COLORS_H
#define MATUGEN_COLORS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    // Primary colors
    uint32_t primary;
    uint32_t on_primary;
    uint32_t primary_container;
    uint32_t on_primary_container;
    uint32_t primary_fixed;
    uint32_t primary_fixed_dim;
    uint32_t on_primary_fixed;
    uint32_t on_primary_fixed_variant;
    
    // Secondary colors
    uint32_t secondary;
    uint32_t on_secondary;
    uint32_t secondary_container;
    uint32_t on_secondary_container;
    uint32_t secondary_fixed;
    uint32_t secondary_fixed_dim;
    uint32_t on_secondary_fixed;
    uint32_t on_secondary_fixed_variant;
    
    // Tertiary colors
    uint32_t tertiary;
    uint32_t on_tertiary;
    uint32_t tertiary_container;
    uint32_t on_tertiary_container;
    uint32_t tertiary_fixed;
    uint32_t tertiary_fixed_dim;
    uint32_t on_tertiary_fixed;
    uint32_t on_tertiary_fixed_variant;
    
    // Error colors
    uint32_t error;
    uint32_t on_error;
    uint32_t error_container;
    uint32_t on_error_container;
    
    // Surface colors
    uint32_t surface;
    uint32_t on_surface;
    uint32_t surface_variant;
    uint32_t on_surface_variant;
    uint32_t surface_dim;
    uint32_t surface_bright;
    uint32_t surface_container_lowest;
    uint32_t surface_container_low;
    uint32_t surface_container;
    uint32_t surface_container_high;
    uint32_t surface_container_highest;
    
    // Background
    uint32_t background;
    uint32_t on_background;
    
    // Outline
    uint32_t outline;
    uint32_t outline_variant;
    
    // Other
    uint32_t shadow;
    uint32_t scrim;
    uint32_t inverse_surface;
    uint32_t inverse_on_surface;
    uint32_t inverse_primary;
} MatugenColors;

// Parse hex color string to uint32_t (0xRRGGBBAA)
static uint32_t _parse_hex_color(const char *hex) {
    if (!hex) return 0xFF000000;
    
    // Skip '#' if present
    if (*hex == '#') hex++;
    
    unsigned int r = 0, g = 0, b = 0;
    sscanf(hex, "%02x%02x%02x", &r, &g, &b);
    
    // Return as 0xRRGGBBAA (full opacity)
    return (r << 24) | (g << 16) | (b << 8) | 0xFF;
}

// Simple JSON value finder (finds "key": "value" patterns)
static char* _find_json_value(const char *json, const char *key) {
    if (!json || !key) return NULL;
    
    char search[256];
    snprintf(search, sizeof(search), "\"%s\"", key);
    
    const char *found = strstr(json, search);
    if (!found) return NULL;
    
    // Skip to the value part (after the ':')
    found = strchr(found, ':');
    if (!found) return NULL;
    found++;
    
    // Skip whitespace
    while (*found == ' ' || *found == '\t' || *found == '\n') found++;
    
    // If it's a string value (starts with ")
    if (*found == '"') {
        found++;
        const char *end = strchr(found, '"');
        if (!end) return NULL;
        
        size_t len = end - found;
        char *result = (char*)malloc(len + 1);
        if (result) {
            memcpy(result, found, len);
            result[len] = '\0';
        }
        return result;
    }
    
    return NULL;
}

// Get default matugen colors.json path
static char* _get_matugen_path(void) {
    const char *home = getenv("HOME");
    if (!home) return NULL;
    
    char *path = (char*)malloc(512);
    if (!path) return NULL;
    
    snprintf(path, 512, "%s/.config/matugen/colors.json", home);
    return path;
}

// Load colors from matugen's colors.json
// If json_path is NULL, uses ~/.config/matugen/colors.json
// Returns true on success, false on failure
static bool matugen_load_colors(const char *json_path, MatugenColors *colors) {
    if (!colors) return false;
    
    // Get path
    char *path = NULL;
    bool free_path = false;
    
    if (json_path) {
        path = (char*)json_path;
    } else {
        path = _get_matugen_path();
        if (!path) return false;
        free_path = true;
    }
    
    // Read file
    FILE *fp = fopen(path, "r");
    if (!fp) {
        if (free_path) free(path);
        fprintf(stderr, "Failed to open matugen colors: %s\n", path);
        return false;
    }
    
    // Get file size
    fseek(fp, 0, SEEK_END);
    long size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    
    // Read entire file
    char *json = (char*)malloc(size + 1);
    if (!json) {
        fclose(fp);
        if (free_path) free(path);
        return false;
    }
    
    fread(json, 1, size, fp);
    json[size] = '\0';
    fclose(fp);
    
    if (free_path) free(path);
    
    // Find the "dark" section (we'll use dark theme colors)
    const char *dark_section = strstr(json, "\"dark\"");
    if (!dark_section) {
        // Fallback to "colors" section if no dark section
        dark_section = strstr(json, "\"colors\"");
        if (!dark_section) {
            free(json);
            fprintf(stderr, "No dark or colors section in matugen JSON\n");
            return false;
        }
    }
    
    // Parse colors
    #define PARSE_COLOR(name) do { \
        char *hex = _find_json_value(dark_section, #name); \
        if (hex) { \
            colors->name = _parse_hex_color(hex); \
            free(hex); \
        } else { \
            colors->name = 0xFF000000; \
        } \
    } while(0)
    
    // Primary
    PARSE_COLOR(primary);
    PARSE_COLOR(on_primary);
    PARSE_COLOR(primary_container);
    PARSE_COLOR(on_primary_container);
    PARSE_COLOR(primary_fixed);
    PARSE_COLOR(primary_fixed_dim);
    PARSE_COLOR(on_primary_fixed);
    PARSE_COLOR(on_primary_fixed_variant);
    
    // Secondary
    PARSE_COLOR(secondary);
    PARSE_COLOR(on_secondary);
    PARSE_COLOR(secondary_container);
    PARSE_COLOR(on_secondary_container);
    PARSE_COLOR(secondary_fixed);
    PARSE_COLOR(secondary_fixed_dim);
    PARSE_COLOR(on_secondary_fixed);
    PARSE_COLOR(on_secondary_fixed_variant);
    
    // Tertiary
    PARSE_COLOR(tertiary);
    PARSE_COLOR(on_tertiary);
    PARSE_COLOR(tertiary_container);
    PARSE_COLOR(on_tertiary_container);
    PARSE_COLOR(tertiary_fixed);
    PARSE_COLOR(tertiary_fixed_dim);
    PARSE_COLOR(on_tertiary_fixed);
    PARSE_COLOR(on_tertiary_fixed_variant);
    
    // Error
    PARSE_COLOR(error);
    PARSE_COLOR(on_error);
    PARSE_COLOR(error_container);
    PARSE_COLOR(on_error_container);
    
    // Surface
    PARSE_COLOR(surface);
    PARSE_COLOR(on_surface);
    PARSE_COLOR(surface_variant);
    PARSE_COLOR(on_surface_variant);
    PARSE_COLOR(surface_dim);
    PARSE_COLOR(surface_bright);
    PARSE_COLOR(surface_container_lowest);
    PARSE_COLOR(surface_container_low);
    PARSE_COLOR(surface_container);
    PARSE_COLOR(surface_container_high);
    PARSE_COLOR(surface_container_highest);
    
    // Background
    PARSE_COLOR(background);
    PARSE_COLOR(on_background);
    
    // Outline
    PARSE_COLOR(outline);
    PARSE_COLOR(outline_variant);
    
    // Other
    PARSE_COLOR(shadow);
    PARSE_COLOR(scrim);
    PARSE_COLOR(inverse_surface);
    PARSE_COLOR(inverse_on_surface);
    PARSE_COLOR(inverse_primary);
    
    #undef PARSE_COLOR
    
    free(json);
    return true;
}

// Convert uint32_t color to hex string (returns static buffer)
static const char* matugen_color_to_hex(uint32_t color) {
    static char hex[10];
    unsigned int r = (color >> 24) & 0xFF;
    unsigned int g = (color >> 16) & 0xFF;
    unsigned int b = (color >> 8) & 0xFF;
    snprintf(hex, sizeof(hex), "#%02x%02x%02x", r, g, b);
    return hex;
}

// Print all colors (for debugging)
static void matugen_print_colors(const MatugenColors *colors) {
    if (!colors) return;
    
    printf("Matugen Colors:\n");
    printf("  Primary: %s\n", matugen_color_to_hex(colors->primary));
    printf("  Secondary: %s\n", matugen_color_to_hex(colors->secondary));
    printf("  Tertiary: %s\n", matugen_color_to_hex(colors->tertiary));
    printf("  Surface: %s\n", matugen_color_to_hex(colors->surface));
    printf("  Background: %s\n", matugen_color_to_hex(colors->background));
    printf("  Error: %s\n", matugen_color_to_hex(colors->error));
}

#endif /* MATUGEN_COLORS_H */
