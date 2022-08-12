#include "errno.h"
#include "string.h"
#include <cjson/cJSON.h>
#include <stdlib.h>

#include "json_config.h"
#include "log.h"

cJSON *json;

#define JSON_BUFFER_SIZE 3000

int init_json_structure_module_main(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "id") == 0) {
      json_config.modules.main_module.id = json->valueint;
    } else if (strcmp(json->string, "name") == 0) {
      json_config.modules.main_module.name =
          (char *)malloc(strlen(json->valuestring));
      memcpy(json_config.modules.main_module.name, json->valuestring,
             strlen(json->valuestring));
    } else if (strcmp(json->string, "local_ip") == 0) {
      json_config.modules.main_module.local_ip =
          (char *)malloc(strlen(json->valuestring));
      memcpy(json_config.modules.main_module.local_ip, json->valuestring,
             strlen(json->valuestring));
    } else if (strcmp(json->string, "target_ip") == 0) {
      json_config.modules.main_module.target_ip =
          (char *)malloc(strlen(json->valuestring));
      memcpy(json_config.modules.main_module.target_ip, json->valuestring,
             strlen(json->valuestring));

    } else {
      print(ERROR, "cant parse argument %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure_module_camera(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "id") == 0) {
      json_config.modules.camera_module.id = json->valueint;
    } else if (strcmp(json->string, "name") == 0) {
      json_config.modules.camera_module.name =
          (char *)malloc(strlen(json->valuestring));
      memcpy(json_config.modules.camera_module.name, json->valuestring,
             strlen(json->valuestring));
    } else if (strcmp(json->string, "state") == 0) {
      json_config.modules.camera_module.state = json->valueint;
    } else if (strcmp(json->string, "frame_width") == 0) {
      json_config.modules.camera_module.frame_width = json->valueint;
    } else if (strcmp(json->string, "frame_height") == 0) {
      json_config.modules.camera_module.frame_height = json->valueint;
    } else if (strcmp(json->string, "frame_elem_size") == 0) {
      json_config.modules.camera_module.frame_elem_size = json->valueint;

    } else {
      print(ERROR, "cant parse argument %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure_module_camera_transmitter(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "id") == 0) {
      json_config.modules.camera_transmitter_module.id = json->valueint;
    } else if (strcmp(json->string, "name") == 0) {
      json_config.modules.camera_transmitter_module.name =
          (char *)malloc(strlen(json->valuestring));
      memcpy(json_config.modules.camera_transmitter_module.name,
             json->valuestring, strlen(json->valuestring));
    } else if (strcmp(json->string, "state") == 0) {
      json_config.modules.camera_transmitter_module.state = json->valueint;
    } else if (strcmp(json->string, "local_port") == 0) {
      json_config.modules.camera_transmitter_module.local_port = json->valueint;
    } else if (strcmp(json->string, "target_port") == 0) {
      json_config.modules.camera_transmitter_module.target_port =
          json->valueint;
    } else if (strcmp(json->string, "local_ack_port") == 0) {
      json_config.modules.camera_transmitter_module.local_ack_port =
          json->valueint;
    } else if (strcmp(json->string, "target_ack_port") == 0) {
      json_config.modules.camera_transmitter_module.target_ack_port =
          json->valueint;
    } else {
      print(ERROR, "cant parse argument %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure_module_gyroscope_receiver(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "id") == 0) {
      json_config.modules.gyroscope_receiver_module.id = json->valueint;
    } else if (strcmp(json->string, "name") == 0) {
      json_config.modules.gyroscope_receiver_module.name =
          (char *)malloc(strlen(json->valuestring));
      memcpy(json_config.modules.gyroscope_receiver_module.name,
             json->valuestring, strlen(json->valuestring));
    } else if (strcmp(json->string, "state") == 0) {
      json_config.modules.gyroscope_receiver_module.state = json->valueint;
    } else if (strcmp(json->string, "local_port") == 0) {
      json_config.modules.gyroscope_receiver_module.local_port = json->valueint;
    } else if (strcmp(json->string, "target_port") == 0) {
      json_config.modules.gyroscope_receiver_module.target_port =
          json->valueint;
    } else {
      print(ERROR, "cant parse argument %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure_module_motor(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "id") == 0) {
      json_config.modules.motor_module.id = json->valueint;
    } else if (strcmp(json->string, "name") == 0) {
      json_config.modules.motor_module.name =
          (char *)malloc(strlen(json->valuestring));
      memcpy(json_config.modules.motor_module.name, json->valuestring,
             strlen(json->valuestring));
    } else if (strcmp(json->string, "state") == 0) {
      json_config.modules.motor_module.state = json->valueint;
    } else if (strcmp(json->string, "motor_delay_ms") == 0) {
      json_config.modules.motor_module.motor_delay_ms = json->valueint;
    } else if (strcmp(json->string, "motor_x_gpio_pin") == 0) {
      json_config.modules.motor_module.motor_x_gpio_pin = json->valueint;
    } else if (strcmp(json->string, "motor_y_gpio_pin") == 0) {
      json_config.modules.motor_module.motor_y_gpio_pin = json->valueint;
    } else {
      print(ERROR, "cant parse argument %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure_module_remote_controller(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "id") == 0) {
      json_config.modules.remote_controller_module.id = json->valueint;
    } else if (strcmp(json->string, "name") == 0) {
      json_config.modules.remote_controller_module.name =
          (char *)malloc(strlen(json->valuestring));
      memcpy(json_config.modules.remote_controller_module.name,
             json->valuestring, strlen(json->valuestring));
    } else {
      print(ERROR, "cant parse argument %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure_modules(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "MAIN") == 0) {
      ret = init_json_structure_module_main(json->child);
    } else if (strcmp(json->string, "CAMERA") == 0) {
      ret = init_json_structure_module_camera(json->child);
    } else if (strcmp(json->string, "CAMERA_TRANSMITTER") == 0) {
      ret = init_json_structure_module_camera_transmitter(json->child);
    } else if (strcmp(json->string, "GYROSCOPE_RECEIVER") == 0) {
      ret = init_json_structure_module_gyroscope_receiver(json->child);
    } else if (strcmp(json->string, "MOTOR") == 0) {
      ret = init_json_structure_module_motor(json->child);
    } else if (strcmp(json->string, "REMOTE_CONTROLLER") == 0) {
      ret = init_json_structure_module_remote_controller(json->child);
    } else {
      print(ERROR, "cant parse module %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure_shell(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "state") == 0) {
      json_config.shell.state = json->valueint;
    } else if (strcmp(json->string, "mgmt_port") == 0) {
      json_config.shell.mgmt_port = json->valueint;
    } else if (strcmp(json->string, "port") == 0) {
      json_config.shell.port = json->valueint;
    } else if (strcmp(json->string, "buffer_size") == 0) {
      json_config.shell.buffer_size = json->valueint;
    } else if (strcmp(json->string, "local_shell_client") == 0) {
      json_config.shell.local_shell_client = json->valueint;
    } else {
      print(ERROR, "cant parse module %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure_fps_checker(cJSON *json) {
  int ret = 0;
  while (json != NULL) {
    if (strcmp(json->string, "state") == 0) {
      json_config.fps_checker.state = json->valueint;
    } else if (strcmp(json->string, "time_to_update") == 0) {
      json_config.fps_checker.time_to_update = json->valueint;
    } else {
      print(ERROR, "cant parse module %s", json->string);
      return -1;
    }
    json = json->next;
  }
  return ret;
}

int init_json_structure() {
  if (json == NULL) {
    print(ERROR, "cjson is null?");
  }
  cJSON *json_next = json->child;
  int ret = 0;
  while (json_next != NULL) {
    if (strcmp(json_next->string, "MODULES") == 0) {
      ret = init_json_structure_modules(json_next->child);
    } else if (strcmp(json_next->string, "SHELL") == 0) {
      ret = init_json_structure_shell(json_next->child);
    } else if (strcmp(json_next->string, "FPS_CHECKER") == 0) {
      ret = init_json_structure_fps_checker(json_next->child);
    }
    json_next = json_next->next;
  }
  return ret;
}

void init_json_config(char *json_filepath) {
  int ret = 0;
  char json_buffer[JSON_BUFFER_SIZE];
  int c;
  FILE *file;
  file = fopen(json_filepath, "r");
  if (file == NULL) {
    print(ERROR, "file is unavailable, json_filepath = %s", json_filepath);
    return;
  }
  int iter = 0;
  while ((c = getc(file)) != EOF) {
    json_buffer[iter++] = c;
  }
  fclose(file);

  json = cJSON_Parse(json_buffer);
  if (json == NULL) {
    print(ERROR, "json is null, errno = %s, filepath = %s", strerror(errno),
          json_filepath);
    return;
  }
  ret = init_json_structure();
  if (ret != 0) {
    print(ERROR, "issue while create json structure");
  }
}

void start_gyroscope_receiver(void *data) {
  if (json_config.modules.gyroscope_receiver_module.state == 1) {
    print(INFO, "gyroscop receiver module already run");
    return;
  }
  json_config.modules.gyroscope_receiver_module.state = 1;
}

void stop_gyroscope_receiver(void *data) {
  if (json_config.modules.gyroscope_receiver_module.state == 0) {
    print(INFO, "gyroscop receiver module already stopped");
    return;
  }
  json_config.modules.gyroscope_receiver_module.state = 0;
}

void start_motor_controller(void *data) {
  if (json_config.modules.motor_module.state == 1) {
    print(INFO, "motor module already run");
    return;
  }
  json_config.modules.motor_module.state = 1;
}

void stop_motor_controller(void *data) {
  if (json_config.modules.motor_module.state == 0) {
    print(INFO, "motor module already stopped");
    return;
  }
  json_config.modules.motor_module.state = 0;
}

void start_camera_module(void *data) {
  if (json_config.modules.camera_module.state == 1) {
    print(INFO, "camera module already run");
    return;
  }
  json_config.modules.camera_module.state = 1;
}

void stop_camera_module(void *data) {
  if (json_config.modules.camera_module.state == 0) {
    print(INFO, "camera module already stopped");
    return;
  }
  json_config.modules.camera_module.state = 0;
}

void start_camera_transmitter(void *data) {
  if (json_config.modules.camera_transmitter_module.state == 1) {
    print(INFO, "camera transmitter module already run");
    return;
  }
  json_config.modules.camera_transmitter_module.state = 1;
}

void print_json_config() {
  printf("modules:\n");

  printf("\tmain_module:\n");
  printf("\t\tid: %d\n", json_config.modules.main_module.id);
  printf("\t\tname: %s\n", json_config.modules.main_module.name);
  printf("\t\tlocal_ip: %s\n", json_config.modules.main_module.local_ip);
  printf("\t\ttarget_ip: %s\n", json_config.modules.main_module.target_ip);

  printf("\tcamera_module:\n");
  printf("\t\tid: %d\n", json_config.modules.camera_module.id);
  printf("\t\tname: %s\n", json_config.modules.camera_module.name);
  printf("\t\tstate: %d\n", json_config.modules.camera_module.state);
  printf("\t\tframe_width: %d\n",
         json_config.modules.camera_module.frame_width);
  printf("\t\tframe_height: %d\n",
         json_config.modules.camera_module.frame_height);
  printf("\t\tframe_elem_size: %d\n",
         json_config.modules.camera_module.frame_elem_size);

  printf("\tcamera_transmitter_module:\n");
  printf("\t\tid: %d\n", json_config.modules.camera_transmitter_module.id);
  printf("\t\tname: %s\n", json_config.modules.camera_transmitter_module.name);
  printf("\t\tstate: %d\n",
         json_config.modules.camera_transmitter_module.state);
  printf("\t\tlocal_port: %d\n",
         json_config.modules.camera_transmitter_module.local_port);
  printf("\t\ttarget_port: %d\n",
         json_config.modules.camera_transmitter_module.target_port);
  printf("\t\tlocal_ack_port: %d\n",
         json_config.modules.camera_transmitter_module.local_ack_port);
  printf("\t\ttarget_ack_port: %d\n",
         json_config.modules.camera_transmitter_module.target_ack_port);

  printf("\tgyroscope_receiver_module:\n");
  printf("\t\tid: %d\n", json_config.modules.gyroscope_receiver_module.id);
  printf("\t\tname: %s\n", json_config.modules.gyroscope_receiver_module.name);
  printf("\t\tstate: %d\n",
         json_config.modules.gyroscope_receiver_module.state);
  printf("\t\tlocal_port: %d\n",
         json_config.modules.gyroscope_receiver_module.local_port);
  printf("\t\ttarget_port: %d\n",
         json_config.modules.gyroscope_receiver_module.target_port);

  printf("\tmotor_module:\n");
  printf("\t\tid: %d\n", json_config.modules.motor_module.id);
  printf("\t\tname: %s\n", json_config.modules.motor_module.name);
  printf("\t\tstate: %d\n", json_config.modules.motor_module.state);
  printf("\t\tmotor_delay_ms: %d\n",
         json_config.modules.motor_module.motor_delay_ms);
  printf("\t\tmotor_x_gpio_pin: %d\n",
         json_config.modules.motor_module.motor_x_gpio_pin);
  printf("\t\tmotor_y_gpio_pin: %d\n",
         json_config.modules.motor_module.motor_y_gpio_pin);

  printf("\tremote_controller_module:\n");
  printf("\t\tid: %d\n", json_config.modules.remote_controller_module.id);
  printf("\t\tname: %s\n", json_config.modules.remote_controller_module.name);

  printf("shell:\n");
  printf("\tstate: %d\n", json_config.shell.state);
  printf("\tmgmt_port: %d\n", json_config.shell.mgmt_port);
  printf("\tport: %d\n", json_config.shell.port);
  printf("\tbuffer_size: %d\n", json_config.shell.buffer_size);
  printf("\tlocal_shell_client: %d\n", json_config.shell.local_shell_client);

  printf("fps_checker:\n");
  printf("\tstate: %d\n", json_config.fps_checker.state);
  printf("\ttime_to_update: %d\n", json_config.fps_checker.time_to_update);
}
