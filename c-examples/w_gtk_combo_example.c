#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "w_gtk.h"

#define _(x) x

GtkWidget * dialog;
GtkWidget * menu;
GtkApplication * app;

#if GTK_CHECK_VERSION(2,4,0)
GtkWidget *wcombo1;      // GtkComboBoxText
GtkWidget *wcomboentry1; // GtkComboBoxText with entry
#endif
#if !GTK_CHECK_VERSION(3,0,0)
GtkWidget *wcombo2;      // GtkOptionMenu
GtkWidget *wcomboentry2; // GtkCombo
#endif


static void on_gtk_button_clicked_stop (GtkButton *widget, gpointer user_data)
{
    GtkWidget *win = GTK_WIDGET (user_data);
    gtk_widget_destroy (win);
}


static const char *items_strv[] =
{
    "one", "two", "three", "four", "five", "six", "seven", "eight", "nine", NULL,
};

const char *hidden_msg = "There is no knowledge that is not power";


static void on_gtk_button_clicked_fill (GtkButton *widget, gpointer user_data)
{
#if GTK_CHECK_VERSION(2,4,0)
    w_gtk_combobox_populate_from_strv (wcombo1, items_strv, 2, FALSE);
    w_gtk_combobox_populate_from_strv (wcomboentry1, items_strv, 2, FALSE);

    w_gtk_combobox_prepend_text (wcombo1, "prepended");
    w_gtk_combobox_prepend_text (wcomboentry1, "prepended");

    w_gtk_combobox_insert_text (wcombo1, 6, "inserted");
    w_gtk_combobox_insert_text (wcomboentry1, 6, "inserted");

    w_gtk_combobox_remove (wcombo1, 8);
    w_gtk_combobox_remove (wcomboentry1, 8);

    w_gtk_combobox_append (wcombo1, "Hidden Message", (gpointer) hidden_msg);
    w_gtk_combobox_append (wcomboentry1, "Hidden Message", (gpointer) hidden_msg);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    w_gtk_combobox_populate_from_strv (wcombo2, items_strv, 2, FALSE);
    w_gtk_combobox_populate_from_strv (wcomboentry2, items_strv, 2, FALSE);

    w_gtk_combobox_prepend_text (wcombo2, "prepended");
    w_gtk_combobox_prepend_text (wcomboentry2, "prepended");

    w_gtk_combobox_insert_text (wcombo2, 6, "inserted");
    w_gtk_combobox_insert_text (wcomboentry2, 6, "inserted");

    w_gtk_combobox_remove (wcombo2, 8);
    w_gtk_combobox_remove (wcomboentry2, 8);

    w_gtk_combobox_append (wcombo2, "Hidden Message", (gpointer) hidden_msg);
    w_gtk_combobox_append (wcomboentry2, "Hidden Message", (gpointer) hidden_msg);
#endif
}


static void on_gtk_button_clicked_print_active_text (GtkButton *widget, gpointer user_data)
{
    char *text;
    puts ("** ACTIVE TEXT");
#if GTK_CHECK_VERSION(2,4,0)
    text = w_gtk_combobox_get_active_text (wcombo1);
    printf ("combo1: %s\n", text);
    g_free (text);
    text = w_gtk_combobox_get_active_text (wcomboentry1);
    printf ("comboentry1: %s\n", text);
    g_free (text);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    text = w_gtk_combobox_get_active_text (wcombo2);
    printf ("combo2: %s\n", text);
    g_free (text);
    text = w_gtk_combobox_get_active_text (wcomboentry2);
    printf ("comboentry2: %s\n", text);
    g_free (text);
#endif
}


static void on_gtk_button_clicked_print_index5 (GtkButton *widget, gpointer user_data)
{
    char *text;
    puts ("** INDEX 5 TEXT");
#if GTK_CHECK_VERSION(2,4,0)
    text = w_gtk_combobox_get_item_text (wcombo1, 5);
    printf ("combo1: %s\n", text);
    g_free (text);
    text = w_gtk_combobox_get_item_text (wcomboentry1, 5);
    printf ("comboentry1: %s\n", text);
    g_free (text);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    text = w_gtk_combobox_get_item_text (wcombo2, 5);
    printf ("combo2: %s\n", text);
    g_free (text);
    text = w_gtk_combobox_get_item_text (wcomboentry2, 5);
    printf ("comboentry2: %s\n", text);
    g_free (text);
#endif
}


static void on_gtk_button_clicked_print_active_index (GtkButton *widget, gpointer user_data)
{
    int x;
    puts ("** COMBO SELECTED INDEX");
#if GTK_CHECK_VERSION(2,4,0)
    x = w_gtk_combobox_get_active_index (wcombo1);
    printf ("combo1: %d\n", x);
    x = w_gtk_combobox_get_active_index (wcomboentry1);
    printf ("comboentry1: %d\n", x);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    x = w_gtk_combobox_get_active_index (wcombo2);
    printf ("combo2: %d\n", x);
    x = w_gtk_combobox_get_active_index (wcomboentry2);
    printf ("comboentry2: %d\n", x);
#endif
}


static void on_gtk_button_clicked_print_count (GtkButton *widget, gpointer user_data)
{
    int x;
#if GTK_CHECK_VERSION(2,4,0)
    puts ("** COMBO COUNT");
    x = w_gtk_combobox_get_count (wcombo1);
    printf ("combo1: %d\n", x);
    x = w_gtk_combobox_get_count (wcomboentry1);
    printf ("comboentry1: %d\n", x);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    x = w_gtk_combobox_get_count (wcombo2);
    printf ("combo2: %d\n", x);
    x = w_gtk_combobox_get_count (wcomboentry2);
    printf ("comboentry2: %d\n", x);
#endif
}


static void on_gtk_button_clicked_find_8 (GtkButton *widget, gpointer user_data)
{
#if GTK_CHECK_VERSION(2,4,0)
    w_gtk_combobox_set_search_case_insensitive (wcombo1, FALSE);
    w_gtk_combobox_set_search_case_insensitive (wcomboentry1, FALSE);
    w_gtk_combobox_search_text (wcombo1, "EIGHT", TRUE, NULL);
    w_gtk_combobox_search_text (wcomboentry1, "EIGHT", TRUE, NULL);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    w_gtk_combobox_set_search_case_insensitive (wcombo2, FALSE);
    w_gtk_combobox_set_search_case_insensitive (wcomboentry2, FALSE);
    w_gtk_combobox_search_text (wcombo2, "EIGHT", TRUE, NULL);
    w_gtk_combobox_search_text (wcomboentry2, "EIGHT", TRUE, NULL);
#endif
}


static void on_gtk_button_clicked_find_8_in (GtkButton *widget, gpointer user_data)
{
#if GTK_CHECK_VERSION(2,4,0)
    w_gtk_combobox_set_search_case_insensitive (wcombo1, TRUE);
    w_gtk_combobox_set_search_case_insensitive (wcomboentry1, TRUE);
    w_gtk_combobox_search_text (wcombo1, "EIGHT", TRUE, NULL);
    w_gtk_combobox_search_text (wcomboentry1, "EIGHT", TRUE, NULL);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    w_gtk_combobox_set_search_case_insensitive (wcombo2, TRUE);
    w_gtk_combobox_set_search_case_insensitive (wcomboentry2, TRUE);
    w_gtk_combobox_search_text (wcombo2, "EIGHT", TRUE, NULL);
    w_gtk_combobox_search_text (wcomboentry2, "EIGHT", TRUE, NULL);
#endif
}


static void on_gtk_button_clicked_find_hidden (GtkButton *widget, gpointer user_data)
{
    char *hm = NULL;
    int index;
#if GTK_CHECK_VERSION(2,4,0)
    index = w_gtk_combobox_search_text (wcombo1, "Hidden Message", FALSE, (gpointer) &hm);
    printf ("Hidden msg w1 : %s\n", hm);
    printf ("index w1 : %d\n", index);
    index = w_gtk_combobox_search_text (wcomboentry1, "Hidden Message", FALSE, (gpointer) &hm);
    printf ("Hidden msg we1: %s\n", hm);
    printf ("index we1: %d\n", index);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    index = w_gtk_combobox_search_text (wcombo2, "Hidden Message", FALSE, (gpointer) &hm);
    printf ("Hidden msg w2 : %s\n", hm);
    printf ("index w2 : %d\n", index);
    index = w_gtk_combobox_search_text (wcomboentry2, "Hidden Message", FALSE, (gpointer) &hm);
    printf ("Hidden msg we2: %s\n", hm);
    printf ("index we2: %d\n", index);
#endif
}


static void on_gtk_button_clicked_select_hidden (GtkButton *widget, gpointer user_data)
{
    int index;
    puts ("Select hidden msg:");
#if GTK_CHECK_VERSION(2,4,0)
    index = w_gtk_combobox_search_data (wcombo1, (gpointer) hidden_msg, TRUE);
    printf ("index w1 : %d\n", index);
    index = w_gtk_combobox_search_data (wcomboentry1, (gpointer) hidden_msg, TRUE);
    printf ("index we1: %d\n", index);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    index = w_gtk_combobox_search_data (wcombo2, (gpointer) hidden_msg, TRUE);
    printf ("index w2 : %d\n", index);
    index = w_gtk_combobox_search_data (wcomboentry2, (gpointer) hidden_msg, TRUE);
    printf ("index we2: %d\n", index);
#endif
}


static void on_gtk_button_clicked_clear (GtkButton *widget, gpointer user_data)
{
#if GTK_CHECK_VERSION(2,4,0)
    w_gtk_combobox_clear (wcombo1);
    w_gtk_combobox_clear (wcomboentry1);
#endif
#if !GTK_CHECK_VERSION(3,0,0)
    w_gtk_combobox_clear (wcombo2);
    w_gtk_combobox_clear (wcomboentry2);
#endif
}

static void on_combo_changed (GtkWidget *widget, gpointer user_data)
{
    char *text;
    GtkWidget *combo = w_gtk_combobox_cb_ensure_combo (widget);
    if (!combo) {
        return;
    }
    text = w_gtk_combobox_get_active_text (combo);
    if (text) {
        puts (text);
        g_free (text);
    }
}

//===========================================================

static void on_gtk_window_destroy (GtkWidget * object, gpointer udata)
{
    g_application_quit (G_APPLICATION (app));
}

// ==========================================================

void create_window (void)
{
    GtkWidget * textwid;
    GtkWidget * main_vbox, * vbox;
    GtkWidget *w;

    dialog = gtk_application_window_new (app);
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER_ALWAYS);
    //gtk_widget_set_size_request (GTK_WIDGET (dialog), 200, 100);

    gtk_window_set_title (GTK_WINDOW (dialog), "WGTKCOMBO TEST");
    gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_MOUSE);

    main_vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_add (GTK_CONTAINER (dialog), main_vbox);

    vbox = gtk_box_new (GTK_ORIENTATION_VERTICAL, 5);
    gtk_container_set_border_width (GTK_CONTAINER (vbox), 10);
    gtk_container_add (GTK_CONTAINER (main_vbox), vbox);

    textwid = gtk_label_new (NULL);
    gtk_box_pack_start (GTK_BOX (vbox), textwid, TRUE, TRUE, 0);


#if GTK_CHECK_VERSION(2,4,0)
    // test W model
    GtkTreeModel *model;
    GtkWidget * combo;
    combo = gtk_combo_box_new ();            // ok
#if GTK_MAJOR_VERSION == 2
    //combo = gtk_combo_box_entry_new ();      // ok
    //combo = gtk_combo_box_text_new ();       // ok
    //combo = gtk_combo_box_new_with_entry (); // ok
#else
    //combo = gtk_combo_box_text_new ();       // ok
    //combo = gtk_combo_box_new_with_entry (); // ok
#endif
    w_gtk_combo_box_set_w_model (combo, TRUE);
    model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
    printf ("model cols: %d\n", gtk_tree_model_get_n_columns (model));
    printf ("has-entry : %d\n", gtk_combo_box_get_has_entry (GTK_COMBO_BOX(combo)));
    w_gtk_combobox_append_text (combo, "mmm");
    w_gtk_combobox_append_text (combo, "zzz");
    w_gtk_combobox_append_text (combo, "bbb");
    w_gtk_combobox_append_text (combo, "abby road");
    w_gtk_combobox_append_text (combo, "might");
    w_gtk_combobox_append_text (combo, "question");
    w_gtk_combobox_set_active_index (combo, 0);
    w_gtk_combobox_search_text (combo, "question", TRUE, NULL);
    gtk_box_pack_start (GTK_BOX(vbox), combo, TRUE, TRUE, 0);
    g_signal_connect (combo,      "changed", G_CALLBACK(on_combo_changed), NULL);
#endif


#if GTK_CHECK_VERSION(2,4,0)
    wcombo1 = gtk_combo_box_text_new ();
    wcomboentry1 = gtk_combo_box_text_new_with_entry ();
    w_gtk_combo_box_set_w_model (wcombo1, FALSE);
    w_gtk_combo_box_set_w_model (wcomboentry1, FALSE);
    ///g_signal_connect (wcombo1,      "changed", G_CALLBACK(on_combo_changed), NULL);
    ///g_signal_connect (wcomboentry1, "changed", G_CALLBACK(on_combo_changed), NULL);
    w_gtk_combobox_add_changed_handler (wcombo1,      on_combo_changed, NULL);
    w_gtk_combobox_add_changed_handler (wcomboentry1, on_combo_changed, NULL);

    gtk_box_pack_start (GTK_BOX(vbox), wcombo1,      TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), wcomboentry1, TRUE, TRUE, 0);
#endif

#if !GTK_CHECK_VERSION(3,0,0)
    wcombo2 = gtk_option_menu_new ();
    GtkWidget *menu = gtk_menu_new();
    gtk_option_menu_set_menu (GTK_OPTION_MENU(wcombo2), menu);

    wcomboentry2 = gtk_combo_new ();
    //gtk_editable_set_editable (GTK_EDITABLE(GTK_COMBO(wcomboentry2)->entry), FALSE);

    ///g_signal_connect (wcombo2,                        "changed", G_CALLBACK(on_combo_changed), NULL);
    ///g_signal_connect (GTK_COMBO(wcomboentry2)->entry, "changed", G_CALLBACK(on_combo_changed), NULL);
    w_gtk_combobox_add_changed_handler (wcombo2,      on_combo_changed, NULL);
    w_gtk_combobox_add_changed_handler (wcomboentry2, on_combo_changed, NULL);

    gtk_box_pack_start (GTK_BOX(vbox), wcombo2, TRUE, TRUE, 0);
    gtk_box_pack_start (GTK_BOX(vbox), wcomboentry2, TRUE, TRUE, 0);
#endif

    //===
    w = gtk_button_new_with_label ("FILL combos");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_fill), NULL);

    w = gtk_button_new_with_label ("Print ITEM count");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_print_count), NULL);

    w = gtk_button_new_with_label ("Print SELECTED index");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_print_active_index), NULL);

    w = gtk_button_new_with_label ("Print ACTIVE text");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_print_active_text), NULL);

    w = gtk_button_new_with_label ("  Print Index 5 TEXT  ");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_print_index5), dialog);

    w = gtk_button_new_with_label ("Find 'EIGHT' (case sensitive)");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_find_8), NULL);

    w = gtk_button_new_with_label ("Find 'EIGHT' (case insensitive)");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_find_8_in), NULL);

    w = gtk_button_new_with_label ("Print Hidden Message");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_find_hidden), NULL);

    w = gtk_button_new_with_label ("Select hidden msg (by data)");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_select_hidden), NULL);

    w = gtk_button_new_with_label ("CLEAR combos");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_clear), NULL);

    w = gtk_button_new_with_label ("  QUIT  ");
    gtk_box_pack_start (GTK_BOX (vbox), w, TRUE, TRUE, 0);
    g_signal_connect (w, "clicked", G_CALLBACK(on_gtk_button_clicked_stop), dialog);

    gtk_widget_add_events (dialog, GDK_BUTTON_RELEASE_MASK);
    g_signal_connect (dialog, "destroy", G_CALLBACK(on_gtk_window_destroy), NULL);

    gtk_widget_show_all (dialog);
}


// ==========================================================

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


/*
  gtk_widget_set_tooltip_text (btn_num_dot, "AAA");
  gtk_widget_set_tooltip_text (btn_num_dot, "bbb");
  gtk_widget_set_has_tooltip (btn_num_dot, FALSE);
  char * aa = gtk_widget_get_tooltip_text (btn_num_dot);
  int xx = gtk_widget_get_has_tooltip (btn_num_dot);
  printf ("%d\n", xx);
  puts (aa);
  g_free (aa);
*/
