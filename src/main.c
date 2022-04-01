#include <gtk-layer-shell.h>

#include <gtk/gtk.h>

const char *prog_name = "gtk-layer-demo";
const char *prog_summary = "A GTK application for demonstrating the functionality of the Layer Shell Wayland protocol";
const char *prog_details = "See https://github.com/wmww/gtk-layer-shell for more information, and to report bugs";

const gchar *image_path = "";

gboolean image_option_callback (const gchar *option_name, const gchar *value, void *data, GError **error);

static const GOptionEntry options[] = {
    {
        .long_name = "image",
        .short_name = 'i',
        .flags = G_OPTION_FLAG_FILENAME,
        .arg = G_OPTION_ARG_CALLBACK,
        .arg_data = (void *)&image_option_callback,
        .description = "The image to use as a background",
        .arg_description = NULL,
    },
    { NULL, 0, 0, 0, NULL, NULL, NULL }
};

gboolean
image_option_callback (const gchar *_option_name, const gchar *value, void *_data, GError **error)
{
    (void)_option_name;
    (void)_data;

    image_path = g_strdup (value);
    return TRUE;
}

static void
process_args (int *argc, char ***argv)
{
    GOptionContext *context = g_option_context_new ("");
    g_option_context_add_group (context, gtk_get_option_group (TRUE));
    g_option_context_set_summary (context, prog_summary);
    g_option_context_set_description (context, prog_details);
    g_option_context_add_main_entries (context, options, NULL);

    GError *error = NULL;
    if (!g_option_context_parse (context, argc, argv, &error)) {
        g_printerr ("%s\n", error->message);
        g_error_free (error);
        g_option_context_free (context);
        exit (1);
    }

    g_option_context_free (context);
}

static void
activate (GtkApplication* app, void *_data)
{
    GtkWidget *window = gtk_application_window_new (app);

    gtk_layer_init_for_window (GTK_WINDOW (window));
    gtk_layer_set_namespace (GTK_WINDOW (window), "background");
    gtk_layer_set_layer (GTK_WINDOW (window), GTK_LAYER_SHELL_LAYER_BACKGROUND);
    gtk_layer_set_exclusive_zone (GTK_WINDOW (window), -1);
    for (int i = 0; i < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER; i++) {
        gtk_layer_set_anchor (GTK_WINDOW (window), i, TRUE);
    }

    g_message ("Loading %s", image_path);
    GtkWidget* background = gtk_image_new_from_file (image_path);
    gtk_container_add (GTK_CONTAINER (window), background);
    gtk_widget_show_all (window);
}

int
main (int argc, char **argv)
{
    process_args (&argc, &argv);

    GtkApplication * app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}
