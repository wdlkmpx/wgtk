/*
 * Public Domain

 Extended compatibility for GTK 2.24 (make apps that require older versions compile)

 Don't include this header, include w_gtk.h instead

 */

#ifndef __W_GTK_COMPAT2_H__
#define __W_GTK_COMPAT2_H__

#ifdef __cplusplus
extern "C"
{
#endif


#if GTK_MAJOR_VERSION <= 2

//==================================================
//                   GTK < 2.24
//==================================================
#if !GTK_CHECK_VERSION(2, 24, 0)
gboolean gtk_combo_box_get_has_entry (GtkComboBox *combo_box);
// GTK < 2.24  &&  GTK >= 2.4
#if GTK_CHECK_VERSION(2,4,0)
#define GtkComboBoxText      GtkComboBox
#define GtkComboBoxTextClass GtkComboBoxClass
#define GtkComboBoxPrivate   GtkComboBoxTextPrivate
#define GTK_TYPE_COMBO_BOX_TEXT            (gtk_combo_box_get_type ())
#define GTK_COMBO_BOX_TEXT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_COMBO_BOX, GtkComboBoxText))
#define GTK_COMBO_BOX_TEXT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_COMBO_BOX, GtkComboBoxTextClass))
#define GTK_IS_COMBO_BOX_TEXT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_COMBO_BOX))
#define GTK_IS_COMBO_BOX_TEXT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_COMBO_BOX))
#define GTK_COMBO_BOX_TEXT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_COMBO_BOX, GtkComboBoxTextClass))
#define gtk_combo_box_text_new()            gtk_combo_box_new_text()
#define gtk_combo_box_text_new_with_entry() gtk_combo_box_entry_new_text()
#define gtk_combo_box_text_append_text(combo,text)     gtk_combo_box_append_text(combo,text)
#define gtk_combo_box_text_insert_text(combo,pos,text) gtk_combo_box_insert_text(combo,pos,text)
#define gtk_combo_box_text_prepend_text(combo,text)    gtk_combo_box_prepend_text(combo,text)
#define gtk_combo_box_text_remove(combo,pos)           gtk_combo_box_remove_text(combo,pos)
#define gtk_combo_box_text_get_active_text(combo)      (gtk_combo_box_get_active_text(combo))
// these 2 may be problematic, may have to fix the code instead..
#define gtk_combo_box_set_entry_text_column(combo,cl)  gtk_combo_box_entry_set_text_column(GTK_COMBO_BOX_ENTRY(combo),cl)
#define gtk_combo_box_get_entry_text_column(combo)     (gtk_combo_box_entry_get_text_column(GTK_COMBO_BOX_ENTRY(combo)))
#endif
#endif

//==================================================
//                   GTK < 2.22
//==================================================
#if !GTK_CHECK_VERSION(2, 22, 0)
gboolean gtk_widget_send_focus_change (GtkWidget *widget, GdkEvent *event);
#endif

//==================================================
//                   GTK < 2.12
//==================================================

#if !GTK_CHECK_VERSION(2, 12, 0)
void gtk_widget_set_tooltip_text (GtkWidget *widget, const char *text);
char *gtk_widget_get_tooltip_text (GtkWidget *widget);
void gtk_widget_set_has_tooltip (GtkWidget *widget, gboolean has_tooltip);
gboolean gtk_widget_get_has_tooltip (GtkWidget *widget);
#define gtk_widget_set_tooltip_markup gtk_widget_set_tooltip_text
#define gtk_widget_get_tooltip_markup gtk_widget_get_tooltip_text
#endif

//==================================================
//                   GTK < 2.4
//==================================================

#if !GTK_CHECK_VERSION(2, 4, 0)
#define GtkComboBox      GtkWidget
#define GtkComboBoxText  GtkWidget
#define GtkComboBoxEntry GtkWidget
#define GTK_COMBO_BOX       GTK_WIDGET
#define GTK_COMBO_BOX_ENTRY GTK_WIDGET
#define GTK_COMBO_BOX_TEXT  GTK_WIDGET
GtkWidget *gtk_combo_box_new_text (void);
GtkWidget *gtk_combo_box_entry_new_text (void);
void gtk_combo_box_append_text (GtkComboBox *combo_box, const gchar *text);
void gtk_combo_box_insert_text (GtkComboBox *combo_box, gint position, const gchar *text);
void gtk_combo_box_prepend_text (GtkComboBox *combo_box, const gchar *text);
void gtk_combo_box_remove_text (GtkComboBox *combo_box, gint position);
gchar *gtk_combo_box_get_active_text  (GtkComboBox *combo_box);
gint gtk_combo_box_get_active (GtkComboBox *combo_box);
void gtk_combo_box_set_active (GtkComboBox *combo_box, gint index_);
#define gtk_combo_box_new()                 gtk_combo_box_new_text()
#define gtk_combo_box_text_new()            gtk_combo_box_new_text()
#define gtk_combo_box_text_new_with_entry()	gtk_combo_box_entry_new_text()
#define gtk_combo_box_text_append_text(combo,text)     gtk_combo_box_append_text(combo,text)
#define gtk_combo_box_text_insert_text(combo,pos,text) gtk_combo_box_insert_text(combo,pos,text)
#define gtk_combo_box_text_prepend_text(combo,text)    gtk_combo_box_prepend_text(combo,text)
#define gtk_combo_box_text_remove(combo,pos)           gtk_combo_box_remove_text(combo,pos)
#define gtk_combo_box_text_get_active_text(combo)      (gtk_combo_box_get_active_text(combo))
#define gtk_combo_box_set_entry_text_column(combo,cl)
#define gtk_combo_box_get_entry_text_column(combo)     (0)
#endif

#endif // GTK2

#ifdef __cplusplus
}
#endif

#endif /* __W_GTK_COMPAT2_H__ */
