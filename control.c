/****************** Processes **************************************/

static void
kv_zoom(kv_viewport_t * VP, double new_zrx, double new_zry, double posx, double posy) {
  posx -= VP->x;
  posy -= VP->y;
  double deltax = posx / VP->zoom_ratio_x * new_zrx - posx;
  double deltay = posy / VP->zoom_ratio_y * new_zry - posy;
  VP->x -= deltax;
  VP->y -= deltay;
  VP->zoom_ratio_x = new_zrx;
  VP->zoom_ratio_y = new_zry;
  kv_viewport_queue_draw(VP);
}

static void
kv_zoomfit_hor_(kv_viewport_t * VP, double * zrx, double * zry, double * myx, double * myy) {
  double w = VP->vpw;
  double zoom_ratio = 1.0;
  double x = KV_MARGIN_WHEN_ZOOMFIT;
  double y = KV_MARGIN_WHEN_ZOOMFIT;
  double d1, d2;
  if (GS->align_start)
    d1 = KV_TIMELINE_START_X + kv_scale_down_span(GS->TS->t_span);
  else
    d1 = KV_TIMELINE_START_X + kv_scale_down_span(GS->TS->end_t - GS->TS->start_t);
  d2 = w - 2 * KV_MARGIN_WHEN_ZOOMFIT;
  if (d1 > d2)
    zoom_ratio = d2 / d1;
  /*
  double h = VP->vph;
  double dh = 2 * KV_RADIUS + GS->TS->n * (2 * KV_RADIUS + KV_GAP_BETWEEN_TIMELINES);
  if (h > dh * zoom_ratio)
    y += (h - dh * zoom_ratio) * 0.4;
  */
  *zrx = *zry  = zoom_ratio;
  *myx = x;
  *myy = y;
}

_unused_ static void
kv_zoomfit_hor(kv_viewport_t * VP) {
  kv_zoomfit_hor_(VP, &VP->zoom_ratio_x, &VP->zoom_ratio_y, &VP->x, &VP->y);
  kv_viewport_queue_draw(VP);
}

static void
kv_zoomfit_ver_(kv_viewport_t * VP, double * zrx, double * zry, double * myx, double * myy) {
  double h = VP->vph;
  double zoom_ratio = 1.0;
  double x = KV_MARGIN_WHEN_ZOOMFIT;
  double y = KV_MARGIN_WHEN_ZOOMFIT;
  double d1, d2;
  d1 = 2 * KV_RADIUS + GS->TS->n * (2 * KV_RADIUS + KV_GAP_BETWEEN_TIMELINES);
  d2 = h - 2 * KV_MARGIN_WHEN_ZOOMFIT;
  if (d1 > d2)
    zoom_ratio = d2 / d1;
  *zrx = *zry = zoom_ratio;
  *myx = x;
  *myy = y;
}

_unused_ static void
kv_zoomfit_ver(kv_viewport_t * VP) {
  kv_zoomfit_ver_(VP, &VP->zoom_ratio_x, &VP->zoom_ratio_y, &VP->x, &VP->y);
  kv_viewport_queue_draw(VP);
}

void
kv_zoomfit_full(kv_viewport_t * VP) {
  double h_zrx, h_zry, h_x, h_y;
  kv_zoomfit_hor_(VP, &h_zrx, &h_zry, &h_x, &h_y);
  double v_zrx, v_zry, v_x, v_y;
  kv_zoomfit_ver_(VP, &v_zrx, &v_zry, &v_x, &v_y);
  if (v_zrx < h_zrx) {
    h_zrx = v_zrx;
    h_zry = v_zry;
    h_x = v_x;
    h_y = v_y;
  }
  VP->zoom_ratio_x = h_zrx;
  VP->zoom_ratio_y = h_zry;
  VP->x = h_x;
  VP->y = h_y;
  kv_viewport_queue_draw(VP);
}

void
kv_toggle_toolbox(int enable) {
  if (GS->toolbox_shown == enable) return;
  kv_gui_t * GUI = GS->GUI;
  GtkWidget * sidebox = kv_gui_get_toolbox_sidebox(GUI);
  GS->toolbox_shown = enable;
  if (enable) {
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(GUI->ontoolbar.toolbox), TRUE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(GUI->onmenubar.toolbox), TRUE);
    gtk_box_pack_start(GTK_BOX(GS->GUI->left_sidebar), sidebox, FALSE, FALSE, 0);
  } else {
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(GUI->ontoolbar.toolbox), FALSE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(GUI->onmenubar.toolbox), FALSE);
    gtk_container_remove(GTK_CONTAINER(GS->GUI->left_sidebar), sidebox);
  }
}

void
kv_toggle_infobox(int enable) {
  if (GS->infobox_shown == enable) return;
  kv_gui_t * GUI = GS->GUI;
  GtkWidget * sidebox = kv_gui_get_infobox_sidebox(GUI);
  GS->infobox_shown = enable;
  if (enable) {
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(GUI->ontoolbar.infobox), TRUE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(GUI->onmenubar.infobox), TRUE);
    gtk_box_pack_start(GTK_BOX(GS->GUI->left_sidebar), sidebox, FALSE, FALSE, 0);
  } else {
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(GUI->ontoolbar.infobox), FALSE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(GUI->onmenubar.infobox), FALSE);
    gtk_container_remove(GTK_CONTAINER(GS->GUI->left_sidebar), sidebox);
  }
}

void
kv_toggle_replaybox(int enable) {
  if (GS->replaybox_shown == enable) return;
  kv_gui_t * GUI = GS->GUI;
  GtkWidget * sidebox = kv_gui_get_replaybox_sidebox(GUI);
  GS->replaybox_shown = enable;
  if (enable) {
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(GUI->ontoolbar.replaybox), TRUE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(GUI->onmenubar.replaybox), TRUE);
    gtk_box_pack_start(GTK_BOX(GS->GUI->left_sidebar), sidebox, FALSE, FALSE, 0);
  } else {
    gtk_toggle_tool_button_set_active(GTK_TOGGLE_TOOL_BUTTON(GUI->ontoolbar.replaybox), FALSE);
    gtk_check_menu_item_set_active(GTK_CHECK_MENU_ITEM(GUI->onmenubar.replaybox), FALSE);
    gtk_container_remove(GTK_CONTAINER(GS->GUI->left_sidebar), sidebox);
  }
}

static kv_timeline_box_t *
kv_find_box_(kv_timeline_t * tl, kv_timeline_box_t * box, double x, double y, double ratio) {
  if (!box || x < box->x)
    return NULL;
  else if (x > box->x + box->w)
    return kv_find_box_(tl, box->next, x, y, ratio);
  else if (x > box->x && x < box->x + box->w) {
    double ychild = tl->y;
    double hchild = tl->h;    
    double slip = hchild * (1 - ratio);
    ychild += slip / 2.0;
    hchild -= slip;
    kv_timeline_box_t * b = NULL;
    if (y > ychild && y < ychild + hchild)
      b = kv_find_box_(tl, box->child, x, y, ratio * KV_NESTED_DECREASE_RATE);
    if (b)
      return b;
    return box;
  }
  return NULL;
}

static kv_timeline_box_t *
kv_find_box(kv_timeline_set_t * TL, double x, double y) {
  kv_timeline_t * tl = TL->head;
  while (tl) {
    if (y >= tl->y && y <= tl->y + tl->h) {
      kv_timeline_box_t * b = kv_find_box_(tl, tl->box, x, y, 1.0);
      if (b)
        return b;
    }
    tl = tl->next;
  }
  return NULL;
}

int
kv_count_ranks_with_boxes(double time) {
  time += GS->TS->start_t;
  int count = 0;
  kv_timeline_t * tl = GS->TL->head;
  while (tl) {
    kv_timeline_box_t * b = tl->box;
    while (b && b->end_e->t < time)
      b = b->next;
    if (b && b->end_e->t >= time && b->start_e->t <= time)
      count++;
    tl = tl->next;
  }
  return count;
}

void
kv_replay_set_current_time(double value) {
  if (value < 0 || value > GS->TS->t_span || value == GS->current_time)
    return;
  GS->current_time = value;
  gtk_range_set_value(GTK_RANGE(GS->GUI->replaybox.scale), value);
  char s[KV_STRING_LENGTH];
  sprintf(s, "%.0lf", value);
  gtk_entry_set_text(GTK_ENTRY(GS->GUI->replaybox.entry), s);
  kv_viewport_queue_draw(GS->VP);

  sprintf(s, "Ranks with boxes: %d", kv_count_ranks_with_boxes(value));
  gtk_label_set_text(GTK_LABEL(GS->GUI->replaybox.filledranks), s);
}

/****************** end of Processes *******************************/



/****************** GUI Callbacks **************************************/

static gboolean
on_window_key_event(_unused_ GtkWidget * widget, GdkEvent * event, _unused_ gpointer user_data) {
  _unused_ GdkModifierType m = gtk_accelerator_get_default_mod_mask();
  GdkEventKey * e = (GdkEventKey *) event;
  switch (e->keyval) {
  case GDK_KEY_Tab: { /* Tab */
    break;
  }
  case GDK_KEY_ISO_Left_Tab: { /* Shift + Tab */
    break;
  }
  default:
    break;
  }
  return FALSE;
}

static void
on_toolbar_toolbox_button_toggled(_unused_ GtkToggleToolButton * toolbtn, _unused_ gpointer user_data) {
  gboolean active = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(toolbtn));
  if (active) {
    kv_toggle_toolbox(1);
  } else {
    kv_toggle_toolbox(0);
  }
}

static void
on_toolbar_infobox_button_toggled(_unused_ GtkToggleToolButton * toolbtn, _unused_ gpointer user_data) {
  gboolean active = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(toolbtn));
  if (active) {
    kv_toggle_infobox(1);
  } else {
    kv_toggle_infobox(0);
  }
}

static void
on_toolbar_replaybox_button_toggled(_unused_ GtkToggleToolButton * toolbtn, _unused_ gpointer user_data) {
  gboolean active = gtk_toggle_tool_button_get_active(GTK_TOGGLE_TOOL_BUTTON(toolbtn));
  if (active) {
    kv_toggle_replaybox(1);
  } else {
    kv_toggle_replaybox(0);
  }
}

static void
on_toolbar_zoomfit_button_clicked(_unused_ GtkToolButton * toolbtn, _unused_ gpointer user_data) {
  long i = (long) user_data;
  switch (i) {
  case 0:
    kv_zoomfit_full(GS->VP);
    break;
  case 1:
    kv_zoomfit_hor(GS->VP);
    break;
  case 2:
    kv_zoomfit_ver(GS->VP);
    break;
  }
}

static gboolean
on_darea_draw_event(_unused_ GtkWidget * widget, cairo_t * cr, gpointer user_data) {
  kv_viewport_t * VP = (kv_viewport_t *) user_data;
  kv_viewport_draw(VP, cr);
  return FALSE;
}

static gboolean
on_darea_scroll_event(_unused_ GtkWidget * widget, _unused_ GdkEventScroll * event, _unused_ gpointer user_data) {
  kv_viewport_t * VP = (kv_viewport_t *) user_data;
  /* Calculate zoom factor */
  double factor = 1.0;
  if (event->direction == GDK_SCROLL_UP)
    factor *= KV_ZOOM_INCREMENT;
  else if (event->direction == GDK_SCROLL_DOWN)
    factor /= KV_ZOOM_INCREMENT;
  /* Apply factor */
  double zoomx = VP->zoom_ratio_x * factor;
  double zoomy = VP->zoom_ratio_y * factor;
  kv_zoom(VP, zoomx, zoomy, event->x, event->y);
  return TRUE;
}

static gboolean
on_darea_button_event(_unused_ GtkWidget * widget, _unused_ GdkEventButton * event, gpointer user_data) {
  kv_viewport_t * VP = (kv_viewport_t *) user_data;

  /* grab focus */
  gtk_widget_grab_focus(VP->darea);
  kv_viewport_queue_draw(VP);
  
  /* drag, click */
  if (event->button == GDK_BUTTON_PRIMARY) { /* left mouse button */
    
    if (event->type == GDK_BUTTON_PRESS) {
      /* Drag */
      VP->drag_on = 1;
      VP->pressx = event->x;
      VP->pressy = event->y;
      VP->accdisx = 0.0;
      VP->accdisy = 0.0;
    }  else if (event->type == GDK_BUTTON_RELEASE) {
      /* Drag */
      VP->drag_on = 0;
      /* Click */
      if (VP->accdisx < KV_SAFE_CLICK_RANGE && VP->accdisy < KV_SAFE_CLICK_RANGE) {
        //double ox = (event->x - VP->x) / VP->zoom_ratio_x;
        //double oy = (event->y - VP->y) / VP->zoom_ratio_y;
        //dv_dag_node_t * node = dv_do_finding_clicked_node(V, ox, oy);
      }
    } else if (event->type == GDK_2BUTTON_PRESS) {
      // nothing to do
    }

  } else if (event->button == GDK_BUTTON_SECONDARY) { /* right mouse button */

    /* TODO: show context menu */
    if (event->type == GDK_BUTTON_PRESS) {

      //double ox = (event->x - VP->x) / VP->zoom_ratio_x;
      //double oy = (event->y - VP->y) / VP->zoom_ratio_y;
      //dv_dag_node_t * node = dv_do_finding_clicked_node(V, ox, oy);
      
    } else if (event->type == GDK_BUTTON_RELEASE) {
      // nothing
    }

  }

  return TRUE;
}

static gboolean
on_darea_motion_event(_unused_ GtkWidget * widget, _unused_ GdkEventMotion * event, gpointer user_data) {
  kv_viewport_t * VP = (kv_viewport_t *) user_data;
  
  /* Drag */
  if (VP->drag_on) {
    double deltax = event->x - VP->pressx;
    double deltay = event->y - VP->pressy;
    VP->x += deltax;
    VP->y += deltay;
    VP->accdisx += deltax;
    VP->accdisy += deltay;
    VP->pressx = event->x;
    VP->pressy = event->y;
  }
  
  /* Hovering */
  double ox = (event->x - VP->x) / VP->zoom_ratio_x;
  double oy = (event->y - VP->y) / VP->zoom_ratio_y;
  kv_timeline_box_t * box = kv_find_box(GS->TL, ox, oy);
  if (!box) {
    if (VP->last_hovered_box) {
      VP->last_hovered_box->focused = 0;
      VP->last_hovered_box = NULL;
    }
  } else if (box != VP->last_hovered_box) {
    if (VP->last_hovered_box)
      VP->last_hovered_box->focused = 0;
    box->focused = 1;
    VP->last_hovered_box = box;
    kv_gui_update_infobox(box);
  }
  
  kv_viewport_queue_draw(VP);
  return TRUE;
}

static gboolean
on_darea_configure_event(_unused_ GtkWidget * widget, GdkEventConfigure * event, gpointer user_data) {
  kv_viewport_t * VP = (kv_viewport_t *) user_data;
  VP->vpw = event->width;
  VP->vph = event->height;
  return TRUE;
}

static gboolean
on_darea_key_press_event(_unused_ GtkWidget * widget, _unused_ GdkEventConfigure * event, _unused_ gpointer user_data) {
  kv_viewport_t * VP = (kv_viewport_t *) user_data;

  _unused_ GdkModifierType mod = gtk_accelerator_get_default_mod_mask();
  GdkEventKey * e = (GdkEventKey *) event;
  switch (e->keyval) {
  case 65361: /* Left */
    VP->x += 15;
    kv_viewport_queue_draw(VP);
    return TRUE;
  case 65362: /* Up */
    VP->y += 15;
    kv_viewport_queue_draw(VP);
    return TRUE;
  case 65363: /* Right */
    VP->x -= 15;
    kv_viewport_queue_draw(VP);
    return TRUE;
  case 65364: /* Down */
    VP->y -= 15;
    kv_viewport_queue_draw(VP);
    return TRUE;
  default:
    return FALSE;
  }
  return FALSE;        
}

static void
on_toolbox_align_start_toggled(GtkWidget * widget, _unused_ gpointer user_data) {
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
    GS->align_start = 1;
  } else {
    GS->align_start = 0;
  }
  kv_layout_timelines(GS->TL);
  kv_viewport_queue_draw(GS->VP);
}

static void
on_toolbox_legend_toggled(GtkWidget * widget, _unused_ gpointer user_data) {
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
    GS->draw_legend = 1;
  } else {
    GS->draw_legend = 0;
  }
  kv_viewport_queue_draw(GS->VP);
}

static void
on_replaybox_enable_toggled(GtkWidget * widget, _unused_ gpointer user_data) {
  if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))) {
    GS->replay_enable = 1;
  } else {
    GS->replay_enable = 0;
  }
  kv_viewport_queue_draw(GS->VP);
}

void
on_replaybox_scale_value_changed(GtkRange * range, _unused_ gpointer user_data) {
  double value = gtk_range_get_value(range);
  kv_replay_set_current_time(value);
}

void
on_replaybox_current_time_entry_activated(GtkEntry * entry, _unused_ gpointer user_data) {
  const char * str = gtk_entry_get_text(entry);
  double value = atof(str);
  kv_replay_set_current_time(value);
}


G_MODULE_EXPORT void
on_menubar_file_exit_activated(_unused_ GtkMenuItem * menuitem, _unused_ gpointer user_data) {
  gtk_main_quit();
}

G_MODULE_EXPORT void
on_menubar_view_toolbox_activated(_unused_ GtkCheckMenuItem * menuitem, _unused_ gpointer user_data) {
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
  if (active) {
    kv_toggle_toolbox(1);
  } else {
    kv_toggle_toolbox(0);
  }
}

G_MODULE_EXPORT void
on_menubar_view_infobox_activated(_unused_ GtkCheckMenuItem * menuitem, _unused_ gpointer user_data) {
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
  if (active) {
    kv_toggle_infobox(1);
  } else {
    kv_toggle_infobox(0);
  }
}

G_MODULE_EXPORT void
on_menubar_view_replaybox_activated(_unused_ GtkCheckMenuItem * menuitem, _unused_ gpointer user_data) {
  gboolean active = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(menuitem));
  if (active) {
    kv_toggle_replaybox(1);
  } else {
    kv_toggle_replaybox(0);
  }
}

G_MODULE_EXPORT void
on_menubar_view_zoomfit_hor_activated(_unused_ GtkMenuItem * menuitem, _unused_ gpointer user_data) {
  kv_zoomfit_hor(GS->VP);
}

G_MODULE_EXPORT void
on_menubar_view_zoomfit_ver_activated(_unused_ GtkMenuItem * menuitem, _unused_ gpointer user_data) {
  kv_zoomfit_ver(GS->VP);
}

G_MODULE_EXPORT void
on_menubar_view_zoomfit_full_activated(_unused_ GtkMenuItem * menuitem, _unused_ gpointer user_data) {
  kv_zoomfit_full(GS->VP);
}

/****************** end of GUI Callbacks *******************************/

