#!/bin/sh
# Public Domain

# removed in gtk4
# - gtk_frame_set_shadow_type

set_target() {
	target=$(
	find $dir -type f -name '*.c' \
	               -o -name '*.cc' \
	               -o -name '*.cpp' \
	               -o -name '*.h' \
	               -o -name '*.hh' | grep -v gtkcompat\. | grep -v glib-compat\.
	)
}

#===========================================================================
# >>> GtkBox
gtk_box () {

# the HOMOGENEOUS property defaults to FALSE
# add this to make it TRUE
#   gtk_box_set_homogeneous (GTK_BOX(hbox), TRUE);
sed -i \
-e 's%gtk_hbox_new(FALSE%gtk_box_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_hbox_new( FALSE%gtk_box_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_hbox_new (FALSE%gtk_box_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_hbox_new ( FALSE%gtk_box_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_hbox_new (%gtk_box_new (GTK_ORIENTATION_HORIZONTAL, ); -- %g' \
-e 's%gtk_hbox_new(%gtk_box_new (GTK_ORIENTATION_HORIZONTAL, ); -- %g' \
-e 's%gtk_vbox_new(FALSE%gtk_box_new (GTK_ORIENTATION_VERTICAL%g' \
-e 's%gtk_vbox_new( FALSE%gtk_box_new (GTK_ORIENTATION_VERTICAL%g' \
-e 's%gtk_vbox_new (FALSE%gtk_box_new (GTK_ORIENTATION_VERTICAL%g' \
-e 's%gtk_vbox_new ( FALSE%gtk_box_new (GTK_ORIENTATION_VERTICAL%g' \
-e 's%gtk_vbox_new (%gtk_box_new (GTK_ORIENTATION_VERTICAL, ); -- %g' \
-e 's%gtk_vbox_new(%gtk_box_new (GTK_ORIENTATION_VERTICAL, ); -- %g' \
${target}

}
#===========================================================================

#===========================================================================
# >>> GtkButtonBox
gtk_button_box () {

sed -i \
-e 's%gtk_hbutton_box_new(%gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_hbutton_box_new (%gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_vbutton_box_new(%gtk_button_box_new (GTK_ORIENTATION_VERTICAL%g' \
-e 's%gtk_vbutton_box_new (%gtk_button_box_new (GTK_ORIENTATION_VERTICAL%g' \
${target}

}
#===========================================================================

#===========================================================================
# >>> GtkFontChooser
gtk_font_chooser () {

sed -i \
-e 's%gtk_font_selection_new%gtk_font_chooser_widget_new%g' \
-e 's%gtk_font_selection_dialog_new%/**ADD NULL param (parent window)**/gtk_font_chooser_dialog_new%g' \
-e 's%gtk_font_selection_set_preview_text (GTK_FONT_SELECTION%gtk_font_chooser_set_preview_text (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_set_preview_text(GTK_FONT_SELECTION%gtk_font_chooser_set_preview_text (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_set_preview_text%gtk_font_chooser_set_preview_text%g' \
-e 's%gtk_font_selection_get_preview_text (GTK_FONT_SELECTION%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_preview_text (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_preview_text(GTK_FONT_SELECTION%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_preview_text (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_preview_text%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_preview_text%g' \
-e 's%gtk_font_selection_get_face (GTK_FONT_SELECTION%gtk_font_chooser_get_font_face (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_face(GTK_FONT_SELECTION%gtk_font_chooser_get_font_face (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_face (%gtk_font_chooser_get_font_face (%g' \
-e 's%gtk_font_selection_get_face(%gtk_font_chooser_get_font_face (%g' \
-e 's%gtk_font_selection_get_family (GTK_FONT_SELECTION%gtk_font_chooser_get_font_family (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_family(GTK_FONT_SELECTION%gtk_font_chooser_get_font_family (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_family (%gtk_font_chooser_get_font_family (%g' \
-e 's%gtk_font_selection_get_family(%gtk_font_chooser_get_font_family (%g' \
-e 's%gtk_font_selection_get_size (GTK_FONT_SELECTION%gtk_font_chooser_get_font_size (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_size(GTK_FONT_SELECTION%gtk_font_chooser_get_font_size (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_size (%gtk_font_chooser_get_font_size (%g' \
-e 's%gtk_font_selection_get_size(%gtk_font_chooser_get_font_size (%g' \
-e 's%gtk_font_selection_set_font_name (GTK_FONT_SELECTION%gtk_font_chooser_set_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_set_font_name(GTK_FONT_SELECTION%gtk_font_chooser_set_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_set_font_name%gtk_font_chooser_set_font%g' \
-e 's%gtk_font_selection_get_font_name (GTK_FONT_SELECTION%gtk_font_chooser_get_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_font_name(GTK_FONT_SELECTION%gtk_font_chooser_get_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_get_font_name%gtk_font_chooser_get_font%g' \
-e 's%gtk_font_button_get_font_name (GTK_FONT_BUTTON%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_button_get_font_name(GTK_FONT_BUTTON%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_button_get_font_name%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_font%g' \
-e 's%gtk_font_button_set_font_name (GTK_FONT_BUTTON%gtk_font_chooser_set_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_button_set_font_name(GTK_FONT_BUTTON%gtk_font_chooser_set_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_button_set_font_name%gtk_font_chooser_set_font%g' \
-e 's%gtk_font_selection_dialog_set_preview_text (GTK_FONT_SELECTION_DIALOG%gtk_font_chooser_set_preview_text (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_dialog_set_preview_text(GTK_FONT_SELECTION_DIALOG%gtk_font_chooser_set_preview_text (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_dialog_set_preview_text%gtk_font_chooser_set_preview_text%g' \
-e 's%gtk_font_selection_dialog_get_preview_text (GTK_FONT_SELECTION_DIALOG%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_preview_text (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_dialog_get_preview_text(GTK_FONT_SELECTION_DIALOG%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_preview_text (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_dialog_get_preview_text%/**RETVAL MUST BE FREED**/gtk_font_chooser_get_preview_text%g' \
-e 's%gtk_font_selection_dialog_set_font_name (GTK_FONT_SELECTION_DIALOG%gtk_font_chooser_set_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_dialog_set_font_name(GTK_FONT_SELECTION_DIALOG%gtk_font_chooser_set_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_dialog_set_font_name%gtk_font_chooser_set_font%g' \
-e 's%gtk_font_selection_dialog_get_font_name (GTK_FONT_SELECTION_DIALOG%gtk_font_chooser_get_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_dialog_get_font_name(GTK_FONT_SELECTION_DIALOG%gtk_font_chooser_get_font (GTK_FONT_CHOOSER%g' \
-e 's%gtk_font_selection_dialog_get_font_name%gtk_font_chooser_get_font%g' \
${target}
}
#===========================================================================

#===========================================================================
# >>> GtkScale
gtk_scale () {

sed -i \
-e 's%gtk_hscale_new (%gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, %g' \
-e 's%gtk_hscale_new(%gtk_scale_new (GTK_ORIENTATION_HORIZONTAL, %g' \
-e 's%gtk_vscale_new (%gtk_scale_new (GTK_ORIENTATION_VERTICAL, %g' \
-e 's%gtk_vscale_new(%gtk_scale_new (GTK_ORIENTATION_VERTICAL, %g' \
${target}

sed -i \
-e 's%gtk_hscale_new_with_range (%gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, %g' \
-e 's%gtk_hscale_new_with_range(%gtk_scale_new_with_range (GTK_ORIENTATION_HORIZONTAL, %g' \
-e 's%gtk_vscale_new_with_range (%gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, %g' \
-e 's%gtk_vscale_new_with_range(%gtk_scale_new_with_range (GTK_ORIENTATION_VERTICAL, %g' \
${target}

}
#===========================================================================

#===========================================================================
# >>> GtkSeparator
gtk_separator () {

sed -i \
-e 's%gtk_hseparator_new (%gtk_separator_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_hseparator_new(%gtk_separator_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_vseparator_new (%gtk_separator_new (GTK_ORIENTATION_VERTICAL%g' \
-e 's%gtk_vseparator_new(%gtk_separator_new (GTK_ORIENTATION_VERTICAL%g' \
${target}

}
#===========================================================================

#===========================================================================
# >>> GtkScrollbar
gtk_scrollbar () {

sed -i \
-e 's%gtk_hscrollbar_new (%gtk_scrollbar_new (GTK_ORIENTATION_HORIZONTAL, %g' \
-e 's%gtk_hscrollbar_new(%gtk_scrollbar_new (GTK_ORIENTATION_HORIZONTAL, %g' \
-e 's%gtk_vscrollbar_new (%gtk_scrollbar_new (GTK_ORIENTATION_VERTICAL, %g' \
-e 's%gtk_vscrollbar_new(%gtk_scrollbar_new (GTK_ORIENTATION_VERTICAL, %g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GTK misc
misc () {

rules_hint="--(complete #if)-- #if GTK_MAJOR_VERSION < 3\ngtk_tree_view_set_rules_hint"

sed -i \
-e 's%gtk_widget_set_margin_left%gtk_widget_set_margin_start%g' \
-e 's%gtk_widget_set_margin_right%gtk_widget_set_margin_end%g' \
-e '/gtk_combo_box_get_add_tearoffs/d' \
-e '/gtk_combo_box_set_add_tearoffs/d' \
-e '/gtk_combo_box_get_title/d' \
-e '/gtk_combo_box_set_title/d' \
-e "s/gtk_tree_view_set_rules_hint/${rules_hint}/" \
-e '/gtk_tree_view_get_rules_hint/d' \
-e 's%gtk_widget_size_request%/**add extra NULL param**/ gtk_widget_get_preferred_size%g' \
-e 's%gtk_widget_modify_font%gtk_widget_override_font%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkPaned
gtk_paned () {

sed -i \
-e 's%gtk_hpaned_new (%gtk_paned_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_hpaned_new(%gtk_paned_new (GTK_ORIENTATION_HORIZONTAL%g' \
-e 's%gtk_vpaned_new (%gtk_paned_new (GTK_ORIENTATION_VERTICAL%g' \
-e 's%gtk_vpaned_new(%gtk_paned_new (GTK_ORIENTATION_VERTICAL%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkStock
gtk_stock ()
{

sed -i \
-e 's%GTK_STOCK_ABOUT%"gtk-about"%g' \
-e 's%GTK_STOCK_ADD%"gtk-add"%g' \
-e 's%GTK_STOCK_APPLY%"gtk-apply"%g' \
-e 's%GTK_STOCK_BOLD%"gtk-bold"%g' \
-e 's%GTK_STOCK_CANCEL%"gtk-cancel"%g' \
-e 's%GTK_STOCK_CAPS_LOCK_WARNING%"gtk-caps-lock-warning"%g' \
-e 's%GTK_STOCK_CDROM%"gtk-cdrom"%g' \
-e 's%GTK_STOCK_CLEAR%"gtk-clear"%g' \
-e 's%GTK_STOCK_CLOSE%"gtk-close"%g' \
-e 's%GTK_STOCK_COLOR_PICKER%"gtk-color-picker"%g' \
-e 's%GTK_STOCK_CONNECT%"gtk-connect"%g' \
-e 's%GTK_STOCK_CONVERT%"gtk-convert"%g' \
-e 's%GTK_STOCK_COPY%"gtk-copy"%g' \
-e 's%GTK_STOCK_CUT%"gtk-cut"%g' \
-e 's%GTK_STOCK_DELETE%"gtk-delete"%g' \
-e 's%GTK_STOCK_DIALOG_AUTHENTICATION%"gtk-dialog-authentication"%g' \
-e 's%GTK_STOCK_DIALOG_INFO%"gtk-dialog-info"%g' \
-e 's%GTK_STOCK_DIALOG_WARNING%"gtk-dialog-warning"%g' \
-e 's%GTK_STOCK_DIALOG_ERROR%"gtk-dialog-error"%g' \
-e 's%GTK_STOCK_DIALOG_QUESTION%"gtk-dialog-question"%g' \
-e 's%GTK_STOCK_DIRECTORY%"gtk-directory"%g' \
-e 's%GTK_STOCK_DISCARD%"gtk-discard"%g' \
-e 's%GTK_STOCK_DISCONNECT%"gtk-disconnect"%g' \
-e 's%GTK_STOCK_DND_MULTIPLE%"gtk-dnd-multiple"%g' \
-e 's%GTK_STOCK_DND%"gtk-dnd"%g' \
-e 's%GTK_STOCK_EDIT%"gtk-edit"%g' \
-e 's%GTK_STOCK_EXECUTE%"gtk-execute"%g' \
-e 's%GTK_STOCK_FILE%"gtk-file"%g' \
-e 's%GTK_STOCK_FIND_AND_REPLACE%"gtk-find-and-replace"%g' \
-e 's%GTK_STOCK_FIND%"gtk-find"%g' \
-e 's%GTK_STOCK_FLOPPY%"gtk-floppy"%g' \
-e 's%GTK_STOCK_FULLSCREEN%"gtk-fullscreen"%g' \
-e 's%GTK_STOCK_GOTO_BOTTOM%"gtk-goto-bottom"%g' \
-e 's%GTK_STOCK_GOTO_FIRST%"gtk-goto-first"%g' \
-e 's%GTK_STOCK_GOTO_LAST%"gtk-goto-last"%g' \
-e 's%GTK_STOCK_GOTO_TOP%"gtk-goto-top"%g' \
-e 's%GTK_STOCK_GO_BACK%"gtk-go-back"%g' \
-e 's%GTK_STOCK_GO_DOWN%"gtk-go-down"%g' \
-e 's%GTK_STOCK_GO_FORWARD%"gtk-go-forward"%g' \
-e 's%GTK_STOCK_GO_UP%"gtk-go-up"%g' \
-e 's%GTK_STOCK_HARDDISK%"gtk-harddisk"%g' \
-e 's%GTK_STOCK_HELP%"gtk-help"%g' \
-e 's%GTK_STOCK_HOME%"gtk-home"%g' \
-e 's%GTK_STOCK_INDEX%"gtk-index"%g' \
-e 's%GTK_STOCK_INDENT%"gtk-indent"%g' \
-e 's%GTK_STOCK_INFO%"gtk-info"%g' \
-e 's%GTK_STOCK_ITALIC%"gtk-italic"%g' \
-e 's%GTK_STOCK_JUMP_TO%"gtk-jump-to"%g' \
-e 's%GTK_STOCK_JUSTIFY_CENTER%"gtk-justify-center"%g' \
-e 's%GTK_STOCK_JUSTIFY_FILL%"gtk-justify-fill"%g' \
-e 's%GTK_STOCK_JUSTIFY_LEFT%"gtk-justify-left"%g' \
-e 's%GTK_STOCK_JUSTIFY_RIGHT%"gtk-justify-right"%g' \
-e 's%GTK_STOCK_LEAVE_FULLSCREEN%"gtk-leave-fullscreen"%g' \
-e 's%GTK_STOCK_MISSING_IMAGE%"gtk-missing-image"%g' \
-e 's%GTK_STOCK_MEDIA_FORWARD%"gtk-media-forward"%g' \
-e 's%GTK_STOCK_MEDIA_NEXT%"gtk-media-next"%g' \
-e 's%GTK_STOCK_MEDIA_PAUSE%"gtk-media-pause"%g' \
-e 's%GTK_STOCK_MEDIA_PLAY%"gtk-media-play"%g' \
-e 's%GTK_STOCK_MEDIA_PREVIOUS%"gtk-media-previous"%g' \
-e 's%GTK_STOCK_MEDIA_RECORD%"gtk-media-record"%g' \
-e 's%GTK_STOCK_MEDIA_REWIND%"gtk-media-rewind"%g' \
-e 's%GTK_STOCK_MEDIA_STOP%"gtk-media-stop"%g' \
-e 's%GTK_STOCK_NETWORK%"gtk-network"%g' \
-e 's%GTK_STOCK_NEW%"gtk-new"%g' \
-e 's%GTK_STOCK_NO%"gtk-no"%g' \
-e 's%GTK_STOCK_OK%"gtk-ok"%g' \
-e 's%GTK_STOCK_OPEN%"gtk-open"%g' \
-e 's%GTK_STOCK_ORIENTATION_PORTRAIT%"gtk-orientation-portrait"%g' \
-e 's%GTK_STOCK_ORIENTATION_LANDSCAPE%"gtk-orientation-landscape"%g' \
-e 's%GTK_STOCK_ORIENTATION_REVERSE_LANDSCAPE%"gtk-orientation-reverse-landscape"%g' \
-e 's%GTK_STOCK_ORIENTATION_REVERSE_PORTRAIT%"gtk-orientation-reverse-portrait"%g' \
-e 's%GTK_STOCK_PAGE_SETUP%"gtk-page-setup"%g' \
-e 's%GTK_STOCK_PASTE%"gtk-paste"%g' \
-e 's%GTK_STOCK_PREFERENCES%"gtk-preferences"%g' \
-e 's%GTK_STOCK_PRINT_ERROR%"gtk-print-error"%g' \
-e 's%GTK_STOCK_PRINT_PAUSED%"gtk-print-paused"%g' \
-e 's%GTK_STOCK_PRINT_PREVIEW%"gtk-print-preview"%g' \
-e 's%GTK_STOCK_PRINT_REPORT%"gtk-print-report"%g' \
-e 's%GTK_STOCK_PRINT_WARNING%"gtk-print-warning"%g' \
-e 's%GTK_STOCK_PRINT%"gtk-print"%g' \
-e 's%GTK_STOCK_PROPERTIES%"gtk-properties"%g' \
-e 's%GTK_STOCK_QUIT%"gtk-quit"%g' \
-e 's%GTK_STOCK_REDO%"gtk-redo"%g' \
-e 's%GTK_STOCK_REFRESH%"gtk-refresh"%g' \
-e 's%GTK_STOCK_REMOVE%"gtk-remove"%g' \
-e 's%GTK_STOCK_REVERT_TO_SAVED%"gtk-revert-to-saved"%g' \
-e 's%GTK_STOCK_SAVE_AS%"gtk-save-as"%g' \
-e 's%GTK_STOCK_SAVE%"gtk-save"%g' \
-e 's%GTK_STOCK_SELECT_ALL%"gtk-select-all"%g' \
-e 's%GTK_STOCK_SELECT_COLOR%"gtk-select-color"%g' \
-e 's%GTK_STOCK_SELECT_FONT%"gtk-select-font"%g' \
-e 's%GTK_STOCK_SORT_ASCENDING%"gtk-sort-ascending"%g' \
-e 's%GTK_STOCK_SORT_DESCENDING%"gtk-sort-descending"%g' \
-e 's%GTK_STOCK_SPELL_CHECK%"gtk-spell-check"%g' \
-e 's%GTK_STOCK_STOP%"gtk-stop"%g' \
-e 's%GTK_STOCK_STRIKETHROUGH%"gtk-strikethrough"%g' \
-e 's%GTK_STOCK_UNDELETE%"gtk-undelete"%g' \
-e 's%GTK_STOCK_UNDERLINE%"gtk-underline"%g' \
-e 's%GTK_STOCK_UNDO%"gtk-undo"%g' \
-e 's%GTK_STOCK_UNINDENT%"gtk-unindent"%g' \
-e 's%GTK_STOCK_YES%"gtk-yes"%g' \
-e 's%GTK_STOCK_ZOOM_100%"gtk-zoom-100"%g' \
-e 's%GTK_STOCK_ZOOM_FIT%"gtk-zoom-fit"%g' \
-e 's%GTK_STOCK_ZOOM_IN%"gtk-zoom-in"%g' \
-e 's%GTK_STOCK_ZOOM_OUT%"gtk-zoom-out"%g' \
${target}

}
#===========================================================================


#===========================================================================
# >>> GtkTable

#gtk_table_set_homogeneous%gtk_grid_set_row_homogeneous
#gtk_table_set_homogeneous%gtk_grid_set_column_homogeneous
#gtk_table_get_homogeneous%gtk_grid_get_row_homogeneous
#gtk_table_gset_homogeneous%gtk_grid_get_column_homogeneous

gtk_table ()
{
sed -i \
-e 's%GTK_TABLE%GTK_GRID%g' \
-e 's%gtk_table_new(%/*remove 3rd field*/ gtkcompat_grid_new (%g' \
-e 's%gtk_table_new (%/*remove 3rd field*/ gtkcompat_grid_new (%g' \
-e 's%gtk_table_attach_defaults(%gtkcompat_grid_attach (%g' \
-e 's%gtk_table_attach_defaults (%gtkcompat_grid_attach (%g' \
-e 's%gtk_table_attach(%/*remove last 4 fields*/ gtkcompat_grid_attach (%g' \
-e 's%gtk_table_attach (%/*remove last 4 fields*/ gtkcompat_grid_attach (%g' \
-e 's%gtk_table_set_col_spacings(%gtk_grid_set_column_spacing (%g' \
-e 's%gtk_table_set_col_spacings (%gtk_grid_set_column_spacing (%g' \
-e 's%gtk_table_set_row_spacings(%gtk_grid_set_row_spacing (%g' \
-e 's%gtk_table_set_row_spacings (%gtk_grid_set_row_spacing (%g' \
-e 's%gtk_table_get_default_col_spacing(%gtk_grid_get_column_spacing (%g' \
-e 's%gtk_table_get_default_col_spacing (%gtk_grid_get_column_spacing (%g' \
-e 's%gtk_table_get_default_row_spacing(%gtk_grid_get_row_spacing (%g' \
-e 's%gtk_table_get_default_row_spacing (%gtk_grid_get_row_spacing (%g' \
${target}

}

#===========================================================================
# >>> GtkWindow

# the grip was removed in gtk 3.14
gtk_window ()
{
sed -i \
-e '/gtk_window_set_has_resize_grip/d' \
-e '/gtk_window_get_has_resize_grip/d' \
-e '/gtk_window_resize_grip_is_visible/d' \
-e '/gtk_window_get_resize_grip_area/d' \
-e 's%gtk_window_set_opacity(GTK_WINDOW%gtk_widget_set_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_set_opacity (GTK_WINDOW%gtk_widget_set_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_set_opacity ( GTK_WINDOW%gtk_widget_set_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_set_opacity%gtk_widget_set_opacity%g' \
-e 's%gtk_window_get_opacity(GTK_WINDOW%gtk_widget_get_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_get_opacity (GTK_WINDOW%gtk_widget_get_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_get_opacity ( GTK_WINDOW%gtk_widget_get_opacity (GTK_WIDGET%g' \
-e 's%gtk_window_get_opacity%gtk_widget_get_opacity%g' \
${target}

}

#===========================================================================
# >>> GDK_KEY
gdk_key () {

echo -n "This will take a while, please wait... "

keys='GDK_0 GDK_1 GDK_2 GDK_3 GDK_4 GDK_5 GDK_6 GDK_7 GDK_8 GDK_9
GDK_a GDK_A
GDK_3270_AltCursor GDK_3270_Attn GDK_3270_BackTab GDK_3270_ChangeScreen
GDK_3270_Copy GDK_3270_CursorBlink GDK_3270_CursorSelect GDK_3270_DeleteWord
GDK_3270_Duplicate GDK_3270_Enter GDK_3270_EraseEOF GDK_3270_EraseInput
GDK_3270_ExSelect GDK_3270_FieldMark GDK_3270_Ident GDK_3270_Jump GDK_3270_KeyClick
GDK_3270_Left2 GDK_3270_PA1 GDK_3270_PA2 GDK_3270_PA3 GDK_3270_Play
GDK_3270_PrintScreen GDK_3270_Quit GDK_3270_Record GDK_3270_Reset
GDK_3270_Right2 GDK_3270_Rule GDK_3270_Setup GDK_3270_Test
GDK_aacute GDK_Aacute
GDK_abelowdot GDK_Abelowdot
GDK_abovedot
GDK_abreve GDK_Abreve
GDK_abreveacute GDK_Abreveacute
GDK_abrevebelowdot GDK_Abrevebelowdot
GDK_abrevegrave GDK_Abrevegrave
GDK_abrevehook GDK_Abrevehook
GDK_abrevetilde GDK_Abrevetilde
GDK_AccessX_Enable
GDK_AccessX_Feedback_Enable
GDK_acircumflex GDK_Acircumflex GDK_acircumflexacute GDK_Acircumflexacute
GDK_acircumflexbelowdot GDK_Acircumflexbelowdot GDK_acircumflexgrave
GDK_Acircumflexgrave GDK_acircumflexhook GDK_Acircumflexhook
GDK_acircumflextilde GDK_Acircumflextilde
GDK_acute
GDK_AddFavorite
GDK_adiaeresis GDK_Adiaeresis
GDK_ae GDK_AE
GDK_agrave GDK_Agrave
GDK_ahook GDK_Ahook
GDK_Alt_L GDK_Alt_R
GDK_amacron GDK_Amacron
GDK_ampersand
GDK_aogonek GDK_Aogonek
GDK_apostrophe
GDK_ApplicationLeft
GDK_ApplicationRight
GDK_approxeq
GDK_approximate
GDK_Arabic_0 GDK_Arabic_1 GDK_Arabic_2 GDK_Arabic_3 GDK_Arabic_4 GDK_Arabic_5
GDK_Arabic_6 GDK_Arabic_7 GDK_Arabic_8 GDK_Arabic_9
GDK_Arabic_ain GDK_Arabic_alef GDK_Arabic_alefmaksura GDK_Arabic_beh
GDK_Arabic_comma GDK_Arabic_dad GDK_Arabic_dal GDK_Arabic_damma GDK_Arabic_dammatan
GDK_Arabic_ddal GDK_Arabic_farsi_yeh GDK_Arabic_fatha GDK_Arabic_fathatan
GDK_Arabic_feh GDK_Arabic_fullstop GDK_Arabic_gaf GDK_Arabic_ghain GDK_Arabic_ha
GDK_Arabic_hah GDK_Arabic_hamza GDK_Arabic_hamza_above GDK_Arabic_hamza_below
GDK_Arabic_hamzaonalef GDK_Arabic_hamzaonwaw GDK_Arabic_hamzaonyeh
GDK_Arabic_hamzaunderalef GDK_Arabic_heh GDK_Arabic_heh_doachashmee
GDK_Arabic_heh_goal GDK_Arabic_jeem GDK_Arabic_jeh GDK_Arabic_kaf
GDK_Arabic_kasra GDK_Arabic_kasratan GDK_Arabic_keheh GDK_Arabic_khah
GDK_Arabic_lam GDK_Arabic_madda_above GDK_Arabic_maddaonalef GDK_Arabic_meem
GDK_Arabic_noon GDK_Arabic_noon_ghunna GDK_Arabic_peh GDK_Arabic_percent
GDK_Arabic_qaf GDK_Arabic_question_mark GDK_Arabic_ra GDK_Arabic_rreh
GDK_Arabic_sad GDK_Arabic_seen GDK_Arabic_semicolon GDK_Arabic_shadda
GDK_Arabic_sheen GDK_Arabic_sukun GDK_Arabic_superscript_alef GDK_Arabic_switch
GDK_Arabic_tah GDK_Arabic_tatweel GDK_Arabic_tcheh GDK_Arabic_teh
GDK_Arabic_tehmarbuta GDK_Arabic_thal GDK_Arabic_theh GDK_Arabic_tteh
GDK_Arabic_veh GDK_Arabic_waw GDK_Arabic_yeh GDK_Arabic_yeh_baree GDK_Arabic_zah
GDK_Arabic_zain
GDK_aring GDK_Aring
GDK_Armenian_accent GDK_Armenian_amanak GDK_Armenian_apostrophe GDK_Armenian_at
GDK_Armenian_AT GDK_Armenian_ayb GDK_Armenian_AYB GDK_Armenian_ben GDK_Armenian_BEN
GDK_Armenian_but GDK_Armenian_cha GDK_Armenian_CHA GDK_Armenian_da GDK_Armenian_DA
GDK_Armenian_dza GDK_Armenian_DZA GDK_Armenian_e GDK_Armenian_E GDK_Armenian_exclam
GDK_Armenian_fe GDK_Armenian_FE GDK_Armenian_full_stop GDK_Armenian_ghat
GDK_Armenian_GHAT GDK_Armenian_gim GDK_Armenian_GIM GDK_Armenian_hi GDK_Armenian_HI
GDK_Armenian_ho GDK_Armenian_HO GDK_Armenian_hyphen GDK_Armenian_ini
GDK_Armenian_INI GDK_Armenian_je GDK_Armenian_JE GDK_Armenian_ke GDK_Armenian_KE
GDK_Armenian_ken GDK_Armenian_KEN GDK_Armenian_khe GDK_Armenian_KHE
GDK_Armenian_ligature_ew GDK_Armenian_lyun GDK_Armenian_LYUN GDK_Armenian_men
GDK_Armenian_MEN GDK_Armenian_nu GDK_Armenian_NU GDK_Armenian_o GDK_Armenian_O
GDK_Armenian_paruyk GDK_Armenian_pe GDK_Armenian_PE GDK_Armenian_pyur
GDK_Armenian_PYUR GDK_Armenian_question GDK_Armenian_ra GDK_Armenian_RA
GDK_Armenian_re GDK_Armenian_RE GDK_Armenian_se GDK_Armenian_SE
GDK_Armenian_separation_mark GDK_Armenian_sha GDK_Armenian_SHA
GDK_Armenian_shesht GDK_Armenian_tche GDK_Armenian_TCHE GDK_Armenian_to
GDK_Armenian_TO GDK_Armenian_tsa GDK_Armenian_TSA GDK_Armenian_tso
GDK_Armenian_TSO GDK_Armenian_tyun GDK_Armenian_TYUN GDK_Armenian_verjaket
GDK_Armenian_vev GDK_Armenian_VEV GDK_Armenian_vo GDK_Armenian_VO
GDK_Armenian_vyun GDK_Armenian_VYUN GDK_Armenian_yech GDK_Armenian_YECH
GDK_Armenian_yentamna GDK_Armenian_za GDK_Armenian_ZA GDK_Armenian_zhe
GDK_Armenian_ZHE
GDK_asciicircum
GDK_asciitilde
GDK_asterisk
GDK_at
GDK_atilde GDK_Atilde
GDK_AudibleBell_Enable
GDK_AudioCycleTrack
GDK_AudioForward
GDK_AudioLowerVolume
GDK_AudioMedia
GDK_AudioMute
GDK_AudioNext
GDK_AudioPause
GDK_AudioPlay
GDK_AudioPrev
GDK_AudioRaiseVolume
GDK_AudioRandomPlay
GDK_AudioRecord
GDK_AudioRepeat
GDK_AudioRewind
GDK_AudioStop
GDK_Away
GDK_b GDK_B
GDK_babovedot GDK_Babovedot
GDK_Back
GDK_BackForward
GDK_backslash
GDK_BackSpace
GDK_ballotcross
GDK_bar
GDK_Battery
GDK_because
GDK_Begin
GDK_blank
GDK_Blue
GDK_Bluetooth
GDK_Book
GDK_botintegral
GDK_botleftparens
GDK_botleftsqbracket
GDK_botleftsummation
GDK_botrightparens
GDK_botrightsqbracket
GDK_botrightsummation
GDK_bott
GDK_botvertsummationconnector
GDK_BounceKeys_Enable
GDK_braceleft
GDK_braceright
GDK_bracketleft
GDK_bracketright
GDK_braille_blank
GDK_braille_dot_1 GDK_braille_dot_2 GDK_braille_dot_3 GDK_braille_dot_4
GDK_braille_dot_5 GDK_braille_dot_6 GDK_braille_dot_7 GDK_braille_dot_8
GDK_braille_dot_9 GDK_braille_dot_10 GDK_braille_dots_1 GDK_braille_dots_12
GDK_braille_dots_123 GDK_braille_dots_1234 GDK_braille_dots_12345
GDK_braille_dots_123456 GDK_braille_dots_1234567 GDK_braille_dots_12345678
GDK_braille_dots_1234568 GDK_braille_dots_123457 GDK_braille_dots_1234578
GDK_braille_dots_123458 GDK_braille_dots_12346 GDK_braille_dots_123467
GDK_braille_dots_1234678 GDK_braille_dots_123468 GDK_braille_dots_12347
GDK_braille_dots_123478 GDK_braille_dots_12348 GDK_braille_dots_1235
GDK_braille_dots_12356 GDK_braille_dots_123567 GDK_braille_dots_1235678
GDK_braille_dots_123568 GDK_braille_dots_12357 GDK_braille_dots_123578
GDK_braille_dots_12358 GDK_braille_dots_1236 GDK_braille_dots_12367
GDK_braille_dots_123678 GDK_braille_dots_12368 GDK_braille_dots_1237
GDK_braille_dots_12378 GDK_braille_dots_1238 GDK_braille_dots_124
GDK_braille_dots_1245 GDK_braille_dots_12456 GDK_braille_dots_124567
GDK_braille_dots_1245678 GDK_braille_dots_124568 GDK_braille_dots_12457
GDK_braille_dots_124578 GDK_braille_dots_12458 GDK_braille_dots_1246
GDK_braille_dots_12467 GDK_braille_dots_124678 GDK_braille_dots_12468
GDK_braille_dots_1247 GDK_braille_dots_12478 GDK_braille_dots_1248
GDK_braille_dots_125 GDK_braille_dots_1256 GDK_braille_dots_12567
GDK_braille_dots_125678 GDK_braille_dots_12568 GDK_braille_dots_1257
GDK_braille_dots_12578 GDK_braille_dots_1258 GDK_braille_dots_126
GDK_braille_dots_1267 GDK_braille_dots_12678 GDK_braille_dots_1268
GDK_braille_dots_127 GDK_braille_dots_1278 GDK_braille_dots_128
GDK_braille_dots_13 GDK_braille_dots_134 GDK_braille_dots_1345
GDK_braille_dots_13456 GDK_braille_dots_134567 GDK_braille_dots_1345678
GDK_braille_dots_134568 GDK_braille_dots_13457 GDK_braille_dots_134578
GDK_braille_dots_13458 GDK_braille_dots_1346 GDK_braille_dots_13467
GDK_braille_dots_134678 GDK_braille_dots_13468 GDK_braille_dots_1347
GDK_braille_dots_13478 GDK_braille_dots_1348 GDK_braille_dots_135
GDK_braille_dots_1356 GDK_braille_dots_13567 GDK_braille_dots_135678
GDK_braille_dots_13568 GDK_braille_dots_1357 GDK_braille_dots_13578
GDK_braille_dots_1358 GDK_braille_dots_136 GDK_braille_dots_1367
GDK_braille_dots_13678 GDK_braille_dots_1368 GDK_braille_dots_137
GDK_braille_dots_1378 GDK_braille_dots_138 GDK_braille_dots_14
GDK_braille_dots_145 GDK_braille_dots_1456 GDK_braille_dots_14567
GDK_braille_dots_145678 GDK_braille_dots_14568 GDK_braille_dots_1457
GDK_braille_dots_14578 GDK_braille_dots_1458 GDK_braille_dots_146
GDK_braille_dots_1467 GDK_braille_dots_14678 GDK_braille_dots_1468
GDK_braille_dots_147 GDK_braille_dots_1478 GDK_braille_dots_148
GDK_braille_dots_15 GDK_braille_dots_156 GDK_braille_dots_1567
GDK_braille_dots_15678 GDK_braille_dots_1568 GDK_braille_dots_157
GDK_braille_dots_1578 GDK_braille_dots_158 GDK_braille_dots_16
GDK_braille_dots_167 GDK_braille_dots_1678 GDK_braille_dots_168
GDK_braille_dots_17 GDK_braille_dots_178 GDK_braille_dots_18
GDK_braille_dots_2 GDK_braille_dots_23 GDK_braille_dots_234
GDK_braille_dots_2345 GDK_braille_dots_23456 GDK_braille_dots_234567
GDK_braille_dots_2345678 GDK_braille_dots_234568 GDK_braille_dots_23457
GDK_braille_dots_234578 GDK_braille_dots_23458 GDK_braille_dots_2346 
GDK_braille_dots_23467 GDK_braille_dots_234678 GDK_braille_dots_23468
GDK_braille_dots_2347 GDK_braille_dots_23478 GDK_braille_dots_2348
GDK_braille_dots_235 GDK_braille_dots_2356 GDK_braille_dots_23567
GDK_braille_dots_235678 GDK_braille_dots_23568 GDK_braille_dots_2357
GDK_braille_dots_23578 GDK_braille_dots_2358 GDK_braille_dots_236
GDK_braille_dots_2367 GDK_braille_dots_23678 GDK_braille_dots_2368
GDK_braille_dots_237 GDK_braille_dots_2378 GDK_braille_dots_238
GDK_braille_dots_24 GDK_braille_dots_245 GDK_braille_dots_2456
GDK_braille_dots_24567 GDK_braille_dots_245678 GDK_braille_dots_24568
GDK_braille_dots_2457 GDK_braille_dots_24578 GDK_braille_dots_2458
GDK_braille_dots_246 GDK_braille_dots_2467 GDK_braille_dots_24678
GDK_braille_dots_2468 GDK_braille_dots_247 GDK_braille_dots_2478
GDK_braille_dots_248 GDK_braille_dots_25 GDK_braille_dots_256
GDK_braille_dots_2567 GDK_braille_dots_25678 GDK_braille_dots_2568
GDK_braille_dots_257 GDK_braille_dots_2578 GDK_braille_dots_258
GDK_braille_dots_26 GDK_braille_dots_267 GDK_braille_dots_2678
GDK_braille_dots_268 GDK_braille_dots_27 GDK_braille_dots_278
GDK_braille_dots_28 GDK_braille_dots_3 GDK_braille_dots_34
GDK_braille_dots_345 GDK_braille_dots_3456 GDK_braille_dots_34567
GDK_braille_dots_345678 GDK_braille_dots_34568 GDK_braille_dots_3457
GDK_braille_dots_34578 GDK_braille_dots_3458 GDK_braille_dots_346
GDK_braille_dots_3467 GDK_braille_dots_34678 GDK_braille_dots_3468
GDK_braille_dots_347 GDK_braille_dots_3478 GDK_braille_dots_348
GDK_braille_dots_35 GDK_braille_dots_356 GDK_braille_dots_3567
GDK_braille_dots_35678 GDK_braille_dots_3568 GDK_braille_dots_357
GDK_braille_dots_3578 GDK_braille_dots_358 GDK_braille_dots_36
GDK_braille_dots_367 GDK_braille_dots_3678 GDK_braille_dots_368
GDK_braille_dots_37 GDK_braille_dots_378 GDK_braille_dots_38
GDK_braille_dots_4 GDK_braille_dots_45 GDK_braille_dots_456
GDK_braille_dots_4567 GDK_braille_dots_45678 GDK_braille_dots_4568
GDK_braille_dots_457 GDK_braille_dots_4578 GDK_braille_dots_458
GDK_braille_dots_46 GDK_braille_dots_467 GDK_braille_dots_4678
GDK_braille_dots_468 GDK_braille_dots_47 GDK_braille_dots_478 
GDK_braille_dots_48 GDK_braille_dots_5 GDK_braille_dots_56
GDK_braille_dots_567 GDK_braille_dots_5678 GDK_braille_dots_568
GDK_braille_dots_57 GDK_braille_dots_578 GDK_braille_dots_58
GDK_braille_dots_6 GDK_braille_dots_67 GDK_braille_dots_678
GDK_braille_dots_68 GDK_braille_dots_7 GDK_braille_dots_78 GDK_braille_dots_8
GDK_Break
GDK_breve
GDK_BrightnessAdjust
GDK_brokenbar
GDK_Byelorussian_shortu
GDK_Byelorussian_SHORTU
GDK_c GDK_C
GDK_cabovedot GDK_Cabovedot
GDK_cacute GDK_Cacute
GDK_Calculator
GDK_Calendar
GDK_Cancel
GDK_Caps_Lock
GDK_careof
GDK_caret
GDK_caron
GDK_ccaron GDK_Ccaron
GDK_ccedilla GDK_Ccedilla
GDK_ccircumflex GDK_Ccircumflex
GDK_CD
GDK_cedilla
GDK_cent
GDK_checkerboard
GDK_checkmark
GDK_circle
GDK_Clear
GDK_ClearGrab
GDK_Close
GDK_club GDK_Codeinput
GDK_colon GDK_ColonSign
GDK_comma
GDK_Community
GDK_containsas
GDK_ContrastAdjust
GDK_Control_L GDK_Control_R
GDK_Copy
GDK_copyright
GDK_cr
GDK_crossinglines
GDK_CruzeiroSign
GDK_cuberoot
GDK_currency
GDK_cursor
GDK_Cut
GDK_CycleAngle
GDK_Cyrillic_a GDK_Cyrillic_A GDK_Cyrillic_be GDK_Cyrillic_BE GDK_Cyrillic_che
GDK_Cyrillic_CHE GDK_Cyrillic_che_descender GDK_Cyrillic_CHE_descender
GDK_Cyrillic_che_vertstroke GDK_Cyrillic_CHE_vertstroke GDK_Cyrillic_de
GDK_Cyrillic_DE GDK_Cyrillic_dzhe GDK_Cyrillic_DZHE GDK_Cyrillic_e
GDK_Cyrillic_E GDK_Cyrillic_ef GDK_Cyrillic_EF GDK_Cyrillic_el
GDK_Cyrillic_EL GDK_Cyrillic_em GDK_Cyrillic_EM GDK_Cyrillic_en
GDK_Cyrillic_EN GDK_Cyrillic_en_descender GDK_Cyrillic_EN_descender
GDK_Cyrillic_er GDK_Cyrillic_ER GDK_Cyrillic_es GDK_Cyrillic_ES
GDK_Cyrillic_ghe GDK_Cyrillic_GHE GDK_Cyrillic_ghe_bar GDK_Cyrillic_GHE_bar
GDK_Cyrillic_ha GDK_Cyrillic_HA GDK_Cyrillic_ha_descender
GDK_Cyrillic_HA_descender GDK_Cyrillic_hardsign GDK_Cyrillic_HARDSIGN
GDK_Cyrillic_i GDK_Cyrillic_I GDK_Cyrillic_ie GDK_Cyrillic_IE
GDK_Cyrillic_i_macron GDK_Cyrillic_I_macron GDK_Cyrillic_io GDK_Cyrillic_IO
GDK_Cyrillic_je GDK_Cyrillic_JE GDK_Cyrillic_ka GDK_Cyrillic_KA
GDK_Cyrillic_ka_descender GDK_Cyrillic_KA_descender GDK_Cyrillic_ka_vertstroke
GDK_Cyrillic_KA_vertstroke GDK_Cyrillic_lje GDK_Cyrillic_LJE GDK_Cyrillic_nje
GDK_Cyrillic_NJE GDK_Cyrillic_o GDK_Cyrillic_O GDK_Cyrillic_o_bar
GDK_Cyrillic_O_bar GDK_Cyrillic_pe GDK_Cyrillic_PE GDK_Cyrillic_schwa
GDK_Cyrillic_SCHWA GDK_Cyrillic_sha GDK_Cyrillic_SHA GDK_Cyrillic_shcha
GDK_Cyrillic_SHCHA GDK_Cyrillic_shha GDK_Cyrillic_SHHA GDK_Cyrillic_shorti
GDK_Cyrillic_SHORTI GDK_Cyrillic_softsign GDK_Cyrillic_SOFTSIGN
GDK_Cyrillic_te GDK_Cyrillic_TE GDK_Cyrillic_tse GDK_Cyrillic_TSE GDK_Cyrillic_u
GDK_Cyrillic_U GDK_Cyrillic_u_macron GDK_Cyrillic_U_macron GDK_Cyrillic_u_straight
GDK_Cyrillic_U_straight GDK_Cyrillic_u_straight_bar GDK_Cyrillic_U_straight_bar
GDK_Cyrillic_ve GDK_Cyrillic_VE GDK_Cyrillic_ya GDK_Cyrillic_YA GDK_Cyrillic_yeru
GDK_Cyrillic_YERU GDK_Cyrillic_yu GDK_Cyrillic_YU GDK_Cyrillic_ze GDK_Cyrillic_ZE
GDK_Cyrillic_zhe GDK_Cyrillic_ZHE GDK_Cyrillic_zhe_descender
GDK_Cyrillic_ZHE_descender
GDK_d GDK_D
GDK_dabovedot GDK_Dabovedot
GDK_dagger
GDK_dcaron GDK_Dcaron
GDK_dead_a GDK_dead_A GDK_dead_abovecomma GDK_dead_abovedot
GDK_dead_abovereversedcomma GDK_dead_abovering GDK_dead_acute
GDK_dead_belowbreve GDK_dead_belowcircumflex GDK_dead_belowcomma
GDK_dead_belowdiaeresis GDK_dead_belowdot GDK_dead_belowmacron
GDK_dead_belowring GDK_dead_belowtilde GDK_dead_breve GDK_dead_capital_schwa
GDK_dead_caron GDK_dead_cedilla GDK_dead_circumflex GDK_dead_currency
GDK_dead_dasia GDK_dead_diaeresis GDK_dead_doubleacute GDK_dead_doublegrave
GDK_dead_e GDK_dead_E GDK_dead_grave GDK_dead_hook GDK_dead_horn GDK_dead_i
GDK_dead_I GDK_dead_invertedbreve GDK_dead_iota GDK_dead_macron GDK_dead_o
GDK_dead_O GDK_dead_ogonek GDK_dead_perispomeni GDK_dead_psili
GDK_dead_semivoiced_sound GDK_dead_small_schwa GDK_dead_stroke
GDK_dead_tilde GDK_dead_u GDK_dead_U GDK_dead_voiced_sound
GDK_decimalpoint
GDK_degree
GDK_Delete
GDK_diaeresis
GDK_diamond
GDK_digitspace
GDK_dintegral
GDK_Display
GDK_division
GDK_Documents
GDK_dollar
GDK_DongSign
GDK_DOS
GDK_doubbaselinedot
GDK_doubleacute
GDK_doubledagger
GDK_doublelowquotemark
GDK_Down GDK_downarrow GDK_downcaret GDK_downshoe GDK_downstile GDK_downtack
GDK_dstroke GDK_Dstroke
GDK_e GDK_E
GDK_eabovedot GDK_Eabovedot
GDK_eacute GDK_Eacute
GDK_ebelowdot GDK_Ebelowdot
GDK_ecaron GDK_Ecaron
GDK_ecircumflex GDK_Ecircumflex GDK_ecircumflexacute GDK_Ecircumflexacute
GDK_ecircumflexbelowdot GDK_Ecircumflexbelowdot GDK_ecircumflexgrave
GDK_Ecircumflexgrave GDK_ecircumflexhook GDK_Ecircumflexhook
GDK_ecircumflextilde GDK_Ecircumflextilde
GDK_EcuSign
GDK_ediaeresis GDK_Ediaeresis
GDK_egrave GDK_Egrave
GDK_ehook GDK_Ehook
GDK_eightsubscript GDK_eightsuperior
GDK_Eisu_Shift
GDK_Eisu_toggle
GDK_Eject
GDK_elementof
GDK_ellipsis
GDK_em3space GDK_em4space
GDK_emacron GDK_Emacron
GDK_emdash
GDK_emfilledcircle GDK_emfilledrect
GDK_emopencircle
GDK_emopenrectangle
GDK_emptyset
GDK_emspace
GDK_End
GDK_endash
GDK_enfilledcircbullet
GDK_enfilledsqbullet
GDK_eng GDK_ENG
GDK_enopencircbullet
GDK_enopensquarebullet
GDK_enspace
GDK_eogonek GDK_Eogonek
GDK_equal
GDK_Escape
GDK_eth GDK_Eth GDK_ETH
GDK_etilde GDK_Etilde
GDK_EuroSign
GDK_Excel
GDK_exclam GDK_exclamdown
GDK_Execute
GDK_Explorer
GDK_f GDK_F
GDK_F1 GDK_F2 GDK_F3 GDK_F4 GDK_F5 GDK_F6 GDK_F7 GDK_F8 GDK_F9
GDK_F10 GDK_F11 GDK_F12 GDK_F13 GDK_F14 GDK_F15 GDK_F16 GDK_F17 GDK_F18 GDK_F19
GDK_F20 GDK_F21 GDK_F22 GDK_F23 GDK_F24 GDK_F25 GDK_F26 GDK_F27 GDK_F28 GDK_F29
GDK_F30 GDK_F31 GDK_F32 GDK_F33 GDK_F34 GDK_F35
GDK_fabovedot GDK_Fabovedot
GDK_Farsi_0 GDK_Farsi_1 GDK_Farsi_2 GDK_Farsi_3 GDK_Farsi_4 GDK_Farsi_5
GDK_Farsi_6 GDK_Farsi_7 GDK_Farsi_8 GDK_Farsi_9 GDK_Farsi_yeh
GDK_Favorites
GDK_femalesymbol
GDK_ff
GDK_FFrancSign
GDK_figdash
GDK_filledlefttribullet GDK_filledrectbullet
GDK_filledrighttribullet GDK_filledtribulletdown GDK_filledtribulletup
GDK_Finance
GDK_Find
GDK_First_Virtual_Screen
GDK_fiveeighths
GDK_fivesixths
GDK_fivesubscript
GDK_fivesuperior
GDK_Forward
GDK_fourfifths GDK_foursubscript GDK_foursuperior GDK_fourthroot
GDK_FrameBack
GDK_FrameForward
GDK_function
GDK_g GDK_G
GDK_gabovedot GDK_Gabovedot
GDK_Game
GDK_gbreve GDK_Gbreve
GDK_gcaron GDK_Gcaron
GDK_gcedilla GDK_Gcedilla
GDK_gcircumflex GDK_Gcircumflex
GDK_Georgian_an GDK_Georgian_ban GDK_Georgian_can GDK_Georgian_char
GDK_Georgian_cil GDK_Georgian_don GDK_Georgian_en GDK_Georgian_fi
GDK_Georgian_gan GDK_Georgian_ghan GDK_Georgian_hae GDK_Georgian_har
GDK_Georgian_he GDK_Georgian_hie GDK_Georgian_hoe GDK_Georgian_in
GDK_Georgian_jhan GDK_Georgian_jil GDK_Georgian_kan GDK_Georgian_khar
GDK_Georgian_las GDK_Georgian_man GDK_Georgian_nar GDK_Georgian_on
GDK_Georgian_par GDK_Georgian_phar GDK_Georgian_qar GDK_Georgian_rae
GDK_Georgian_san GDK_Georgian_shin GDK_Georgian_tan GDK_Georgian_tar
GDK_Georgian_un GDK_Georgian_vin GDK_Georgian_we GDK_Georgian_xan
GDK_Georgian_zen GDK_Georgian_zhar GDK_Georgian_chin
GDK_Go
GDK_grave
GDK_greater
GDK_greaterthanequal
GDK_Greek_accentdieresis GDK_Greek_alpha GDK_Greek_ALPHA
GDK_Greek_alphaaccent GDK_Greek_ALPHAaccent GDK_Greek_beta
GDK_Greek_BETA GDK_Greek_chi GDK_Greek_CHI GDK_Greek_delta
GDK_Greek_DELTA GDK_Greek_epsilon GDK_Greek_EPSILON
GDK_Greek_epsilonaccent GDK_Greek_EPSILONaccent GDK_Greek_eta
GDK_Greek_ETA GDK_Greek_etaaccent GDK_Greek_ETAaccent
GDK_Greek_finalsmallsigma GDK_Greek_gamma GDK_Greek_GAMMA
GDK_Greek_horizbar GDK_Greek_iota GDK_Greek_IOTA GDK_Greek_iotaaccent
GDK_Greek_IOTAaccent GDK_Greek_iotaaccentdieresis GDK_Greek_IOTAdiaeresis
GDK_Greek_iotadieresis GDK_Greek_IOTAdieresis GDK_Greek_kappa
GDK_Greek_KAPPA GDK_Greek_lambda GDK_Greek_LAMBDA GDK_Greek_lamda GDK_Greek_LAMDA
GDK_Greek_mu GDK_Greek_MU GDK_Greek_nu GDK_Greek_NU GDK_Greek_omega
GDK_Greek_OMEGA GDK_Greek_omegaaccent GDK_Greek_OMEGAaccent GDK_Greek_omicron
GDK_Greek_OMICRON GDK_Greek_omicronaccent GDK_Greek_OMICRONaccent GDK_Greek_phi
GDK_Greek_PHI GDK_Greek_pi GDK_Greek_PI GDK_Greek_psi GDK_Greek_PSI GDK_Greek_rho
GDK_Greek_RHO GDK_Greek_sigma GDK_Greek_SIGMA GDK_Greek_switch GDK_Greek_tau
GDK_Greek_TAU GDK_Greek_theta GDK_Greek_THETA GDK_Greek_upsilon GDK_Greek_UPSILON
GDK_Greek_upsilonaccent GDK_Greek_UPSILONaccent GDK_Greek_upsilonaccentdieresis
GDK_Greek_upsilondieresis GDK_Greek_UPSILONdieresis GDK_Greek_xi GDK_Greek_XI
GDK_Greek_zeta GDK_Greek_ZETA
GDK_Green
GDK_guillemotleft GDK_guillemotright
GDK_h GDK_H
GDK_hairspace
GDK_Hangul GDK_Hangul_A GDK_Hangul_AE GDK_Hangul_AraeA GDK_Hangul_AraeAE
GDK_Hangul_Banja GDK_Hangul_Cieuc GDK_Hangul_Codeinput GDK_Hangul_Dikeud
GDK_Hangul_E GDK_Hangul_End GDK_Hangul_EO GDK_Hangul_EU GDK_Hangul_Hanja
GDK_Hangul_Hieuh GDK_Hangul_I GDK_Hangul_Ieung GDK_Hangul_Jamo
GDK_Hangul_J_Cieuc GDK_Hangul_J_Dikeud GDK_Hangul_Jeonja GDK_Hangul_J_Hieuh
GDK_Hangul_Jieuj GDK_Hangul_J_Ieung GDK_Hangul_J_Jieuj GDK_Hangul_J_Khieuq
GDK_Hangul_J_Kiyeog GDK_Hangul_J_KiyeogSios GDK_Hangul_J_KkogjiDalrinIeung
GDK_Hangul_J_Mieum GDK_Hangul_J_Nieun GDK_Hangul_J_NieunHieuh
GDK_Hangul_J_NieunJieuj GDK_Hangul_J_PanSios GDK_Hangul_J_Phieuf
GDK_Hangul_J_Pieub GDK_Hangul_J_PieubSios GDK_Hangul_J_Rieul
GDK_Hangul_J_RieulHieuh GDK_Hangul_J_RieulKiyeog GDK_Hangul_J_RieulMieum
GDK_Hangul_J_RieulPhieuf GDK_Hangul_J_RieulPieub GDK_Hangul_J_RieulSios
GDK_Hangul_J_RieulTieut GDK_Hangul_J_Sios GDK_Hangul_J_SsangKiyeog
GDK_Hangul_J_SsangSios GDK_Hangul_J_Tieut GDK_Hangul_J_YeorinHieuh
GDK_Hangul_Khieuq GDK_Hangul_Kiyeog GDK_Hangul_KiyeogSios
GDK_Hangul_KkogjiDalrinIeung GDK_Hangul_Mieum GDK_Hangul_MultipleCandidate
GDK_Hangul_Nieun GDK_Hangul_NieunHieuh GDK_Hangul_NieunJieuj GDK_Hangul_O
GDK_Hangul_OE GDK_Hangul_PanSios GDK_Hangul_Phieuf GDK_Hangul_Pieub
GDK_Hangul_PieubSios GDK_Hangul_PostHanja GDK_Hangul_PreHanja
GDK_Hangul_PreviousCandidate GDK_Hangul_Rieul GDK_Hangul_RieulHieuh
GDK_Hangul_RieulKiyeog GDK_Hangul_RieulMieum GDK_Hangul_RieulPhieuf
GDK_Hangul_RieulPieub GDK_Hangul_RieulSios GDK_Hangul_RieulTieut
GDK_Hangul_RieulYeorinHieuh GDK_Hangul_Romaja GDK_Hangul_SingleCandidate
GDK_Hangul_Sios GDK_Hangul_Special GDK_Hangul_SsangDikeud
GDK_Hangul_SsangJieuj GDK_Hangul_SsangKiyeog GDK_Hangul_SsangPieub
GDK_Hangul_SsangSios GDK_Hangul_Start GDK_Hangul_SunkyeongeumMieum
GDK_Hangul_SunkyeongeumPhieuf GDK_Hangul_SunkyeongeumPieub GDK_Hangul_switch
GDK_Hangul_Tieut GDK_Hangul_U GDK_Hangul_WA GDK_Hangul_WAE GDK_Hangul_WE
GDK_Hangul_WEO GDK_Hangul_WI GDK_Hangul_YA GDK_Hangul_YAE GDK_Hangul_YE
GDK_Hangul_YEO GDK_Hangul_YeorinHieuh GDK_Hangul_YI GDK_Hangul_YO GDK_Hangul_YU
GDK_Hankaku
GDK_hcircumflex GDK_Hcircumflex
GDK_heart
GDK_hebrew_aleph GDK_hebrew_ayin GDK_hebrew_bet GDK_hebrew_beth
GDK_hebrew_chet GDK_hebrew_dalet GDK_hebrew_daleth GDK_hebrew_doublelowline
GDK_hebrew_finalkaph GDK_hebrew_finalmem GDK_hebrew_finalnun
GDK_hebrew_finalpe GDK_hebrew_finalzade GDK_hebrew_finalzadi
GDK_hebrew_gimel GDK_hebrew_gimmel GDK_hebrew_he GDK_hebrew_het
GDK_hebrew_kaph GDK_hebrew_kuf GDK_hebrew_lamed GDK_hebrew_mem
GDK_hebrew_nun GDK_hebrew_pe GDK_hebrew_qoph GDK_hebrew_resh
GDK_hebrew_samech GDK_hebrew_samekh GDK_hebrew_shin GDK_Hebrew_switch
GDK_hebrew_taf GDK_hebrew_taw GDK_hebrew_tet GDK_hebrew_teth GDK_hebrew_waw
GDK_hebrew_yod GDK_hebrew_zade GDK_hebrew_zadi GDK_hebrew_zain GDK_hebrew_zayin
GDK_Help
GDK_Henkan
GDK_Henkan_Mode
GDK_hexagram
GDK_Hibernate
GDK_Hiragana GDK_Hiragana_Katakana
GDK_History
GDK_Home
GDK_HomePage
GDK_horizconnector
GDK_horizlinescan1 GDK_horizlinescan3 GDK_horizlinescan5 GDK_horizlinescan7 GDK_horizlinescan9
GDK_HotLinks
GDK_hstroke GDK_Hstroke
GDK_ht
GDK_Hyper_L GDK_Hyper_R
GDK_hyphen
GDK_i GDK_I
GDK_Iabovedot
GDK_iacute GDK_Iacute
GDK_ibelowdot GDK_Ibelowdot
GDK_ibreve GDK_Ibreve
GDK_icircumflex GDK_Icircumflex
GDK_identical
GDK_idiaeresis GDK_Idiaeresis
GDK_idotless
GDK_ifonlyif
GDK_igrave GDK_Igrave
GDK_ihook GDK_Ihook
GDK_imacron GDK_Imacron
GDK_implies
GDK_includedin
GDK_includes
GDK_infinity
GDK_Insert
GDK_integral
GDK_intersection
GDK_iogonek GDK_Iogonek
GDK_ISO_Center_Object GDK_ISO_Continuous_Underline GDK_ISO_Discontinuous_Underline
GDK_ISO_Emphasize GDK_ISO_Enter GDK_ISO_Fast_Cursor_Down GDK_ISO_Fast_Cursor_Left
GDK_ISO_Fast_Cursor_Right GDK_ISO_Fast_Cursor_Up GDK_ISO_First_Group
GDK_ISO_First_Group_Lock GDK_ISO_Group_Latch GDK_ISO_Group_Lock GDK_ISO_Group_Shift
GDK_ISO_Last_Group GDK_ISO_Last_Group_Lock GDK_ISO_Left_Tab GDK_ISO_Level2_Latch
GDK_ISO_Level3_Latch GDK_ISO_Level3_Lock GDK_ISO_Level3_Shift GDK_ISO_Level5_Latch
GDK_ISO_Level5_Lock GDK_ISO_Level5_Shift GDK_ISO_Lock GDK_ISO_Move_Line_Down
GDK_ISO_Move_Line_Up GDK_ISO_Next_Group GDK_ISO_Next_Group_Lock GDK_ISO_Partial_Line_Down
GDK_ISO_Partial_Line_Up GDK_ISO_Partial_Space_Left GDK_ISO_Partial_Space_Right
GDK_ISO_Prev_Group GDK_ISO_Prev_Group_Lock GDK_ISO_Release_Both_Margins
GDK_ISO_Release_Margin_Left GDK_ISO_Release_Margin_Right GDK_ISO_Set_Margin_Left GDK_ISO_Set_Margin_Right
GDK_itilde GDK_Itilde
GDK_iTouch
GDK_j GDK_J
GDK_jcircumflex GDK_Jcircumflex
GDK_jot
GDK_k GDK_K
GDK_kana_a GDK_kana_A GDK_kana_CHI GDK_kana_closingbracket GDK_kana_comma
GDK_kana_conjunctive GDK_kana_e GDK_kana_E GDK_kana_FU GDK_kana_fullstop
GDK_kana_HA GDK_kana_HE GDK_kana_HI GDK_kana_HO GDK_kana_HU GDK_kana_i
GDK_kana_I GDK_kana_KA GDK_kana_KE GDK_kana_KI GDK_kana_KO GDK_kana_KU
GDK_Kana_Lock GDK_kana_MA GDK_kana_ME GDK_kana_MI GDK_kana_middledot
GDK_kana_MO GDK_kana_MU GDK_kana_N GDK_kana_NA GDK_kana_NE GDK_kana_NI
GDK_kana_NO GDK_kana_NU GDK_kana_o GDK_kana_O GDK_kana_openingbracket
GDK_kana_RA GDK_kana_RE GDK_kana_RI GDK_kana_RO GDK_kana_RU GDK_kana_SA
GDK_kana_SE GDK_kana_SHI GDK_Kana_Shift GDK_kana_SO GDK_kana_SU
GDK_kana_switch GDK_kana_TA GDK_kana_TE GDK_kana_TI GDK_kana_TO
GDK_kana_tsu GDK_kana_TSU GDK_kana_tu GDK_kana_TU GDK_kana_u GDK_kana_U
GDK_kana_WA GDK_kana_WO GDK_kana_ya GDK_kana_YA GDK_kana_yo GDK_kana_YO
GDK_kana_yu GDK_kana_YU
GDK_Kanji GDK_Kanji_Bangou GDK_kappa GDK_Katakana
GDK_KbdBrightnessDown
GDK_KbdBrightnessUp
GDK_KbdLightOnOff
GDK_kcedilla GDK_Kcedilla
GDK_Korean_Won
GDK_KP_0 GDK_KP_1 GDK_KP_2 GDK_KP_3 GDK_KP_4 GDK_KP_5 GDK_KP_6 GDK_KP_7 GDK_KP_8 GDK_KP_9
GDK_KP_Add GDK_KP_Begin GDK_KP_Decimal GDK_KP_Delete GDK_KP_Divide
GDK_KP_Down GDK_KP_End GDK_KP_Enter GDK_KP_Equal
GDK_KP_F1 GDK_KP_F2 GDK_KP_F3 GDK_KP_F4
GDK_KP_Home GDK_KP_Insert GDK_KP_Left GDK_KP_Multiply
GDK_KP_Next GDK_KP_Page_Down GDK_KP_Page_Up GDK_KP_Prior
GDK_KP_Right GDK_KP_Separator GDK_KP_Space GDK_KP_Subtract
GDK_KP_Tab GDK_KP_Up
GDK_kra
GDK_l GDK_L
GDK_L1 GDK_L2 GDK_L3 GDK_L4 GDK_L5 GDK_L6 GDK_L7 GDK_L8 GDK_L9 GDK_L10
GDK_lacute GDK_Lacute
GDK_Last_Virtual_Screen
GDK_latincross
GDK_Launch0 GDK_Launch1 GDK_Launch2 GDK_Launch3 GDK_Launch4 GDK_Launch5
GDK_Launch6 GDK_Launch7 GDK_Launch8 GDK_Launch9
GDK_LaunchA GDK_LaunchB GDK_LaunchC GDK_LaunchD GDK_LaunchE GDK_LaunchF
GDK_lbelowdot GDK_Lbelowdot
GDK_lcaron GDK_Lcaron
GDK_lcedilla GDK_Lcedilla
GDK_Left
GDK_leftanglebracket
GDK_leftarrow
GDK_leftcaret
GDK_leftdoublequotemark
GDK_leftmiddlecurlybrace
GDK_leftopentriangle
GDK_leftpointer
GDK_leftradical
GDK_leftshoe
GDK_leftsinglequotemark
GDK_leftt
GDK_lefttack
GDK_less
GDK_lessthanequal
GDK_lf
GDK_LightBulb
GDK_Linefeed
GDK_LiraSign
GDK_logicaland
GDK_logicalor
GDK_LogOff
GDK_lowleftcorner
GDK_lowrightcorner
GDK_lstroke
GDK_Lstroke
GDK_m GDK_M
GDK_mabovedot GDK_Mabovedot
GDK_Macedonia_dse GDK_Macedonia_DSE GDK_Macedonia_gje
GDK_Macedonia_GJE GDK_Macedonia_kje GDK_Macedonia_KJE
GDK_macron
GDK_Mae_Koho
GDK_Mail
GDK_MailForward
GDK_malesymbol
GDK_maltesecross
GDK_marker
GDK_Market
GDK_masculine
GDK_Massyo
GDK_Meeting
GDK_Memo
GDK_Menu
GDK_MenuKB
GDK_MenuPB
GDK_Messenger
GDK_Meta_L
GDK_Meta_R
GDK_MillSign
GDK_minus
GDK_minutes
GDK_ModeLock
GDK_Mode_switch
GDK_MonBrightnessDown
GDK_MonBrightnessUp
GDK_MouseKeys_Accel_Enable
GDK_MouseKeys_Enable
GDK_mu
GDK_Muhenkan
GDK_Multi_key
GDK_MultipleCandidate
GDK_multiply
GDK_Music
GDK_musicalflat
GDK_musicalsharp
GDK_MyComputer
GDK_MySites
GDK_n GDK_N
GDK_nabla
GDK_nacute GDK_Nacute
GDK_NairaSign
GDK_ncaron GDK_Ncaron
GDK_ncedilla GDK_Ncedilla
GDK_New
GDK_News
GDK_NewSheqelSign
GDK_Next
GDK_Next_Virtual_Screen
GDK_Next_VMode
GDK_ninesubscript
GDK_ninesuperior
GDK_nl
GDK_nobreakspace
GDK_notapproxeq
GDK_notelementof
GDK_notequal
GDK_notidentical
GDK_notsign
GDK_ntilde GDK_Ntilde
GDK_numbersign
GDK_numerosign
GDK_Num_Lock
GDK_o GDK_O
GDK_oacute GDK_Oacute
GDK_obarred GDK_Obarred
GDK_obelowdot GDK_Obelowdot
GDK_ocaron GDK_Ocaron
GDK_ocircumflex GDK_Ocircumflex GDK_ocircumflexacute GDK_Ocircumflexacute
GDK_ocircumflexbelowdot GDK_Ocircumflexbelowdot GDK_ocircumflexgrave
GDK_Ocircumflexgrave GDK_ocircumflexhook GDK_Ocircumflexhook
GDK_ocircumflextilde GDK_Ocircumflextilde
GDK_odiaeresis GDK_Odiaeresis
GDK_odoubleacute GDK_Odoubleacute
GDK_oe GDK_OE
GDK_OfficeHome
GDK_ogonek
GDK_ograve GDK_Ograve
GDK_ohook GDK_Ohook
GDK_ohorn GDK_Ohorn
GDK_ohornacute GDK_Ohornacute
GDK_ohornbelowdot GDK_Ohornbelowdot
GDK_ohorngrave GDK_Ohorngrave
GDK_ohornhook GDK_Ohornhook
GDK_ohorntilde GDK_Ohorntilde
GDK_omacron GDK_Omacron
GDK_oneeighth
GDK_onefifth
GDK_onehalf
GDK_onequarter
GDK_onesixth
GDK_onesubscript
GDK_onesuperior
GDK_onethird
GDK_ooblique GDK_Ooblique
GDK_Open
GDK_openrectbullet
GDK_openstar
GDK_opentribulletdown
GDK_opentribulletup
GDK_OpenURL
GDK_Option
GDK_ordfeminine
GDK_oslash GDK_Oslash
GDK_otilde GDK_Otilde
GDK_overbar
GDK_Overlay1_Enable GDK_Overlay2_Enable
GDK_overline
GDK_p GDK_P
GDK_pabovedot GDK_Pabovedot
GDK_Page_Down
GDK_Page_Up
GDK_paragraph
GDK_parenleft
GDK_parenright
GDK_partdifferential
GDK_partialderivative
GDK_Paste
GDK_Pause
GDK_percent
GDK_period
GDK_periodcentered
GDK_PesetaSign
GDK_Phone
GDK_phonographcopyright
GDK_Pictures
GDK_plus
GDK_plusminus
GDK_Pointer_Accelerate
GDK_Pointer_Button1 GDK_Pointer_Button2 GDK_Pointer_Button3
GDK_Pointer_Button4 GDK_Pointer_Button5 GDK_Pointer_Button_Dflt
GDK_Pointer_DblClick1 GDK_Pointer_DblClick2 GDK_Pointer_DblClick3
GDK_Pointer_DblClick4 GDK_Pointer_DblClick5 GDK_Pointer_DblClick_Dflt
GDK_Pointer_DfltBtnNext
GDK_Pointer_DfltBtnPrev
GDK_Pointer_Down
GDK_Pointer_DownLeft
GDK_Pointer_DownRight
GDK_Pointer_Drag1 GDK_Pointer_Drag2 GDK_Pointer_Drag3 GDK_Pointer_Drag4
GDK_Pointer_Drag5 GDK_Pointer_Drag_Dflt
GDK_Pointer_EnableKeys GDK_Pointer_Left GDK_Pointer_Right
GDK_Pointer_Up GDK_Pointer_UpLeft GDK_Pointer_UpRight
GDK_PowerDown GDK_PowerOff
GDK_prescription
GDK_PreviousCandidate
GDK_Prev_Virtual_Screen
GDK_Prev_VMode
GDK_Print GDK_Prior
GDK_prolongedsound
GDK_punctspace
GDK_q GDK_Q
GDK_quad
GDK_question
GDK_questiondown
GDK_quotedbl GDK_quoteleft GDK_quoteright
GDK_r GDK_R
GDK_R1 GDK_R2 GDK_R3 GDK_R4 GDK_R5 GDK_R6 GDK_R7 GDK_R8 GDK_R9
GDK_R10 GDK_R11 GDK_R12 GDK_R13 GDK_R14 GDK_R15
GDK_racute GDK_Racute
GDK_radical
GDK_rcaron GDK_Rcaron
GDK_rcedilla GDK_Rcedilla
GDK_Red
GDK_Redo
GDK_Refresh
GDK_registered
GDK_Reload
GDK_RepeatKeys_Enable
GDK_Reply
GDK_Return
GDK_Right
GDK_rightanglebracket GDK_rightarrow GDK_rightcaret GDK_rightdoublequotemark
GDK_rightmiddlecurlybrace GDK_rightmiddlesummation GDK_rightopentriangle
GDK_rightpointer GDK_rightshoe GDK_rightsinglequotemark GDK_rightt GDK_righttack
GDK_RockerDown
GDK_RockerEnter
GDK_RockerUp
GDK_Romaji
GDK_RotateWindows
GDK_RotationKB
GDK_RotationPB
GDK_RupeeSign
GDK_s GDK_S
GDK_sabovedot GDK_Sabovedot
GDK_sacute GDK_Sacute
GDK_Save
GDK_scaron GDK_Scaron
GDK_scedilla GDK_Scedilla
GDK_schwa GDK_SCHWA
GDK_scircumflex GDK_Scircumflex
GDK_ScreenSaver
GDK_script_switch
GDK_ScrollClick GDK_ScrollDown GDK_Scroll_Lock GDK_ScrollUp
GDK_Search
GDK_seconds
GDK_section
GDK_Select
GDK_SelectButton
GDK_semicolon
GDK_semivoicedsound
GDK_Send
GDK_Serbian_dje GDK_Serbian_DJE GDK_Serbian_dze GDK_Serbian_DZE
GDK_Serbian_je GDK_Serbian_JE GDK_Serbian_lje GDK_Serbian_LJE
GDK_Serbian_nje GDK_Serbian_NJE GDK_Serbian_tshe GDK_Serbian_TSHE
GDK_seveneighths
GDK_sevensubscript
GDK_sevensuperior
GDK_Shift_L
GDK_Shift_Lock
GDK_Shift_R
GDK_Shop
GDK_signaturemark
GDK_signifblank
GDK_similarequal
GDK_SingleCandidate
GDK_singlelowquotemark
GDK_sixsubscript
GDK_sixsuperior
GDK_slash
GDK_Sleep
GDK_SlowKeys_Enable
GDK_soliddiamond
GDK_space
GDK_Spell
GDK_SplitScreen
GDK_squareroot
GDK_ssharp
GDK_Standby
GDK_Start
GDK_sterling
GDK_StickyKeys_Enable
GDK_Stop
GDK_stricteq
GDK_Subtitle
GDK_Super_L GDK_Super_R
GDK_Support
GDK_Suspend
GDK_Switch_VT_1 GDK_Switch_VT_2 GDK_Switch_VT_3 GDK_Switch_VT_4
GDK_Switch_VT_5 GDK_Switch_VT_6 GDK_Switch_VT_7 GDK_Switch_VT_8
GDK_Switch_VT_9 GDK_Switch_VT_10 GDK_Switch_VT_11 GDK_Switch_VT_12
GDK_Sys_Req
GDK_t GDK_T
GDK_Tab
GDK_tabovedot GDK_Tabovedot
GDK_TaskPane
GDK_tcaron GDK_Tcaron
GDK_tcedilla GDK_Tcedilla
GDK_telephone
GDK_telephonerecorder
GDK_Terminal
GDK_Terminate_Server
GDK_Thai_baht GDK_Thai_bobaimai GDK_Thai_chochan GDK_Thai_chochang
GDK_Thai_choching GDK_Thai_chochoe GDK_Thai_dochada GDK_Thai_dodek
GDK_Thai_fofa GDK_Thai_fofan GDK_Thai_hohip GDK_Thai_honokhuk
GDK_Thai_khokhai GDK_Thai_khokhon GDK_Thai_khokhuat GDK_Thai_khokhwai
GDK_Thai_khorakhang GDK_Thai_kokai GDK_Thai_lakkhangyao GDK_Thai_lekchet
GDK_Thai_lekha GDK_Thai_lekhok GDK_Thai_lekkao GDK_Thai_leknung
GDK_Thai_lekpaet GDK_Thai_leksam GDK_Thai_leksi GDK_Thai_leksong
GDK_Thai_leksun GDK_Thai_lochula GDK_Thai_loling GDK_Thai_lu
GDK_Thai_maichattawa GDK_Thai_maiek GDK_Thai_maihanakat
GDK_Thai_maihanakat_maitho GDK_Thai_maitaikhu GDK_Thai_maitho
GDK_Thai_maitri GDK_Thai_maiyamok GDK_Thai_moma GDK_Thai_ngongu
GDK_Thai_nikhahit GDK_Thai_nonen GDK_Thai_nonu GDK_Thai_oang
GDK_Thai_paiyannoi GDK_Thai_phinthu GDK_Thai_phophan GDK_Thai_phophung
GDK_Thai_phosamphao GDK_Thai_popla GDK_Thai_rorua GDK_Thai_ru GDK_Thai_saraa
GDK_Thai_saraaa GDK_Thai_saraae GDK_Thai_saraaimaimalai GDK_Thai_saraaimaimuan
GDK_Thai_saraam GDK_Thai_sarae GDK_Thai_sarai GDK_Thai_saraii GDK_Thai_sarao
GDK_Thai_sarau GDK_Thai_saraue GDK_Thai_sarauee GDK_Thai_sarauu GDK_Thai_sorusi
GDK_Thai_sosala GDK_Thai_soso GDK_Thai_sosua GDK_Thai_thanthakhat
GDK_Thai_thonangmontho GDK_Thai_thophuthao GDK_Thai_thothahan GDK_Thai_thothan
GDK_Thai_thothong GDK_Thai_thothung GDK_Thai_topatak GDK_Thai_totao
GDK_Thai_wowaen GDK_Thai_yoyak GDK_Thai_yoying
GDK_therefore
GDK_thinspace
GDK_thorn GDK_Thorn GDK_THORN
GDK_threeeighths
GDK_threefifths
GDK_threequarters
GDK_threesubscript
GDK_threesuperior
GDK_Time
GDK_tintegral
GDK_ToDoList
GDK_Tools
GDK_topintegral
GDK_topleftparens
GDK_topleftradical
GDK_topleftsqbracket
GDK_topleftsummation
GDK_TopMenu
GDK_toprightparens
GDK_toprightsqbracket
GDK_toprightsummation
GDK_topt
GDK_topvertsummationconnector
GDK_TouchpadToggle
GDK_Touroku
GDK_trademark
GDK_trademarkincircle
GDK_Travel
GDK_tslash
GDK_Tslash
GDK_twofifths
GDK_twosubscript
GDK_twosuperior
GDK_twothirds
GDK_u GDK_U
GDK_uacute GDK_Uacute
GDK_ubelowdot GDK_Ubelowdot
GDK_ubreve GDK_Ubreve
GDK_ucircumflex GDK_Ucircumflex
GDK_udiaeresis GDK_Udiaeresis
GDK_udoubleacute GDK_Udoubleacute
GDK_ugrave GDK_Ugrave
GDK_uhook GDK_Uhook
GDK_uhorn GDK_Uhorn
GDK_uhornacute GDK_Uhornacute
GDK_uhornbelowdot GDK_Uhornbelowdot
GDK_uhorngrave GDK_Uhorngrave
GDK_uhornhook GDK_Uhornhook
GDK_uhorntilde GDK_Uhorntilde
GDK_Ukrainian_ghe_with_upturn GDK_Ukrainian_GHE_WITH_UPTURN
GDK_Ukrainian_i GDK_Ukrainian_I GDK_Ukrainian_ie GDK_Ukrainian_IE
GDK_Ukrainian_yi GDK_Ukrainian_YI GDK_Ukranian_i GDK_Ukranian_I
GDK_Ukranian_je GDK_Ukranian_JE GDK_Ukranian_yi GDK_Ukranian_YI
GDK_umacron GDK_Umacron
GDK_underbar GDK_underscore
GDK_Undo
GDK_Ungrab
GDK_union
GDK_uogonek GDK_Uogonek
GDK_Up
GDK_uparrow
GDK_upcaret
GDK_upleftcorner
GDK_uprightcorner
GDK_upshoe
GDK_upstile
GDK_uptack
GDK_uring GDK_Uring
GDK_User1KB GDK_User2KB GDK_UserPB
GDK_utilde GDK_Utilde
GDK_UWB
GDK_v GDK_V
GDK_variation
GDK_VendorHome
GDK_vertbar
GDK_vertconnector
GDK_Video
GDK_View
GDK_voicedsound
GDK_VoidSymbol
GDK_vt
GDK_w GDK_W
GDK_wacute GDK_Wacute
GDK_WakeUp
GDK_wcircumflex GDK_Wcircumflex
GDK_wdiaeresis GDK_Wdiaeresis
GDK_WebCam
GDK_wgrave GDK_Wgrave
GDK_WheelButton
GDK_WindowClear
GDK_WLAN
GDK_WonSign
GDK_Word
GDK_WWW
GDK_x GDK_X
GDK_xabovedot GDK_Xabovedot
GDK_Xfer
GDK_y GDK_Y
GDK_yacute GDK_Yacute
GDK_ybelowdot GDK_Ybelowdot
GDK_ycircumflex GDK_Ycircumflex
GDK_ydiaeresis GDK_Ydiaeresis
GDK_Yellow
GDK_yen
GDK_ygrave GDK_Ygrave
GDK_yhook GDK_Yhook
GDK_ytilde GDK_Ytilde
GDK_z GDK_Z
GDK_zabovedot GDK_Zabovedot
GDK_zacute GDK_Zacute
GDK_zcaron GDK_Zcaron
GDK_Zenkaku GDK_Zenkaku_Hankaku
GDK_Zen_Koho
GDK_zerosubscript GDK_zerosuperior
GDK_ZoomIn GDK_ZoomOut
GDK_zstroke GDK_Zstroke'

for key in ${keys}
do
	new_key=${key/_/_KEY_} # GDK_KP_0 -> GDK_KEY_KP_0
	if grep -q "$key" ${target} ; then
		sed -i \
			-e "s%${key} %${new_key} %g" \
			-e "s%${key}	%${new_key}	%g" \
			-e "s%${key})%${new_key})%g" \
			-e "s%${key},%${new_key},%g" \
			-e "s%${key}]%${new_key}]%g" \
			-e "s%${key}}%${new_key}}%g" \
			-e "s%${key}:%${new_key}:%g" \
			-e "s% ${key}$% ${new_key}%g" \
			-e "s%	${key}$%	${new_key}%g" \
		${target}
	fi
done

echo

}
#===========================================================================

usage() {
	echo "Usage:"
	echo "  $0 <directory> <gtk_x>"
	echo "  $0 <directory> gdk_key"
	echo "  $0 <directory> gtk_table"
	echo "  $0 <directory> all"
	echo
	exit
}

if ! [ -d "$1" ] ; then
	usage
fi

dir=$1
set_target
shift

case $1 in
	gtk_box)        gtk_box        ;;
	gtk_button_box) gtk_button_box ;;
	gtk_font_chooser) gtk_font_chooser  ;;
	gtk_scale)      gtk_scale      ;;
	gtk_separator)  gtk_separator  ;;
	gtk_scrollbar)  gtk_scrollbar  ;;
	gtk_paned)      gtk_paned      ;;
	gtk_stock)      gtk_stock      ;;
	gtk_table)      gtk_table      ;;
	gtk_window)     gtk_window     ;;
	gdk_key)        gdk_key        ;;
	misc)           misc           ;;
	all)
		gtk_box
		gtk_button_box
		gtk_font_chooser
		gtk_scale
		gtk_separator
		gtk_scrollbar
		gtk_paned
		gtk_stock
		#gtk_table
		gtk_window
		misc
		#gdk_key
		;;
	*)
		usage
		;;
esac
