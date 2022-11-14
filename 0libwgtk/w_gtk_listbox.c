/*
 * Public Domain
 */

// - item index starts at 0
// - invalid item [active] index is -1
// - returned item [active] text must be freed

// Supported widgets:
// - GtkTreeView   (gtk2/3/4)
// - GtkCList      (gtk1/2)
// - GtkList       (gtk1/2)

// GtkListView 

// all w_gtk_listbox_..() functions take GtkWidget as parameter

/*
*/

#if defined(__clang__)
#   pragma clang diagnostic ignored "-Wdeprecated-declarations"
#elif defined(__GNUC__)
#   pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "w_gtk.h"

#ifdef ENABLE_NLS
#include <locale.h>
#include <libintl.h>
#else
#define gettext(x) (x)
#endif

///#define W_LISTBOX_DEBUG

#ifdef W_LISTBOX_DEBUG
# define W_DEBUG_PUTS(text) puts(text) 
#else
# define W_DEBUG_PUTS(text)
#endif


/*--------------------------------------------------------------
/                  w_gtk_listbox_simple_new
/-------------------------------------------------------------*/

/*
   Simple listbox: `text` and `data` fields
   simple listbox doesn't include a header by default
   text is a char array (char*)
   data is a [hidden] gpointer (void*), a pointer to any object

   - GtkTreeView
     * text is field 0 of a GtkListStore
     * data is field 1 of a GtkListStore
   - GtkCList
     * text is row_text
     * data is row_data
   - GtkList
     * text is a GtkListItem label
     * data is GtkListItem "wdata" (object data)
*/

GtkWidget * w_gtk_listbox_simple_new (int rows)
{
    GtkWidget *listbox = NULL;
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    W_DEBUG_PUTS ("w_gtk_listbox: new GtkTreeView");
    GtkListStore *store;
    store = gtk_list_store_new (2,
                                G_TYPE_STRING,
                                G_TYPE_POINTER);
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    listbox = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));
    g_object_unref (G_OBJECT(store));
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(listbox), FALSE);

    renderer = gtk_cell_renderer_text_new();
    column   = gtk_tree_view_column_new_with_attributes ("", renderer,
                                                         "text", 0, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(listbox), column);
#else // GTK1
    W_DEBUG_PUTS ("w_gtk_listbox: new GtkCList");
    listbox = gtk_clist_new (1);
    gtk_clist_column_titles_hide (GTK_CLIST(listbox));
    /// GtkList is too primitive and buggy
    ///listbox = gtk_list_new ();
#endif
    return listbox;
}


/*--------------------------------------------------------------
/         w_gtk_listbox_add_changed_handler
/-------------------------------------------------------------*/

/* the "changed" signal
 
GtkTreeView: changed            [GtkTreeSelection]
GtkCList   : select_row
GtkList    : selection-changed

How to handle the "changed" signal:

static void on_listbox_changed_cb (GtkWidget *widget, gpointer user_data)
{
    char *text;
    GtkWidget *listbox = w_gtk_listbox_cb_ensure_list (widget);
    if (!combo) {
        return;
    }
    text = w_gtk_listbox_get_active_text (listbox);
    if (text) {
        puts (text);
        g_free (text);
    }
}
*/
int w_gtk_listbox_add_changed_handler (GtkWidget *listbox, gpointer cb, gpointer cb_data)
{
    int handler = -1;
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        W_DEBUG_PUTS ("w_gtk_listbox: add changed cb for GtkTreeView");
        // ...
        handler = g_signal_connect (G_OBJECT(listbox), "changed", G_CALLBACK(cb), cb_data);
        return handler;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        W_DEBUG_PUTS ("w_gtk_listbox: add changed cb for GtkCList");
        g_signal_connect (listbox, "select_row", G_CALLBACK(cb), cb_data);
    }
    else if (GTK_IS_LIST(listbox))
    {
        W_DEBUG_PUTS ("w_gtk_listbox: add changed cb for GtkList");
        /// void selection_cb (GtkList *list, gpointer user_data)
        g_signal_connect (listbox, "selection-changed", G_CALLBACK(cb), cb_data);
    }
#endif
    if (handler == -1) {
        g_warning ("Invalid wlistbox widget");
    }
    return handler;
}


/*--------------------------------------------------------------
/          w_gtk_listbox_cb_ensure_list
/-------------------------------------------------------------*/

GtkWidget *w_gtk_listbox_cb_ensure_list (GtkWidget *widget)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(widget)) {
        W_DEBUG_PUTS ("wlistbox changed signal: it's GtkTreeView");
        return widget;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(widget)) {
        W_DEBUG_PUTS ("wlistbox changed signal: it's GtkCList");
        return widget;
    }
    else if (GTK_IS_LIST(widget)) {
        W_DEBUG_PUTS ("wlistbox changed signal: it's GtkList");
        return widget;
    }
#endif
    g_return_val_if_fail (widget != NULL, NULL);
    return NULL;
}



/*--------------------------------------------------------------
/                 w_gtk_listbox_clear
/-------------------------------------------------------------*/

void w_gtk_listbox_clear (GtkWidget *listbox)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        gtk_list_store_clear (GTK_LIST_STORE(model)); 
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_clear (GTK_CLIST(listbox));
    }
    else if (GTK_IS_LIST(listbox))
    {
        gtk_list_clear_items (GTK_LIST(listbox), 0, -1);
    }
#endif
}


/*--------------------------------------------------------------
/                 w_gtk_listbox_get_count
/-------------------------------------------------------------*/

int w_gtk_listbox_get_count (GtkWidget *listbox)
{
    int count = 0;
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        return gtk_tree_model_iter_n_children (model, NULL);
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        count = GTK_CLIST(listbox)->rows;
        ///GList *rows = GTK_CLIST(listbox)->row_list;
        ///count = g_list_length (rows);
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList *rows = GTK_LIST(listbox)->children;
        count = g_list_length (rows);
    }
#endif
    return count;
}


/*--------------------------------------------------------------
/                  w_gtk_listbox_insert
/-------------------------------------------------------------*/

void w_gtk_listbox_insert (GtkWidget *listbox, int position, const char *text, gpointer wdata)
{
    // position -1: append
    // position  0: prepend
    // position  0+: insert
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkListStore *list  = GTK_LIST_STORE (model);
        GtkTreeIter iter;
        switch (position) {
           case -1: gtk_list_store_append (list, &iter); break;
           case  0: gtk_list_store_prepend (list, &iter); break;
           default: gtk_list_store_insert (list, &iter, position);
        }
        gtk_list_store_set (list, &iter, 0, text, -1);
        if (wdata) {
            // the W listbox created with w_gtk_listbox_new()
            gtk_list_store_set (list, &iter, 1, wdata, -1);
        }
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        static char *empty_row[] = { NULL, NULL };
        int row;
        switch (position) {
           case -1: row = gtk_clist_append (GTK_CLIST(listbox), empty_row); break;
           case  0: row = gtk_clist_prepend (GTK_CLIST(listbox), empty_row); break;
           default: row = gtk_clist_insert (GTK_CLIST(listbox), position, empty_row);
        }
        ///if (text) {
        ///    gtk_clist_set_text (GTK_CLIST(listbox), row, 0, text);
        ///}
        if (wdata) {
            gtk_clist_set_row_data (GTK_CLIST(listbox), row, wdata);
        }
    }
    else if (GTK_IS_LIST(listbox))
    {
        GtkWidget *item; // GtkListem
        GList     *glist = NULL;
        if (!text) return;
        item  = gtk_list_item_new_with_label (text);
        glist = g_list_append (glist, item);
        gtk_widget_show (item);
        /// position -1 = gtk_list_append_items (GTK_LIST(list), glist); // gtk_container_add(GTK_CONTAINER(list), item);
        /// position  0 = gtk_list_prepend_items (GTK_LIST(list), glist);
        gtk_list_insert_items (GTK_LIST(listbox), glist, position);
        if (wdata) {
            g_object_set_data (G_OBJECT(item), "wdata", wdata);
        }
    }
#endif
}


/*--------------------------------------------------------------
/                w_gtk_listbox_append
/-------------------------------------------------------------*/

void w_gtk_listbox_append (GtkWidget *listbox, const char *text, gpointer wdata)
{
    w_gtk_listbox_insert (listbox, -1, text, wdata);
}


/*--------------------------------------------------------------
/                w_gtk_listbox_prepend
/-------------------------------------------------------------*/

void w_gtk_listbox_prepend (GtkWidget *listbox, const char *text, gpointer wdata)
{
    w_gtk_listbox_insert (listbox, 0, text, wdata);
}


/*--------------------------------------------------------------
/               w_gtk_listbox_insert_text
/-------------------------------------------------------------*/

void w_gtk_listbox_insert_text (GtkWidget *listbox, int position, const char *text)
{
    w_gtk_listbox_insert (listbox, position, text, NULL);
}


/*--------------------------------------------------------------
/               w_gtk_listbox_append_text
/-------------------------------------------------------------*/

void w_gtk_listbox_append_text (GtkWidget *listbox, const char *text)
{
    w_gtk_listbox_insert (listbox, -1, text, NULL);
}


/*--------------------------------------------------------------
/              w_gtk_listbox_prepend_text
/-------------------------------------------------------------*/

void w_gtk_listbox_prepend_text (GtkWidget *listbox, const char *text)
{
    w_gtk_listbox_insert (listbox, 0, text, NULL);
}


/*--------------------------------------------------------------
/                 w_gtk_listbox_remove
/-------------------------------------------------------------*/

void w_gtk_listbox_remove (GtkWidget *listbox, int position) // index
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkTreePath  *path  = gtk_tree_path_new_from_indices (position, -1);
        GtkTreeIter iter;
        if (gtk_tree_model_get_iter (model, &iter, path)) {
            gtk_list_store_remove (GTK_LIST_STORE(model), &iter);
        }
        gtk_tree_path_free (path);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_remove (GTK_CLIST(listbox), position);
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList     *items = GTK_LIST(listbox)->children;
        GtkWidget *witem = GTK_WIDGET (g_list_nth_data (items, position));
        if (witem) { // witem = GtkListItem
            gtk_widget_destroy (witem); ///gtk_container_remove
        }
    }
#endif
}


/*--------------------------------------------------------------
/               w_gtk_listbox_get_item
/-------------------------------------------------------------*/

void w_gtk_listbox_get_item (GtkWidget *listbox, int position, WGtkComboItem *out_comboitem)
{
    // if out_comboitem->text != NULL, it must be freed
    g_return_if_fail (out_comboitem != NULL);
    //g_free (out_comboitem->text);
    memset (out_comboitem, 0, sizeof(*out_comboitem));
    out_comboitem->index = position;
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkTreePath  *path  = gtk_tree_path_new_from_indices (position, -1);
        GtkTreeIter iter;
        if (gtk_tree_model_get_n_columns(model) == 2) {
            // the W combo created with w_gtk_listbox_new()
            if (gtk_tree_model_get_iter (model, &iter, path)) {
                gtk_tree_model_get (model, &iter,
                                    0, &(out_comboitem->text),
                                    1, &(out_comboitem->data), -1);
            }
        } else { // normal ComboBoxText
            if (gtk_tree_model_get_iter (model, &iter, path)) {
                gtk_tree_model_get (model, &iter,
                                    0, &(out_comboitem->text), -1);
            }
        }
        gtk_tree_path_free (path);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_get_text (GTK_CLIST(listbox), position, 0, &(out_comboitem->text)); 
        out_comboitem->data = gtk_clist_get_row_data (GTK_CLIST(listbox), position);
    }
    else if (GTK_IS_LIST(listbox))
    {
        GtkWidget *label_w, *list_item;
        GList *items = GTK_LIST(listbox)->children;
        GList *nitem = g_list_nth (items, position);
        if (nitem) { // nitem->data = GtkListItem
            list_item = GTK_WIDGET(nitem->data); 
            label_w   = gtk_bin_get_child (GTK_BIN(list_item));
            out_comboitem->text = g_strdup (gtk_label_get_text (GTK_LABEL(label_w)));
            out_comboitem->data = g_object_get_data (G_OBJECT(list_item), "wdata");
        }
    }
#endif
}

/*--------------------------------------------------------------
/              w_gtk_listbox_get_item_text
/-------------------------------------------------------------*/

char *w_gtk_listbox_get_item_text (GtkWidget *listbox, int position)
{
    WGtkComboItem combo_item;
    w_gtk_listbox_get_item (listbox, position, &combo_item);
    return combo_item.text; //must be freed
}

/*--------------------------------------------------------------
/              w_gtk_listbox_get_item_data
/-------------------------------------------------------------*/

char *w_gtk_listbox_get_item_data (GtkWidget *listbox, int position)
{
    WGtkComboItem combo_item;
    w_gtk_listbox_get_item (listbox, position, &combo_item);
    return combo_item.data;
}


/*--------------------------------------------------------------
/              w_gtk_listbox_get_selected
/-------------------------------------------------------------*/

int w_gtk_listbox_get_selected (GtkWidget *listbox, WGtkComboItem *out_comboitem)
{
    // Returns an integer which is the index of the currently active item
    //   or -1 if there's no active item.
    // if out_comboitem->text != NULL, it must be freed
    int active = -1;
    if (out_comboitem) {
        memset (out_comboitem, 0, sizeof(*out_comboitem));
        out_comboitem->index = -1;
    }
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        int index = -1;
        GtkTreeModel *model   = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW(listbox));
        GtkTreeIter iter;
        gboolean valid = gtk_tree_model_get_iter_first (model, &iter);
        while (valid)
        {
            index++;
            if (gtk_tree_selection_iter_is_selected (sel, &iter)) {
                // only the 1st selected item
                // &iter is the selected item
                active = index;
                break;
            }
            valid = gtk_tree_model_iter_next (model, &iter);
        }

        if (out_comboitem && active > -1)
        {
            out_comboitem->index = active;
            if (gtk_tree_model_get_n_columns(model) == 2) {
                // the W combo created with w_gtk_listbox_new()
                gtk_tree_model_get (model, &iter,
                                    0, &(out_comboitem->text),
                                    1, &(out_comboitem->data), -1);
            } else { // normal ComboBoxText
                gtk_tree_model_get (model, &iter,
                                    0, &(out_comboitem->text), -1);
            }
        }
        return active;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        GList *selection = GTK_CLIST(listbox)->selection;
        if (selection) {
            active = GPOINTER_TO_INT (selection->data);
            if (out_comboitem && active > -1) {
                out_comboitem->index = active;
                gtk_clist_get_text (GTK_CLIST(listbox), active, 0, &(out_comboitem->text)); 
                out_comboitem->data = gtk_clist_get_row_data (GTK_CLIST(listbox), active); 
            }
        }
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList *selection = GTK_LIST(listbox)->selection;
        if (selection) { // selection->data = GtkListItem
            active = g_list_index (GTK_LIST(listbox)->children, selection->data);
            ///active = gtk_list_child_position (GTK_LIST(list), selection->data);
            if (out_comboitem && active > -1) {
                out_comboitem->index = active;
                GtkWidget *list_item = GTK_WIDGET(selection->data);
                GtkWidget *label_w   = gtk_bin_get_child (GTK_BIN(list_item));
                out_comboitem->text = g_strdup (gtk_label_get_text (GTK_LABEL(label_w)));
                out_comboitem->data = g_object_get_data (G_OBJECT(list_item), "wdata"); 
            }
        }
    }
#endif
    return active;
}


/*--------------------------------------------------------------
/              w_gtk_listbox_get_active_text
/-------------------------------------------------------------*/

char *w_gtk_listbox_get_active_text (GtkWidget *listbox)
{
    // if returned string != NULL, is must be freed
    char *itext = NULL;
    WGtkComboItem combo_item;
    w_gtk_listbox_get_selected (listbox, &combo_item);
    itext = combo_item.text;
    return itext;
}


/*--------------------------------------------------------------
/             w_gtk_listbox_get_active_index
/-------------------------------------------------------------*/

int w_gtk_listbox_get_active_index (GtkWidget *listbox)
{
    return w_gtk_listbox_get_selected (listbox, NULL);
}


/*--------------------------------------------------------------
/            w_gtk_listbox_set_active_index
/-------------------------------------------------------------*/

#define w_gtk_listbox_set_active_index w_gtk_listbox_select_row

void w_gtk_listbox_set_active_index (GtkWidget *listbox, int index)
{
    // index -1: no active item
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeView      *tree  = GTK_TREE_VIEW(listbox);
        GtkTreeSelection *tsel  = gtk_tree_view_get_selection (tree);
        GtkTreePath      *tpath = gtk_tree_path_new_from_indices (index, -1);
        gtk_tree_selection_select_path (tsel, tpath);
        gtk_tree_path_free (tpath);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_select_row (GTK_CLIST(listbox), index, 0);
    }
    else if (GTK_IS_LIST(listbox))
    {
        gtk_list_select_item (GTK_LIST(listbox), index);
    }
#endif
}


/*--------------------------------------------------------------
/               w_gtk_listbox_search_text
/-------------------------------------------------------------*/

int w_gtk_listbox_search_text (GtkWidget *listbox, const char *str,
                                gboolean select, void **out_data)
{
    // returns item index or -1 (error)
    intptr_t case_insensitive;
    int (*strcmp_func) (const char*, const char*);
    char *value = NULL;
    int index = -1;
    if (out_data) {
        *out_data = NULL;
    }
    if (!str) {
        return -1;
    }
    case_insensitive = GPOINTER_TO_INT (g_object_get_data (G_OBJECT(listbox), "w_search_insensitive"));
    if (case_insensitive) {
        strcmp_func = strcasecmp;
    } else {
        strcmp_func = strcmp;
    }
    ///printf ("case_insensitive: %d\n", case_insensitive);
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model   = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW(listbox));
        GtkTreeIter iter;
        gboolean valid = gtk_tree_model_get_iter_first (model, &iter);
        while (valid)
        {
            index++;
            gtk_tree_model_get (model, &iter, 0, &value, -1);
            g_warn_if_fail (value != NULL);
            if (strcmp_func (value, str) == 0)
            { // found
                g_free (value); // string values must be freed
                if (out_data) {
                    g_warn_if_fail (gtk_tree_model_get_n_columns(model) == 2);
                    gtk_tree_model_get (model, &iter, 1, out_data, -1);
                }
                if (select) {
                    gtk_tree_selection_select_iter (sel, &iter);
                }
                return index;
                break;
            }
            g_free (value);
            valid = gtk_tree_model_iter_next (model, &iter);
        }
        return -1;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList    *items = GTK_LIST(listbox)->children;
        GtkWidget *label_w, *list_item;
        GList *igl;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            index++;
            list_item = GTK_WIDGET(igl->data); // igl->data = GtkListItem
            label_w   = gtk_bin_get_child (GTK_BIN(list_item));
            value     = (char*) gtk_label_get_text (GTK_LABEL(label_w));
            if (strcmp_func (value, str) == 0)
            { // found
                if (out_data) {
                    *out_data = g_object_get_data (G_OBJECT(list_item), "wdata");
                }
                if (select) {
                    gtk_list_select_child (GTK_LIST(listbox), list_item);
                }
                return index;
            }
        }
    }
#endif
    return -1;
}


/*--------------------------------------------------------------
/              w_gtk_listbox_search_data
/-------------------------------------------------------------*/

int w_gtk_listbox_search_data (GtkWidget *listbox, gpointer wdata, gboolean select)
{
    // returns item index or -1 (error)
    int index = -1;
    gpointer value = NULL;
    if (!wdata) {
        return -1;
    }
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model   = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW(listbox));
        g_warn_if_fail (gtk_tree_model_get_n_columns(model) == 2);
        GtkTreeIter iter;
        gboolean valid = gtk_tree_model_get_iter_first (model, &iter);
        while (valid)
        {
            index++;
            gtk_tree_model_get (model, &iter, 1, &value, -1);
            if (value == wdata) { // found
                if (select) {
                    gtk_tree_selection_select_iter (sel, &iter);
                }
                return index;
            }
            valid = gtk_tree_model_iter_next (model, &iter);
        }
        return -1;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        index = gtk_clist_find_row_from_data (GTK_CLIST(listbox), wdata);
        if (index > -1 && select) {
            gtk_clist_select_row (GTK_CLIST(listbox), index, 0);
        }
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList    *items = GTK_LIST(listbox)->children;
        GList *igl;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            index++; // igl->data = GtkListItem
            value = g_object_get_data (G_OBJECT(igl->data), "wdata");
            if (value == wdata) { // found
                if (select) {
                    gtk_list_item_select (GTK_LIST_ITEM(G_OBJECT(igl->data)));
                    ///gtk_list_select_child (GTK_LIST(list), list_item);
                }
                return index;
            }
        }
    }
#endif
    return -1;
}


/*--------------------------------------------------------------
/         w_gtk_listbox_set_search_case_insensitive
/-------------------------------------------------------------*/

void w_gtk_listbox_set_search_case_insensitive (GtkWidget *listbox, gboolean insensitive)
{
    if (insensitive) {
        g_object_set_data (G_OBJECT(listbox), "w_search_insensitive", GINT_TO_POINTER(1));
    } else {
        g_object_set_data (G_OBJECT(listbox), "w_search_insensitive", GINT_TO_POINTER(0));
    }
}

/*--------------------------------------------------------------
/         w_gtk_listbox_get_search_case_insensitive
/-------------------------------------------------------------*/

gboolean w_gtk_listbox_get_search_case_insensitive (GtkWidget *listbox)
{
    intptr_t ret;
    ret = GPOINTER_TO_INT (g_object_get_data (G_OBJECT(listbox), "w_search_insensitive"));
    return ret;
}


/*--------------------------------------------------------------
/            w_gtk_listbox_populate_from_glist
/-------------------------------------------------------------*/

void w_gtk_listbox_populate_from_glist (GtkWidget *listbox, GList *strings, int default_index)
{
    GList * list;
    char * text;
    int len = 0;

    w_gtk_listbox_clear (listbox);
    if (!strings) {
        return; // nothing to add
    }
    for (list = strings;  list;  list = list->next)
    {
        text = (char *) list->data;
        w_gtk_listbox_append_text (listbox, text);
        len++;
    }
    if (default_index >= len) {
        default_index = 0;
    }
    if (default_index >= 0) {
        w_gtk_listbox_set_active_index (listbox, default_index);
    }
}


/*--------------------------------------------------------------
/             w_gtk_listbox_populate_from_strv
/-------------------------------------------------------------*/

void w_gtk_listbox_populate_from_strv (GtkWidget *listbox,
                                       const char **strv,
                                       int default_index,
                                       gboolean gettext)
{
    int i;
    char *str;

    w_gtk_listbox_clear (listbox);
    if (!strv || !*strv) {
        return; /* nothing to add */
    }
    for (i = 0; strv[i]; i++) {
#ifdef ENABLE_NLS
        str = gettext ? gettext(strv[i]) : (char*) strv[i];
#else
        str = (char*) strv[i];
#endif
        w_gtk_listbox_append_text (listbox, str);
    }
    if (default_index >= i) {
        default_index = 0;
    }
    if (default_index >= 0) {
        w_gtk_listbox_set_active_index (listbox, default_index);
    }
}


/*--------------------------------------------------------------
/            w_gtk_listbox_select_or_prepend
/-------------------------------------------------------------*/

void w_gtk_listbox_select_or_prepend (GtkWidget *listbox, const char *str)
{
    if (!str || !*str) {
        return;
    }
    if (!w_gtk_listbox_search_text (listbox, str, TRUE, NULL)) {
        w_gtk_listbox_prepend_text (listbox, str);
        w_gtk_listbox_set_active_index (listbox, 0);
    }
}


/*--------------------------------------------------------------
/            w_gtk_listbox_scroll_to_selected
/-------------------------------------------------------------*/

void w_gtk_listbox_scroll_to_selected (GtkWidget *listbox)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        //GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        //GtkTreePath  *path  = gtk_tree_path_new_from_indices (position, -1);
        //GtkTreeIter iter;
        //if (gtk_tree_model_get_iter (model, &iter, path)) {
        //    gtk_list_store_remove (GTK_LIST_STORE(model), &iter);
        //}
        //gtk_tree_path_free (path);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        int selrow;
        GList *selection = GTK_CLIST(listbox)->selection;
        if (selection) {
            selrow = GPOINTER_TO_INT (selection->data);
            gtk_clist_moveto (GTK_CLIST(listbox), selrow, 0, 0.5, 0.5);
        }
    }
    // GtkList doesn't have a (usable) function to scroll to selected row
#endif
}


/*--------------------------------------------------------------
/              w_gtk_listbox_select_all
/-------------------------------------------------------------*/

void w_gtk_listbox_select_all (GtkWidget *listbox)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeView      *tree = GTK_TREE_VIEW (listbox);
        GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
        gtk_tree_selection_select_all (tsel);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_select_all (GTK_CLIST(listbox));
    }
    else if (GTK_IS_LIST(listbox))
    {
        gtk_list_select_all (GTK_LIST(listbox));
    }
#endif
}


/*--------------------------------------------------------------
/            w_gtk_listbox_unselect_all
/-------------------------------------------------------------*/

void w_gtk_listbox_unselect_all (GtkWidget *listbox)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeView      *tree = GTK_TREE_VIEW(listbox);
        GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
        gtk_tree_selection_unselect_all (tsel);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_unselect_all (GTK_CLIST(listbox));
    }
    else if (GTK_IS_LIST(listbox))
    {
        gtk_list_unselect_all (GTK_LIST(listbox));
    }
#endif
}


/*--------------------------------------------------------------
/            w_gtk_listbox_get_selected_count
/-------------------------------------------------------------*/

int w_gtk_listbox_get_selected_count (GtkWidget *listbox)
{
    int count = 0;
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeView      *tree = GTK_TREE_VIEW(listbox);
        GtkTreeSelection *tsel = gtk_tree_view_get_selection (tree);
        count = gtk_tree_selection_count_selected_rows (tsel);
        return count;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        GList *sel_rows = GTK_CLIST(listbox)->selection;
        count = g_list_length (sel_rows);
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList *sel_rows = GTK_LIST(listbox)->selection;
        count = g_list_length (sel_rows);
    }
#endif
    return count;
}


/*--------------------------------------------------------------
/            w_gtk_listbox_set_selection_mode
/-------------------------------------------------------------*/

void w_gtk_listbox_set_selection_mode (GtkWidget *listbox, GtkSelectionMode mode)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeSelection *tsel;
        tsel = gtk_tree_view_get_selection (GTK_TREE_VIEW(listbox));
        gtk_tree_selection_set_mode (tsel, mode);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
# if GTK_MAJOR_VERSION == 1
    if (mode == GTK_SELECTION_MULTIPLE) {
        mode = GTK_SELECTION_EXTENDED;
    }
# endif
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_set_selection_mode (GTK_CLIST(listbox), mode);
    }
    else if (GTK_IS_LIST(listbox))
    {
        gtk_list_set_selection_mode (GTK_LIST(listbox), mode);
    }
#endif
}


/*--------------------------------------------------------------
/            w_gtk_listbox_get_selection_mode
/-------------------------------------------------------------*/

GtkSelectionMode w_gtk_listbox_get_selection_mode (GtkWidget *listbox)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeSelection *tsel;
        tsel = gtk_tree_view_get_selection (GTK_TREE_VIEW(listbox));
        return gtk_tree_selection_get_mode (tsel);
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    guint mode = GTK_SELECTION_BROWSE;
    if (GTK_IS_CLIST(listbox))
    {
        mode = GTK_CLIST(listbox)->selection_mode;
    }
    else if (GTK_IS_LIST(listbox))
    {
        mode = GTK_LIST(listbox)->selection_mode;
    }
# if GTK_MAJOR_VERSION == 1
    if (mode == GTK_SELECTION_MULTIPLE) {
        mode = GTK_SELECTION_EXTENDED;
    }
# endif
#endif
    return mode;
}


/*--------------------------------------------------------------
/            w_gtk_listbox_set_headers_visible
/-------------------------------------------------------------*/

void w_gtk_listbox_set_headers_visible (GtkWidget *listbox, gboolean visible)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(listbox), visible);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        if (visible) {
            gtk_clist_column_titles_show (GTK_CLIST(listbox));
        } else {
            gtk_clist_column_titles_hide (GTK_CLIST(listbox));
        }
    }
    // GtkList doesn't deal with columns
#endif
}


/*--------------------------------------------------------------
/           w_gtk_listbox_set_headers_clickable
/-------------------------------------------------------------*/

void w_gtk_listbox_set_headers_clickable (GtkWidget *listbox, gboolean clickable)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        gtk_tree_view_set_headers_clickable (GTK_TREE_VIEW(listbox), clickable);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        if (clickable) {
            gtk_clist_column_titles_active (GTK_CLIST(listbox));
        } else {
            gtk_clist_column_titles_passive (GTK_CLIST(listbox));
        }
    }
    // GtkList doesn't deal with columns
#endif
}


/*--------------------------------------------------------------
/           w_gtk_listbox_set_column_visibility
/-------------------------------------------------------------*/

void w_gtk_listbox_set_column_visibility (GtkWidget *listbox, int column, gboolean visible)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeViewColumn *col;
        col = gtk_tree_view_get_column (GTK_TREE_VIEW(listbox), column);
        gtk_tree_view_column_set_visible (col, visible);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_set_column_visibility (GTK_CLIST(listbox), column, visible);
    }
    // GtkList doesn't deal with columns
#endif
}


/*--------------------------------------------------------------
/        w_gtk_listbox_set_column_header_clickable
/-------------------------------------------------------------*/

void w_gtk_listbox_set_column_header_clickable (GtkWidget *listbox, int column, gboolean clickable)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeViewColumn *col;
        col = gtk_tree_view_get_column (GTK_TREE_VIEW(listbox), column);
        gtk_tree_view_column_set_clickable (col, clickable);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        if (clickable) {
            gtk_clist_column_title_active (GTK_CLIST(listbox), column);
        } else {
            gtk_clist_column_title_passive (GTK_CLIST(listbox), column);
        }
    }
    // GtkList doesn't deal with columns
#endif
}


/*--------------------------------------------------------------
/           w_gtk_listbox_set_column_title
/-------------------------------------------------------------*/

void w_gtk_listbox_set_column_title (GtkWidget *listbox, int column, const char *title)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeViewColumn *col;
        col = gtk_tree_view_get_column (GTK_TREE_VIEW(listbox), column);
        gtk_tree_view_column_set_title (col, title);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_set_column_title (GTK_CLIST(listbox), column, title);
    }
    // GtkList doesn't deal with columns
#endif
}


/*--------------------------------------------------------------
/           w_gtk_listbox_set_column_auto_resize
/-------------------------------------------------------------*/

void w_gtk_listbox_set_column_auto_resize (GtkWidget *listbox, int column, gboolean autoresize)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeViewColumn *col;
        col = gtk_tree_view_get_column (GTK_TREE_VIEW(listbox), column);
        if (autoresize) {
            gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_AUTOSIZE);
        } else {
            gtk_tree_view_column_set_sizing (col, GTK_TREE_VIEW_COLUMN_FIXED);
        }
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_set_column_auto_resize (GTK_CLIST(listbox), column, autoresize);
    }
    // GtkList doesn't deal with columns
#endif
}


/*--------------------------------------------------------------
/           w_gtk_listbox_set_column_resizeable
/-------------------------------------------------------------*/

void w_gtk_listbox_set_column_resizeable (GtkWidget *listbox, int column, gboolean resizeable)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeViewColumn *col;
        col = gtk_tree_view_get_column (GTK_TREE_VIEW(listbox), column);
        gtk_tree_view_column_set_resizable (col, resizeable);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_set_column_resizeable (GTK_CLIST(listbox), column, resizeable);
    }
    // GtkList doesn't deal with columns
#endif
}
