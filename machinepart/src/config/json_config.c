#include "errno.h"
#include "string.h"
#include <cjson/cJSON.h>
#include <stdlib.h>

#include "json_config.h"
#include "log.h"

cJSON *json;

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
    }
    json_next = json_next->next;
  }
  return ret;
}

void init_json_config(char *json_filepath) {
  int ret = 0;
  char json_buffer[1000];
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
    printf("ERROR: json is null, errno = %s\n", strerror(errno));
    return;
  }
  ret = init_json_structure();
  if (ret != 0) {
    print(ERROR, "issue while create json structure");
  }
}
