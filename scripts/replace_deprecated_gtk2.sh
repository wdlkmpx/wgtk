#!/bin/sh
# Public Domain


#set -x

set_target() {
	target=$(
	find $dir -type f -name '*.c' \
	               -o -name '*.cc' \
	               -o -name '*.cpp' \
	               -o -name '*.h' \
	               -o -name '*.hh' | grep -v gtkcompat\. | grep -v glib-compat\.
	)
}

# Functions that are replaced by the script but require manual fixing:
#
# * g_strdown (str)
#   --> g_utf8_strdown (str, -1)
#          (add argument, -1)
#
# * g_source_get_current_time (source, timeval)
#   --> g_get_current_time (timeval)
#          (remove 1st argument)
#
# * gtk_idle_add_priority (priority, function, data)
#   --> g_idle_add_full (priority, function, data, NULL)
#          (add extra param with NULL)
#
# * gtk_window_set_wmclass (window, name, class)
#   --> gtk_window_set_role (window, name)
#
# * gtk_signal_disconnect_by_data (object, data)
#   --> g_signal_handlers_disconnect_matched (object, G_SIGNAL_MATCH_DATA, 0, 0, NULL, NULL, data);
#
# ===================================================

# How to fix stuff:

# - GtkCombo:
#   * combo->entry
#        ### replace GtkCombo with GtkComboBoxText
#        #before: gtk_entry_get_text (GTK_ENTRY (GTK_COMBO (combo)->entry);
#        #after : gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT (combo));
#
# - GtkDialog:
#   * dialog->vbox
#   * dialog->main_vbox
#        ### gtk_dialog_get_content_area() gtk >= 2.14
#        main_vbox = gtk_dialog_get_content_area (dialog);
#
# - GtkLabel
#   * gtk_label_parse_uline
#        ### see https://github.com/wdlkmpx/gwaveedit/commit/ad5603b4cbd7aa996896fbc9ef2fb6c65d67da01
#
# - GtkMisc
#    * gtk_misc_set_alignment (misc, halign, valign);
#         # gtk2 default:             0.5,    0.5 (depends??)
#         # gtk3 default:             0.0,    ???
#         ### x halign :    0.0 left    /    0.5 center    /  1.0 right
#         ### y valign :    0.0 top     /    0.5 center    /  1.0 bottom
#         ### from 3.0: GTK_ALIGN_START / GTK_ALIGN_CENTER / GTK_ALIGN_END
#         gtk_widget_set_halign (widget, GTK_ALIGN_*)
#         gtk_widget_set_valign (widget, GTK_ALIGN_*)
#
# - GtkToggleButton:
#   * button->active
#        active = gtk_toggle_button_get_active (button);
#        gtk_toggle_button_set_active (button, GBOOLEAN);
#
# - GtkWidget:
#   * widget->allocation.x
#   * widget->allocation.y
#   * widget->allocation.width
#   * widget->allocation.height
#        ### gtk_widget_get_allocation() gtk >= 2.18
#        GtkAllocation rect;
#        gtk_widget_get_allocation (widget, &rect);
#        rect.x
#        rect.y
#        rect.width
#        rect.height
#   * widget->requisition.width
#   * widget->requisition.height
#        GtkRequisition requisition;
#        gtk_widget_size_request (widget, &requisition);
#        ### gtk_widget_get_requisition () gtk >= 2.20
#        GtkRequisition requisition;
#        gtk_widget_get_requisition (widget, &requisition);
#   * widget->window
#        ### gtk_widget_get_window gtk >= 2.14
#        GdkWindow * gdk_window = gtk_widget_get_window (widget);
#   * GTK_WIDGET_FLAGS()
#        ### Use the proper function to test individual states:
#        gtk_widget_get_app_paintable ()
#        gtk_widget_get_can_default ()
#        gtk_widget_get_can_focus ()
#        gtk_widget_get_double_buffered ()
#        gtk_widget_has_default ()
#        gtk_widget_is_drawable ()
#        gtk_widget_has_focus ()
#        gtk_widget_has_grab ()
#        gtk_widget_get_mapped ()
#        gtk_widget_get_has_window ()
#        gtk_widget_has_rc_style ()
#        gtk_widget_get_realized ()
#        gtk_widget_get_receives_default ()
#        gtk_widget_get_sensitive ()
#        gtk_widget_is_sensitive ()
#        gtk_widget_is_toplevel ()
#        gtk_widget_get_visible ()
#   * GTK_WIDGET_SET_FLAGS()
#        ### Use the proper function instead: 
#        gtk_widget_set_app_paintable (w, TRUE);
#        gtk_widget_set_can_default (w, TRUE);
#        gtk_widget_set_can_focus (w, TRUE);
#        gtk_widget_set_double_buffered (w, TRUE);
#        gtk_widget_set_has_window (w, TRUE);
#        gtk_widget_set_mapped (w, TRUE);
#        gtk_widget_set_no_show_all (w, TRUE);
#        gtk_widget_set_realized (w, TRUE);
#        gtk_widget_set_receives_default (w, TRUE);
#        gtk_widget_set_sensitive (w, TRUE);
#        gtk_widget_set_visible (w, TRUE);
#   * GTK_WIDGET_UNSET_FLAGS()
#        ### Use the proper function instead.
#        gtk_widget_set_app_paintable (w, FALSE);
#        gtk_widget_set_can_default (w, FALSE);
#        gtk_widget_set_can_focus (w, FALSE);
#        gtk_widget_set_double_buffered (w, FALSE);
#        gtk_widget_set_has_window (w, FALSE);
#        gtk_widget_set_mapped (w, FALSE);
#        gtk_widget_set_no_show_all (w, FALSE);
#        gtk_widget_set_realized (w, FALSE);
#        gtk_widget_set_receives_default (w, FALSE);
#        gtk_widget_set_sensitive (w, FALSE);
#        gtk_widget_set_visible (w, FALSE);
#   * GTK_WIDGET_PARENT_SENSITIVE
#        ### use gtk_widget_get_sensitive() on the parent widget instead
#   * GTK_WIDGET_COMPOSITE_CHILD()
#        ### Use the “composite-child” property instead.


#===========================================================================
# >>> Glib 2.32+   https://developer.gnome.org/glib/2.32/api-index-deprecated.html
glib ()
{

sed -i \
-e 's%g_main_destroy%g_main_loop_unref%' \
-e 's%g_main_is_running%g_main_loop_is_running%' \
-e 's%g_main_iteration%g_main_context_iteration%' \
-e 's%g_main_new%g_main_loop_new%' \
-e 's%g_main_quit%g_main_loop_quit%' \
-e 's%g_main_run%g_main_loop_run%' \
-e 's%g_main_set_poll_func%g_main_context_set_poll_func%' \
-e 's%g_mapped_file_free%g_mapped_file_unref%g' \
-e '/g_hash_table_freeze/d' \
-e '/g_hash_table_thaw/d' \
-e 's%g_dirname%g_path_get_dirname%' \
-e 's%g_basename%/*fix me - possibly need to free returned string*/ g_path_get_basename%' \
-e 's%g_format_size_for_display%g_format_size%' \
-e 's%g_strcasecmp%strcasecmp%' \
-e 's%g_strncasecmp%strncasecmp%' \
-e 's%g_strdown%g_utf8_strdown%' \
-e 's%g_source_get_current_time%g_get_current_time%g' \
-e 's%g_date_set_time(%g_date_set_time_t (%' \
-e 's%g_date_set_time (%g_date_set_time_t (%' \
-e 's%g_io_channel_seek(%g_io_channel_seek_position (%g' \
-e 's%g_io_channel_seek (%g_io_channel_seek_position (%g' \
-e 's%g_io_channel_close(%g_io_channel_shutdown (%g' \
-e 's%g_io_channel_close (%g_io_channel_shutdown (%g' \
-e 's%g_io_channel_write(%g_io_channel_write_chars (%g' \
-e 's%g_io_channel_write (%g_io_channel_write_chars (%g' \
-e 's%g_io_channel_read(%g_io_channel_read_chars (%g' \
-e 's%g_io_channel_read (%g_io_channel_read_chars (%g' \
${target}

#sed -i \
#-e '/g_thread_init/d' \
#-e '/g_thread_supported/d' \
#-e '/g_thread_get_initialized/d' \
#-e '/g_thread_set_priority/d' \
#${target}

sed -i \
-e 's%GStaticRecMutex%GRecMutex%g' \
-e 's%g_static_rec_mutex_init%g_rec_mutex_init%g' \
-e 's%g_static_rec_mutex_lock%g_rec_mutex_lock%g' \
-e 's%g_static_rec_mutex_unlock%g_rec_mutex_unlock%g' \
${target}

return
# (GStaticMutex *) can be initiliazed with (G_STATIC_MUTEX_INIT)
# .. must replace it with (GMutex) and (g_mutex_init)
# - a statically created GMutex require changes in scope, pass by reference, etc.
sed -i \
-e 's%GStaticMutex%WGMutex%g' \
-e 's%g_static_mutex_lock%Wg_mutex_lock%g' \
-e 's%g_static_mutex_trylock%Wg_mutex_trylock%g' \
-e 's%g_static_mutex_unlock%Wg_mutex_unlock%g' \
-e '/g_static_mutex_get_mutex/d' \
-e '/g_static_mutex_free/d' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GSignal - replaces GtkSignal
g_signal ()
{

# -- gtk_signal_emit_stop > g_signal_stop_emission
#      add extra param: , 0)
sed -i \
-e 's%GTK_SIGNAL_FUNC%G_CALLBACK%g' \
-e 's%(GtkSignalFunc)%(GCallback)%g' \
-e 's%GtkSignalFunc%GCallback%g' \
-e 's%gtk_signal_connect %g_signal_connect %g' \
-e 's%gtk_signal_connect(%g_signal_connect (%g' \
-e 's%gtk_signal_connect_after%g_signal_connect_after%g' \
-e 's%gtk_signal_connect_object%g_signal_connect_swapped%g' \
-e 's%gtk_signal_disconnect%g_signal_handler_disconnect%g' \
-e 's%gtk_signal_emit_by_name%g_signal_emit%g' \
-e 's%gtk_signal_emit_stop %-- g_signal_stop_emission%g' \
-e 's%gtk_signal_emit_stop(%-- g_signal_stop_emission (%g' \
-e 's%gtk_signal_emit_stop_by_name%g_signal_stop_emission_by_name%g' \
-e 's%gtk_signal_handler_pending_by_id%g_signal_handler_is_connected%g' \
-e 's%gtk_signal_handler_pending %g_signal_handler_is_connected%g' \
-e 's%gtk_signal_handler_pending(%g_signal_handler_is_connected%g' \
-e '/gtk_signal_handlers_destroy/d' \
-e '/gtk_object_class_add_signals/d' \
${target}

sed -i \
-e 's%gtk_signal_default_marshaller%g_cclosure_marshal_VOID__VOID%g' \
-e 's%gtk_signal_lookup%g_signal_lookup%g' \
-e 's%gtk_signal_name%g_signal_name%g' \
-e 's%gtk_signal_handler_block%g_signal_handler_block%g' \
-e 's%gtk_signal_handler_unblock%g_signal_handler_unblock%g' \
-e 's%GTK_SIGNAL_OFFSET%G_STRUCT_OFFSET%g' \
-e 's%gtk_signal_disconnect_by_data%g_signal_handlers_disconnect_matched%g' \
${target}

# now fix some specific GTK_OBJECT casts
sed -i \
-e 's%gtk_window_set_opacity (GTK_WINDOW%gtk_widget_set_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_set_opacity ( GTK_WINDOW%gtk_widget_set_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_set_opacity(GTK_WINDOW%gtk_widget_set_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_set_opacity%gtk_widget_set_opacity%g' \
-e 's%g_signal_connect(GTK_OBJECT%g_signal_connect (G_OBJECT%g' \
-e 's%g_signal_connect ( GTK_OBJECT%g_signal_connect (G_OBJECT%g' \
-e 's%g_signal_connect  ( GTK_OBJECT%g_signal_connect (G_OBJECT%g' \
-e 's%g_signal_connect_after (GTK_OBJECT%g_signal_connect_after (G_OBJECT%' \
-e 's%g_signal_connect_after(GTK_OBJECT%g_signal_connect_after (G_OBJECT%' \
-e 's%g_signal_connect_after ( GTK_OBJECT%g_signal_connect_after (G_OBJECT%' \
-e 's%g_signal_connect_swapped (GTK_OBJECT%g_signal_connect_swapped (G_OBJECT%' \
-e 's%g_signal_connect_swapped(GTK_OBJECT%g_signal_connect_swapped (G_OBJECT%' \
-e 's%g_signal_connect_swapped ( GTK_OBJECT%g_signal_connect_swapped (G_OBJECT%' \
-e 's%g_signal_stop_emission_by_name(GTK_OBJECT%g_signal_stop_emission_by_name (G_OBJECT%' \
-e 's%g_signal_stop_emission_by_name (GTK_OBJECT%g_signal_stop_emission_by_name (G_OBJECT%' \
-e 's%g_signal_stop_emission_by_name ( GTK_OBJECT%g_signal_stop_emission_by_name (G_OBJECT%' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GObject - replace some GtkObject functions
g_object ()
{

sed -i \
-e 's%gtk_object_set_data_full(GTK_OBJECT%g_object_set_data_full (G_OBJECT%g' \
-e 's%gtk_object_set_data_full (GTK_OBJECT%g_object_set_data_full (G_OBJECT%g' \
-e 's%gtk_object_set_data_full ( GTK_OBJECT%g_object_set_data_full (G_OBJECT%g' \
-e 's%gtk_object_set_data_full%g_object_set_data_full%g' \
-e 's%gtk_object_set_data(GTK_OBJECT%g_object_set_data (G_OBJECT%g' \
-e 's%gtk_object_set_data (GTK_OBJECT%g_object_set_data (G_OBJECT%g' \
-e 's%gtk_object_set_data ( GTK_OBJECT%g_object_set_data (G_OBJECT%g' \
-e 's%gtk_object_set_data%g_object_set_data%g' \
-e 's%gtk_object_get_data(GTK_OBJECT%g_object_get_data (G_OBJECT%g' \
-e 's%gtk_object_get_data (GTK_OBJECT%g_object_get_data (G_OBJECT%g' \
-e 's%gtk_object_get_data ( GTK_OBJECT%g_object_get_data (G_OBJECT%g' \
-e 's%gtk_object_get_data%g_object_get_data%g' \
-e 's%GtkDestroyNotify%GDestroyNotify%g' \
-e 's%gtk_object_set_user_data(GTK_OBJECT%-(add 2nd param: "userdata")-  g_object_set_data (G_OBJECT%g' \
-e 's%gtk_object_set_user_data (GTK_OBJECT%-(add 2nd param: "userdata")-  g_object_set_data (G_OBJECT%g' \
-e 's%gtk_object_set_user_data ( GTK_OBJECT%-(add 2nd param: "userdata")-  g_object_set_data (G_OBJECT%g' \
-e 's%gtk_object_set_user_data%-(add 2nd param: "userdata")-  g_object_set_data%g' \
-e 's%gtk_object_get_user_data(GTK_OBJECT%-(add 2nd param: "userdata")-  g_object_get_data (G_OBJECT%g' \
-e 's%gtk_object_get_user_data (GTK_OBJECT%-(add 2nd param: "userdata")-  g_object_get_data (G_OBJECT%g' \
-e 's%gtk_object_get_user_data ( GTK_OBJECT%-(add 2nd param: "userdata")-  g_object_get_data (G_OBJECT%g' \
-e 's%gtk_object_get_user_data%-(add 2nd param: "userdata")-  g_object_get_data%g' \
${target}
}


#===========================================================================
# >>> GtkButtonBox
gtk_button_box ()
{

sed -i \
-e 's%gtk_vbutton_box_get_spacing_default%gtk_box_get_spacing%g' \
-e 's%gtk_hbutton_box_get_spacing_default%gtk_box_get_spacing%g' \
-e 's%gtk_vbutton_box_set_spacing_default%gtk_box_set_spacing%g' \
-e 's%gtk_hbutton_box_set_spacing_default%gtk_box_set_spacing%g' \
-e 's%gtk_vbutton_box_get_layout_default%gtk_button_box_get_layout%g' \
-e 's%gtk_hbutton_box_get_layout_default%gtk_button_box_get_layout%g' \
-e 's%gtk_vbutton_box_set_layout_default%gtk_button_box_set_layout%g' \
-e 's%gtk_hbutton_box_set_layout_default%gtk_button_box_set_layout%g' \
-e 's%GtkHButtonBox%GtkButtonBox%g' \
-e 's%GtkVButtonBox%GtkButtonBox%g' \
-e 's%GTK_HBUTTON_BOX%GTK_BUTTON_BOX%g' \
-e 's%GTK_VBUTTON_BOX%GTK_BUTTON_BOX%g' \
-e 's%GTK_TYPE_HBUTTON_BOX%GTK_TYPE_BUTTON_BOX%g' \
-e 's%GTK_TYPE_VBUTTON_BOX%GTK_TYPE_BUTTON_BOX%g' \
-e 's%gtk_button_box_get_spacing(GTK_BUTTON_BOX%gtk_box_get_spacing (GTK_BOX%g' \
-e 's%gtk_button_box_get_spacing (GTK_BUTTON_BOX%gtk_box_get_spacing (GTK_BOX%g' \
-e 's%gtk_button_box_get_spacing ( GTK_BUTTON_BOX%gtk_box_get_spacing (GTK_BOX%g' \
-e 's%gtk_button_box_get_spacing%gtk_box_get_spacing%g' \
-e 's%gtk_button_box_set_spacing(GTK_BUTTON_BOX%gtk_box_set_spacing (GTK_BOX%g' \
-e 's%gtk_button_box_set_spacing (GTK_BUTTON_BOX%gtk_box_set_spacing (GTK_BOX%g' \
-e 's%gtk_button_box_set_spacing ( GTK_BUTTON_BOX%gtk_box_set_spacing (GTK_BOX%g' \
-e 's%gtk_button_box_set_spacing%gtk_box_set_spacing%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkCombo

# this helps porting GtkCombo to GtkComboboxText
# gtk_combo_box_text_new_with_entry
gtk_combo ()
{
sed -i \
-e '/gtk_combo_set_use_arrows/d' \
-e '/gtk_combo_set_use_arrows_always/d' \
-e 's%gtk_combo_new%gtk_combo_box_text_new_with_entry%g' \
-e 's%GtkCombo %GtkComboBoxText %g' \
-e 's%GTK_COMBO %GTK_COMBO_BOX_TEXT %g' \
-e 's%GTK_COMBO(%GTK_COMBO_BOX_TEXT(%g' \
-e 's%%%g' \
${target}
}

#===========================================================================


#===========================================================================
# >>> GtkComboBox
gtk_combo_box ()
{

sed -i \
-e 's%gtk_combo_box_new_text%gtk_combo_box_text_new%g' \
-e 's%gtk_combo_box_entry_new_text%gtk_combo_box_text_new_with_entry%g' \
-e 's%gtk_combo_box_append_text(GTK_COMBO_BOX% gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_append_text (GTK_COMBO_BOX% gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_append_text ( GTK_COMBO_BOX% gtk_combo_box_text_append_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_append_text%gtk_combo_box_text_append_text%g' \
-e 's%gtk_combo_box_insert_text(GTK_COMBO_BOX%gtk_combo_box_text_insert_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_insert_text (GTK_COMBO_BOX%gtk_combo_box_text_insert_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_insert_text ( GTK_COMBO_BOX%gtk_combo_box_text_insert_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_insert_text%gtk_combo_box_text_insert_text%g' \
-e 's%gtk_combo_box_prepend_text(GTK_COMBO_BOX%gtk_combo_box_text_prepend_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_prepend_text (GTK_COMBO_BOX%gtk_combo_box_text_prepend_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_prepend_text ( GTK_COMBO_BOX%gtk_combo_box_text_prepend_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_prepend_text%gtk_combo_box_text_prepend_text%g' \
-e 's%gtk_combo_box_remove_text(GTK_COMBO_BOX%gtk_combo_box_text_remove (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_remove_text (GTK_COMBO_BOX%gtk_combo_box_text_remove (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_remove_text ( GTK_COMBO_BOX%gtk_combo_box_text_remove (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_remove_text%gtk_combo_box_text_remove%g' \
-e 's%gtk_combo_box_get_active_text(GTK_COMBO_BOX%gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_get_active_text (GTK_COMBO_BOX%gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT%g' \
-e 's%gtk_combo_box_get_active_text ( GTK_COMBO_BOX%gtk_combo_box_text_get_active_text (GTK_COMBO_BOX_TEXT%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkEntry
gtk_entry ()
{

# -- GTK_ENTRY() > GTK_EDITABLE()
sed -i \
-e 's%gtk_entry_select_region (GTK_ENTRY%gtk_editable_select_region (GTK_EDITABLE%g' \
-e 's%gtk_entry_select_region(GTK_ENTRY%gtk_editable_select_region (GTK_EDITABLE%g' \
-e 's%gtk_entry_select_region ( GTK_ENTRY%gtk_editable_select_region (GTK_EDITABLE%g' \
-e 's%gtk_entry_select_region(%gtk_editable_select_region (GTK_EDITABLE%g' \
-e 's%gtk_entry_select_region (%gtk_editable_select_region (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_editable (GTK_ENTRY%gtk_editable_set_editable (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_editable(GTK_ENTRY%gtk_editable_set_editable (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_editable ( GTK_ENTRY%gtk_editable_set_editable (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_editable(%gtk_editable_set_editable (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_editable (%gtk_editable_set_editable (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_position (GTK_ENTRY%gtk_editable_set_position (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_position(GTK_ENTRY%gtk_editable_set_position (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_position ( GTK_ENTRY%gtk_editable_set_position (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_position(%gtk_editable_set_position (GTK_EDITABLE%g' \
-e 's%gtk_entry_set_position (%gtk_editable_set_position (GTK_EDITABLE%g' \
-e "s%gtk_entry_new_with_max_length%gtk_entry_new ();\n    gtk_entry_set_max_length (GTK_ENTRY(xxx), ); max_length-->%" \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkMarshall
gtk_marshall ()
{

sed -i \
-e 's%gtk_marshal_BOOL__NONE%gtk_marshal_BOOLEAN__VOID%g' \
-e 's%gtk_marshal_BOOL__POINTER%gtk_marshal_BOOLEAN__POINTER%g' \
-e 's%gtk_marshal_BOOL__POINTER_POINTER_INT_INT%gtk_marshal_BOOLEAN__POINTER_POINTER_INT_INT%g' \
-e 's%gtk_marshal_BOOL__POINTER_INT_INT%gtk_marshal_BOOLEAN__POINTER_INT_INT%g' \
-e 's%gtk_marshal_BOOL__POINTER_INT_INT_UINT%gtk_marshal_BOOLEAN__POINTER_INT_INT_UINT%g' \
-e 's%gtk_marshal_BOOL__POINTER_STRING_STRING_POINTER%gtk_marshal_BOOLEAN__POINTER_STRING_STRING_POINTER%g' \
-e 's%gtk_marshal_VOID__BOOLEAN%g_cclosure_marshal_VOID__BOOLEAN%g' \
-e 's%gtk_marshal_NONE__BOOL%gtk_marshal_VOID__BOOLEAN%g' \
-e 's%gtk_marshal_VOID__BOXED%g_cclosure_marshal_VOID__BOXED%g' \
-e 's%gtk_marshal_NONE__BOXED%gtk_marshal_VOID__BOXED%g' \
-e 's%gtk_marshal_VOID__ENUM%g_cclosure_marshal_VOID__ENUM%g' \
-e 's%gtk_marshal_NONE__ENUM%gtk_marshal_VOID__ENUM%g' \
-e 's%gtk_marshal_NONE__ENUM_FLOAT%gtk_marshal_VOID__ENUM_FLOAT%g' \
-e 's%gtk_marshal_NONE__ENUM_FLOAT_BOOL%gtk_marshal_VOID__ENUM_FLOAT_BOOLEAN%g' \
-e 's%gtk_marshal_VOID__INT%g_cclosure_marshal_VOID__INT%g' \
-e 's%gtk_marshal_NONE__INT%gtk_marshal_VOID__INT%g' \
-e 's%gtk_marshal_NONE__INT_INT%gtk_marshal_VOID__INT_INT%g' \
-e 's%gtk_marshal_NONE__INT_INT_POINTER%gtk_marshal_VOID__INT_INT_POINTER%g' \
-e 's%gtk_marshal_VOID__VOID%g_cclosure_marshal_VOID__VOID%g' \
-e 's%gtk_marshal_NONE__NONE%gtk_marshal_VOID__VOID%g' \
-e 's%gtk_marshal_VOID__OBJECT%g_cclosure_marshal_VOID__OBJECT%g' \
-e 's%gtk_marshal_NONE__OBJECT%gtk_marshal_VOID__OBJECT%g' \
-e 's%gtk_marshal_VOID__POINTER%g_cclosure_marshal_VOID__POINTER%g' \
-e 's%gtk_marshal_NONE__POINTER%gtk_marshal_VOID__POINTER%g' \
-e 's%gtk_marshal_NONE__POINTER_INT%gtk_marshal_VOID__POINTER_INT%g' \
-e 's%gtk_marshal_NONE__POINTER_POINTER%gtk_marshal_VOID__POINTER_POINTER%g' \
-e 's%gtk_marshal_NONE__POINTER_POINTER_POINTER%gtk_marshal_VOID__POINTER_POINTER_POINTER%g' \
-e 's%gtk_marshal_NONE__POINTER_STRING_STRING%gtk_marshal_VOID__POINTER_STRING_STRING%g' \
-e 's%gtk_marshal_NONE__POINTER_UINT%gtk_marshal_VOID__POINTER_UINT%g' \
-e 's%gtk_marshal_NONE__POINTER_UINT_ENUM%gtk_marshal_VOID__POINTER_UINT_ENUM%g' \
-e 's%gtk_marshal_NONE__POINTER_POINTER_UINT_UINT%gtk_marshal_VOID__POINTER_POINTER_UINT_UINT%g' \
-e 's%gtk_marshal_NONE__POINTER_INT_INT_POINTER_UINT_UINT%gtk_marshal_VOID__POINTER_INT_INT_POINTER_UINT_UINT%g' \
-e 's%gtk_marshal_NONE__POINTER_UINT_UINT%gtk_marshal_VOID__POINTER_UINT_UINT%g' \
-e 's%gtk_marshal_VOID__STRING%g_cclosure_marshal_VOID__STRING%g' \
-e 's%gtk_marshal_NONE__STRING%gtk_marshal_VOID__STRING%g' \
-e 's%gtk_marshal_NONE__STRING_INT_POINTER%gtk_marshal_VOID__STRING_INT_POINTER%g' \
-e 's%gtk_marshal_VOID__UINT%g_cclosure_marshal_VOID__UINT%g' \
-e 's%gtk_marshal_NONE__UINT%gtk_marshal_VOID__UINT%g' \
-e 's%gtk_marshal_NONE__UINT_POINTER_UINT_ENUM_ENUM_POINTER%gtk_marshal_VOID__UINT_POINTER_UINT_ENUM_ENUM_POINTER%g' \
-e 's%gtk_marshal_NONE__UINT_POINTER_UINT_UINT_ENUM%gtk_marshal_VOID__UINT_POINTER_UINT_UINT_ENUM%g' \
-e 's%gtk_marshal_NONE__UINT_STRING%gtk_marshal_VOID__UINT_STRING%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkMenu / GtkMenuBar / GtkMenuItem
gtk_menu ()
{

sed -i \
-e 's%gtk_menu_append(GTK_MENU(%gtk_menu_shell_append (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_append (GTK_MENU(%gtk_menu_shell_append (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_append (GTK_MENU (%gtk_menu_shell_append (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_append ( GTK_MENU (%gtk_menu_shell_append (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_append(%gtk_menu_shell_append (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_append (%gtk_menu_shell_append (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_insert(GTK_MENU(%gtk_menu_shell_insert (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_insert (GTK_MENU(%gtk_menu_shell_insert (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_insert (GTK_MENU (%gtk_menu_shell_insert (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_insert ( GTK_MENU (%gtk_menu_shell_insert (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_insert(%gtk_menu_shell_insert (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_insert (%gtk_menu_shell_insert (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_prepend(GTK_MENU(%gtk_menu_shell_prepend (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_prepend (GTK_MENU(%gtk_menu_shell_prepend (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_prepend (GTK_MENU (%gtk_menu_shell_prepend (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_prepend ( GTK_MENU (%gtk_menu_shell_prepend (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_prepend(%gtk_menu_shell_prepend (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_prepend (%gtk_menu_shell_prepend (GTK_MENU_SHELL (%g' \
-e 's%gtk_menu_bar_append(GTK_MENU_BAR%gtk_menu_shell_append (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_append (GTK_MENU_BAR%gtk_menu_shell_append (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_append ( GTK_MENU_BAR%gtk_menu_shell_append (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_append(%gtk_menu_shell_append (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_append (%gtk_menu_shell_append (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_insert(GTK_MENU_BAR%gtk_menu_shell_insert (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_insert (GTK_MENU_BAR%gtk_menu_shell_insert (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_insert ( GTK_MENU_BAR%gtk_menu_shell_insert (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_insert(%gtk_menu_shell_insert (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_insert (%gtk_menu_shell_insert (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_prepend(GTK_MENU_BAR%gtk_menu_shell_prepend (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_prepend (GTK_MENU_BAR%gtk_menu_shell_prepend (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_prepend ( GTK_MENU_BAR%gtk_menu_shell_prepend (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_prepend(%gtk_menu_shell_prepend (GTK_MENU_SHELL%g' \
-e 's%gtk_menu_bar_prepend (%gtk_menu_shell_prepend (GTK_MENU_SHELL%g' \
${target}

# gtk_menu_item_set_right_justified was deprecated in 3.2 .. not really important
sed -i \
-e '/gtk_menu_item_right_justify/d' \
-e '/gtk_menu_item_set_right_justified/d' \
-e '/gtk_menu_item_get_right_justified/d' \
-e 's%gtk_menu_item_remove_submenu%-(add 2nd param: NULL)-  gtk_menu_item_set_submenu%g' \
-e 's%gtk_check_menu_item_set_state%gtk_check_menu_item_set_active%g' \
-e '/gtk_check_menu_item_set_show_toggle/d' \
${target}

}
#===========================================================================


#===========================================================================
# >>> Misc
misc ()
{

# >>> General Gtk

sed -i \
-e '/gtk_set_locale/d' \
-e '/gtk_disable_setlocale/d' \
-e 's%gtk_exit%exit%g' \
-e 's%gtk_timeout_add%g_timeout_add%g' \
-e 's%gtk_timeout_remove%g_source_remove%g' \
-e 's%gtk_timeout_add_full%g_timeout_add_full%g' \
-e 's%GtkFunction%GSourceFunc%g' \
-e 's%gtk_idle_add%g_idle_add%g' \
-e 's%gtk_idle_add_priority%g_idle_add_full%g' \
-e 's%gtk_idle_add_full%g_idle_add_full%g' \
-e 's%gtk_idle_remove%g_source_remove%g' \
-e 's%gtk_idle_remove_by_data%g_idle_remove_by_data%g' \
-e 's%GTK_PRIORITY_REDRAW%GDK_PRIORITY_REDRAW%g' \
-e 's%GTK_PRIORITY_HIGH%G_PRIORITY_HIGH%g' \
-e '/GTK_PRIORITY_INTERNAL/d' \
-e 's%GTK_PRIORITY_DEFAULT%G_PRIORITY_DEFAULT_IDLE%g' \
-e 's%GTK_PRIORITY_LOW%G_PRIORITY_LOW%g' \
-e 's%gtk_calendar_display_options%gtk_calendar_set_display_options%g' \
-e '/gtk_calendar_freeze/d' \
-e '/gtk_calendar_thaw/d' \
-e '/gtk_layout_freeze/d' \
-e '/gtk_layout_thaw/d' \
-e '/gtk_paned_gutter_size/d' \
-e '/gtk_paned_set_gutter_size/d' \
-e '/gtk_color_selection_set_update_policy/d' \
-e 's%gtk_color_selection_set_change_palette_hook%gtk_color_selection_set_change_palette_hook%g' \
-e 's%gtk_color_selection_get_color%gtk_color_selection_get_current_color%g' \
-e 's%gtk_color_selection_set_color%gtk_color_selection_set_current_color%g' \
-e 's%gtk_style_unref%g_object_unref%g' \
-e 's%gtk_accel_group_ref%g_object_ref%g' \
-e 's%gtk_accel_group_unref%g_object_unref%g' \
-e 's%gtk_style_ref%g_object_ref%g' \
-e 's%gtk_style_unref%g_object_unref%g' \
-e 's%gtk_icon_view_get_orientation%gtk_icon_view_get_item_orientation%g' \
-e 's%gtk_icon_view_set_orientation%gtk_icon_view_set_item_orientation%g' \
-e 's%gtk_status_icon_set_tooltip(%gtk_status_icon_set_tooltip_text(%g' \
-e 's%gtk_status_icon_set_tooltip (%gtk_status_icon_set_tooltip_text(%g' \
-e 's%gtk_spin_button_get_value_as_float%gtk_spin_button_get_value%g' \
-e '/gtk_status_icon_get_blinking/d' \
-e '/gtk_status_icon_set_blinking/d' \
${target}

# >>> GtkContainer
sed -i \
-e 's%gtk_container_border_width%gtk_container_set_border_width%g' \
-e 's%gtk_container_children%gtk_container_get_children%g' \
${target}

# >>> GtkDrawingArea
sed -i \
-e 's%gtk_drawing_area_size(GTK_DRAWING_AREA%gtk_widget_set_size_request (GTK_WIDGET%g' \
-e 's%gtk_drawing_area_size (GTK_DRAWING_AREA%gtk_widget_set_size_request (GTK_WIDGET%g' \
-e 's%gtk_drawing_area_size ( GTK_DRAWING_AREA%gtk_widget_set_size_request (GTK_WIDGET%g' \
-e 's%gtk_drawing_area_size%gtk_widget_set_size_request%g' \
${target}

# >>> GtkLabel
sed -i \
-e 's%gtk_label_set %gtk_label_set_text %g' \
-e 's%gtk_label_set(%gtk_label_set_text (%g' \
${target}

# >>> GtkNoteBook
sed -i \
-e 's%gtk_notebook_current_page%gtk_notebook_get_current_page%g' \
-e 's%gtk_notebook_set_page%gtk_notebook_set_current_page%g' \
-e 's%gtk_notebook_get_group %gtk_notebook_get_group_name %g' \
-e 's%gtk_notebook_get_group (%gtk_notebook_get_group_name (%g' \
-e 's%gtk_notebook_set_group %gtk_notebook_set_group_name %g' \
-e 's%gtk_notebook_set_group (%gtk_notebook_set_group_name (%g' \
-e '/gtk_notebook_set_tab_border/d' \
-e '/gtk_notebook_set_tab_hborder/d' \
-e '/gtk_notebook_set_tab_vborder/d' \
${target}

# >>> GtkRadioButton / GtkRadioMenuItem
sed -i 's%gtk_radio_button_group%gtk_radio_button_get_group%g' ${target}
sed -i 's%gtk_radio_menu_item_group%gtk_radio_menu_item_get_group%g' ${target}

# >>> GtkSpinButton
sed -i 's%gtk_spin_button_get_value_as_float%gtk_spin_button_get_value%g' ${target}

# >>> GtkTreeModel
sed -i \
-e 's%gtk_tree_path_new_root%gtk_tree_path_new_first%g' \
-e 's%gtk_tree_model_get_iter_root%gtk_tree_model_get_iter_first%g' \
${target}

# misc vertical/horizontal orientation
sed -i \
-e 's%GTK_TYPE_VBOX%GTK_TYPE_BOX%g' \
-e 's%GTK_TYPE_HBOX%GTK_TYPE_BOX%g' \
-e 's%GTK_VBOX%GTK_BOX%g' \
-e 's%GTK_HBOX%GTK_BOX%g' \
-e 's%GtkVBox%GtkBox%g' \
-e 's%GtkHBox%GtkBox%g' \
-e 's%GTK_TYPE_VSCALE%GTK_TYPE_SCALE%g' \
-e 's%GTK_TYPE_HSCALE%GTK_TYPE_SCALE%g' \
-e 's%GTK_VSCALE%GTK_SCALE%g' \
-e 's%GTK_HSCALE%GTK_SCALE%g' \
-e 's%GtkVScale%GtkScale%g' \
-e 's%GtkHScale%GtkScale%g' \
-e 's%GTK_TYPE_VSEPARATOR%GTK_TYPE_SEPARATOR%g' \
-e 's%GTK_TYPE_HSEPARATOR%GTK_TYPE_SEPARATOR%g' \
-e 's%GTK_VSEPARATOR%GTK_SEPARATOR%g' \
-e 's%GTK_HSEPARATOR%GTK_SEPARATOR%g' \
-e 's%GtkVSeparator%GtkSeparator%g' \
-e 's%GtkHSeparator%GtkSeparator%g' \
-e 's%GTK_TYPE_VSCROLLBAR%GTK_TYPE_SCROLLBAR%g' \
-e 's%GTK_TYPE_HSCROLLBAR%GTK_TYPE_SCROLLBAR%g' \
-e 's%GTK_VSCROLLBAR%GTK_SCROLLBAR%g' \
-e 's%GTK_HSCROLLBAR%GTK_SCROLLBAR%g' \
-e 's%GtkVScrollbar%GtkScrollbar%g' \
-e 's%GtkHScrollbar%GtkScrollbar%g' \
-e 's%GTK_TYPE_VPANED%GTK_TYPE_PANED%g' \
-e 's%GTK_TYPE_HPANED%GTK_TYPE_PANED%g' \
-e 's%GTK_VPANED%GTK_PANED%g' \
-e 's%GTK_HPANED%GTK_PANED%g' \
-e 's%GtkVPaned%GtkPaned%g' \
-e 's%GtkHPaned%GtkPaned%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkTooltips  -> gtk widget tooltip [gtk 2.12+]
gtk_tooltip ()
{

#https://developer.gnome.org/gtk2/stable/gtk-migrating-tooltips.html
sed -i \
-e '/GtkTooltips/d' \
-e '/gtk_tooltips_new/d' \
-e '/gtk_tooltips_enable/d' \
-e '/gtk_tooltips_disable/d' \
-e '/gtk_tooltips_set_delay/d' \
-e '/gtk_tooltips_data_get/d' \
-e '/gtk_tooltips_force_window/d' \
-e '/gtk_tooltips_get_info_from_tip_window/d' \
-e 's%gtk_tooltips_set_tip%-(remove 1st and 4th params)-  gtk_widget_set_tooltip_text%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkWidget
gtk_widget ()
{

sed -i \
-e 's%GTK_WIDGET_STATE%gtk_widget_get_state%g' \
-e '/GTK_WIDGET_SAVED_STATE/d' \
-e 's%GTK_WIDGET_TOPLEVEL%gtk_widget_is_toplevel%g' \
-e 's%GTK_WIDGET_NO_WINDOW%gtk_widget_get_has_window%g' \
-e 's%GTK_WIDGET_REALIZED%gtk_widget_get_realized%g' \
-e 's%GTK_WIDGET_MAPPED%gtk_widget_get_mapped%g' \
-e 's%GTK_WIDGET_VISIBLE%gtk_widget_get_visible%g' \
-e 's%GTK_WIDGET_DRAWABLE%gtk_widget_is_drawable%g' \
-e 's%GTK_WIDGET_SENSITIVE%gtk_widget_get_sensitive%g' \
-e 's%GTK_WIDGET_IS_SENSITIVE%gtk_widget_is_sensitive%g' \
-e 's%GTK_WIDGET_CAN_FOCUS%gtk_widget_get_can_focus%g' \
-e 's%GTK_WIDGET_HAS_FOCUS%gtk_widget_has_focus%g' \
-e 's%GTK_WIDGET_CAN_DEFAULT%gtk_widget_get_can_default%g' \
-e 's%GTK_WIDGET_RECEIVES_DEFAULT% gtk_widget_get_receives_default%g' \
-e 's%GTK_WIDGET_HAS_DEFAULT%gtk_widget_has_default%g' \
-e 's%GTK_WIDGET_HAS_GRAB%gtk_widget_has_grab%g' \
-e 's%GTK_WIDGET_RC_STYLE%gtk_widget_has_rc_style%g' \
-e 's%GTK_WIDGET_APP_PAINTABLE%gtk_widget_get_app_paintable%g' \
-e 's%GTK_WIDGET_DOUBLE_BUFFERED%gtk_widget_get_double_buffered%g' \
-e 's%GTK_WIDGET_TYPE%G_OBJECT_TYPE%g' \
-e 's%gtk_widget_hide_all%gtk_widget_hide%g' \
-e '/gtk_widget_pop_visual/d' \
-e '/gtk_widget_push_visual/d' \
-e 's%gtk_widget_queue_clear%gtk_widget_queue_draw%g' \
-e 's%gtk_widget_queue_clear_area%gtk_widget_queue_draw_area%g' \
-e '/gtk_widget_reset_shapes/d' \
-e 's%GTK_WIDGET_SENSITIVE%gtk_widget_get_sensitive%g' \
-e '/gtk_widget_set_default_visual/d' \
-e 's%gtk_widget_set_usize%gtk_widget_set_size_request%g' \
-e '/gtk_widget_set_visual/d' \
-e 's%gtk_widget_ref%g_object_ref%g' \
-e 's%gtk_widget_unref%g_object_unref%g' \
-e 's%gtk_widget_set (%g_object_set (%g' \
-e 's%gtk_widget_set(%g_object_set(%g' \
-e 's%gtk_object_destroy%gtk_widget_destroy%g' \
${target}

sed -i \
-e 's%GTK_WIDGET_UNSET_FLAGS.*GTK_CAN_FOCUS%gtk_widget_set_can_focus (WWIDGETT, FALSE%g' \
-e 's%GTK_WIDGET_SET_FLAGS.*GTK_CAN_FOCUS%gtk_widget_set_can_focus (WWIDGETT, TRUE%g' \
-e 's%GTK_WIDGET_UNSET_FLAGS.*GTK_CAN_DEFAULT%gtk_widget_set_can_default (WWIDGETT, FALSE%g' \
-e 's%GTK_WIDGET_SET_FLAGS.*GTK_CAN_DEFAULT%gtk_widget_set_can_default (WWIDGETT, TRUE%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkWindow / GtkDialog
gtk_window ()
{

sed -i \
-e 's%gtk_about_dialog_get_name%gtk_about_dialog_get_program_name%g' \
-e 's%gtk_about_dialog_set_name%gtk_about_dialog_set_program_name%g' \
-e '/gtk_dialog_get_has_separator/d' \
-e '/gtk_dialog_set_has_separator/d' \
-e '/gtk_window_set_frame_dimensions/d' \
-e '/gtk_window_get_frame_dimensions/d' \
-e '/gtk_window_set_has_frame/d' \
-e '/gtk_window_get_has_frame/d' \
-e 's%gtk_window_position%gtk_window_set_position%' \
-e 's%gdk_window_get_size%gdk_drawable_get_size%' \
-e 's%GTK_WINDOW_DIALOG%GTK_WINDOW_TOPLEVEL%g'  \
-e 's%gtk_window_set_wmclass%-(remove 3rd param)- gtk_window_set_role%g' \
-e 's%gtk_widget_set_uposition(GTK_WIDGET%gtk_window_move (GTK_WINDOW%g' \
-e 's%gtk_widget_set_uposition (GTK_WIDGET%gtk_window_move (GTK_WINDOW%g' \
-e 's%gtk_widget_set_uposition ( GTK_WIDGET%gtk_window_move (GTK_WINDOW%g' \
-e 's%gtk_widget_set_uposition%gtk_window_move%g' \
-e 's%gtk_window_position%gtk_window_set_position%g' \
-e 's%->action_area%##-> gtk_dialog_get_action_area(dialog); <-##%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GDK
gdk ()
{
#gdk_drawable_get_display gdk_window_get_display -- 2.24
#gdk_drawable_get_screen  gdk_window_get_screen  -- 
#gdk_drawable_get_visual  gdk_window_get_visual  -- 2.24
sed -i \
-e 's%gdk_draw_pixmap%gdk_draw_drawable%g' \
-e 's%gdk_draw_bitmap%gdk_draw_drawable%g' \
-e 's%gdk_window_get_size%gdk_drawable_get_size%g' \
-e 's%gdk_window_get_type%gdk_window_get_window_type%g' \
-e 's%gdk_window_get_colormap%gdk_drawable_get_colormap%g' \
-e 's%gdk_window_set_colormap%gdk_drawable_set_colormap%g' \
-e 's%gdk_window_ref%g_object_ref%g' \
-e 's%gdk_window_unref%g_object_unref%g' \
-e 's%gdk_bitmap_ref%g_object_ref%g' \
-e 's%gdk_bitmap_unref%g_object_unref%g' \
-e 's%gdk_pixmap_ref%g_object_ref%g' \
-e 's%gdk_pixmap_unref%g_object_unref%g' \
-e 's%gdk_gc_destroy%g_object_unref%g' \
-e 's%gdk_gc_unref%g_object_unref%g' \
-e 's%gdk_gc_ref%g_object_ref%g' \
-e 's%gdk_image_destroy%g_object_unref%g' \
-e 's%gdk_colormap_ref%g_object_ref%g' \
-e 's%gdk_colormap_unref%g_object_unref%g' \
-e 's%gdk_image_ref%g_object_ref%g' \
-e 's%gdk_image_unref%g_object_unref%g' \
-e 's%gdk_visual_ref%g_object_ref%g' \
-e 's%gdk_visual_unref%g_object_unref%g' \
-e 's%gdk_cursor_destroy%gdk_cursor_unref%g' \
-e 's%gdk_rgb_get_cmap%gdk_rgb_get_colormap%g' \
-e 's%gdk_drawable_set_data(GDK_DRAWABLE%g_object_set_data_full (G_OBJECT%g' \
-e 's%gdk_drawable_set_data (GDK_DRAWABLE%g_object_set_data_full (G_OBJECT%g' \
-e 's%gdk_drawable_set_data ( GDK_DRAWABLE%g_object_set_data_full (G_OBJECT%g' \
-e 's%gdk_drawable_set_data%g_object_set_data_full%g' \
-e 's%gdk_drawable_get_data(GDK_DRAWABLE%g_object_get_data (G_OBJECT%g' \
-e 's%gdk_drawable_get_data (GDK_DRAWABLE%g_object_get_data (G_OBJECT%g' \
-e 's%gdk_drawable_get_data ( GDK_DRAWABLE%g_object_get_data (G_OBJECT%g' \
-e 's%gdk_drawable_get_data%g_object_get_data%g' \
-e 's%gdk_drawable_ref(GDK_DRAWABLE%g_object_ref%g' \
-e 's%gdk_drawable_ref (GDK_DRAWABLE%g_object_ref%g' \
-e 's%gdk_drawable_ref%g_object_ref%g' \
-e 's%gdk_drawable_unref(GDK_DRAWABLE%g_object_unref%g' \
-e 's%gdk_drawable_unref (GDK_DRAWABLE%g_object_unref%g' \
-e 's%gdk_drawable_unref%g_object_unref%g' \
-e 's%gdk_pixbuf_render_to_drawable%-(move 1st param to 3rd position)- gdk_draw_pixbuf%g' \
-e 's%gdk_pixbuf_ref%g_object_ref%g' \
-e 's%gdk_pixbuf_unref%g_object_unref%g' \
-e 's%gdk_pixbuf_animation_ref%g_object_ref%g' \
-e 's%gdk_pixbuf_animation_unref%g_object_unref%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> 
gtk_a ()
{

sed -i \
-e 's%%%g' \
${target}

}
#===========================================================================

usage() {
	echo "Usage:
	$0 <directory> all
	$0 <directory> <other>
	$0 <directory> gtk_combo

<other>
   glib              g_signal        g_object
   gtk_button_box    gtk_menu
   gtk_combox_box    gtk_entry       gtk_marshall
   gtk_tooltip       gtk_widget      gtk_window
   misc              gdk

sepcial cases:
   gtk_combo
"
	exit
}

if ! [ -d "$1" ] ; then
	usage
fi

dir=$1
set_target
shift

case $1 in
	glib)              glib             ;;
	g_signal)          g_signal         ;;
	g_object)          g_object         ;;
	gtk_button_box)    gtk_button_box   ;;
	gtk_combo)         gtk_combo        ;; #not included in 'all'
	gtk_combo_box)     gtk_combox_box   ;;
	gtk_entry)         gtk_entry        ;;
	gtk_marshall)      gtk_marshall     ;;
	gtk_tooltip)       gtk_tooltip      ;;
	gtk_menu)          gtk_menu         ;;
	gtk_widget)        gtk_widget       ;;
	gtk_window)        gtk_window       ;;
	misc)              misc;;
	gdk)               gdk ;;
	all)
		glib
		g_signal
		g_object
		gtk_button_box
		gtk_combo_box
		gtk_entry
		gtk_marshall
		gtk_menu
		gtk_tooltip
		gtk_widget
		gtk_window
		misc
		gdk
		;;
	*)
		usage
esac


# --requires extra work:

#gtk_widget_draw
#gtk_widget_get_action ()

# gtk_window_set_policy > gtk_window_set_resizable
#      this requires manual fixing
#      https://github.com/wdlkmpx/gwaveedit/commit/4506f1411dfd7dbfd01a0f63259ff916520b6a9a
#  FALSE,FALSE,TRUE -> FALSE
#  FALSE,TRUE,FALSE -> TRUE
#  TRUE,TRUE,FALSE -> TRUE

#define gtk_widget_restore_default_style(widget) (gtk_widget_set_style (widget, NULL))
#define gtk_widget_set_rc_style(widget)          (gtk_widget_set_style (widget, NULL))
#define gtk_style_apply_default_pixmap(s,gw,st,a,x,y,w,h) gtk_style_apply_default_background (s,gw,1,st,a,x,y,w,h)
#define gdk_window_copy_area(drawable,gc,x,y,source_drawable,source_x,source_y,width,height) \
#        gdk_draw_pixmap     (drawable,gc,source_drawable,source_x,source_y,x,y,width,height)
# -- keep param 1, 3 -- remove params 2, 4.. probably merge 4 into 3
#sed -i 's%gtk_menu_tool_button_set_arrow_tooltip%gtk_menu_tool_button_set_arrow_tooltip_text%g' ${target}

#==================================================================
# Gobject

#http://inkscape.13.x6.nabble.com/GtkObject-deprecation-td2820853.html
#http://blogs.gnome.org/desrt/2012/02/26/a-gentle-introduction-to-gobject-construction/

#Replace GtkObject with GObject [!DANGER!]
#gtk_object_unref

#gtk_object_ref + gtk_object_sink -> g_object_ref_sink
#gtk_object_ref   -> g_object_ref_sink
#gtk_object_unref -> g_object_unref
#gtk_widget_unref -> g_object_unref
#gtk_object_sink  -> g_object_ref_sink
#gtk_object_set_data -> g_object_set_data
#gtk_object_get_data -> g_object_get_data

#GtkObject        -> GObject
#GtkObjectClass   -> GObjectClass
#GTK_OBJECT       -> G_OBJECT
#GTK_OBJECT_CLASS -> G_OBJECT_CLASS
#GTK_TYPE_OBJECT  -> G_TYPE_OBJECT

#sed -i 's%GtkObject%GObject%g' $target
#sed -i 's%GTK_OBJECT%G_OBJECT%g' $target
#sed -i 's%GTK_TYPE_OBJECT%G_TYPE_OBJECT%g' $target
#sed -i 's%gtk_object_sink%gtk_object_sink_compat%g' $target
#sed -i 's%gtk_object_ref%g_object_ref%g' $target
#sed -i 's%gtk_object_unref%g_object_unref%g' $target

