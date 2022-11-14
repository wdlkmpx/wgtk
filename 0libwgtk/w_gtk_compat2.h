/*
 * Public Domain

 Extended compatibility for GTK 2.24 (make apps compile with versions older than GTK2.24)

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
#define gtk_range_get_round_digits(range) (GTK_RANGE(range)->round_digits)
//#define gdk_window_get_visual(w)  (gdk_drawable_get_visual(GDK_DRAWABLE(w)))
#define gdk_window_get_screen(w)  (gdk_drawable_get_screen(GDK_DRAWABLE(w)))
#define gdk_window_get_display(w) (gdk_drawable_get_display(GDK_DRAWABLE(w)))
#define gtk_notebook_get_group_name gtk_notebook_get_group
#define gtk_notebook_set_group_name gtk_notebook_set_group
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
gboolean gtk_combo_box_get_has_entry (GtkComboBox *combo_box);
#endif
#endif


//==================================================
//                   GTK < 2.22
//==================================================

#if !GTK_CHECK_VERSION(2, 22, 0)
gboolean gtk_widget_send_focus_change (GtkWidget *widget, GdkEvent *event);
#if GTK_CHECK_VERSION(2,8,0) // Cairo stuff was added in gtk 2.8
cairo_surface_t * gdk_window_create_similar_surface (GdkWindow *window,cairo_content_t content, int width, int height);
#endif
#define gtk_statusbar_remove_all gtk_statusbar_pop
#define gtk_accessible_get_widget(a) ((a)->widget)
#define gtk_window_has_group(w) (GTK_WINDOW(w)->group != NULL)
#define gtk_window_group_get_current_grab(wg) \
  ((GTK_WINDOW_GROUP(wg)->grabs) ? GTK_WIDGET(GTK_WINDOW_GROUP(wg)->grabs->data) : NULL)
#define gtk_font_selection_dialog_get_font_selection(fsd)(GTK_FONT_SELECTION_DIALOG(fsd)->fontsel)
#define gtk_notebook_get_tab_hborder(n) (GTK_NOTEBOOK(n)->tab_hborder)
#define gtk_notebook_get_tab_vborder(n) (GTK_NOTEBOOK(n)->tab_vborder)
#define gtk_button_get_event_window(button) (GTK_BUTTON(button)->event_window)
#define gdk_visual_get_visual_type(visual)   (GDK_VISUAL(visual)->type)
#define gdk_visual_get_depth(visual)         (GDK_VISUAL(visual)->depth)
#define gdk_visual_get_byte_order(visual)    (GDK_VISUAL(visual)->byte_order)
#define gdk_visual_get_colormap_size(visual) (GDK_VISUAL(visual)->colormap_size)
#define gdk_visual_get_bits_per_rgb(visual)  (GDK_VISUAL(visual)->bits_per_rgb)
#define gtk_icon_view_get_item_orientation gtk_icon_view_get_orientation
#define gtk_icon_view_set_item_orientation gtk_icon_view_set_orientation
#endif


//==================================================
//                   GTK < 2.20
//==================================================

#if !GTK_CHECK_VERSION(2,20,0)
#define gtk_statusbar_get_message_area(sb) (gtk_bin_get_child (GTK_BIN (GTK_STATUSBAR(sb)->frame)))
#define gtk_widget_get_mapped(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_MAPPED) != 0)
#define gtk_widget_get_realized(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_REALIZED) != 0)
#define gtk_window_get_window_type(window) (GTK_WINDOW(window)->type)
#define gtk_widget_get_requisition(w,r) (*(r) = GTK_WIDGET(w)->requisition)
#define gtk_widget_set_mapped(w,yes) { \
   if (yes) GTK_WIDGET_SET_FLAGS(w,GTK_MAPPED); \
   else GTK_WIDGET_UNSET_FLAGS(w,GTK_MAPPED); \
}
#define gtk_widget_set_realized(w,yes) { \
   if (yes) GTK_WIDGET_SET_FLAGS(w,GTK_REALIZED); \
   else GTK_WIDGET_UNSET_FLAGS(w,GTK_REALIZED); \
}
#define gtk_range_get_slider_size_fixed(range) (GTK_RANGE(range)->slider_size_fixed)
#define gtk_range_get_min_slider_size(range) (GTK_RANGE(range)->min_slider_size)
#define gtk_entry_get_text_window(entry) (GTK_ENTRY(entry)->text_area)
#endif


//==================================================
//                   GTK < 2.18
//==================================================

#if !GTK_CHECK_VERSION(2,18,0)
#define gtk_widget_get_state(wid) (GTK_WIDGET (wid)->state)
#define gtk_widget_is_toplevel(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_TOPLEVEL) != 0)
#define gtk_widget_get_has_window(wid) !((GTK_WIDGET_FLAGS (wid) & GTK_NO_WINDOW) != 0)
#define gtk_widget_get_visible(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_VISIBLE) != 0)
#define gtk_widget_is_drawable(wid)  (GTK_WIDGET_VISIBLE (wid) && GTK_WIDGET_MAPPED (wid))
#define gtk_widget_get_sensitive(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_SENSITIVE) != 0)
#define gtk_widget_get_can_focus(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_CAN_FOCUS) != 0)
#define gtk_widget_has_focus(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_HAS_FOCUS) != 0)
#define gtk_widget_get_can_default(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_CAN_DEFAULT) != 0)
#define gtk_widget_get_receives_default(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_RECEIVES_DEFAULT) != 0)
#define gtk_widget_has_default(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_HAS_DEFAULT) != 0)
#define gtk_widget_has_grab(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_HAS_GRAB) != 0)
#define gtk_widget_get_app_paintable(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_APP_PAINTABLE) != 0)
#define gtk_widget_get_double_buffered(wid) ((GTK_WIDGET_FLAGS (wid) & GTK_DOUBLE_BUFFERED) != 0)
#define gtk_widget_set_allocation(w,alloc) (GTK_WIDGET(w)->allocation = *(alloc))
#define gtk_widget_get_allocation(w,alloc) (*(alloc) = GTK_WIDGET(w)->allocation)
#define gtk_widget_set_can_default(w,yes) { \
   if (yes) GTK_WIDGET_SET_FLAGS(w,GTK_CAN_DEFAULT); \
   else GTK_WIDGET_UNSET_FLAGS(w,GTK_CAN_DEFAULT); \
}
#define gtk_widget_set_can_focus(w,yes) { \
   if (yes) GTK_WIDGET_SET_FLAGS(w,GTK_CAN_FOCUS); \
   else GTK_WIDGET_UNSET_FLAGS(w,GTK_CAN_FOCUS); \
}
#define gtk_widget_set_has_window(w,yes) { \
   if (yes) GTK_WIDGET_UNSET_FLAGS(w,GTK_NO_WINDOW); \
   else GTK_WIDGET_SET_FLAGS(w,GTK_NO_WINDOW); \
}
#define gtk_widget_set_visible(w,yes) { \
   if (yes) gtk_widget_show(w); \
   else gtk_widget_hide(w); \
}
#define gtk_range_get_flippable(range) (GTK_RANGE(range)->flippable)
#define gdk_window_is_destroyed(w) (GDK_WINDOW_DESTROYED (GDK_WINDOW(w)))
#endif


//==================================================
//                   GTK < 2.16
//==================================================

#if !GTK_CHECK_VERSION(2,16,0)
#define GtkOrientable GtkWidget
#define GTK_ORIENTABLE GTK_WIDGET
GtkOrientation gtk_orientable_get_orientation (GtkOrientable  *widget);
#define gtk_menu_item_get_label(i) (gtk_label_get_label (GTK_LABEL(GTK_BIN(i)->child)))
#define gtk_menu_item_set_label(i,label) gtk_label_set_label(GTK_LABEL(GTK_BIN(i)->child), (label) ? label : "")
#define gtk_menu_item_get_use_underline(i) (gtk_label_get_use_underline (GTK_LABEL(GTK_BIN(i)->child)))
#define gtk_status_icon_set_tooltip_text gtk_status_icon_set_tooltip
#endif


//==================================================
//                   GTK < 2.14
//==================================================

#if !GTK_CHECK_VERSION(2,14,0)
#define gtk_dialog_get_action_area(dialog)    (GTK_DIALOG(dialog)->action_area)
#define gtk_dialog_get_content_area(dialog)   (GTK_DIALOG(dialog)->vbox)
#define gtk_widget_get_window(widget)         (GTK_WIDGET(widget)->window)
#define gtk_window_get_default_widget(window) (GTK_WINDOW(window)->default_widget)
#define gtk_menu_item_get_accel_path(i)       (GTK_MENU_ITEM(i)->accel_path)
#define gtk_menu_get_accel_path(menu)         (GTK_MENU(menu)->accel_path)
#define gtk_message_dialog_get_image(m)       (GTK_MESSAGE_DIALOG(m)->image)
#define gtk_entry_get_overwrite_mode(e)       (GTK_ENTRY(e)->overwrite_mode)
#define gtk_selection_data_get_length(data)   ((data)->length)
// -- GTK_ADJUSTMENT
#define gtk_adjustment_set_page_increment(a,val) ((a)->page_increment = (val))
#define gtk_adjustment_set_page_size(a,val)      ((a)->page_size = (val))
#define gtk_adjustment_set_lower(a,val)          ((a)->lower = (val))
#define gtk_adjustment_set_upper(a,val)          ((a)->upper = (val))
#define gtk_adjustment_get_page_increment ((a)->page_increment)
#define gtk_adjustment_get_page_size(a)   ((a)->page_size)
#define gtk_adjustment_get_lower(a)       ((a)->lower)
#define gtk_adjustment_get_upper(a)       ((a)->upper) 
// -- GTK_FONT_SELECTION_DIALOG
#define gtk_font_selection_dialog_get_ok_button(d)     ((d)->ok_button)
#define gtk_font_selection_dialog_get_cancel_button(d) ((d)->cancel_button)
#define gtk_font_selection_dialog_get_apply_button(d)  ((d)->apply_button)
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
#define gtk_window_set_startup_id(window,id)
#define gtk_window_set_opacity(window,opacity)
#define gtk_window_get_opacity(window) (1.0) // fully opaque
#endif

//==================================================
//                   GTK < 2.8
//==================================================

#if !GTK_CHECK_VERSION(2, 8, 0)
#define gtk_window_set_urgency_hint(window,bool)
#define gtk_window_get_urgency_hint(window) (FALSE)
#endif

//==================================================
//                   GTK < 2.6
//==================================================

#if !GTK_CHECK_VERSION(2, 6, 0)
#define gtk_window_set_focus_on_map(window,bool)
#define gtk_window_get_focus_on_map(window) (FALSE)
#endif

//==================================================
//                   GTK < 2.4
//==================================================

#if !GTK_CHECK_VERSION(2, 4, 0)
#define gtk_window_get_accept_focus(window) (TRUE)
#define gtk_window_set_accept_focus(window,bool)
// GtkComboBox implemented with w_gtk_combobox (GtkWidget)
// see w_gtk_combobox.c
#define GtkComboBox      GtkWidget
#define GtkComboBoxText  GtkWidget
#define GtkComboBoxEntry GtkWidget
#define GTK_COMBO_BOX       GTK_WIDGET
#define GTK_COMBO_BOX_ENTRY GTK_WIDGET
#define GTK_COMBO_BOX_TEXT  GTK_WIDGET
void gtk_combo_box_append_text (GtkComboBox *combo_box, const gchar *text);
void gtk_combo_box_insert_text (GtkComboBox *combo_box, gint position, const gchar *text);
void gtk_combo_box_prepend_text (GtkComboBox *combo_box, const gchar *text);
void gtk_combo_box_remove_text (GtkComboBox *combo_box, gint position);
gchar *gtk_combo_box_get_active_text  (GtkComboBox *combo_box);
gint gtk_combo_box_get_active (GtkComboBox *combo_box);
void gtk_combo_box_set_active (GtkComboBox *combo_box, gint index_);
#define gtk_combo_box_new                 w_gtk_combobox_new
#define gtk_combo_box_new_text            w_gtk_combobox_new
#define gtk_combo_box_text_new            w_gtk_combobox_new
#define dtk_combo_box_entry_new_text      w_gtk_combobox_new_with_entry
#define gtk_combo_box_text_new_with_entry w_gtk_combobox_new_with_entry
#define gtk_combo_box_text_append_text(combo,text)     gtk_combo_box_append_text(combo,text)
#define gtk_combo_box_text_insert_text(combo,pos,text) gtk_combo_box_insert_text(combo,pos,text)
#define gtk_combo_box_text_prepend_text(combo,text)    gtk_combo_box_prepend_text(combo,text)
#define gtk_combo_box_text_remove(combo,pos)           gtk_combo_box_remove_text(combo,pos)
#define gtk_combo_box_text_get_active_text(combo)      (gtk_combo_box_get_active_text(combo))
#define gtk_combo_box_set_entry_text_column(combo,cl)
#define gtk_combo_box_get_entry_text_column(combo)     (0)
#endif

//==================================================
//                   GTK < 2.2
//==================================================

#if !GTK_CHECK_VERSION(2, 2, 0)
#define gtk_window_set_skip_pager_hint(window,bool)
#define gtk_window_set_skip_taskbar_hint(window,bool)
#define gtk_window_get_skip_taskbar_hint(window) (FALSE)
#define gtk_window_get_skip_pager_hint(window)   (FALSE)
#define gtk_window_set_auto_startup_notification(bool)
#endif


//==================================================
//             GTK1 (assume GTK 1.2)
//==================================================

// gtk2.0 http://irtfweb.ifa.hawaii.edu/SoftwareDocs/gtk20/gtk/
// gtk2.6 https://refspecs.linuxbase.org/gtk/2.6/
// gtk1.2 http://irtfweb.ifa.hawaii.edu/SoftwareDocs/

// https://www.rpi.edu/dept/acm/packages1/gtk/1.2.10/gtk+-1.2.10/docs/html/

#if GTK_MAJOR_VERSION == 1

// GLIB1
int g_strcmp0 (const char *s1, const char *s2);
char *g_path_get_basename (const char *filename);
char *g_path_get_dirname (const char *filename);

// GTK1
void gtk_table_get_size (GtkTable *table, guint *rows, guint *columns);
gint gtk_option_menu_get_history (GtkOptionMenu *option_menu);

#define GTK_SELECTION_NONE GTK_SELECTION_SINGLE
#define GTK_WIN_POS_CENTER_ON_PARENT GTK_WIN_POS_CENTER
#define GTK_ACCEL_GROUP(a) ((GtkAccelGroup*)(a))

#define gtk_label_new_with_mnemonic gtk_label_new              // TODO: make a function and remove _
#define gtk_label_set_label gtk_label_set_text
#define gtk_label_get_text(w)  ((const char*)GTK_LABEL(w)->label)
#define gtk_label_get_label(w) ((const char*)GTK_LABEL(w)->label)
#define gtk_label_get_use_markup(label) (FALSE)
#define gtk_label_set_use_markup(label,bool)

#define gtk_widget_set_size_request       gtk_widget_set_usize
#define gtk_widget_get_parent(w)          (GTK_WIDGET(w)->parent)
#define gtk_container_get_children        gtk_container_children
#define gtk_container_get_border_width(w) (GTK_CONTAINER(w)->border_width)
#define gtk_container_get_resize_mode(w)  (GTK_CONTAINER(w)->resize_mode)
#define gtk_bin_get_child(w) (GTK_BIN(w)->child)
#define gtk_frame_get_label(w) ((const char*)GTK_FRAME(w)->label)

GtkWidget* gtk_button_new_with_mnemonic (const char *label);
GtkWidget* gtk_check_button_new_with_mnemonic (const char *label);
GtkWidget* gtk_toggle_button_new_with_mnemonic (const char *label);
GtkWidget* gtk_radio_button_new_with_mnemonic (GSList *group, const gchar *label);
GtkWidget* gtk_radio_button_new_with_mnemonic_from_widget (GtkRadioButton *radio_group_member, const gchar *label);
#define gtk_radio_button_get_group(rb) (GTK_RADIO_BUTTON(rb)->group)
#define gtk_button_set_focus_on_click(button,bool)
#define gtk_button_get_focus_on_click(button) (FALSE)
#define gtk_button_set_image_position(button,pos)
#define gtk_button_set_image(button,img)
#define gtk_button_get_label(i) (gtk_label_get_label (GTK_LABEL(GTK_BIN(i)->child)))
#define gtk_button_set_label(i,label) gtk_label_set_label(GTK_LABEL(GTK_BIN(i)->child), (label) ? label : "")

#define gtk_toolbar_get_orientation(tb) (GTK_TOOLBAR(tb)->orientation)
#define gtk_separator_menu_item_new gtk_menu_item_new // TODO
#define gtk_menu_item_new_with_mnemonic       gtk_menu_item_new_with_label        // see w_gtk_menu.h
#define gtk_radio_menu_item_new_with_mnemonic gtk_radio_menu_item_new_with_label  // see w_gtk_menu.h
#define gtk_check_menu_item_new_with_mnemonic gtk_check_menu_item_new_with_label  // see w_gtk_menu.h
#define gtk_radio_menu_item_get_group(w)      (GTK_RADIO_MENU_ITEM(w)->group)
#define gtk_check_menu_item_get_active(w) (GTK_CHECK_MENU_ITEM(w)->active)
#define gtk_menu_item_set_accel_path(item,path)

#define GError void
#define gtk_style_get_font(s) (((GtkStyle*)(s))->font)
#define gtk_calendar_set_display_options gtk_calendar_display_options
#define gtk_adjustment_get_value(adj) (GTK_ADJUSTMENT(adj)->value)

#define gtk_entry_set_width_chars gtk_entry_set_max_length
#define gtk_entry_get_width_chars(w) (GTK_ENTRY(w)->text_max_length)

#define gtk_window_set_resizable(window,bool)
#define gtk_window_set_destroy_with_parent(window,bool)
#define gtk_window_set_type_hint(window,hint)
#define gtk_window_set_role(window,role)
#define GDK_WINDOW_TYPE_HINT_DIALOG 0
#define gtk_scrolled_window_set_shadow_type(sc,type)
#define gtk_color_selection_set_has_opacity_control gtk_color_selection_set_opacity
#define gtk_color_selection_get_has_opacity_control(w) (GTK_COLOR_SELECTION(w)->use_opacity)

#define G_OBJECT GTK_OBJECT
#define g_object_set_data gtk_object_set_data
#define g_object_get_data gtk_object_get_data
#define g_object_set_data_full gtk_object_set_data_full
#define g_object_set gtk_object_set
gpointer g_object_ref (gpointer object);
void g_object_unref (gpointer object);

#define g_object_freeze_notify(o) // TODO
#define g_object_notify(o,str)    // TODO
#define g_object_thaw_notify(o)   // TODO

#define G_CALLBACK GTK_SIGNAL_FUNC
#define GCallback  GtkSignalFunc
// TODO: g_signal_* accept gpointers, this triggers warnings, force convertion to GtkObject
#define g_signal_connect(object,signal_str,cb,cb_data) \
            gtk_signal_connect(GTK_OBJECT(object),(signal_str),(cb),(cb_data))
#define g_signal_connect_after gtk_signal_connect_after
#define g_signal_connect_swapped(object,signal_str,cb,cb_data) \
            gtk_signal_connect_object(GTK_OBJECT(object),(signal_str),(cb),GTK_OBJECT(cb_data))
#define g_signal_handler_disconnect gtk_signal_disconnect
///#define g_signal_emit            gtk_signal_emit_by_name TODO: replace_gtk2.sh
#define g_signal_emit_by_name       gtk_signal_emit_by_name
#define g_signal_handler_is_connected gtk_signal_handler_pending_by_id

#define g_signal_lookup   gtk_signal_lookup
#define g_signal_name     gtk_signal_name
#define g_signal_handler_block gtk_signal_handler_block
#define g_signal_handler_unblock gtk_signal_handler_unblock
//#define G_STRUCT_OFFSET GTK_SIGNAL_OFFSET
#define g_signal_handlers_disconnect_matched gtk_signal_disconnect_by_data

#define GSourceFunc GtkFunction
#define g_timeout_add   gtk_timeout_add
#define g_source_remove gtk_timeout_remove

#define GTK_WIDGET_GET_CLASS(w)    (GTK_WIDGET_CLASS(GTK_OBJECT(w)->klass))
#define GTK_WINDOW_GET_CLASS(w)    (GTK_WINDOW_CLASS(GTK_OBJECT(w)->klass))
#define GTK_DIALOG_GET_CLASS(w)    (GTK_DIALOG_CLASS(GTK_OBJECT(w)->klass))
#define GTK_BUTTON_GET_CLASS(w)    (GTK_BUTTON_CLASS(GTK_OBJECT(w)->klass))
#define GTK_BOX_GET_CLASS(w)       (GTK_BOX_CLASS(GTK_OBJECT(w)->klass))
#define GTK_CONTAINER_GET_CLASS(w) (GTK_CONTAINER_CLASS(GTK_OBJECT(w)->klass))

#define gtk_marshal_BOOLEAN__VOID gtk_marshal_BOOL__NONE
#define gtk_marshal_BOOLEAN__POINTER gtk_marshal_BOOL__POINTER
#define gtk_marshal_BOOLEAN__POINTER_POINTER_INT_INT gtk_marshal_BOOL__POINTER_POINTER_INT_INT
#define gtk_marshal_BOOLEAN__POINTER_INT_INT gtk_marshal_BOOL__POINTER_INT_INT
#define gtk_marshal_BOOLEAN__POINTER_INT_INT_UINT gtk_marshal_BOOL__POINTER_INT_INT_UINT
#define gtk_marshal_BOOLEAN__POINTER_STRING_STRING_POINTER gtk_marshal_BOOL__POINTER_STRING_STRING_POINTER
#define g_cclosure_marshal_VOID__BOOLEAN gtk_marshal_VOID__BOOLEAN
#define gtk_marshal_VOID__BOOLEAN gtk_marshal_NONE__BOOL
#define g_cclosure_marshal_VOID__BOXED gtk_marshal_VOID__BOXED
#define gtk_marshal_VOID__BOXED gtk_marshal_NONE__BOXED
#define g_cclosure_marshal_VOID__ENUM gtk_marshal_VOID__ENUM
#define gtk_marshal_VOID__ENUM gtk_marshal_NONE__ENUM
#define gtk_marshal_VOID__ENUM_FLOAT gtk_marshal_NONE__ENUM_FLOAT
#define gtk_marshal_VOID__ENUM_FLOAT_BOOLEAN gtk_marshal_NONE__ENUM_FLOAT_BOOL
#define g_cclosure_marshal_VOID__INT gtk_marshal_VOID__INT
#define gtk_marshal_VOID__INT gtk_marshal_NONE__INT
#define gtk_marshal_VOID__INT_INT gtk_marshal_NONE__INT_INT
#define gtk_marshal_VOID__INT_INT_POINTER gtk_marshal_NONE__INT_INT_POINTER
#define g_cclosure_marshal_VOID__VOID gtk_marshal_VOID__VOID
#define gtk_marshal_VOID__VOID gtk_marshal_NONE__NONE
#define g_cclosure_marshal_VOID__OBJECT gtk_marshal_VOID__OBJECT
#define gtk_marshal_VOID__OBJECT gtk_marshal_NONE__OBJECT
#define g_cclosure_marshal_VOID__POINTER gtk_marshal_VOID__POINTER
#define gtk_marshal_VOID__POINTER gtk_marshal_NONE__POINTER
#define gtk_marshal_VOID__POINTER_INT gtk_marshal_NONE__POINTER_INT
#define gtk_marshal_VOID__POINTER_POINTER gtk_marshal_NONE__POINTER_POINTER
#define gtk_marshal_VOID__POINTER_POINTER_POINTER gtk_marshal_NONE__POINTER_POINTER_POINTER
#define gtk_marshal_VOID__POINTER_STRING_STRING gtk_marshal_NONE__POINTER_STRING_STRING
#define gtk_marshal_VOID__POINTER_UINT gtk_marshal_NONE__POINTER_UINT
#define gtk_marshal_VOID__POINTER_UINT_ENUM gtk_marshal_NONE__POINTER_UINT_ENUM
#define gtk_marshal_VOID__POINTER_POINTER_UINT_UINT gtk_marshal_NONE__POINTER_POINTER_UINT_UINT
#define gtk_marshal_VOID__POINTER_INT_INT_POINTER_UINT_UINT gtk_marshal_NONE__POINTER_INT_INT_POINTER_UINT_UINT
#define gtk_marshal_VOID__POINTER_UINT_UINT gtk_marshal_NONE__POINTER_UINT_UINT
#define g_cclosure_marshal_VOID__STRING gtk_marshal_VOID__STRING
#define gtk_marshal_VOID__STRING gtk_marshal_NONE__STRING
#define gtk_marshal_VOID__STRING_INT_POINTER gtk_marshal_NONE__STRING_INT_POINTER
#define g_cclosure_marshal_VOID__UINT gtk_marshal_VOID__UINT
#define gtk_marshal_VOID__UINT gtk_marshal_NONE__UINT
#define gtk_marshal_VOID__UINT_POINTER_UINT_ENUM_ENUM_POINTER gtk_marshal_NONE__UINT_POINTER_UINT_ENUM_ENUM_POINTER
#define gtk_marshal_VOID__UINT_POINTER_UINT_UINT_ENUM gtk_marshal_NONE__UINT_POINTER_UINT_UINT_ENUM
#define gtk_marshal_VOID__UINT_STRING gtk_marshal_NONE__UINT_STRING

typedef enum
{
  GTK_ICON_SIZE_INVALID,
  GTK_ICON_SIZE_MENU,
  GTK_ICON_SIZE_SMALL_TOOLBAR,
  GTK_ICON_SIZE_LARGE_TOOLBAR,
  GTK_ICON_SIZE_BUTTON,
  GTK_ICON_SIZE_DND,
  GTK_ICON_SIZE_DIALOG
} GtkIconSize;


#define GTK_STOCK_ABOUT            "gtk-about" // 2.6 About
#define GTK_STOCK_ADD              "gtk-add" //Add
#define GTK_STOCK_APPLY            "gtk-apply" //Apply
#define GTK_STOCK_BOLD             "gtk-bold" //Bold
#define GTK_STOCK_CANCEL           "gtk-cancel" //Cancel
#define GTK_STOCK_CAPS_LOCK_WARNING "gtk-caps-lock-warning" // 2.16 Caps Lock Warning
#define GTK_STOCK_CDROM            "gtk-cdrom" //CD-Rom
#define GTK_STOCK_CLEAR            "gtk-clear" //Clear
#define GTK_STOCK_CLOSE            "gtk-close" //Close
#define GTK_STOCK_COLOR_PICKER     "gtk-color-picker" //2.2 Color Picker
#define GTK_STOCK_CONNECT          "gtk-connect" //2.6 Connect
#define GTK_STOCK_CONVERT          "gtk-convert" //Convert
#define GTK_STOCK_COPY             "gtk-copy" //Copy
#define GTK_STOCK_CUT              "gtk-cut" //Cut
#define GTK_STOCK_DELETE           "gtk-delete" //Delete
#define GTK_STOCK_DIALOG_AUTHENTICATION "gtk-dialog-authentication" //2.4 Authentification
#define GTK_STOCK_DIALOG_INFO      "gtk-dialog-info" //Information
#define GTK_STOCK_DIALOG_WARNING   "gtk-dialog-warning" //Warning
#define GTK_STOCK_DIALOG_ERROR     "gtk-dialog-error" //Error
#define GTK_STOCK_DIALOG_QUESTION  "gtk-dialog-question" //Question
#define GTK_STOCK_DIRECTORY        "gtk-directory" //2.6 Directory
#define GTK_STOCK_DISCARD          "gtk-discard" //2.12 Discard
#define GTK_STOCK_DISCONNECT       "gtk-disconnect" //2.6 Disconnect
#define GTK_STOCK_DND              "gtk-dnd" //Drag-And-Drop
#define GTK_STOCK_DND_MULTIPLE     "gtk-dnd-multiple" //Drag-And-Drop multiple
#define GTK_STOCK_EDIT             "gtk-edit" //2.6 Edit
#define GTK_STOCK_EXECUTE          "gtk-execute" //Execute
#define GTK_STOCK_FILE             "gtk-file" //File
#define GTK_STOCK_FIND             "gtk-find" //Find
#define GTK_STOCK_FIND_AND_REPLACE "gtk-find-and-replace" //Find and Replace
#define GTK_STOCK_FLOPPY           "gtk-floppy" //Floppy
#define GTK_STOCK_FULLSCREEN       "gtk-fullscreen" //2.8 Fullscreen
#define GTK_STOCK_GOTO_BOTTOM      "gtk-goto-bottom" // Bottom
#define GTK_STOCK_GOTO_FIRST       "gtk-goto-first" //First
#define GTK_STOCK_GOTO_LAST        "gtk-goto-last" //Last
#define GTK_STOCK_GOTO_TOP         "gtk-goto-top" //Top
#define GTK_STOCK_GO_BACK          "gtk-go-back" //Back
#define GTK_STOCK_GO_DOWN          "gtk-go-down" //Down
#define GTK_STOCK_GO_FORWARD       "gtk-go-forward" //Forward
#define GTK_STOCK_GO_UP            "gtk-go-up" //Up
#define GTK_STOCK_HARDDISK         "gtk-harddisk" //2.4 Harddisk
#define GTK_STOCK_HELP             "gtk-help" //Help
#define GTK_STOCK_HOME             "gtk-home" //Home
#define GTK_STOCK_INDEX            "gtk-index" //Index
#define GTK_STOCK_INDENT           "gtk-indent" //2.4 Indent
#define GTK_STOCK_INFO             "gtk-info" //2.8 Info
#define GTK_STOCK_ITALIC           "gtk-italic" //Italic
#define GTK_STOCK_JUMP_TO          "gtk-jump-to" //Jump to
#define GTK_STOCK_JUSTIFY_CENTER   "gtk-justify-center" //Center
#define GTK_STOCK_JUSTIFY_FILL     "gtk-justify-fill" //Full
#define GTK_STOCK_JUSTIFY_LEFT     "gtk-justify-left" //Left
#define GTK_STOCK_JUSTIFY_RIGHT    "gtk-justify-right" //Right
#define GTK_STOCK_LEAVE_FULLSCREEN "gtk-leave-fullscreen" //2.8 Leave Fullscreen
#define GTK_STOCK_MISSING_IMAGE    "gtk-missing-image" // Missing Image
#define GTK_STOCK_MEDIA_FORWARD    "gtk-media-forward" // 2.6 Media Foward
#define GTK_STOCK_MEDIA_NEXT       "gtk-media-next" // 2.6 Media Next
#define GTK_STOCK_MEDIA_PAUSE      "gtk-media-pause" // 2.6 Media Pause
#define GTK_STOCK_MEDIA_PLAY       "gtk-media-play" // 2.6 Media Play
#define GTK_STOCK_MEDIA_PREVIOUS   "gtk-media-previous" //2.6 Media Previous
#define GTK_STOCK_MEDIA_RECORD     "gtk-media-record" //2.6 Media Record
#define GTK_STOCK_MEDIA_REWIND     "gtk-media-rewind" //2.6 Media Rewind
#define GTK_STOCK_MEDIA_STOP       "gtk-media-stop" //2.6 Media Stop
#define GTK_STOCK_NETWORK          "gtk-network" //Network
#define GTK_STOCK_NEW              "gtk-new" //New
#define GTK_STOCK_NO               "gtk-no" //No
#define GTK_STOCK_OK               "gtk-ok" //OK
#define GTK_STOCK_OPEN             "gtk-open" //Open
#define GTK_STOCK_ORIENTATION_PORTRAIT "gtk-orientation-portrait" //2.10 Portrait Orientation
#define GTK_STOCK_ORIENTATION_LANDSCAPE "gtk-orientation-landscape" //2.10 Landscape Orientation
#define GTK_STOCK_ORIENTATION_REVERSE_LANDSCAPE "gtk-orientation-reverse-landscape" //2.10 Reverse Landscape Orientation
#define GTK_STOCK_ORIENTATION_REVERSE_PORTRAIT "gtk-orientation-reverse-portrait" // 2.10 Reverse Portrait Orientation
#define GTK_STOCK_PAGE_SETUP       "gtk-page-setup" //2.14 Page Setup
#define GTK_STOCK_PASTE            "gtk-paste" //Paste
#define GTK_STOCK_PREFERENCES      "gtk-preferences" //Preferences
#define GTK_STOCK_PRINT            "gtk-print" // Print
#define GTK_STOCK_PRINT_ERROR      "gtk-print-error" //2.14 Print Error
#define GTK_STOCK_PRINT_PAUSED     "gtk-print-paused" // 2.14 Print Paused
#define GTK_STOCK_PRINT_PREVIEW    "gtk-print-preview" // Print Preview
#define GTK_STOCK_PRINT_REPORT     "gtk-print-report"  //2.14 Print Report
#define GTK_STOCK_PRINT_WARNING    "gtk-print-warning" //2.14 Print Warning
#define GTK_STOCK_PROPERTIES       "gtk-properties" //Properties
#define GTK_STOCK_QUIT             "gtk-quit" // Quit
#define GTK_STOCK_REDO             "gtk-redo" //Redo
#define GTK_STOCK_REFRESH          "gtk-refresh" //Refresh
#define GTK_STOCK_REMOVE           "gtk-remove" // Remove
#define GTK_STOCK_REVERT_TO_SAVED  "gtk-revert-to-saved" // Revert
#define GTK_STOCK_SAVE             "gtk-save" // Save
#define GTK_STOCK_SAVE_AS          "gtk-save-as" // Save As
#define GTK_STOCK_SELECT_ALL       "gtk-select-all" // 2.10 Select All
#define GTK_STOCK_SELECT_COLOR     "gtk-select-color" //Color
#define GTK_STOCK_SELECT_FONT      "gtk-select-font" // Font
#define GTK_STOCK_SORT_ASCENDING   "gtk-sort-ascending" // Ascending
#define GTK_STOCK_SORT_DESCENDING  "gtk-sort-descending" // Descending
#define GTK_STOCK_SPELL_CHECK      "gtk-spell-check" //Spell Check
#define GTK_STOCK_STOP             "gtk-stop" // Stop
#define GTK_STOCK_STRIKETHROUGH    "gtk-strikethrough" //Strikethrough
#define GTK_STOCK_UNDELETE         "gtk-undelete" // Undelete
#define GTK_STOCK_UNDERLINE        "gtk-underline" // Underline
#define GTK_STOCK_UNDO             "gtk-undo"  // Undo
#define GTK_STOCK_UNINDENT         "gtk-unindent" // 2.4 Unindent
#define GTK_STOCK_YES              "gtk-yes"  // Yes
#define GTK_STOCK_ZOOM_100         "gtk-zoom-100" //Zoom 100%
#define GTK_STOCK_ZOOM_FIT         "gtk-zoom-fit" //Zoom to Fit
#define GTK_STOCK_ZOOM_IN          "gtk-zoom-in"  //Zoom In
#define GTK_STOCK_ZOOM_OUT         "gtk-zoom-out" //Zoom Out


// gtkdialog.h
typedef enum
{
  GTK_RESPONSE_NONE = -1,
  GTK_RESPONSE_REJECT = -2,
  GTK_RESPONSE_ACCEPT = -3,
  GTK_RESPONSE_DELETE_EVENT = -4,
  GTK_RESPONSE_OK     = -5,
  GTK_RESPONSE_CANCEL = -6,
  GTK_RESPONSE_CLOSE  = -7,
  GTK_RESPONSE_YES    = -8,
  GTK_RESPONSE_NO     = -9,
  GTK_RESPONSE_APPLY  = -10,
  GTK_RESPONSE_HELP   = -11
} GtkResponseType;

typedef enum
{
  GTK_DIALOG_MODAL               = 1 << 0, /* call gtk_window_set_modal (win, TRUE) */
  GTK_DIALOG_DESTROY_WITH_PARENT = 1 << 1, /* call gtk_window_set_destroy_with_parent () */
  GTK_DIALOG_NO_SEPARATOR        = 1 << 2  /* no separator bar above buttons */
} GtkDialogFlags;


#endif

#endif // GTK2 <= 2

#ifdef __cplusplus
}
#endif

#endif /* __W_GTK_COMPAT2_H__ */
