#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "w_gtk.h"

// gcc -o zw_gtk_combo w_gtk.c w_gtk_menu.c zw_gtk_combo_example.c -g `pkg-config --cflags gtk+-2.0` `pkg-config --libs gtk+-2.0` -Wall
// gcc -o zw_gtk_combo w_gtk.c w_gtk_menu.c zw_gtk_combo_example.c -g `pkg-config --cflags gtk+-3.0` `pkg-config --libs gtk+-3.0` -Wall

#define _(x) x

GtkWidget * dialog;
GtkWidget * menu;
GtkApplication * app;

static void on_gtk_button_clicked_stop (GtkButton *widget, gpointer user_data)
{
    GtkWidget *win = GTK_WIDGET (user_data);
    gtk_widget_destroy (win);
}


static void on_gtk_window_destroy (GtkWidget * object, gpointer udata)
{
    g_application_quit (G_APPLICATION (app));
} 


static void w_gtk_combo_box_append_text (GtkWidget *combo, const char *text)
{
    GtkTreeModel *model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
    GtkListStore *store = GTK_LIST_STORE(model);
    GtkTreeIter iter;
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,  0, text, -1);
}


void create_window (void)
{
    GtkWidget * textwid, * stopwid;
    GtkWidget * main_vbox, * vbox;
    GtkWidget * combo;

    dialog = gtk_application_window_new (app);
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ALWAYS);
    gtk_widget_set_size_request (GTK_WIDGET (dialog), 200, 100);

    gtk_window_set_title (GTK_WINDOW (dialog), "Getting directory listings");
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

    main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add (GTK_CONTAINER (dialog), main_vbox);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
    gtk_container_add (GTK_CONTAINER (main_vbox), vbox);

    textwid = gtk_label_new (NULL);
    gtk_box_pack_start (GTK_BOX (vbox), textwid, TRUE, TRUE, 0);

    textwid = gtk_label_new ("xxxx");
   
    GtkTreeModel *model;
    combo = gtk_combo_box_new_with_entry ();
    w_gtk_combo_box_set_w_model (combo, TRUE);
    model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
    printf ("model cols: %d\n", gtk_tree_model_get_n_columns (model));
    printf ("has-entry : %d\n", gtk_combo_box_get_has_entry (GTK_COMBO_BOX(combo)));
    w_gtk_combo_box_append_text (combo, "mmm");
    w_gtk_combo_box_append_text (combo, "zzz");
    w_gtk_combo_box_append_text (combo, "bbb");
    w_gtk_combo_box_append_text (combo, "abby road");
    w_gtk_combo_box_append_text (combo, "might");
    w_gtk_combo_box_append_text (combo, "question");
    //gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), "mmm");
    //gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), "zzz");
    //gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT(combo), "bbb");
    gtk_combo_box_set_active (GTK_COMBO_BOX(combo), 0);
    w_gtk_combo_box_find_str (GTK_COMBO_BOX(combo), "question", TRUE, NULL);
    gtk_box_pack_start (GTK_BOX(vbox), combo, TRUE, TRUE, 0);
  
    stopwid = gtk_button_new_with_label ("  Stop  ");
    gtk_box_pack_start (GTK_BOX (vbox), stopwid, TRUE, TRUE, 0);

    gtk_widget_add_events (dialog, GDK_BUTTON_RELEASE_MASK);

    g_signal_connect (dialog, "destroy", G_CALLBACK(on_gtk_window_destroy), NULL);
    g_signal_connect (stopwid, "clicked", G_CALLBACK(on_gtk_button_clicked_stop), dialog);

    gtk_widget_show_all (dialog);
}


int main (int argc, char *argv[])
{
#if GTK_MAJOR_VERSION >= 3
    app = gtk_application_new ("org.gtk.app", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK(create_window), NULL);
    g_application_run (G_APPLICATION (app), argc, argv);
    return 0;
#else  
    gtk_init (&argc, &argv);

    create_window ();

    gtk_main ();
#endif
    return 0;
}

