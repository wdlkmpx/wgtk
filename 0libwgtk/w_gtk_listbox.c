/*
 * Public Domain
 */

// - item index starts at 0
// - invalid item [active] index is -1
// - returned item [active] text must be freed

// Supported widgets:
// - GtkListBox    (gtk3.10+) [TODO: has not been tested]
// - GtkTreeView   (gtk2/3/4)
// - GtkCList      (gtk1/2)
// - GtkList       (gtk1/2)

// support is incomplete

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

#if GTK_MAJOR_VERSION == 3
// Gtk4 removed GtkBin / GtkContainer
#define gtk_list_box_row_get_child(row) gtk_bin_get_child(GTK_BIN(row))
#define gtk_list_box_row_set_child(row,label) gtk_container_add(GTK_CONTAINER(row),label)
///#define gtk_list_box_remove(listbox,row) gtk_container_remove(GTK_CONTAINER(listbox),GTK_WIDGET(row))
#define gtk_list_box_remove(listbox,row)   gtk_widget_destroy(GTK_WIDGET(row))
#define gtk_list_box_append(listbox,child) gtk_container_add(GTK_CONTAINER(listbox),child)
#endif


void w_gtk_listbox_set_selection_mode (GtkWidget *listbox, GtkSelectionMode mode);
void w_gtk_listbox_scroll_to_selected (GtkWidget *listbox);
void w_gtk_listbox_remove_selected_rows (GtkWidget *listbox);
void w_gtk_listbox_append (GtkWidget *listbox, const char *text, gpointer wdata);

/*--------------------------------------------------------------
/                  w_gtk_listbox_simple_new
/-------------------------------------------------------------*/

/*
   Simple listbox: `text` and `data` fields
   simple listbox doesn't include a header by default
   text is a char array (char*)
   data is a [hidden] gpointer (void*), a pointer to any object

   - GtkListbox
     * text is a GtkLabel text
     * data is GtkListBoxRow "itemdata" (object data)
   - GtkTreeView
     * text is field 0 of a GtkListStore
     * data is field 1 of a GtkListStore
   - GtkCList
     * text is row_text
     * data is row_data
   - GtkList
     * text is a GtkListItem label
     * data is GtkListItem "itemdata" (object data)
*/

#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
GtkWidget * w_gtk_tree_view_new_simple_list ()
{
    GtkWidget *treeview;
    GtkListStore *store;
    store = gtk_list_store_new (2,
                                G_TYPE_STRING,
                                G_TYPE_POINTER); // hidden, this is "itemdata_col"
    GtkCellRenderer *renderer;
    GtkTreeViewColumn *column;
    treeview = gtk_tree_view_new_with_model (GTK_TREE_MODEL(store));
    g_object_unref (G_OBJECT(store));
    gtk_tree_view_set_headers_visible (GTK_TREE_VIEW(treeview), FALSE);

    g_object_set_data (G_OBJECT(treeview), "itemdata_col", GINT_TO_POINTER(1));

    renderer = gtk_cell_renderer_text_new();
    column   = gtk_tree_view_column_new_with_attributes ("", renderer,
                                                         "text", 0, NULL);
    gtk_tree_view_append_column (GTK_TREE_VIEW(treeview), column);
    return treeview;
}
#endif

GtkWidget * w_gtk_listbox_simple_new (gboolean selection_multiple)
{
    GtkWidget *listbox = NULL;
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    W_DEBUG_PUTS ("w_gtk_listbox: new GtkTreeView simple list");
    listbox = w_gtk_tree_view_new_simple_list (selection_multiple);
#else // GTK1
    W_DEBUG_PUTS ("w_gtk_listbox: new GtkCList");
    listbox = gtk_clist_new (1);
    gtk_clist_column_titles_hide (GTK_CLIST(listbox));
    gtk_clist_columns_autosize (GTK_CLIST(listbox));
    /// GtkList is too primitive and buggy
    ///listbox = gtk_list_new ();
#endif
    if (selection_multiple) {
        w_gtk_listbox_set_selection_mode (listbox, GTK_SELECTION_MULTIPLE);
    }
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(widget)) {
        W_DEBUG_PUTS ("wlistbox changed signal: it's GtkListBox");
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
#   if GTK_CHECK_VERSION(4,0,0) // Gtk4 removed GtkContainer
        // TODO: dubious logic, has not been tested
        GtkListBoxRow *row = gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), 0);
        while (row) {
            gtk_list_box_remove (GTK_LIST_BOX(listbox), GTK_WIDGET(row));
            row = gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), 0);
        }
#   else
        ///gtk_container_foreach (GTK_CONTAINER(listbox), (void*)gtk_widget_destroy, NULL);
        GList *children = gtk_container_get_children (GTK_CONTAINER(listbox));
        GList *igl;
        for (igl = children; igl != NULL; igl = igl->next) {
            gtk_widget_destroy (GTK_WIDGET(igl->data));
        }
        g_list_free (children);
#   endif
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_clear (GTK_CLIST(listbox));
        ///gtk_clist_columns_autosize (GTK_CLIST(listbox));
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
#   if GTK_CHECK_VERSION(4,0,0) // Gtk4 removed GtkContainer
        while (gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), count)) {
            count++;
        }
#   else
        GList *children = gtk_container_get_children (GTK_CONTAINER(listbox));
        count = g_list_length (children);
        g_list_free (children);
#   endif
        return count;
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        GtkWidget *wlabel  = gtk_label_new (text);
        GtkWidget *row     = gtk_list_box_row_new ();
        gtk_label_set_xalign (GTK_LABEL(wlabel), 0.0); // left
        gtk_list_box_row_set_child (GTK_LIST_BOX_ROW(row), wlabel);
        switch (position) {
           ///case -1: GTK4: gtk_list_box_append / GTK3: gtk_container_add
           case  0: gtk_list_box_prepend (GTK_LIST_BOX(listbox), row); break;
           default: gtk_list_box_insert (GTK_LIST_BOX(listbox), row, position);
        }
        if (wdata) {
            g_object_set_data (G_OBJECT(row), "itemdata", wdata);
        }
        // this is required, otherwise the row may be hidden
        gtk_widget_show (wlabel);
        gtk_widget_show (row);
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
        if (text) {
            gtk_clist_set_text (GTK_CLIST(listbox), row, 0, text);
        }
        if (wdata) {
            gtk_clist_set_row_data (GTK_CLIST(listbox), row, wdata);
        }
    }
    else if (GTK_IS_LIST(listbox))
    {
        GtkWidget *item;          // GtkListItem
        GList     *glist = NULL;  // list of GtkListItems
        if (!text) return;
        item  = gtk_list_item_new_with_label (text);
        glist = g_list_append (glist, item);
        /// position -1 = gtk_list_append_items (GTK_LIST(list), glist); // gtk_container_add(GTK_CONTAINER(list), item);
        /// position  0 = gtk_list_prepend_items (GTK_LIST(list), glist);
        gtk_list_insert_items (GTK_LIST(listbox), glist, position);
        gtk_widget_show_all (item);
        if (wdata) {
            g_object_set_data (G_OBJECT(item), "itemdata", wdata);
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        GtkListBoxRow *row;
        row = gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), position);
        if (row) {
            gtk_list_box_remove (GTK_LIST_BOX(listbox), GTK_WIDGET(row));
        }
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
/           w_gtk_listbox_remove_selected_rows
/-------------------------------------------------------------*/

#if GTK_MAJOR_VERSION <= 2
static gint clist_sort_rownum (gconstpointer a, gconstpointer b)
{ // this is for GtkCList
    int aa = GPOINTER_TO_INT(a);
    int bb = GPOINTER_TO_INT(b);
    if (aa < bb) return -1;
    else if (aa == bb) return 0;
    else return 1;
}
#endif


void w_gtk_listbox_remove_selected_rows (GtkWidget *listbox)
{
    // - Care must be taken if the widget doesn't have a special function to delete all selected rows
    // - Always remove from last to first selected items (otherwise some rows are not deleted)
    // - Copy selection GLists where possible (otherwise segfaults / some rows are not deleted)
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkTreeSelection *tsel = gtk_tree_view_get_selection (GTK_TREE_VIEW(listbox));
        GtkTreeIter iter;
        gboolean valid;
        GtkTreePath *tpath;
        int count = gtk_tree_model_iter_n_children (model, NULL);
        if (count < 1) {
            return;
        }
        tpath = gtk_tree_path_new_from_indices (count-1, -1);
        valid = TRUE;
        while (valid) {
            if (gtk_tree_selection_path_is_selected(tsel, tpath)) {
                gtk_tree_model_get_iter (model, &iter, tpath);
                gtk_list_store_remove (GTK_LIST_STORE(model), &iter);
            }
            valid = gtk_tree_path_prev (tpath);
        }
        return;
    }
#endif
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        GList *selection = gtk_list_box_get_selected_rows (GTK_LIST_BOX(listbox));
        GList *igl = g_list_last (selection);
        for (  ; igl != NULL; igl = igl->prev)
        {
            gtk_list_box_remove (GTK_LIST_BOX(listbox), GTK_WIDGET(igl->data));
        }
        g_list_free (selection);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        GList *selection, *igl;
        selection = g_list_copy (GTK_CLIST(listbox)->selection);
        // the ->selection contains row numbers, but the order may be wrong (depends on how selection is done)
        // need to sort the GList so that the last selected rows are at the end of the GList
        selection = g_list_sort (selection, clist_sort_rownum);
        igl = g_list_last (selection);
        int rownum;
        for (  ; igl != NULL; igl = igl->prev)
        {
            rownum = GPOINTER_TO_INT (igl->data);
            gtk_clist_remove (GTK_CLIST(listbox), rownum);
        }
        g_list_free (selection);
        ///gtk_clist_columns_autosize (GTK_CLIST(listbox));
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList *selected = g_list_copy (GTK_LIST(listbox)->selection);
        gtk_list_remove_items (GTK_LIST(listbox), selected);
        g_list_free (selected);
    }
#endif
}


/*--------------------------------------------------------------
/               w_gtk_listbox_get_item
/-------------------------------------------------------------*/

void w_gtk_listbox_get_item (GtkWidget *listbox, int position, WGtkSimpleListItem *out_item)
{
    // if out_item->text != NULL, it must be freed
    g_return_if_fail (out_item != NULL);
    //g_free (out_item->text);
    memset (out_item, 0, sizeof(*out_item));
    out_item->index = position;
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
                                    0, &(out_item->text),
                                    1, &(out_item->data), -1);
            }
        } else { // normal ComboBoxText
            if (gtk_tree_model_get_iter (model, &iter, path)) {
                gtk_tree_model_get (model, &iter,
                                    0, &(out_item->text), -1);
            }
        }
        gtk_tree_path_free (path);
        return;
    }
#endif
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        // TODO
        GtkListBoxRow *row;
        GtkWidget *label_w;
        row = gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), position);
        if (row) {
            label_w = gtk_list_box_row_get_child (GTK_LIST_BOX_ROW(row));
            out_item->text = g_strdup (gtk_label_get_text (GTK_LABEL(label_w)));
            out_item->data = g_object_get_data (G_OBJECT(row), "itemdata");
        }
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_get_text (GTK_CLIST(listbox), position, 0, &(out_item->text)); 
        out_item->data = gtk_clist_get_row_data (GTK_CLIST(listbox), position);
    }
    else if (GTK_IS_LIST(listbox))
    {
        GtkWidget *label_w, *list_item;
        GList *items = GTK_LIST(listbox)->children;
        GList *nitem = g_list_nth (items, position);
        if (nitem) { // nitem->data = GtkListItem
            list_item = GTK_WIDGET(nitem->data); 
            label_w   = gtk_bin_get_child (GTK_BIN(list_item));
            out_item->text = g_strdup (gtk_label_get_text (GTK_LABEL(label_w)));
            out_item->data = g_object_get_data (G_OBJECT(list_item), "itemdata");
        }
    }
#endif
}

/*--------------------------------------------------------------
/              w_gtk_listbox_get_item_text
/-------------------------------------------------------------*/

char *w_gtk_listbox_get_item_text (GtkWidget *listbox, int position)
{
    WGtkSimpleListItem item;
    w_gtk_listbox_get_item (listbox, position, &item);
    return item.text; //must be freed
}

/*--------------------------------------------------------------
/              w_gtk_listbox_get_item_data
/-------------------------------------------------------------*/

char *w_gtk_listbox_get_item_data (GtkWidget *listbox, int position)
{
    WGtkSimpleListItem item;
    w_gtk_listbox_get_item (listbox, position, &item);
    return item.data;
}


/*--------------------------------------------------------------
/              w_gtk_listbox_get_selected
/-------------------------------------------------------------*/

int w_gtk_listbox_get_selected (GtkWidget *listbox, WGtkSimpleListItem *out_item)
{
    // Returns an integer which is the index of the currently active item
    //   or -1 if there's no active item.
    // if out_item->text != NULL, it must be freed
    int active = -1;
    if (out_item) {
        memset (out_item, 0, sizeof(*out_item));
        out_item->index = -1;
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

        if (out_item && active > -1)
        {
            out_item->index = active;
            if (gtk_tree_model_get_n_columns(model) == 2) {
                // the W combo created with w_gtk_listbox_new()
                gtk_tree_model_get (model, &iter,
                                    0, &(out_item->text),
                                    1, &(out_item->data), -1);
            } else { // normal ComboBoxText
                gtk_tree_model_get (model, &iter,
                                    0, &(out_item->text), -1);
            }
        }
        return active;
    }
#endif
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        GtkListBoxRow *selrow = gtk_list_box_get_selected_row (GTK_LIST_BOX(listbox));
        if (selrow) {
            active = gtk_list_box_row_get_index (selrow);
        }
        if (out_item && active > -1) {
            out_item->index = active;
            // TODO
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
            if (out_item && active > -1) {
                out_item->index = active;
                gtk_clist_get_text (GTK_CLIST(listbox), active, 0, &(out_item->text)); 
                out_item->data = gtk_clist_get_row_data (GTK_CLIST(listbox), active); 
            }
        }
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList *selection = GTK_LIST(listbox)->selection;
        if (selection) { // selection->data = GtkListItem
            active = g_list_index (GTK_LIST(listbox)->children, selection->data);
            ///active = gtk_list_child_position (GTK_LIST(list), selection->data);
            if (out_item && active > -1) {
                out_item->index = active;
                GtkWidget *list_item = GTK_WIDGET(selection->data);
                GtkWidget *label_w   = gtk_bin_get_child (GTK_BIN(list_item));
                out_item->text = g_strdup (gtk_label_get_text (GTK_LABEL(label_w)));
                out_item->data = g_object_get_data (G_OBJECT(list_item), "itemdata"); 
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
    WGtkSimpleListItem item;
    w_gtk_listbox_get_selected (listbox, &item);
    itext = item.text;
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
/            w_gtk_listbox_select_row
/-------------------------------------------------------------*/

void w_gtk_listbox_select_row (GtkWidget *listbox, int index)
{
    // index -1: no active item
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeView      *tree  = GTK_TREE_VIEW(listbox);
        GtkTreeSelection *tsel  = gtk_tree_view_get_selection (tree);
        GtkTreePath      *tpath = gtk_tree_path_new_from_indices (index, -1);
        gtk_tree_selection_select_path (tsel, tpath);
        if (gtk_tree_selection_path_is_selected (tsel, tpath)) {
            // scroll to (verified) selected row
            gtk_tree_view_set_cursor (GTK_TREE_VIEW(listbox), tpath, NULL, FALSE);
            gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(listbox), tpath, NULL, TRUE, 0.5, 0.5);
        }
        gtk_tree_path_free (tpath);
        return;
    }
#endif
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        GtkListBoxRow *row = gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), index);
        if (row) {
            gtk_list_box_select_row (GTK_LIST_BOX(listbox), row);
        }
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        gtk_clist_select_row (GTK_CLIST(listbox), index, 0);
        w_gtk_listbox_scroll_to_selected (listbox);
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        // TODO
        return -1;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        // TODO
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
                    *out_data = g_object_get_data (G_OBJECT(list_item), "itemdata");
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        // TODO
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
        GList *items = GTK_LIST(listbox)->children;
        GList *igl;
        for (igl = items; igl != NULL; igl = igl->next)
        {
            index++; // igl->data = GtkListItem
            value = g_object_get_data (G_OBJECT(igl->data), "itemdata");
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
        w_gtk_listbox_append (listbox, text, NULL);
        len++;
    }
    if (default_index >= len) {
        default_index = 0;
    }
    if (default_index >= 0) {
        w_gtk_listbox_select_row (listbox, default_index);
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
        w_gtk_listbox_append (listbox, str, NULL);
    }
    if (default_index >= i) {
        default_index = 0;
    }
    if (default_index >= 0) {
        w_gtk_listbox_select_row (listbox, default_index);
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
        w_gtk_listbox_prepend (listbox, str, NULL);
        w_gtk_listbox_select_row (listbox, 0);
    }
}


/*--------------------------------------------------------------
/            w_gtk_listbox_scroll_to_selected
/-------------------------------------------------------------*/

#if GTK_MAJOR_VERSION <= 2
static gboolean clist_scroll_to_selected_timeout (gpointer listbox)
{  /* HACK - Workaround GtkClist bug */
    GtkCList *clist = GTK_CLIST(listbox);
    if (clist->selection) {
        gtk_clist_moveto (listbox, GPOINTER_TO_INT(clist->selection->data), 0, 0.5, 0.5);
    }
    return G_SOURCE_REMOVE; /* run only once */
}
#endif

#if GTK_CHECK_VERSION(3,10,0)
static gboolean gtklistbox_scroll_to_selected_timeout (gpointer listbox)
{
    // https://stackoverflow.com/questions/62453446/scroll-to-selected-row-in-gtklistbox
    int y = -1;
    GtkListBoxRow *selrow;
    GtkAdjustment *adj;
    int row_height;
    selrow = gtk_list_box_get_selected_row (GTK_LIST_BOX(listbox));
    if (selrow) {
        // convert the row's Y coordinate to listbox coordinatee
        gtk_widget_translate_coordinates (GTK_WIDGET(selrow), listbox,
                                          0, 0,      /* src x,y */ 
                                          NULL, &y); /* dst x,y */ 
        adj = gtk_list_box_get_adjustment (GTK_LIST_BOX(listbox));
        if (y >= 0 && adj) {
            // scroll the vertical adjustment to center the row in the view port
            gtk_widget_get_preferred_height (GTK_WIDGET(selrow), NULL, &row_height);
            gtk_adjustment_set_value (adj, (double) y -
                                           (gtk_adjustment_get_page_size(adj)-(double)row_height)/2);
        }
    }
    return G_SOURCE_REMOVE; /* run only once */
}
#endif


void w_gtk_listbox_scroll_to_selected (GtkWidget *listbox)
{
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeSelection *sel = gtk_tree_view_get_selection (GTK_TREE_VIEW(listbox));
        GtkTreeModel *model;
        GtkTreeIter iter;
        GtkTreePath *tpath;
        if (gtk_tree_selection_get_selected (sel, &model, &iter)) {
            tpath = gtk_tree_model_get_path (model, &iter);
            gtk_tree_view_set_cursor (GTK_TREE_VIEW(listbox), tpath, NULL, FALSE);
            gtk_tree_view_scroll_to_cell (GTK_TREE_VIEW(listbox), tpath, NULL, TRUE, 0.5, 0.5);
            gtk_tree_path_free (tpath);
        }
        return;
    }
#endif
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        // TODO
        g_timeout_add (100, gtklistbox_scroll_to_selected_timeout, (gpointer) listbox);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        if (!gtk_widget_get_realized(listbox)) {
            // workaround GtkCList bug:
            // - scrolling doesn't happen when creating the widget
            // - apply timer (hack) to call the function after 100 ms
            g_timeout_add (100, clist_scroll_to_selected_timeout, (gpointer) listbox);
            return;
        }
        int selrow;
        GList *selection = GTK_CLIST(listbox)->selection;
        if (selection) {
            selrow = GPOINTER_TO_INT(selection->data);
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        gtk_list_box_select_all (GTK_LIST_BOX(listbox));
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        gtk_list_box_unselect_all (GTK_LIST_BOX(listbox));
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        GList *selrows = gtk_list_box_get_selected_rows (GTK_LIST_BOX(listbox));
        count = g_list_length (selrows);
        g_list_free (selrows);
        return count;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        GList *selrows = GTK_CLIST(listbox)->selection;
        count = g_list_length (selrows);
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList *selrows = GTK_LIST(listbox)->selection;
        count = g_list_length (selrows);
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        return gtk_list_box_set_selection_mode (GTK_LIST_BOX(listbox), mode);
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
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        return gtk_list_box_get_selection_mode (GTK_LIST_BOX(listbox));
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
    return mode;
#endif
    return GTK_SELECTION_BROWSE; // avoid gcc warnings
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


/*--------------------------------------------------------------
/           w_gtk_listbox_get_is_empty
/-------------------------------------------------------------*/

gboolean w_gtk_listbox_get_is_empty (GtkWidget *listbox)
{
    // This is more efficient than _get_count()
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkTreeIter iter;
        return (gtk_tree_model_get_iter_first(model,&iter) == FALSE);
    }
#endif
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        GtkListBoxRow *row = gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), 0);
        return (row == NULL);
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        return (GTK_CLIST(listbox)->row_list == NULL);
    }
    else if (GTK_IS_LIST(listbox))
    {
        return (GTK_LIST(listbox)->children == NULL);
    }
#endif
    return FALSE;
}


/*--------------------------------------------------------------
/       w_gtk_listbox_simple_move_selected_to_dest_list
/-------------------------------------------------------------*/

void w_gtk_listbox_simple_move_selected_to_dest_list (GtkWidget *listsrc,
                                                      GtkWidget *listdest)
{
    // - removing selected rows requires a different logic
    // - see w_gtk_listbox_remove_selected_rows()
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listsrc))
    {
        GtkTreeModel *modelsrc, *modeldest;
        GtkTreeIter itersrc, iterdest;
        GtkTreeSelection* selection;
        char *text = NULL;
        gpointer wdata = NULL;
        gboolean valid;

        modelsrc  = gtk_tree_view_get_model (GTK_TREE_VIEW(listsrc));
        selection = gtk_tree_view_get_selection (GTK_TREE_VIEW(listsrc));
        modeldest = gtk_tree_view_get_model (GTK_TREE_VIEW(listdest));

        valid = gtk_tree_model_get_iter_first (modelsrc, &itersrc);
        while (valid)
        {
            if (gtk_tree_selection_iter_is_selected(selection, &itersrc))
            {
                gtk_tree_model_get (modelsrc, &itersrc,
                                    0, &text,
                                    1, &wdata,
                                    -1);
                ///gtk_list_store_remove (GTK_LIST_STORE(modelsrc), &itersrc);
                //--
                gtk_list_store_append (GTK_LIST_STORE(modeldest), &iterdest);
                gtk_list_store_set (GTK_LIST_STORE(modeldest), &iterdest,
                                    0, text,
                                    1, wdata,
                                    -1);
                g_free (text);
            }
            valid = gtk_tree_model_iter_next (modelsrc, &itersrc);
        }
        w_gtk_listbox_remove_selected_rows (listsrc);
        return;
    }
#endif
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listsrc))
    {
        GList *selrows = gtk_list_box_get_selected_rows (GTK_LIST_BOX(listsrc));
        GList *igl;
        char *text = NULL;
        gpointer wdata = NULL;
        GtkWidget *label_w;
        for (igl = selrows ; igl != NULL; igl = igl->next)
        {
            wdata = g_object_get_data (G_OBJECT(igl->data), "itemdata");
            label_w = gtk_list_box_row_get_child (GTK_LIST_BOX_ROW(igl->data));
            text = (char*) gtk_label_get_text (GTK_LABEL(label_w));
            w_gtk_listbox_append (listdest, text, wdata);
        }
        g_list_free (selrows);
        w_gtk_listbox_remove_selected_rows (listsrc);
        return;
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listsrc))
    {
        static char *empty_row[] = { NULL, NULL };
        GList *selrows, *igl;
        int rnum, new_row;
        char *text = NULL;
        gpointer wdata = NULL;
        selrows = g_list_copy (GTK_CLIST(listsrc)->selection);
        // need to sort the selrows GList to get a better list...
        selrows = g_list_sort (selrows, clist_sort_rownum);
        for (igl = selrows; igl != NULL; igl = igl->next)
        {
            rnum = GPOINTER_TO_INT(igl->data);
            gtk_clist_get_text (GTK_CLIST(listsrc), rnum, 0, &text);
            wdata = gtk_clist_get_row_data (GTK_CLIST(listsrc), rnum);
            //--
            new_row = gtk_clist_append (GTK_CLIST(listdest), empty_row);
            gtk_clist_set_text (GTK_CLIST(listdest), new_row, 0, text);
            gtk_clist_set_row_data (GTK_CLIST(listdest), new_row, wdata);
        }
        g_list_free (selrows);
        w_gtk_listbox_remove_selected_rows (listsrc);
    }
    else if (GTK_IS_LIST(listsrc))
    {
        GList *selected;
        selected = g_list_copy (GTK_LIST(listsrc)->selection);
        gtk_list_remove_items_no_unref (GTK_LIST(listsrc), selected);
        gtk_list_append_items (GTK_LIST(listdest), selected);
    }
#endif
}


/*--------------------------------------------------------------
/           w_gtk_listbox_get_all_rows_data
/-------------------------------------------------------------*/

GList * w_gtk_listbox_get_all_rows_data (GtkWidget *listbox)
{
    GList *rows_data = NULL;
    gpointer wdata = NULL;
#if (GTK_CHECK_VERSION(2,0,0) && GTK_MAJOR_VERSION <= 4)
    if (GTK_IS_TREE_VIEW(listbox))
    {
        GtkTreeModel *model = gtk_tree_view_get_model (GTK_TREE_VIEW(listbox));
        GtkTreeIter iter;
        int wdatacol = GPOINTER_TO_INT (g_object_get_data (G_OBJECT(listbox), "itemdata_col"));
        //fprintf (stderr, "wdatacol = %d\n", wdatacol); // debug
        gboolean valid;
        valid = gtk_tree_model_get_iter_first (model, &iter);
        while (valid)
        {
            gtk_tree_model_get (model, &iter,  wdatacol, &wdata,  -1);
            rows_data = g_list_prepend (rows_data, wdata);
            valid = gtk_tree_model_iter_next (model, &iter);
        }
        return g_list_reverse (rows_data);
    }
#endif
#if GTK_CHECK_VERSION(3,10,0)
    if (GTK_IS_LIST_BOX(listbox))
    {
        int i = 0;
        GtkListBoxRow *row = gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), i);
        while (row) {
            wdata = g_object_get_data (G_OBJECT(row), "itemdata");
            rows_data = g_list_prepend (rows_data, wdata);
            i++; //!!//
            row = gtk_list_box_get_row_at_index (GTK_LIST_BOX(listbox), i);
        }
        return g_list_reverse (rows_data);
    }
#endif
#if GTK_MAJOR_VERSION <= 2
    if (GTK_IS_CLIST(listbox))
    {
        GList *igl = GTK_CLIST(listbox)->row_list;
        GtkCListRow *row;
        for ( ; igl != NULL; igl = igl->next)
        {
            row = GTK_CLIST_ROW(igl);
            wdata = row->data;
            rows_data = g_list_prepend (rows_data, wdata);
        }
    }
    else if (GTK_IS_LIST(listbox))
    {
        GList *igl = GTK_LIST(listbox)->children;
        while (igl) { // igl->data = GtkListItem
            wdata = g_object_get_data (G_OBJECT(igl->data), "itemdata");
            rows_data = g_list_prepend (rows_data, wdata);
            igl = igl->next;
        }
    }
#endif
    return g_list_reverse (rows_data);
}


/*--------------------------------------------------------------
/           w_gtk_listbox_get_selected_rows_data
/-------------------------------------------------------------*/

GList * w_gtk_listbox_get_selected_rows_data (GtkWidget *listbox)
{
    return NULL;
}
