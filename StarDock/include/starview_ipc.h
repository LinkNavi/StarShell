/*
 * starview_ipc.h - Header-only IPC client library for StarView compositor
 * FIXED FOR C++ COMPILATION
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

enum {
    IPC_COMMAND = 0,
    IPC_GET_WORKSPACES = 1,
    IPC_SUBSCRIBE = 2,
    IPC_GET_OUTPUTS = 3,
    IPC_GET_TREE = 4,
    IPC_GET_VERSION = 7,
};

enum {
    IPC_EVENT_WORKSPACE = (1 << 0),
    IPC_EVENT_WINDOW = (1 << 3),
    IPC_EVENT_MODE = (1 << 2),
};

#define IPC_EVENT_MASK 0x80000000

#pragma pack(push, 1)
typedef struct {
    char magic[6];
    uint32_t size;
    uint32_t type;
} IPCHeader;
#pragma pack(pop)

static char* _get_socket_path(void) {
    char *path = getenv("SWAYSOCK");
    if (!path || !*path) {
        path = (char*)"/tmp/starview-ipc.sock";
    }
    return path;
}

static int _send_message(StarViewIPC *ipc, uint32_t type, const char *payload) {
    if (!ipc || ipc->socket_fd < 0) return -1;
    
    IPCHeader header;
    memcpy(header.magic, "i3-ipc", 6);
    header.size = payload ? strlen(payload) : 0;
    header.type = type;
    
    if (write(ipc->socket_fd, &header, sizeof(header)) != sizeof(header)) {
        return -1;
    }
    
    if (payload && header.size > 0) {
        if (write(ipc->socket_fd, payload, header.size) != (ssize_t)header.size) {
            return -1;
        }
    }
    
    return 0;
}

static char* _receive_message(StarViewIPC *ipc, uint32_t *out_type) {
    if (!ipc || ipc->socket_fd < 0) return NULL;
    
    IPCHeader header;
    ssize_t n = read(ipc->socket_fd, &header, sizeof(header));
    if (n != sizeof(header)) {
        return NULL;
    }
    
    if (memcmp(header.magic, "i3-ipc", 6) != 0) {
        fprintf(stderr, "Invalid IPC magic\n");
        return NULL;
    }
    
    if (out_type) *out_type = header.type;
    
    if (header.size == 0) {
        return (char*)strdup("");
    }
    
    char *payload = (char*)malloc(header.size + 1);
    if (!payload) return NULL;
    
    n = read(ipc->socket_fd, payload, header.size);
    if (n != (ssize_t)header.size) {
        free(payload);
        return NULL;
    }
    
    payload[header.size] = '\0';
    return payload;
}

static char* _find_json_value(const char *json, const char *key) {
    if (!json || !key) return NULL;
    
    char search[256];
    snprintf(search, sizeof(search), "\"%s\":", key);
    
    const char *found = strstr(json, search);
    if (!found) return NULL;
    
    found += strlen(search);
    while (*found == ' ' || *found == '\t') found++;
    
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
    } else if (*found >= '0' && *found <= '9') {
        const char *end = found;
        while (*end >= '0' && *end <= '9') end++;
        
        size_t len = end - found;
        char *result = (char*)malloc(len + 1);
        if (result) {
            memcpy(result, found, len);
            result[len] = '\0';
        }
        return result;
    } else if (strncmp(found, "true", 4) == 0) {
        return (char*)strdup("true");
    } else if (strncmp(found, "false", 5) == 0) {
        return (char*)strdup("false");
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

static StarViewIPC* starview_ipc_connect(void) {
    StarViewIPC *ipc = (StarViewIPC*)calloc(1, sizeof(StarViewIPC));
    if (!ipc) return NULL;
    
    ipc->socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (ipc->socket_fd < 0) {
        free(ipc);
        return NULL;
    }
    
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

static void starview_ipc_disconnect(StarViewIPC *ipc) {
    if (!ipc) return;
    if (ipc->socket_fd >= 0) close(ipc->socket_fd);
    free(ipc->read_buffer);
    free(ipc);
}

static bool starview_ipc_command(StarViewIPC *ipc, const char *cmd) {
    if (_send_message(ipc, IPC_COMMAND, cmd) < 0) return false;
    
    char *response = _receive_message(ipc, NULL);
    if (!response) return false;
    
    bool success = strstr(response, "\"success\":true") != NULL;
    free(response);
    return success;
}

static bool starview_ipc_switch_workspace(StarViewIPC *ipc, int workspace) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "workspace %d", workspace);
    return starview_ipc_command(ipc, cmd);
}

static bool starview_ipc_move_to_workspace(StarViewIPC *ipc, int workspace) {
    char cmd[64];
    snprintf(cmd, sizeof(cmd), "move_to_workspace %d", workspace);
    return starview_ipc_command(ipc, cmd);
}

static bool starview_ipc_close_window(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "close");
}

static bool starview_ipc_toggle_fullscreen(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "fullscreen");
}

static bool starview_ipc_toggle_floating(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "toggle_floating");
}

static bool starview_ipc_spawn(StarViewIPC *ipc, const char *program) {
    char cmd[512];
    snprintf(cmd, sizeof(cmd), "spawn %s", program);
    return starview_ipc_command(ipc, cmd);
}

static WorkspaceInfo* starview_ipc_get_workspaces(StarViewIPC *ipc, int *count) {
    if (_send_message(ipc, IPC_GET_WORKSPACES, NULL) < 0) return NULL;
    
    char *response = _receive_message(ipc, NULL);
    if (!response) return NULL;
    
    int ws_count = 0;
    for (const char *p = response; *p; p++) {
        if (*p == '{') ws_count++;
    }
    
    if (ws_count == 0) {
        free(response);
        return NULL;
    }
    
    WorkspaceInfo *workspaces = (WorkspaceInfo*)calloc(ws_count, sizeof(WorkspaceInfo));
    if (!workspaces) {
        free(response);
        return NULL;
    }
    
    const char *ptr = response;
    for (int i = 0; i < ws_count; i++) {
        ptr = strchr(ptr, '{');
        if (!ptr) break;
        
        const char *end = strchr(ptr, '}');
        if (!end) break;
        
        size_t len = end - ptr + 1;
        char *ws_json = (char*)malloc(len + 1);
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

static char* starview_ipc_get_version(StarViewIPC *ipc) {
    if (_send_message(ipc, IPC_GET_VERSION, NULL) < 0) return NULL;
    
    char *response = _receive_message(ipc, NULL);
    if (!response) return NULL;
    
    char *version = _find_json_value(response, "human_readable");
    free(response);
    return version;
}

static bool starview_ipc_reload_config(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "reload");
}

static bool starview_ipc_exit(StarViewIPC *ipc) {
    return starview_ipc_command(ipc, "exit");
}

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

static uint32_t starview_ipc_wait_event(StarViewIPC *ipc, char **payload_out) {
    uint32_t type;
    char *payload = _receive_message(ipc, &type);
    if (!payload) return 0;
    
    if (payload_out) {
        *payload_out = payload;
    } else {
        free(payload);
    }
    
    if (type & IPC_EVENT_MASK) {
        return type & ~IPC_EVENT_MASK;
    }
    
    return 0;
}

#ifdef __cplusplus
}
#endif

#endif /* STARVIEW_IPC_H */
