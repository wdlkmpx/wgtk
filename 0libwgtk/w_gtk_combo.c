/*
 * Public Domain
 */

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>
#include "w_gtk.h"

#ifdef ENABLE_NLS
#include <locale.h>
#include <libintl.h>
#else
#define gettext(x) (x)
#endif

/* ================================================== */
/*                  COMBO BOX                         */
/* ================================================== */


void w_gtk_combo_box_populate_from_glist (GtkWidget *combo, GList *strings, int default_index)
{
    GtkListStore *store;
    GtkTreeIter iter;
    GList * list;
    char * text;
    int len = 0;

    store = GTK_LIST_STORE (gtk_combo_box_get_model(GTK_COMBO_BOX(combo)));
    gtk_list_store_clear (store);
    //gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT (combo));
    if (!strings) {
        return; // nothing to add
    }
    for (list = strings;  list;  list = list->next)
    {
        text = (char *) list->data;
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter, 0, text, -1);
        //gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), text);
        len++;
    }
    if (default_index >= len) {
        default_index = 0;
    }
    if (default_index >= 0) {
        gtk_combo_box_set_active (GTK_COMBO_BOX (combo), default_index);
    }
}


void w_gtk_combo_box_populate_from_strv (GtkWidget *combo,
                                         const char **strv,
                                         int default_index,
                                         gboolean gettext)
{
    int i;
    GtkListStore * store;
    GtkTreeIter iter;
    char *str;
    store = GTK_LIST_STORE (gtk_combo_box_get_model (GTK_COMBO_BOX(combo)));
    gtk_list_store_clear (store);
    //gtk_combo_box_text_remove_all (GTK_COMBO_BOX_TEXT (combo));
    if (!strv || !*strv) {
        return; /* nothing to add */
    }
    for (i = 0; strv[i]; i++) {
#ifdef ENABLE_NLS
        str = gettext ? gettext(strv[i]) : (char*) strv[i];
#else
        str = (char*) strv[i];
#endif
        gtk_list_store_append (store, &iter);
        gtk_list_store_set (store, &iter, 0, str, -1);
        //gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT (combo), strv[i]);
    }
    if (default_index >= i) {
        default_index = 0;
    }
    if (default_index >= 0) {
        gtk_combo_box_set_active (GTK_COMBO_BOX (combo), default_index);
    }
}


int w_gtk_combo_box_get_count (GtkWidget *combo)
{
    GtkTreeModel * model;
    GtkTreeIter iter;
    gboolean valid;
    int count = 0;
    model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
    valid = gtk_tree_model_get_iter_first (model, &iter);
    while (valid) {
        count++;
        valid = gtk_tree_model_iter_next (model, &iter);
    }
    return count;
}


gboolean w_gtk_combo_box_find_str (GtkWidget *combo,
                                   const char *str,
                                   gboolean select,
                                   GtkTreeIter *out_iter)
{
    gboolean case_sensitive = TRUE;
    GtkTreeModel *model;
    GtkTreeIter local_iter;
    GtkTreeIter *iter = out_iter ? out_iter : &local_iter;
    gboolean valid;
    gboolean found = FALSE;
    char *value = NULL;
    int ret;
    if (!str) {
        return found;
    }
    model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
    valid = gtk_tree_model_get_iter_first (model, iter);
    while (valid)
    {
        gtk_tree_model_get (model, iter, 0, &value, -1);
        if (!value) {
            continue;
        }
        if (case_sensitive) {
            ret = strcmp (value, str);
        } else {
            ret = strcasecmp (value, str);
        }
        if (ret == 0) {
            found = TRUE;
            g_free (value);
            break;
        }
        // string values have to be freed
        g_free (value);
        valid = gtk_tree_model_iter_next (model, iter);
    }
    if (found && select) {
        gtk_combo_box_set_active_iter (GTK_COMBO_BOX(combo), iter);
    }
    return found;
}


void w_gtk_combo_box_select_or_prepend (GtkWidget *combo, const char *str)
{
    if (!str || !*str) {
        return;
    }
    if (!w_gtk_combo_box_find_str (combo, str, TRUE, NULL)) {
        gtk_combo_box_text_prepend_text (GTK_COMBO_BOX_TEXT(combo), str);
        gtk_combo_box_set_active (GTK_COMBO_BOX(combo), 0);
    }
}


void w_gtk_combo_box_set_w_model (GtkWidget *combo, gboolean sort)
{
    // column 0: text
    // column 1: pointer (hidden)
    GtkCellRenderer * cell;
    GtkListStore * store;
    GtkTreeSortable * sortable;

    if (combo)
    {   // unset previous model
#if GTK_MAJOR_VERSION >= 3
        //Gtk-CRITICAL **: gtk_cell_layout_set_attributes: assertion 'GTK_IS_CELL_RENDERER (cell)' failed
        if (gtk_combo_box_get_has_entry (GTK_COMBO_BOX(combo))) {
            gtk_combo_box_set_entry_text_column (GTK_COMBO_BOX(combo), 0);
        }
#endif
        gtk_combo_box_set_model (GTK_COMBO_BOX(combo), NULL);
        gtk_cell_layout_clear (GTK_CELL_LAYOUT(combo));
    }

    store = gtk_list_store_new (2, G_TYPE_STRING, G_TYPE_POINTER);
    gtk_combo_box_set_model (GTK_COMBO_BOX(combo), GTK_TREE_MODEL(store));
    g_object_unref (store);

    cell = gtk_cell_renderer_text_new ();
    gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (combo), cell, TRUE);
    gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (combo), cell,
                                    "text", 0, NULL);
    if (sort) {
        sortable = GTK_TREE_SORTABLE(store);
        gtk_tree_sortable_set_sort_column_id (sortable, 0, GTK_SORT_ASCENDING);
    }
#if GTK_CHECK_VERSION(2, 24, 0)
    /* gtk 2.24 using gtk_combo_box_new_with_entry() */
    //GLib-GObject-WARNING **: unable to set property 'text' of type 'gchararray' from value of type 'guchar'
    if (gtk_combo_box_get_has_entry (GTK_COMBO_BOX(combo))) {
        gtk_combo_box_set_entry_text_column (GTK_COMBO_BOX(combo), 0);
    }
#endif
}


char *w_gtk_combo_get_active_text (GtkWidget *combo)
{
#if GTK_MAJOR_VERSION >= 2
    GtkTreeIter iter;
    char *text = NULL;
    GtkTreeModel *model;
    if (gtk_combo_box_get_active_iter(GTK_COMBO_BOX(combo), &iter)) {
        model = gtk_combo_box_get_model (GTK_COMBO_BOX(combo));
        gtk_tree_model_get (model, &iter, 0, &text, -1);
    }
#endif
    return text; //must be freed
}
