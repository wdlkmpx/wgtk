/*
 * Public Domain
 */ 

#include "w_gtk.h"

#include <string.h>
#include <ctype.h>

#if GTK_CHECK_VERSION(2,0,0)

void w_gtk_text_view_append (GtkWidget *tw, const char *tag, char *str)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tw));
    GtkTextIter end_iter;
    gtk_text_buffer_get_end_iter (buf, &end_iter);
    //gtk_text_buffer_get_iter_at_offset (buffer, &end_iter, -1);
    if (tag) {
        gtk_text_buffer_insert_with_tags_by_name (buf, &end_iter, str, -1, tag, NULL);
    } else {
        gtk_text_buffer_insert (buf, &end_iter, str, -1);
    }
}


void w_gtk_text_view_scroll_to_end (GtkWidget *tw)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tw));
    GtkTextIter iter;
    gtk_text_buffer_get_end_iter (buf, &iter);
    gtk_text_buffer_move_mark_by_name (buf, "end", &iter);
    gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW (tw),
                                        gtk_text_buffer_get_mark (buf, "end"));
}


void w_gtk_text_view_scroll_to_start (GtkWidget *tw)
{
    GtkTextBuffer *buf = gtk_text_view_get_buffer (GTK_TEXT_VIEW (tw));
    GtkTextIter iter;
    gtk_text_buffer_get_start_iter (buf, &iter);
    //gtk_text_view_scroll_to_iter (GTK_TEXT_VIEW(tw), &iter, TRUE, TRUE, 0, 0);
    gtk_text_buffer_move_mark_by_name (buf, "start", &iter);
    gtk_text_view_scroll_mark_onscreen (GTK_TEXT_VIEW (tw),
                                        gtk_text_buffer_get_mark (buf, "start"));
}


/******************************************
 * GPL2
    copyright   : (C)2007 Paul Schuurmans
    email       : paul.schuurmans@home.nl
*******************************************/

void EditorClear (GtkTextView *tw)
{
    GtkTextIter startiter, enditer;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_bounds(buffer, &startiter, &enditer);
    gtk_text_buffer_delete(buffer, &startiter, &enditer);
}


void EditorDeleteLine (GtkTextView *tw, int linenum)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    int startpos, endpos;
    GtkTextIter startiter, enditer;
    EditorGetLineOffsets(tw, linenum, &startpos, &endpos);
    gtk_text_buffer_get_iter_at_offset(buffer, &startiter, startpos);
    gtk_text_buffer_get_iter_at_offset(buffer, &enditer, endpos+1);
    gtk_text_buffer_delete(buffer, &startiter, &enditer);
}


void EditorGetCurIter (GtkTextView *tw, GtkTextIter *iter)
{
    GtkTextMark *mark;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    mark = gtk_text_buffer_get_insert(buffer);
    gtk_text_buffer_get_iter_at_mark(buffer, iter, mark);
}


char *EditorGetCurLine (GtkTextView *tw)
{
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    gint curline, linelen;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start, &end);
    curline = EditorGetCurLineNum(tw);
    gtk_text_buffer_get_iter_at_line(buffer, &start, curline);
    linelen = EditorGetCurLineLen(tw);
    gtk_text_iter_set_line_offset(&end, linelen-1);
    return(gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}


int EditorGetCurLineLen (GtkTextView *tw)
{
    GtkTextBuffer *buffer;
    GtkTextIter start_iter, end_iter;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);
    return(gtk_text_iter_get_chars_in_line(&start_iter));
}


int EditorGetCurLineNum (GtkTextView *tw)
{
    GtkTextBuffer *buffer;
    GtkTextIter start_iter, end_iter;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);
    return(gtk_text_iter_get_line(&start_iter));
}


char *EditorGetCurWord (GtkTextView *tw, GtkTextIter *start_iter, GtkTextIter *end_iter)
{
    GtkTextBuffer *buffer;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, start_iter, end_iter);
    if(gtk_text_iter_starts_word(start_iter)) {
        gtk_text_iter_forward_word_end(end_iter);
    }
    else if(gtk_text_iter_ends_word(start_iter)) {
        gtk_text_iter_backward_word_start(start_iter);
    }
    else if(gtk_text_iter_inside_word(start_iter)) {
        gtk_text_iter_backward_word_start(start_iter);
        gtk_text_iter_forward_word_end(end_iter);
    }
    return(gtk_text_buffer_get_text(buffer, start_iter, end_iter, FALSE));
}


char *EditorGetLine (GtkTextView *tw, int linenum)
{
    GtkTextIter start_iter, end_iter;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_bounds(buffer, &start_iter, &end_iter);
    gtk_text_iter_set_line(&start_iter, linenum);
    gtk_text_iter_set_line(&end_iter, linenum);
    if(!gtk_text_iter_ends_line(&end_iter))  gtk_text_iter_forward_to_line_end(&end_iter);
    return(gtk_text_iter_get_text(&start_iter, &end_iter));
}


void EditorGetLineIters (GtkTextView *tw, int linenum, GtkTextIter *start, GtkTextIter *end)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_bounds(buffer, start, end);
    gtk_text_iter_set_line(start, linenum);
    gtk_text_iter_set_line(end, linenum);
    gtk_text_iter_forward_to_line_end(end);
}


void EditorGetLineOffsets (GtkTextView *tw, int linenum, int *start, int *end)
{
    GtkTextIter start_iter, end_iter;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_bounds(buffer, &start_iter, &end_iter);
    gtk_text_iter_set_line(&start_iter, linenum);
    *start = gtk_text_iter_get_offset(&start_iter);
    gtk_text_iter_set_line(&end_iter, linenum);
    gtk_text_iter_forward_to_line_end(&end_iter);
    *end = gtk_text_iter_get_offset(&end_iter);
}


int EditorGetNumLines (GtkTextView *tw)
{
    return(gtk_text_buffer_get_line_count(gtk_text_view_get_buffer(tw)));
}


int EditorGetSelectedLineNums (GtkTextView *tw, int *startline, int *endline)
{
    GtkTextBuffer *buffer;
    GtkTextIter start_iter, end_iter;
    int linecount = 0;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);
    *startline = gtk_text_iter_get_line(&start_iter);
    *endline = gtk_text_iter_get_line(&end_iter);
    if(gtk_text_iter_starts_line(&end_iter))  (*endline)--;
    linecount = (*endline - *startline) + 1;
    return(linecount);
}


char *EditorGetSelectedText (GtkTextView *tw, int *start, int *end, int *selcount)
{
    GtkTextBuffer *buffer;
    GtkTextIter start_iter, end_iter;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);
    *start = gtk_text_iter_get_offset(&start_iter);
    *end = gtk_text_iter_get_offset(&end_iter);
    *selcount = *end - *start;
    return(gtk_text_buffer_get_text(buffer, &start_iter, &end_iter, FALSE));
}


gboolean EditorHasSelection (GtkTextView *tw)
{
    GtkTextIter start_iter, end_iter;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    return(gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter));
}


int EditorInsertLine (GtkTextView *tw, int linenum, char *txt)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    GtkTextIter iter;
    gtk_text_buffer_get_iter_at_line_offset(buffer, &iter, linenum, 0);
    gtk_text_buffer_place_cursor(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, txt, strlen(txt));
    return(gtk_text_iter_get_offset(&iter));
}


int EditorMoveToLineEnd (GtkTextView *tw)
{
    GtkTextBuffer *buffer;
    GtkTextIter start_iter, end_iter;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);
    gtk_text_iter_set_line_offset(&end_iter, strlen(EditorGetCurLine(tw)));
    gtk_text_buffer_place_cursor(buffer, &end_iter);
    return(gtk_text_iter_get_offset(&end_iter));
}


int EditorMoveToLineStart (GtkTextView *tw)
{
    GtkTextBuffer *buffer;
    GtkTextIter start_iter, end_iter;
    gint pos;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);
    pos = gtk_text_iter_get_line_offset(&start_iter);
    gtk_text_iter_backward_chars(&start_iter, pos);
    gtk_text_buffer_place_cursor(buffer, &start_iter);
    return(gtk_text_iter_get_offset(&start_iter));
}


gboolean EditorSelectLine (GtkTextView *tw, int linenum, gboolean del_eol)
{
    GtkTextBuffer *buffer;
    GtkTextIter start_iter, end_iter;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_bounds(buffer, &start_iter, &end_iter);
    gtk_text_iter_set_line(&start_iter, linenum);
    gtk_text_iter_set_line(&end_iter, linenum);
    if(gtk_text_iter_starts_line(&start_iter) && gtk_text_iter_ends_line(&start_iter))  return(FALSE);
    if(del_eol)  gtk_text_iter_forward_line(&end_iter);
    gtk_text_iter_forward_to_line_end(&end_iter);
    /*int start =*/ gtk_text_iter_get_offset(&start_iter);
    /*int end   =*/ gtk_text_iter_get_offset(&end_iter);
    gtk_text_buffer_select_range(buffer, &start_iter, &end_iter);
    return(TRUE);
}


gboolean EditorSelectOneChar (GtkTextView *tw)
{
    GtkTextIter start_iter, end_iter;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);
    end_iter = start_iter;
    if(gtk_text_iter_forward_char(&end_iter) == FALSE)  return(FALSE);
    gtk_text_buffer_select_range(buffer, &start_iter, &end_iter);
    return(TRUE);
}


void EditorSetSelectedText (GtkTextView *tw, char *text)
{
    GtkTextBuffer *buffer;
    GtkTextIter start_iter, end_iter;
    buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_selection_bounds(buffer, &start_iter, &end_iter);
    gtk_text_buffer_delete(buffer,  &start_iter, &end_iter);
    gtk_text_buffer_insert(buffer, &start_iter, text, -1);
}

/*
void EditorTextCen (GtkTextView *tw, int center, int width, int usetab)
{
    GtkTextIter iter;
    char line[1024], temp[1024];
    int i, startline, endline, linecnt, linelen;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    linecnt = EditorGetSelectedLineNums(tw, &startline, &endline);
    if(!linecnt) {
        startline = endline = EditorGetCurLineNum(tw);
        linecnt = 1;
    }
    for(i = startline; i < startline + linecnt; i++) {
        strcpy(line, EditorGetLine(tw, i));
        linelen = strlen(line);
        if(!linelen || (width <= linelen) || (!center && line[0] != ' ')) {
            EditorGetCurIter(tw, &iter);
            if(gtk_text_iter_forward_line(&iter)) {
                gtk_text_buffer_place_cursor(buffer, &iter);
            }
            continue;
        }
        StrCvt(line, RLWHITE | RTWHITE);
        if(center) {
            strcpy(temp, line);
            StrJustify(line, temp, ' ', width, JUST_CENTER);
            if(usetab)  StrTabfy(line, 8);
            StrCvt(line, RTWHITE);
        }
        EditorDeleteLine(tw, i);
        strcat(line, "\n");
        EditorInsertLine(tw, i, line);
    }
}
*/

void EditorTextChangeCase (GtkTextView *tw)
{
    gboolean ok;
    int i;
    char *text;
    int start, end, selcount;
    if(!EditorHasSelection(tw)) {
        ok = EditorSelectOneChar (tw);
        if(!ok)  return;
    }
    text = EditorGetSelectedText(tw, &start, &end, &selcount);
    // toggle case
    for(i=0; text[i]; i++)  
    {
        if(text[i]>=65 && text[i]<=90)
            text[i]+=32;
        else if(text[i]>=97 && text[i]<=122)
            text[i]-=32;
    }
    EditorSetSelectedText(tw, text);
    g_free(text);
}


void EditorTextConcatenateLines (GtkTextView *tw)
{
    gchar *txt;
    char *line;
    int len, ndx;
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    if(!gtk_text_buffer_get_selection_bounds(buffer, &start, &end))  return;
    txt = gtk_text_iter_get_text(&start, &end);
    len = strlen(txt);
    line = g_malloc0(len+2);
    strcpy(line, txt);
    for(ndx = 0; ndx < strlen(line); ndx++) {
        if(line[ndx] == '\n')  line[ndx] = ' ';
    }
    EditorSetSelectedText(tw, line);
    g_free(line);
}


void EditorTextSplitLine (GtkTextView *tw, int linelen)
{
    GtkTextIter start, end;
    char *txt, *line;
    int len, ndx;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    EditorGetCurIter(tw, &start);
    if(gtk_text_iter_is_end(&start))  return;
    txt = EditorGetCurLine(tw);
    len = strlen(txt);
    if(len <= linelen)  return;
    line = g_malloc0(len+2);
    strcpy(line, txt);
    for(ndx = linelen; ndx; ndx--) {
        if(line[ndx] == ' ') {
            line[ndx] = '\n';
            break;
        }
    }
    ndx = EditorGetCurLineNum(tw);
    EditorDeleteLine(tw, ndx);
    strcat(line, "\n");
    EditorInsertLine(tw, ndx, line);
    g_free(line);
    EditorGetLineIters(tw, ndx+1, &start, &end);
    gtk_text_buffer_place_cursor(buffer, &start);
}


void EditorTextTitleCase (GtkTextView *tw)
{
    char *text;
    int i, start, end, selcount;
    text = EditorGetSelectedText(tw, &start, &end, &selcount);
    if(!selcount) { g_free(text); return; }
    text[0] = toupper(text[0]);
    for(i = 1; i < selcount; i++) {
        switch(text[i-1]) {
          case ' ': case '-': case '\r': case '\n': case '.': case '(': case '[':
          case '\"':  case '{': case '/':
            text[i] = toupper(text[i]);
            break;
          default:
            text[i] = tolower(text[i]);
            break;
        }
    }
    EditorSetSelectedText(tw, text);
    g_free(text);
}


gchar *TextViewGetText (GtkTextView *tw)
{
    GtkTextIter start, end;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    return(gtk_text_buffer_get_text(buffer, &start, &end, FALSE));
}


void TextViewSetText (GtkTextView *tw, gchar *text)
{
    gchar *utf8str;
    gssize len;
    gsize bytes_read, bytes_written;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    len = strlen(text);
    utf8str = g_locale_to_utf8(text, len, &bytes_read, &bytes_written, NULL);
    gtk_text_buffer_set_text(buffer, utf8str, -1);
    g_free(utf8str);
}

/*
void EditorTextTabify (GtkTextView *tw, int tabify, int tabsize)
{
    GtkTextIter iter;
    char line[1024], temp[1024];
    int i, startline, endline, linecnt, linelen;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(tw);
    linecnt = EditorGetSelectedLineNums(tw, &startline, &endline);
    if(!linecnt) {
        startline = endline = EditorGetCurLineNum(tw);
        linecnt = 1;
    }
    for(i = startline; i < startline + linecnt; i++) {
        strcpy(line, EditorGetLine(tw, i));
        linelen = strlen(line);
        if(!linelen) {
            EditorGetCurIter(tw, &iter);
            if(gtk_text_iter_forward_line(&iter)) {
                gtk_text_buffer_place_cursor(buffer, &iter);
            }
            continue;
        }
        if(tabify) {
            StrTabfy(line, tabsize);
        }
        else {
            strcpy(temp, line);
            StrExpand(line, temp, tabsize, linelen * tabsize);
        }
        EditorDeleteLine(tw, i);
        strcat(line, "\n");
        EditorInsertLine(tw, i, line);
    }
}
*/

#endif
