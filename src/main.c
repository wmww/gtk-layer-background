#include <gtk-layer-shell.h>

#include <gtk/gtk.h>

static void
activate (GtkApplication* app, void *_data)
{
    GtkWidget *window = gtk_application_window_new (app);

    gtk_layer_init_for_window (GTK_WINDOW (window));
    gtk_layer_set_namespace (GTK_WINDOW (window), "background");
    gtk_layer_set_layer (GTK_WINDOW (window), GTK_LAYER_SHELL_LAYER_BACKGROUND);
    for (int i = 0; i < GTK_LAYER_SHELL_EDGE_ENTRY_NUMBER; i++) {
        gtk_layer_set_anchor (GTK_WINDOW (window), i, TRUE);
    }

    GtkWidget* background = gtk_box_new (GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add (GTK_CONTAINER (window), background);
    gtk_widget_show_all (window);
}

int
main (int argc, char **argv)
{
    GtkApplication * app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    int status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}
