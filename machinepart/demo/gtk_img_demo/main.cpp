#include <gtk/gtk.h>
#include <cerrno>

static void
sanity_check(const gchar *filename)
{
    GdkPixbufLoader *loader;
    GSList *formats, *l;
    GError *err = NULL;
    GdkPixbuf *pixbuf;
    gchar *buf = NULL;
    gsize size;
    FILE *fh;
    gboolean ok, fileok = TRUE;

    g_printerr("Performing sanity check for %s.\n", filename);
    fileok &= ok = g_file_test(filename, G_FILE_TEST_EXISTS);
    g_printerr("Does it exist: %s\n", ok ? "YES" : "NO");
    fileok &= ok = g_file_test(filename, G_FILE_TEST_IS_REGULAR);
    g_printerr("Is it a regular file: %s\n", ok ? "YES" : "NO");

    fh = fopen(filename, "rb");
    fileok &= ok = !!fh;
    g_printerr("Can we open it for reading: %s\n", ok ? "YES" : "NO");
    if (fh)
        fclose(fh);
    else
        g_printerr("fopen() fails with: %s\n", g_strerror(errno));

    fileok &= ok = g_file_get_contents(filename, &buf, &size, &err);
    g_printerr("Can we use g_file_get_contents(): %s\n", ok ? "YES" : "NO");
    if (!ok) {
        g_printerr("g_file_get_contents() fails with: %s\n", err->message);
        g_clear_error(&err);
    }

    if (!fileok) {
        if (buf)
            g_free(buf);
        g_printerr("The file does not seem OK.  No point continuing.\n");
        return;
    }
    g_printerr("The file seems OK, continuing checks.\n");

    pixbuf = gdk_pixbuf_new_from_file(filename, &err);
    if (pixbuf) {
        g_printerr("Apparently we can load the pixbuf (%dx%d) now?!\n",
                   gdk_pixbuf_get_width(pixbuf),
                   gdk_pixbuf_get_height(pixbuf));
        g_printerr("What has changed?\n");
        g_printerr("This is completely fucked up.\n");
        g_object_unref(pixbuf);
    }
    else {
        g_printerr("gdk_pixbuf_new_from_file() fails with: %s\n", err->message);
        g_clear_error(&err);
    }

    g_printerr("Checking the pixbuf loaders.\n");

    formats = gdk_pixbuf_get_formats();
    for (l = formats; l; l = g_slist_next(l)) {
        GdkPixbufFormat *pixbuf_format = (GdkPixbufFormat*)l->data;
        gchar **ext;
        gchar *fmtname, *desc, *exts;

        fmtname = gdk_pixbuf_format_get_name(pixbuf_format);
		printf("fmtname = %s\n", fmtname);
		if (strcmp(fmtname, "jpeg")) {
			continue;
		}
        desc = gdk_pixbuf_format_get_description(pixbuf_format);
        ext = gdk_pixbuf_format_get_extensions(pixbuf_format);
        exts = g_strjoinv(" ", ext);
        g_printerr("Found format %s: %s (%s)\n", fmtname, desc, exts);
        g_free(exts);
        g_strfreev(ext);
        g_free(desc);

        // format_check(pixbuf_format, buf, size);

        loader = gdk_pixbuf_loader_new_with_type(fmtname, &err);
        if (!loader) {
            g_printerr("  Cannot create loader for %s: %s\n",
                       fmtname, err->message);
            g_clear_error(&err);
            g_free(fmtname);
            continue;
        }

        ok = gdk_pixbuf_loader_write(loader, (const unsigned char*)buf, size, &err);
        if (ok) {
            g_printerr("  Loader %s accepts the file content.\n", fmtname);
            ok = gdk_pixbuf_loader_close(loader, &err);
            if (ok) {
                g_printerr("  Loader %s accepts the entire file.\n", fmtname);
                pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
                if (pixbuf) {
                    g_printerr("  Obtained pixbuf %dx%d from the loader.\n",
                               gdk_pixbuf_get_width(pixbuf),
                               gdk_pixbuf_get_height(pixbuf));
                }
                else {
                    g_printerr("  Cannot obtain pixbuf from the loader.\n");
                }
            }
            else {
                g_printerr("  Loader %s fails at close(): %s.\n",
                           fmtname, err->message);
                g_clear_error(&err);
            }
        }
        else {
            g_printerr("  Loader %s does not accept the file content: %s\n",
                       fmtname, err->message);
            g_clear_error(&err);
        }
        g_object_unref(loader);

        g_free(fmtname);
    }

    g_slist_free(formats);
}

int main() {
	sanity_check("/home/user/work/machinepart/machinepart/rsc/5.jpg");
	return 0;
}