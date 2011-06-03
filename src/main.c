/***
  This file is part of avahi-http-browser.

  avahi-http-browser is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as
  published by the Free Software Foundation; either version 2.1 of the
  License, or (at your option) any later version.

  avahi is distributed in the hope that it will be useful, but WITHOUT
  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
  or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General
  Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with avahi; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#include <glib.h>
#include <gtk/gtk.h>
#include <avahi-client/client.h>
#include <avahi-client/publish.h>
#include <avahi-common/alternative.h>
#include <avahi-common/error.h>
#include <avahi-glib/glib-watch.h>
#include <avahi-glib/glib-malloc.h>

#include <avahi-ui/avahi-ui.h>

static void
dialog_run(GtkWidget *avahi_browser) {

  char address[AVAHI_ADDRESS_STR_MAX];
  
  char *service_type = NULL;
  char *service_name = NULL;
  char *host_name = NULL;
  char *path = NULL;
  int port = 80;
  const AvahiStringList *txt = NULL;

  char *url = NULL;

  if (gtk_dialog_run(GTK_DIALOG(avahi_browser)) == GTK_RESPONSE_ACCEPT) {

    service_type = g_strdup(aui_service_dialog_get_service_type(AUI_SERVICE_DIALOG(avahi_browser)));
    service_name = g_strdup(aui_service_dialog_get_service_name(AUI_SERVICE_DIALOG(avahi_browser)));
    
    host_name = g_strdup(avahi_address_snprint(address, sizeof(address), aui_service_dialog_get_address(AUI_SERVICE_DIALOG(avahi_browser))));

    port = aui_service_dialog_get_port(AUI_SERVICE_DIALOG(avahi_browser));

    for (txt = aui_service_dialog_get_txt_data(AUI_SERVICE_DIALOG(avahi_browser)); txt; txt = txt->next) {
      char *key, *value;
                
      if (avahi_string_list_get_pair((AvahiStringList*) txt, &key, &value, NULL) < 0) {
	break;
      }
      
      if (strcmp(key, "path") == 0) {
	path = g_strdup(value);
      }
      
      avahi_free(key);
      avahi_free(value);
    }

    if (path != NULL) {
      url = g_strdup_printf("http://%s:%d%s", host_name, port, path);
    } else {
      url = g_strdup_printf("http://%s:%d", host_name, port);
    }

    g_print("%s\n", url);

    gtk_widget_destroy(avahi_browser);

    execlp("gnome-open", "gnome-open", url, NULL);

    g_free(service_type);
    g_free(service_name);
    g_free(host_name);
    g_free(path);
    g_free(url);

  }
}

static void
show_avahi_browser () {
  GtkWidget *avahi_browser;

  avahi_browser = aui_service_dialog_new("Choose HTTP Server", NULL, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_CONNECT, GTK_RESPONSE_ACCEPT, NULL);
  aui_service_dialog_set_browse_service_types(AUI_SERVICE_DIALOG(avahi_browser), "_http._tcp", NULL);
  aui_service_dialog_set_service_type_name(AUI_SERVICE_DIALOG(avahi_browser), "_http._tcp", "HTTP Server");
  aui_service_dialog_set_resolve_service(AUI_SERVICE_DIALOG(avahi_browser), TRUE);

  gtk_window_present (GTK_WINDOW(avahi_browser));

  dialog_run(avahi_browser);
}

int
main (int argc, char **argv) {

  gtk_init(&argc, &argv);
  show_avahi_browser();

  return 0;
}
