/*
 * Public Domain

 Include this header instead of gtkcompat.h
  (only use gtkcompat.h in projects that don't use this header)

 */


#ifndef __W_GTK_H__
#define __W_GTK_H__

#ifdef __cplusplus
extern "C"
{
#endif

#define W_GTK 1
#include "gtkcompat.h"
#include "w_gtk_compat2.h"

struct _WGtkActionEntry 
{
  const char *name;
  const char *icon_name;
  const char *label;
  const char *accel_str; /* accelerator */
  const char *tooltip;
  gpointer  callback;
};
typedef struct _WGtkActionEntry WGtkActionEntry;

// =================================================

void w_gtk_check_version (int gtk_ver);

void w_gtk_widget_change_tooltip (GtkWidget *widget, const char *new_text);
GtkWidget *w_gtk_widget_set_scrolled_window (GtkWidget *widget,
                                             GtkWidget *parent_box);

GtkWidget * w_gtk_window_new (const char * title,
                              GtkWindow * parent,
                              GtkApplication * app, // NULL gtkcompat.h < 3 = `void * app`
                              gboolean resizable,
                              GtkWidget ** main_vbox  /* out */);

GtkWidget * w_gtk_dialog_new (const char * title,
                              GtkWindow * parent,
                              gboolean resizable,
                              GtkWidget ** main_vbox); /* out */

GtkWidget * w_gtk_frame_new (char * label, /* supports markup */
                             gboolean center_label,
                             GtkWidget * parent_box,
                             GtkWidget * label_widget);

GtkWidget * w_gtk_frame_vbox_new (char * label, /* supports markup */
                                  int children_spacing,
                                  GtkWidget * parent_box,
                                  GtkWidget * frame_label_widget); /* NULL */

GtkWidget * w_gtk_expander_vbox_new (char * label,
                                     int children_spacing,
                                     GtkWidget * parent_box);

GtkWidget * w_gtk_button_new (const char * label, /* supports markup if not using icons */
                              const char * icon_name,
                              gpointer clicked_cb,
                              gpointer cdata);

GtkWidget * w_gtk_toolbar_separator_new  (GtkWidget *box_toolbar);
GtkWidget * w_gtk_toolbar_button_new (GtkWidget *box_toolbar,
                                      const char *label, /* DOES NOT support markup */
                                      const char *tooltip,
                                      const char *icon_name,
                                      GtkWidget *wicon,
                                      gpointer clicked_cb, gpointer cdata);

GtkWidget * w_gtk_button_new_from_actions (WGtkActionEntry *actions,
                                           const char *action_name,
                         /* can be NULL */ GtkWidget *toolbar);

void w_gtk_button_flat (GtkWidget * button, gboolean reduce_size);

GtkWidget * w_gtk_image_new_from_icon_name (const char *icon_name, GtkIconSize size);
void w_gtk_image_set_from_icon_name (GtkImage *img, const char *icon_name, GtkIconSize size);
void w_gtk_button_set_icon_name (GtkButton *button, const char *icon_name);

GtkWidget * w_gtk_notebook_add_tab_grid (GtkWidget * notebook, char * label_str);
GtkWidget * w_gtk_notebook_add_tab_box (GtkWidget * notebook, char * label_str);

#if GTK_CHECK_VERSION(2,0,0)
int  w_gtk_tree_view_get_num_selected (GtkWidget *tv);
void w_gtk_tree_view_clear (GtkWidget *tv);
void w_gtk_tree_view_select_all (GtkWidget *tv);
void w_gtk_tree_view_deselect_all (GtkWidget *tv);
void w_gtk_tree_view_select_row (GtkWidget *tv, int n);
#endif

// ==================================================================

struct WGtkSimpleListItem_
{
    int index;
    char *text;
    gpointer data;
};

typedef struct WGtkSimpleListItem_ WGtkSimpleListItem;

// ==================================================================
// w_gtk_combobox.h

GtkWidget * w_gtk_combobox_new ();
GtkWidget * w_gtk_combobox_new_with_entry ();
GtkWidget * w_gtk_combobox_get_entry (GtkWidget *combo);

int w_gtk_combobox_add_changed_handler (GtkWidget *combo, gpointer cb, gpointer cb_data);
GtkWidget *w_gtk_combobox_cb_ensure_combo (GtkWidget *widget);

const char *w_gtk_combobox_get_entry_text (GtkWidget *combo);
void w_gtk_combobox_set_enable_entry (GtkWidget *combo, gboolean enabled);
void w_gtk_combobox_set_entry_text (GtkWidget *combo, const char *text);

int  w_gtk_combobox_get_count (GtkWidget *combo);
void w_gtk_combobox_clear (GtkWidget *combo);

void w_gtk_combobox_insert (GtkWidget *combo, int position, const char *text, gpointer wdata);
void w_gtk_combobox_append (GtkWidget *combo, const char *text, gpointer wdata);
void w_gtk_combobox_prepend (GtkWidget *combo, const char *text, gpointer wdata);

void w_gtk_combobox_insert_text (GtkWidget *combo, int position, const char *text);
void w_gtk_combobox_append_text (GtkWidget *combo, const char *text);
void w_gtk_combobox_prepend_text (GtkWidget *combo, const char *text);
void w_gtk_combobox_remove (GtkWidget *combo, int position); // index

void w_gtk_combobox_get_item (GtkWidget *combo, int position, WGtkSimpleListItem *out_item);
char *w_gtk_combobox_get_item_text (GtkWidget *combo, int position);
char *w_gtk_combobox_get_item_data (GtkWidget *combo, int position);

int w_gtk_combobox_get_selected (GtkWidget *combo, WGtkSimpleListItem *out_item);
char *w_gtk_combobox_get_active_text (GtkWidget *combo);
int  w_gtk_combobox_get_active_index (GtkWidget *combo);
void w_gtk_combobox_set_active_index (GtkWidget *combo, int index);

int w_gtk_combobox_search_text (GtkWidget *combo, const char *str,
                                gboolean select, void **out_data);
int w_gtk_combobox_search_data (GtkWidget *combo, gpointer wdata, gboolean select);
void w_gtk_combobox_set_search_case_insensitive (GtkWidget *combo, gboolean csensitive);
gboolean w_gtk_combobox_get_search_case_insensitive (GtkWidget *combo);

void w_gtk_combobox_populate_from_glist (GtkWidget *combo, GList *strings, int default_index);
void w_gtk_combobox_populate_from_strv (GtkWidget *combo,
                                        const char **strv,
                                        int default_index,
                                        gboolean gettext);
void w_gtk_combobox_select_or_prepend (GtkWidget *combo, const char *str);
void w_gtk_combo_box_set_w_model (GtkWidget *combo, gboolean sort);

// ==================================================================

#if GTK_MAJOR_VERSION <= 2
void gtk_widget_set_hexpand (GtkWidget *widget, gboolean expand);
void gtk_widget_set_vexpand (GtkWidget *widget, gboolean expand);
void gtk_widget_set_halign (GtkWidget *widget, GtkAlign align);
void gtk_widget_set_valign (GtkWidget *widget, GtkAlign align);
void gtk_widget_set_margin_start (GtkWidget *widget, gint margin);
void gtk_widget_set_margin_end (GtkWidget *widget, gint margin);
// deprecated since 3.12+: gtk_widget_set_margin_left() / gtk_widget_set_margin_right()
#define gtk_widget_set_margin_left gtk_widget_set_margin_start
#define gtk_widget_set_margin_right gtk_widget_set_margin_end
void gtk_widget_set_margin_bottom (GtkWidget *widget, gint margin);
void gtk_widget_set_margin_top (GtkWidget *widget, gint margin);
GtkWidget * gtk_grid_new (void);
// gtk_widget_set_*align / gtk_widget_set_*expand / gtk_widget_set_margin_*
//   will only work after gtk_grid_attach, calling them before has no effect...
void gtk_grid_attach (GtkGrid *grid, GtkWidget *child, gint left, gint top, gint width, gint height);

#define GTK_FONT_CHOOSER GTK_WIDGET  // GtkFontChooser: gtk3.2+
#define GtkFontChooser GtkWidget //can't be GtkFontSelection, several wigdet classes are involved
char *gtk_font_chooser_get_font (GtkFontChooser* fontchooser);
void gtk_font_chooser_set_font (GtkFontChooser *fontchooser, const char *fontname);
void gtk_font_chooser_set_preview_text (GtkFontChooser *fontchooser, const char *text);
char * gtk_font_chooser_get_preview_text (GtkFontChooser *fontchooser);
// 2.14: GtkFontSelection adopts Pango
#if GTK_CHECK_VERSION(2,14,0)
PangoFontFace * gtk_font_chooser_get_font_face (GtkFontChooser *fontchooser);
PangoFontFamily * gtk_font_chooser_get_font_family (GtkFontChooser *fontchooser);
#endif
int gtk_font_chooser_get_font_size (GtkFontChooser *fontchooser);
GtkWidget* gtk_font_chooser_widget_new (void);
#define gtk_font_chooser_dialog_new(title,parent) (gtk_font_selection_dialog_new(title))


// GTK < 2.24
#if !GTK_CHECK_VERSION(2, 24, 0)
gboolean gtk_combo_box_get_has_entry (GtkComboBox *combo_box);
#endif
#if !GTK_CHECK_VERSION(2, 22, 0)
gboolean gtk_widget_send_focus_change (GtkWidget *widget, GdkEvent *event);
#endif
#endif

// =================================================
//                  GtkButton
// =================================================

typedef struct _WGtkButtonParams
{
    GtkWidget *w;
} WGtkButtonParams;

// =================================================
//            GtkGrid / GtkTable
// =================================================

typedef struct _WGtkAlignmentParams
{
    GtkWidget *w;
    GtkAlign align;
    int margin_start;
    int margin_end;
    int margin_top;
    int margin_bottom;
    int width;
} WGtkAlignmentParams;

// the same WGtkGridParams can be used with any compatible GtkGrid/GtkTable widget
// just change .table / .cols and reset .row (0) to begin adding more rows...
typedef struct _WGtkGridParams
{
    int cols;
    int row;
    GtkWidget *table;
    WGtkAlignmentParams c1;
    WGtkAlignmentParams c2;
    WGtkAlignmentParams c3;
    WGtkAlignmentParams c4;
} WGtkGridParams;

#define WGtkTableParams WGtkGridParams

GtkWidget * w_gtk_grid_new (GtkWidget *parent_box, int row_spacing);
void w_gtk_grid_append_row (WGtkTableParams *t);
#define w_gtk_table_new w_gtk_grid_new
#define w_gtk_table_append_row(WGtkTableParams) w_gtk_table_append_row(WGtkTableParams)

GtkWidget  * w_gtk_recent_menu_new (const char * application, gpointer activated_cb);
#if GTK_CHECK_VERSION(2,4,0)
void w_gtk_action_group_destroy_action (GtkActionGroup *action_group, const char *action_name);
#endif

// =================================================

#if GTK_CHECK_VERSION(2,0,0)

void w_gtk_text_view_append (GtkWidget *tw, const char *tag, char *str);
void w_gtk_text_view_scroll_to_end (GtkWidget *tw);
void w_gtk_text_view_scroll_to_start (GtkWidget *tw);

void EditorClear (GtkTextView *tw);
void EditorDeleteLine (GtkTextView *tw, int linenum);
void EditorGetCurIter (GtkTextView *tw, GtkTextIter *iter);
char *EditorGetCurLine (GtkTextView *tw);
int EditorGetCurLineLen (GtkTextView *tw);
int EditorGetCurLineNum (GtkTextView *tw);
char *EditorGetCurWord (GtkTextView *tw, GtkTextIter *start_iter, GtkTextIter *end_iter);
char *EditorGetLine (GtkTextView *tw, int linenum);
void EditorGetLineIters (GtkTextView *tw, int linenum, GtkTextIter *start, GtkTextIter *end);
void EditorGetLineOffsets (GtkTextView *tw, int linenum, int *start, int *end);
int EditorGetNumLines (GtkTextView *tw);
int EditorGetSelectedLineNums (GtkTextView *tw, int *startline, int *endline);
char *EditorGetSelectedText (GtkTextView *tw, int *start, int *end, int *selcount);
gboolean EditorHasSelection (GtkTextView *tw);
int EditorInsertLine (GtkTextView *tw, int linenum, char *txt);
int EditorMoveToLineEnd (GtkTextView *tw);
int EditorMoveToLineStart (GtkTextView *tw);
gboolean EditorSelectLine (GtkTextView *tw, int linenum, gboolean del_eol);
gboolean EditorSelectOneChar (GtkTextView *tw);
void EditorSetSelectedText (GtkTextView *tw, char *text);
//void EditorTextCen (GtkTextView *tw, int center, int width, int usetab);
void EditorTextChangeCase (GtkTextView *tw);
void EditorTextConcatenateLines (GtkTextView *tw);
void EditorTextSplitLine (GtkTextView *tw, int linelen);
//void EditorTextTabify (GtkTextView *tw, int tabify, int tabsize);
void EditorTextTitleCase (GtkTextView *tw);

gchar *TextViewGetText (GtkTextView *tw);
void TextViewSetText (GtkTextView *tw, gchar *text);

#endif

#ifdef __cplusplus
}
#endif

#endif /* __W_GTK_H__ */
