#include <errno.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <stdlib.h>

#include <json_config.h>
#include <log.h>

#define JSON_BUFFER_SIZE 3000

static json_config_t *json_config = NULL;

static int init_json_structure_module_main(cJSON *json,
                                           main_module_t *main_module)
{
    while (json != NULL) {
        if (strcmp(json->string, "id") == 0) {
            main_module->id = json->valueint;
        } else if (strcmp(json->string, "name") == 0) {
            if ((strlen(json->valuestring) >= MODULES_MAIN_NAME_STR_SIZE))
                goto error_and_exit;
            memcpy(main_module->name, json->valuestring,
                   strlen(json->valuestring));
        } else if (strcmp(json->string, "mp_ip") == 0) {
            if ((strlen(json->valuestring) >= MODULES_MAIN_MP_IP_STR_SIZE))
                goto error_and_exit;
            memcpy(main_module->mp_ip, json->valuestring,
                   strlen(json->valuestring));
        } else if (strcmp(json->string, "gp_ip") == 0) {
            if ((strlen(json->valuestring) >= MODULES_MAIN_GP_IP_STR_SIZE))
                goto error_and_exit;
            memcpy(main_module->gp_ip, json->valuestring,
                   strlen(json->valuestring));
        } else {
            hm_log_error("json_config: cant parse argument %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;

error_and_exit:
    hm_log_error(
            "json_config: can`t parse json_config: can`t init json_config->modules.main\n");
    return -1;
}

static int init_json_structure_module_camera(cJSON *json,
                                             camera_module_t *camera_module)
{
    while (json != NULL) {
        if (strcmp(json->string, "id") == 0) {
            camera_module->id = json->valueint;
        } else if (strcmp(json->string, "name") == 0) {
            if ((strlen(json->valuestring) >= MODULES_CAMERA_NAME_STR_SIZE))
                goto error_and_exit;
            memcpy(camera_module->name, json->valuestring,
                   strlen(json->valuestring));
        } else if (strcmp(json->string, "state") == 0) {
            camera_module->state = json->valueint;
        } else if (strcmp(json->string, "frame_width") == 0) {
            camera_module->frame_width = json->valueint;
        } else if (strcmp(json->string, "frame_height") == 0) {
            camera_module->frame_height = json->valueint;
        } else if (strcmp(json->string, "frame_elem_size") == 0) {
            camera_module->frame_elem_size = json->valueint;
        } else {
            hm_log_error("json_config: cant parse argument %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;

error_and_exit:
    hm_log_error("json_config: can`t init json_config->modules.camera\n");
    return -1;
}

static int init_json_structure_module_camera_transmitter(
        cJSON *json, camera_transmitter_module_t *camera_transmitter_module)
{
    while (json != NULL) {
        if (strcmp(json->string, "id") == 0) {
            camera_transmitter_module->id = json->valueint;
        } else if (strcmp(json->string, "name") == 0) {
            memcpy(camera_transmitter_module->name, json->valuestring,
                   strlen(json->valuestring));
        } else if (strcmp(json->string, "state") == 0) {
            camera_transmitter_module->state = json->valueint;
        } else if (strcmp(json->string, "mp_port") == 0) {
            camera_transmitter_module->mp_port = json->valueint;
        } else if (strcmp(json->string, "gp_port") == 0) {
            camera_transmitter_module->gp_port = json->valueint;
        } else if (strcmp(json->string, "local_ack_port") == 0) {
            camera_transmitter_module->local_ack_port = json->valueint;
        } else if (strcmp(json->string, "target_ack_port") == 0) {
            camera_transmitter_module->target_ack_port = json->valueint;
        } else {
            hm_log_error("json_config: cant parse argument %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;
}

static int init_json_structure_module_gyroscope_receiver(
        cJSON *json, gyroscope_receiver_module_t *gyroscope_receiver_module)
{
    while (json != NULL) {
        if (strcmp(json->string, "id") == 0) {
            gyroscope_receiver_module->id = json->valueint;
        } else if (strcmp(json->string, "name") == 0) {
            memcpy(gyroscope_receiver_module->name, json->valuestring,
                   strlen(json->valuestring));
        } else if (strcmp(json->string, "state") == 0) {
            gyroscope_receiver_module->state = json->valueint;
        } else if (strcmp(json->string, "mp_port") == 0) {
            gyroscope_receiver_module->mp_port = json->valueint;
        } else if (strcmp(json->string, "gp_port") == 0) {
            gyroscope_receiver_module->gp_port = json->valueint;
        } else {
            hm_log_error("json_config: cant parse argument %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;
}

static int init_json_structure_module_motor(cJSON *json,
                                            motor_module_t *motor_module)
{
    while (json != NULL) {
        if (strcmp(json->string, "id") == 0) {
            motor_module->id = json->valueint;
        } else if (strcmp(json->string, "name") == 0) {
            memcpy(motor_module->name, json->valuestring,
                   strlen(json->valuestring));
        } else if (strcmp(json->string, "state") == 0) {
            motor_module->state = json->valueint;
        } else if (strcmp(json->string, "motor_delay_ms") == 0) {
            motor_module->motor_delay_ms = json->valueint;
        } else if (strcmp(json->string, "motor_x_pin") == 0) {
            motor_module->motor_x_pin = json->valueint;
        } else if (strcmp(json->string, "motor_y_pin") == 0) {
            motor_module->motor_y_pin = json->valueint;
        } else {
            hm_log_error("json_config: cant parse argument %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;
}

static int init_json_structure_module_remote_controller(
        cJSON *json, remote_controller_module_t *remote_controller_module)
{
    while (json != NULL) {
        if (strcmp(json->string, "id") == 0) {
            remote_controller_module->id = json->valueint;
        } else if (strcmp(json->string, "name") == 0) {
            memcpy(remote_controller_module->name, json->valuestring,
                   strlen(json->valuestring));
        } else {
            hm_log_error("json_config: cant parse argument %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;
}

static int init_json_structure_modules(cJSON *json, modules_t *modules)
{
    while (json != NULL) {
        if (strcmp(json->string, "MAIN") == 0) {
            init_json_structure_module_main(json->child,
                                                  &modules->main_module);
        } else if (strcmp(json->string, "CAMERA") == 0) {
            init_json_structure_module_camera(json->child,
                                                    &modules->camera_module);
        } else if (strcmp(json->string, "CAMERA_TRANSMITTER") == 0) {
            init_json_structure_module_camera_transmitter(
                    json->child, &modules->camera_transmitter_module);
        } else if (strcmp(json->string, "GYROSCOPE_RECEIVER") == 0) {
            init_json_structure_module_gyroscope_receiver(
                    json->child, &modules->gyroscope_receiver_module);
        } else if (strcmp(json->string, "MOTOR") == 0) {
            init_json_structure_module_motor(json->child,
                                                   &modules->motor_module);
        } else if (strcmp(json->string, "REMOTE_CONTROLLER") == 0) {
            init_json_structure_module_remote_controller(
                    json->child, &modules->remote_controller_module);
        } else {
            hm_log_error("json_config: cant parse module %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;
}

static int init_json_structure_shell(cJSON *json, shell_t *shell)
{
    while (json != NULL) {
        if (strcmp(json->string, "state") == 0) {
            shell->state = json->valueint;
        } else if (strcmp(json->string, "mgmt_port") == 0) {
            shell->mgmt_port = json->valueint;
        } else if (strcmp(json->string, "port") == 0) {
            shell->port = json->valueint;
        } else if (strcmp(json->string, "buffer_size") == 0) {
            shell->buffer_size = json->valueint;
        } else if (strcmp(json->string, "local_shell_client") == 0) {
            shell->local_shell_client = json->valueint;
        } else {
            hm_log_error("json_config: cant parse module %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;
}

static int init_json_structure_fps_check(cJSON *json, fps_check_t *fps_check)
{
    while (json != NULL) {
        if (strcmp(json->string, "state") == 0) {
            fps_check->state = json->valueint;
        } else if (strcmp(json->string, "time_to_update") == 0) {
            fps_check->time_to_update = json->valueint;
        } else {
            hm_log_error("json_config: cant parse module %s\n", json->string);
            return -1;
        }
        json = json->next;
    }
    return 0;
}

static int init_json_structure(cJSON *json, json_config_t *json_config)
{
    cJSON *json_next = json->child;
    while (json_next != NULL) {
        if (strcmp(json_next->string, "MODULES") == 0) {
            init_json_structure_modules(json_next->child,
                                              &json_config->modules);
        } else if (strcmp(json_next->string, "SHELL") == 0) {
            init_json_structure_shell(json_next->child,
                                            &json_config->shell);
        } else if (strcmp(json_next->string, "FPS_CHECK") == 0) {
            init_json_structure_fps_check(json_next->child,
                                                &json_config->fps_check);
        }
        json_next = json_next->next;
    }
    return 0;
}

json_config_t *init_json_config(char *json_filepath)
{
    int ret = 0;
    if (json_config) {
        hm_log_error("json_config: json config insance exist\n");
        return NULL;
    }
    char json_buffer[JSON_BUFFER_SIZE];

    FILE *file = fopen(json_filepath, "r");
    if (!file) {
        hm_log_error("json_config: file is unavailable, json_filepath = %s\n",
                     json_filepath);
        return NULL;
    }

    int iter = 0;
    int c;
    while ((c = getc(file)) != EOF) {
        json_buffer[iter++] = c;
    }
    fclose(file);

    cJSON *json = cJSON_Parse(json_buffer);
    if (!json) {
        hm_log_error("json_config: json is null, errno = %s, filepath = %s\n",
                     strerror(errno), json_filepath);
        return NULL;
    }

    json_config = (json_config_t *)malloc(sizeof(json_config_t));
    if (!json_config) {
        hm_log_error("json_config: can`t allocate memory\n");
        return NULL;
    }
    ret = init_json_structure(json, json_config);
    if (ret) {
        hm_log_error("json_config: issue while create json structure\n");
        free(json_config);
        return NULL;
    }
    return json_config;
}

int destroy_json_config(json_config_t *json_config)
{
    if (!json_config) {
        hm_log_error("json_config: can`t destroy, instance is not exist\n");
        return -1;
    }
    free(json_config);
    return 0;
}

void config_start_gyroscope_receiver(void *data)
{
    if (json_config->modules.gyroscope_receiver_module.state == 1) {
        hm_log_info("json_config: gyroscop receiver module already run\n");
        return;
    }
    json_config->modules.gyroscope_receiver_module.state = 1;
}

void config_stop_gyroscope_receiver(void *data)
{
    if (json_config->modules.gyroscope_receiver_module.state == 0) {
        hm_log_info("json_config: gyroscop receiver module already stopped\n");
        return;
    }
    json_config->modules.gyroscope_receiver_module.state = 0;
}

void config_start_motor_controller(void *data)
{
    if (json_config->modules.motor_module.state == 1) {
        hm_log_info("json_config: motor module already run\n");
        return;
    }
    json_config->modules.motor_module.state = 1;
}

void config_stop_motor_controller(void *data)
{
    if (json_config->modules.motor_module.state == 0) {
        hm_log_info("json_config: motor module already stopped\n");
        return;
    }
    json_config->modules.motor_module.state = 0;
}

void config_start_camera_module(void *data)
{
    if (json_config->modules.camera_module.state == 1) {
        hm_log_info("json_config: camera module already run\n");
        return;
    }
    json_config->modules.camera_module.state = 1;
}

void config_stop_camera_module(void *data)
{
    if (json_config->modules.camera_module.state == 0) {
        hm_log_info("json_config: camera module already stopped\n");
        return;
    }
    json_config->modules.camera_module.state = 0;
}

void config_start_camera_transmitter(void *data)
{
    if (json_config->modules.camera_transmitter_module.state == 1) {
        hm_log_info("json_config: camera transmitter module already run\n");
        return;
    }
    json_config->modules.camera_transmitter_module.state = 1;
}

void print_json_config()
{
    printf("modules:\n");

    printf("\tmain_module:\n");
    printf("\t\tid: %d\n", json_config->modules.main_module.id);
    printf("\t\tname: %s\n", json_config->modules.main_module.name);
    printf("\t\tmp_ip: %s\n", json_config->modules.main_module.mp_ip);
    printf("\t\tgp_ip: %s\n", json_config->modules.main_module.gp_ip);

    printf("\tcamera_module:\n");
    printf("\t\tid: %d\n", json_config->modules.camera_module.id);
    printf("\t\tname: %s\n", json_config->modules.camera_module.name);
    printf("\t\tstate: %d\n", json_config->modules.camera_module.state);
    printf("\t\tframe_width: %d\n",
           json_config->modules.camera_module.frame_width);
    printf("\t\tframe_height: %d\n",
           json_config->modules.camera_module.frame_height);
    printf("\t\tframe_elem_size: %d\n",
           json_config->modules.camera_module.frame_elem_size);

    printf("\tcamera_transmitter_module:\n");
    printf("\t\tid: %d\n", json_config->modules.camera_transmitter_module.id);
    printf("\t\tname: %s\n",
           json_config->modules.camera_transmitter_module.name);
    printf("\t\tstate: %d\n",
           json_config->modules.camera_transmitter_module.state);
    printf("\t\tmp_port: %d\n",
           json_config->modules.camera_transmitter_module.mp_port);
    printf("\t\tgp_port: %d\n",
           json_config->modules.camera_transmitter_module.gp_port);
    printf("\t\tlocal_ack_port: %d\n",
           json_config->modules.camera_transmitter_module.local_ack_port);
    printf("\t\ttarget_ack_port: %d\n",
           json_config->modules.camera_transmitter_module.target_ack_port);

    printf("\tgyroscope_receiver_module:\n");
    printf("\t\tid: %d\n", json_config->modules.gyroscope_receiver_module.id);
    printf("\t\tname: %s\n",
           json_config->modules.gyroscope_receiver_module.name);
    printf("\t\tstate: %d\n",
           json_config->modules.gyroscope_receiver_module.state);
    printf("\t\tmp_port: %d\n",
           json_config->modules.gyroscope_receiver_module.mp_port);
    printf("\t\tgp_port: %d\n",
           json_config->modules.gyroscope_receiver_module.gp_port);

    printf("\tmotor_module:\n");
    printf("\t\tid: %d\n", json_config->modules.motor_module.id);
    printf("\t\tname: %s\n", json_config->modules.motor_module.name);
    printf("\t\tstate: %d\n", json_config->modules.motor_module.state);
    printf("\t\tmotor_delay_ms: %d\n",
           json_config->modules.motor_module.motor_delay_ms);
    printf("\t\tmotor_x_pin: %d\n",
           json_config->modules.motor_module.motor_x_pin);
    printf("\t\tmotor_y_pin: %d\n",
           json_config->modules.motor_module.motor_y_pin);

    printf("\tremote_controller_module:\n");
    printf("\t\tid: %d\n", json_config->modules.remote_controller_module.id);
    printf("\t\tname: %s\n",
           json_config->modules.remote_controller_module.name);

    printf("shell:\n");
    printf("\tstate: %d\n", json_config->shell.state);
    printf("\tmgmt_port: %d\n", json_config->shell.mgmt_port);
    printf("\tport: %d\n", json_config->shell.port);
    printf("\tbuffer_size: %d\n", json_config->shell.buffer_size);
    printf("\tlocal_shell_client: %d\n", json_config->shell.local_shell_client);

    printf("fps_check:\n");
    printf("\tstate: %d\n", json_config->fps_check.state);
    printf("\ttime_to_update: %d\n", json_config->fps_check.time_to_update);
}
