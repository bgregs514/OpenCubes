#ifndef ACTIONS_H
#define ACTIONS_H

#include <gtk/gtk.h>

#define CONFIG_FILE "cubes.conf"
#define GROUP_API "api"
#define IP "IP"
#define PORT "PORT"
#define USER "USER"
#define PASS "PASSWORD"
#define PASS_COMMENT "Use the shell to pull this in from a secure file if you prefer"

#define IP_DEFAULT "Enter IP Here"
#define PORT_DEFAULT "Enter Port Here"
#define USER_DEFAULT "Enter User Here"
#define PASS_DEFAULT "Enter Password Here"

#define LINUX_WEB_DOC "xdg-open https://www.ibm.com/support/knowledgecenter/SSD29G_2.0.0/main/welcome.html"

struct Settings {
	char *ip;
	char *port;
	char *user;
	char *pass;
};

/* g_signal_connect passes both the source (thing that called it) and an optional data argument (pointer);
   we don't need the source (for now), but our definition must match the objects being sent in order to
   properly use the data parameter */
struct Settings *load_settings(gpointer src, gpointer window);
int save_settings(gpointer entries);
int is_config_exists();
int web_open_doc();

#endif
