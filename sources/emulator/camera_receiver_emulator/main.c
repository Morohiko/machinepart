#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <gtk/gtk.h>
#include <sys/socket.h>

#include <json_config.h>
#include <log.h>

#define MAX_MSG_SIZE 22500

pthread_mutex_t lock;
char buffer_file[MAX_MSG_SIZE];
GtkWidget *image = NULL;

int camera_receiver_logic(json_config_t *json_config)
{
    int sockfd;
    struct sockaddr_in servaddr;

    // socket create and verification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        hm_log_error("camera_receiver_emulator: socket creation failed...\n");
        exit(0);
    }

    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port =
            htons(json_config->modules.camera_transmitter_module.mp_port);
    int retval = 0;

    do {
        retval =
                connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
    } while (retval != 0);

    hm_log_info(
            "camera_receiver_emulator: connection accepted retval = %d, errno = %s\n",
            retval, strerror(errno));
    ssize_t max_size = MAX_MSG_SIZE;

    char *buffer = (char *)malloc(max_size);
    while (1) {
        ssize_t size = recv(sockfd, buffer, max_size, 0);
        pthread_mutex_lock(&lock);
        memcpy(buffer_file, buffer, size);
        pthread_mutex_unlock(&lock);
    }
    return 0;
}

int GtkTickCallback_clbk(GtkWidget *widget, GdkFrameClock *frame_clock,
                         gpointer user_data)
{
    GtkWidget *frame = (GtkWidget *)user_data;
    GdkPixbufLoader *loader;
    GdkPixbuf *pixbuf;
    GError *err = NULL;
    const char *formatt = "jpeg";
    loader = gdk_pixbuf_loader_new_with_type(formatt, &err);
    if (!loader) {
        hm_log_error(
                "camera_receiver_emulator: cannot create loader for %s: %s\n",
                formatt, err->message);
        return -1;
    }
    gsize length = MAX_MSG_SIZE;
    pthread_mutex_lock(&lock);
    gdk_pixbuf_loader_write(loader, (const char *)buffer_file, length,
                                     NULL);
    pthread_mutex_unlock(&lock);
    pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);

    if (image != NULL) {
        gtk_container_remove(GTK_CONTAINER(frame), image);
    }

    image = gtk_image_new_from_pixbuf(pixbuf);

    gtk_container_add(GTK_CONTAINER(frame), image);

    gtk_widget_show_all(frame);
    return 0;
}

void activate(GtkApplication *app, gpointer user_data)
{
    GtkWidget *window = gtk_application_window_new(app);
    GtkWidget *frame;
    gtk_window_set_title(GTK_WINDOW(window), "Drawing Area");

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    gtk_container_set_border_width(GTK_CONTAINER(window), 8);

    frame = gtk_frame_new(NULL);

    gtk_container_add(GTK_CONTAINER(window), frame);

    gtk_widget_set_size_request(frame, 350, 350);

    // call callback every tick, window update
    gtk_widget_add_tick_callback(frame, GtkTickCallback_clbk, frame, NULL);

    // g_timeout_add(1000,(GSourceFunc)deal_time, frame);

    gtk_widget_show_all(window);
}

void *run_gtk(void *user_data)
{
    GtkApplication *app;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
    return NULL;
}

int main()
{
    json_config_t *json_config = init_json_config(JSON_CONFIG_FILE);
    memset(buffer_file, '\0', MAX_MSG_SIZE);
    pthread_t thread_id;
    pthread_mutex_init(&lock, NULL);
    pthread_create(&thread_id, NULL, run_gtk, NULL);

    camera_receiver_logic(json_config);
    hm_log_info("camera_receiver_emulator: from camera receiver emulator\n");
    return 0;
}
