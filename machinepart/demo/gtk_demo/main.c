#include "unistd.h"
#include <gtk/gtk.h>

#include "json_config.h"
#include "log.h"

char buffer_files[10][100];
int iter = 0;
GtkWidget *image = NULL;

int GtkTickCallback_clbk(GtkWidget *widget, GdkFrameClock *frame_clock,
                         gpointer user_data) {
  GtkWidget *frame = (GtkWidget *)user_data;

  if (image != NULL) {
    gtk_container_remove(GTK_CONTAINER(frame), image);
  }

  image = gtk_image_new_from_file(buffer_files[iter]);
  gtk_container_add(GTK_CONTAINER(frame), image);

  gtk_widget_show_all(frame);

  if (++iter == 10)
    iter = 0;
}

void *deal_time(void *user_data) {
  GtkWidget *frame = (GtkWidget *)user_data;

  if (image != NULL) {
    gtk_container_remove(GTK_CONTAINER(frame), image);
  }

  image = gtk_image_new_from_file(buffer_files[iter]);
  gtk_container_add(GTK_CONTAINER(frame), image);

  gtk_widget_show_all(frame);

  if (++iter == 10)
    iter = 0;
}

void activate(GtkApplication *app, gpointer user_data) {
  GtkWidget *window = gtk_application_window_new(app);
  GtkWidget *frame;
  gtk_window_set_title(GTK_WINDOW(window), "Drawing Area");

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_container_set_border_width(GTK_CONTAINER(window), 8);

  frame = gtk_frame_new(NULL);

  gtk_container_add(GTK_CONTAINER(window), frame);

  gtk_widget_set_size_request(frame, 350, 350);

  // call callback every tick, window update
  // gtk_widget_add_tick_callback(frame, GtkTickCallback_clbk, frame, NULL);

  g_timeout_add(1000, (GSourceFunc)deal_time, frame);

  gtk_widget_show_all(window);
}

int init_buffer_files() {
  DIR *d;
  struct dirent *dir;
  d = opendir(RSC_PATH);

  if (d == NULL) {
    print(ERROR, "cant open dir with resources RSC_PATH = %s", RSC_PATH);
    return 0;
  }
  int iter = 0;

  for (int i = 0; i < 10; i++) {
    memset(buffer_files[iter], '\0', 100);
    memcpy(buffer_files[iter++], RSC_PATH, strlen(RSC_PATH));
  }

  iter = 0;
  while ((dir = readdir(d)) != NULL) {
    if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
      continue;
    }
    memcpy(&buffer_files[iter++][strlen(RSC_PATH)], dir->d_name,
           strlen(dir->d_name));
    print(DEBUG, "file found: %s", dir->d_name);
  }

  closedir(d);

  return 0;
}

int main() {
  set_log_level(DEBUG);
  init_json_config(JSON_CONFIG_FILE);

  GtkApplication *app;

  init_buffer_files();

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  g_application_run(G_APPLICATION(app), 0, NULL);
  g_object_unref(app);

  return 0;
}
