/*
 * starview_ipc.h - Header-only IPC client library for StarView compositor
 * 
 * This single-header library makes it dead simple to communicate with the
 * StarView compositor via IPC (Inter-Process Communication).
 * 
 * USAGE:
 * 
 * 1. Include this header in your C/C++ program:
 *    #include "starview_ipc.h"
 * 
 * 2. Connect to the compositor:
 *    StarViewIPC *ipc = starview_ipc_connect();
 * 
 * 3. Do stuff:
 *    starview_ipc_switch_workspace(ipc, 2);
 *    int ws = starview_ipc_get_current_workspace(ipc);
 *    printf("Current workspace: %d\n", ws);
 * 
 * 4. Clean up:
 *    starview_ipc_disconnect(ipc);
 * 
 * EXAMPLES AT THE BOTTOM OF THIS FILE!
 */

#ifndef STARVIEW_IPC_H
#define STARVIEW_IPC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdint.h>
#include <stdbool.h>

/* ============================================================================
 * DATA STRUCTURES
 * ============================================================================ */

typedef struct {
    int socket_fd;
    char *read_buffer;
    size_t buffer_size;
    size_t buffer_len;
} StarViewIPC;

typedef struct {
    int num;
    char name[64];
    bool visible;
    bool focused;
    bool urgent;
} WorkspaceInfo;

typedef struct {
    char name[256];
    int width;
    int height;
    bool active;
    bool primary;
    int current_workspace;
} OutputInfo;

/* IPC Message Types */
enum {
    IPC_COMMAND = 0,
    IPC_GET_WORKSPACES = 1,
    IPC_SUBSCRIBE = 2,
    IPC_GET_OUTPUTS = 3,
    IPC_GET_TREE = 4,
    IPC_GET_VERSION = 7,
};

/* IPC Event Types */
enum {
    IPC_EVENT_WORKSPACE = (1 << 0),
    IPC_EVENT_WINDOW = (1 << 3),
    IPC_EVENT_MODE = (1 << 2),
};

#define IPC_EVENT_MASK 0x80000000

/* IPC Message Header */
#pragma pack(push, 1)
typedef struct {
    char magic[6];      /* "i3-ipc" */
    uint32_t size;      /* Payload size */
    uint32_t type;      /* Message type */
} IPCHeader;
#pragma pack(pop)

/* ============================================================================
 * HELPER FUNCTIONS (Internal)
 * ============================================================================ */

static char* _get_socket_path(void) {
    char *path = getenv("SWAYSOCK");
    if (!path || !*path) {
        path = "/tmp/starview-ipc.sock";
    }
    return path;
}

static int _send_message(StarViewIPC *ipc, uint32_t type, const char *payload) {
    if (!ipc || ipc->socket_fd < 0) return -1;
    
    IPCHeader header;
    memcpy(header.magic, "i3-ipc", 6);
    header.size = payload ? strlen(payload) : 0;
    header.type = type;
    
    /* Send header */
    if (write(ipc->socket_fd, &header, sizeof(header)) != sizeof(header)) {
        return -1;
    }
    
    /* Send payload if present */
    if (payload && header.size > 0) {
        if (write(ipc->socket_fd, payload, header.size) != (ssize_t)header.size) {
            return -1;
        }
    }
    
    return 0;
}

static char* _receive_message(StarViewIPC *ipc, uint32_t *out_type) {
    if (!ipc || ipc->socket_fd < 0) return NULL;
    
    /* Read header */
    IPCHeader header;
    ssize_t n = read(ipc->socket_fd, &header, sizeof(header));
    if (n != sizeof(header)) {
        return NULL;
    }
    
    /* Verify magic */
    if (memcmp(header.magic, "i3-ipc", 6) != 0) {
        fprintf(stderr, "Invalid IPC magic\n");
        return NULL;
    }
    
    if (out_type) *out_type = header.type;
    
    /* Read payload */
    if (header.size == 0) {
        return strdup("");
    }
    
    char *payload = malloc(header.size + 1);
    if (!payload) return NULL;
    
    n = read(ipc->socket_fd, payload, header.size);
    if (n != (ssize_t)header.size) {
        free(payload);
        return NULL;
    }
    
    payload[header.size] = '\0';
    return payload;
}

/* Simple JSON parsing helpers (no external dependencies!) */
static char* _find_json_value(const char *json, const char *key) {
    if (!json || !key) return NULL;
    
    char search[256];
    snprintf(search, sizeof(search), "\"%s\":", key);
    
    const char *found = strstr(json, search);
    if (!found) return NULL;
    
    found += strlen(search);
    while (*found == ' ' || *found == '\t') found++;
    
    if (*found == '"') {
        /* String value */
        found++;
        const char *end = strchr(found, '"');
        if (!end) return NULL;
        
        size_t len = end - found;
        char *result = malloc(len + 1);
        if (result) {
            memcpy(result, found, len);
            result[len] = '\0';
        }
        return result;
    } else if (*found >= '0' && *found <= '9') {
        /* Number value */
        const char *end = found;
        while (*end >= '0' && *end <= '9') end++;
        
        size_t len = end - found;
        char *result = malloc(len + 1);
        if (result) {
            memcpy(result, found, len);
            result[len] = '\0';
        }
        return result;
    } else if (strncmp(found, "true", 4) == 0) {
        return strdup("true");
    } else if (strncmp(found, "false", 5) == 0) {
        return strdup("false");
    }
    
    return NULL;
}

static bool _json_bool_value(const char *json, const char *key) {
    char *val = _find_json_value(json, key);
    if (!val) return false;
    bool result = strcmp(val, "true") == 0;
    free(val);
    return result;
}

static int _json_int_value(const char *json, const char *key) {
    char *val = _find_json_value(json, key);
    if (!val) return 0;
    int result = atoi(val);
    free(val);
    return result;
}

/* ============================================================================
 * PUBLIC API
 * ============================================================================ */

/*
 * Connect to the StarView compositor
 * Returns: IPC handle, or NULL on failure
 */
static StarViewIPC* starview_ipc_connect(void) {
    StarViewIPC *ipc = calloc(1, sizeof(StarViewIPC));
    if (!ipc) return NULL;
    
    /* Create socket */
    ipc->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (ipc->socket_fd < 0) {
        free(ipc);
        return NULL;
    }
    
    /* Connect to compositor */
    struct sockaddr_un addr = {0};
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, _get_socket_path(), sizeof(addr.sun_path) - 1);
    
    if (connect(ipc->socket_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        close(ipc->socket_fd);
        free(ipc);
        return NULL;
    }
    
    return ipc;
}

/*
 * Disconnect from the compositor
 */
static void starview_ipc_disconnect(StarViewIPC *ipc) {
    if (!ipc) return;
    if (ipc->socket_fd >= 0) close(ipc->socket_fd);
    free(ipc->read_buffer);
    free(ipc);
}

/*
 * Send a command to the compositor
 * Example: starview_ipc_command(ipc, "workspace 2")
 */
static bool starview_ipc_command(StarViewIPC *ipc, const char *cmd) {
    if (_send_message(ipc, IPC_COMMAND, cmd) < 0) return false;
    
    char *response = _receive_message(ipc, NULL);
    if (!response) return false;
    
    bool success = strstr(response, "\"success\":true") != NULL;
    free(response);
    return success;
}

/*
 * Switch to a specific workspace
 * Example: starview_ipc_switch_workspace(ipc, 3)
 */
static bool starview_ipc_switch_workspace(StarViewIPC *ipc, int workspace) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "workspace %d", workspace);
    return starview_ipc_command(ipc, cmd);
}

/*
 * Move focused window to a workspace
 * Example: starview_ipc_move_to_workspace(ipc, 5)
 */
static bool starview_ipc_move_to_workspace(StarViewIPC *ipc, int workspace) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "move_to_workspace %d", workspace);
    return starview_ipc_command(ipc, cmd);
}

/*
 * Close the focused window
 */
static bool starview_ipc_close_window(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "close");
}

/*
 * Toggle fullscreen for focused window
 */
static bool starview_ipc_toggle_fullscreen(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "fullscreen");
}

/*
 * Toggle floating mode for focused window
 */
static bool starview_ipc_toggle_floating(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "toggle_floating");
}

/*
 * Spawn a program
 * Example: starview_ipc_spawn(ipc, "kitty")
 */
static bool starview_ipc_spawn(StarViewIPC *ipc, const char *program) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "spawn %s", program);
    return starview_ipc_command(ipc, cmd);
}

/*
 * Get list of workspaces
 * Returns: Array of WorkspaceInfo, or NULL on failure
 * Note: Caller must free() the returned array
 */
static WorkspaceInfo* starview_ipc_get_workspaces(StarViewIPC *ipc, int *count) {
    if (_send_message(ipc, IPC_GET_WORKSPACES, NULL) < 0) return NULL;
    
    char *response = _receive_message(ipc, NULL);
    if (!response) return NULL;
    
    /* Count workspaces (count '{' characters in array) */
    int ws_count = 0;
    for (const char *p = response; *p; p++) {
        if (*p == '{') ws_count++;
    }
    
    if (ws_count == 0) {
        free(response);
        return NULL;
    }
    
    WorkspaceInfo *workspaces = calloc(ws_count, sizeof(WorkspaceInfo));
    if (!workspaces) {
        free(response);
        return NULL;
    }
    
    /* Parse each workspace (simple parser) */
    const char *ptr = response;
    for (int i = 0; i < ws_count; i++) {
        ptr = strchr(ptr, '{');
        if (!ptr) break;
        
        const char *end = strchr(ptr, '}');
        if (!end) break;
        
        size_t len = end - ptr + 1;
        char *ws_json = malloc(len + 1);
        memcpy(ws_json, ptr, len);
        ws_json[len] = '\0';
        
        workspaces[i].num = _json_int_value(ws_json, "num");
        workspaces[i].visible = _json_bool_value(ws_json, "visible");
        workspaces[i].focused = _json_bool_value(ws_json, "focused");
        workspaces[i].urgent = _json_bool_value(ws_json, "urgent");
        
        char *name = _find_json_value(ws_json, "name");
        if (name) {
            strncpy(workspaces[i].name, name, sizeof(workspaces[i].name) - 1);
            free(name);
        }
        
        free(ws_json);
        ptr = end + 1;
    }
    
    free(response);
    if (count) *count = ws_count;
    return workspaces;
}

/*
 * Get current (focused) workspace number
 * Returns: Workspace number (1-9), or -1 on failure
 */
static int starview_ipc_get_current_workspace(StarViewIPC *ipc) {
    int count = 0;
    WorkspaceInfo *workspaces = starview_ipc_get_workspaces(ipc, &count);
    if (!workspaces) return -1;
    
    int current = -1;
    for (int i = 0; i < count; i++) {
        if (workspaces[i].focused) {
            current = workspaces[i].num;
            break;
        }
    }
    
    free(workspaces);
    return current;
}

/*
 * Get compositor version
 * Returns: Version string, or NULL on failure
 * Note: Caller must free() the returned string
 */
static char* starview_ipc_get_version(StarViewIPC *ipc) {
    if (_send_message(ipc, IPC_GET_VERSION, NULL) < 0) return NULL;
    
    char *response = _receive_message(ipc, NULL);
    if (!response) return NULL;
    
    char *version = _find_json_value(response, "human_readable");
    free(response);
    return version;
}

/*
 * Reload compositor configuration
 */
static bool starview_ipc_reload_config(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "reload");
}

/*
 * Exit/quit the compositor
 */
static bool starview_ipc_exit(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "exit");
}

/*
 * Subscribe to events (advanced usage)
 * events: Bitmask of IPC_EVENT_* constants
 * Example: starview_ipc_subscribe(ipc, IPC_EVENT_WORKSPACE | IPC_EVENT_WINDOW)
 */
static bool starview_ipc_subscribe(StarViewIPC *ipc, uint32_t events) {
    char payload[256] = "[";
    bool first = true;
    
    if (events & IPC_EVENT_WORKSPACE) {
        strcat(payload, "\"workspace\"");
        first = false;
    }
    if (events & IPC_EVENT_WINDOW) {
        if (!first) strcat(payload, ",");
        strcat(payload, "\"window\"");
        first = false;
    }
    if (events & IPC_EVENT_MODE) {
        if (!first) strcat(payload, ",");
        strcat(payload, "\"mode\"");
    }
    
    strcat(payload, "]");
    
    if (_send_message(ipc, IPC_SUBSCRIBE, payload) < 0) return false;
    
    char *response = _receive_message(ipc, NULL);
    if (!response) return false;
    
    bool success = strstr(response, "\"success\":true") != NULL;
    free(response);
    return success;
}

/*
 * Wait for and receive an event (blocking)
 * Returns: Event type, or 0 on error
 * payload_out: Receives the JSON payload (caller must free)
 */
static uint32_t starview_ipc_wait_event(StarViewIPC *ipc, char **payload_out) {
    uint32_t type;
    char *payload = _receive_message(ipc, &type);
    if (!payload) return 0;
    
    if (payload_out) {
        *payload_out = payload;
    } else {
        free(payload);
    }
    
    /* Remove event mask bit to get actual event type */
    if (type & IPC_EVENT_MASK) {
        return type & ~IPC_EVENT_MASK;
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* STARVIEW_IPC_H */

/* ============================================================================
 * EXAMPLE PROGRAMS
 * ============================================================================ */

#ifdef STARVIEW_IPC_EXAMPLES

/*
 * Example 1: Simple workspace switcher
 * Compile: gcc -DSTARVIEW_IPC_EXAMPLES -DEXAMPLE_1 starview_ipc.h -o switch_ws
 */
#ifdef EXAMPLE_1
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc != 2) {
        printf("Usage: %s <workspace_number>\n", argv[0]);
        return 1;
    }
    
    StarViewIPC *ipc = starview_ipc_connect();
    if (!ipc) {
        fprintf(stderr, "Failed to connect to StarView\n");
        return 1;
    }
    
    int workspace = atoi(argv[1]);
    if (starview_ipc_switch_workspace(ipc, workspace)) {
        printf("Switched to workspace %d\n", workspace);
    } else {
        fprintf(stderr, "Failed to switch workspace\n");
    }
    
    starview_ipc_disconnect(ipc);
    return 0;
}
#endif

/*
 * Example 2: Show current workspace
 * Compile: gcc -DSTARVIEW_IPC_EXAMPLES -DEXAMPLE_2 starview_ipc.h -o current_ws
 */
#ifdef EXAMPLE_2
#include <stdio.h>

int main(void) {
    StarViewIPC *ipc = starview_ipc_connect();
    if (!ipc) {
        fprintf(stderr, "Failed to connect to StarView\n");
        return 1;
    }
    
    int current = starview_ipc_get_current_workspace(ipc);
    if (current >= 0) {
        printf("Current workspace: %d\n", current);
    } else {
        fprintf(stderr, "Failed to get current workspace\n");
    }
    
    starview_ipc_disconnect(ipc);
    return 0;
}
#endif

/*
 * Example 3: List all workspaces
 * Compile: gcc -DSTARVIEW_IPC_EXAMPLES -DEXAMPLE_3 starview_ipc.h -o list_ws
 */
#ifdef EXAMPLE_3
#include <stdio.h>

int main(void) {
    StarViewIPC *ipc = starview_ipc_connect();
    if (!ipc) {
        fprintf(stderr, "Failed to connect to StarView\n");
        return 1;
    }
    
    int count;
    WorkspaceInfo *workspaces = starview_ipc_get_workspaces(ipc, &count);
    
    if (workspaces) {
        printf("Workspaces:\n");
        for (int i = 0; i < count; i++) {
            printf("  %d: %s %s%s%s\n", 
                   workspaces[i].num,
                   workspaces[i].name,
                   workspaces[i].focused ? "[FOCUSED]" : "",
                   workspaces[i].visible ? "[VISIBLE]" : "",
                   workspaces[i].urgent ? "[URGENT]" : "");
        }
        free(workspaces);
    }
    
    starview_ipc_disconnect(ipc);
    return 0;
}
#endif

/*
 * Example 4: Workspace event monitor
 * Compile: gcc -DSTARVIEW_IPC_EXAMPLES -DEXAMPLE_4 starview_ipc.h -o monitor_ws
 */
#ifdef EXAMPLE_4
#include <stdio.h>

int main(void) {
    StarViewIPC *ipc = starview_ipc_connect();
    if (!ipc) {
        fprintf(stderr, "Failed to connect to StarView\n");
        return 1;
    }
    
    /* Subscribe to workspace events */
    if (!starview_ipc_subscribe(ipc, IPC_EVENT_WORKSPACE)) {
        fprintf(stderr, "Failed to subscribe to events\n");
        starview_ipc_disconnect(ipc);
        return 1;
    }
    
    printf("Monitoring workspace events (Ctrl+C to quit)...\n");
    
    while (1) {
        char *payload = NULL;
        uint32_t event_type = starview_ipc_wait_event(ipc, &payload);
        
        if (event_type == IPC_EVENT_WORKSPACE) {
            printf("Workspace event: %s\n", payload ? payload : "");
        }
        
        free(payload);
    }
    
    starview_ipc_disconnect(ipc);
    return 0;
}
#endif

/*
 * Example 5: Program launcher
 * Compile: gcc -DSTARVIEW_IPC_EXAMPLES -DEXAMPLE_5 starview_ipc.h -o launch
 */
#ifdef EXAMPLE_5
#include <stdio.h>

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s <program> [args...]\n", argv[0]);
        return 1;
    }
    
    StarViewIPC *ipc = starview_ipc_connect();
    if (!ipc) {
        fprintf(stderr, "Failed to connect to StarView\n");
        return 1;
    }
    
    /* Build command */
    char cmd[1024] = "";
    for (int i = 1; i < argc; i++) {
        strcat(cmd, argv[i]);
        if (i < argc - 1) strcat(cmd, " ");
    }
    
    if (starview_ipc_spawn(ipc, cmd)) {
        printf("Launched: %s\n", cmd);
    } else {
        fprintf(stderr, "Failed to launch program\n");
    }
    
    starview_ipc_disconnect(ipc);
    return 0;
}
#endif

/*
 * Example 6: Workspace status bar widget
 * Compile: gcc -DSTARVIEW_IPC_EXAMPLES -DEXAMPLE_6 starview_ipc.h -o workspace_widget
 */
#ifdef EXAMPLE_6
#include <stdio.h>
#include <unistd.h>

int main(void) {
    while (1) {
        StarViewIPC *ipc = starview_ipc_connect();
        if (ipc) {
            int current = starview_ipc_get_current_workspace(ipc);
            if (current >= 0) {
                /* Output format suitable for status bars */
                printf("WS:%d\n", current);
                fflush(stdout);
            }
            starview_ipc_disconnect(ipc);
        }
        sleep(1);
    }
    return 0;
}
#endif

#endif /* STARVIEW_IPC_EXAMPLES */
