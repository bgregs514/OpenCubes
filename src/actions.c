#include <stdio.h>
#include <glib.h>
#include <unistd.h>
#include "actions.h"
#include "gtk_err.h"
#include "gtk_menu.h"
#include "gtk_setup.h"

/***********************
* internal declarations
************************/
static int create_config();
static GKeyFile *get_key_file();

/***********************
* public definitions
************************/
struct Settings *load_settings(gpointer src, gpointer window)
{
	GKeyFile *key_file = get_key_file();
	struct Settings *conf;

	if (!is_config_exists()) {
		key_file = g_key_file_new();
		const gchar *message = ERR_CONFIG_CREATE;
		printf("-> actions.c: config file doesn't exist\n");
		printf("\t-> actions.c: creating with dummy values...\n");
		create_config(key_file);
		display_error(window, message);
		return NULL;
	}

	if (src)
		printf("we have a source\n");

	conf = g_slice_new(struct Settings);
	conf->ip = g_key_file_get_string(key_file, GROUP_API, IP, NULL);
	conf->port = g_key_file_get_string(key_file, GROUP_API, PORT, NULL);
	conf->user = g_key_file_get_string(key_file, GROUP_API, USER, NULL);
	conf->pass = g_key_file_get_string(key_file, GROUP_API, PASS, NULL);

	printf("-> actions.c: ip = %s\n", conf->ip);
	printf("-> actions.c: port = %s\n", conf->port);
	printf("-> actions.c: user = %s\n", conf->user);
	printf("-> actions.c: pass = %s\n", conf->pass);
	g_key_file_free(key_file);

	return conf;
}

int save_settings(gpointer entries)
{
	struct Entries *ents = entries;
	GKeyFile *key_file = get_key_file();
	const gchar *ip = gtk_entry_get_text(GTK_ENTRY(ents->ent_ip));
	const gchar *port = gtk_entry_get_text(GTK_ENTRY(ents->ent_port));
	const gchar *user = gtk_entry_get_text(GTK_ENTRY(ents->ent_user));
	const gchar *pass = gtk_entry_get_text(GTK_ENTRY(ents->ent_pass));

	g_key_file_set_string(key_file, GROUP_API, IP, ip);
	g_key_file_set_string(key_file, GROUP_API, PORT, port);
	g_key_file_set_string(key_file, GROUP_API, USER, user);
	g_key_file_set_string(key_file, GROUP_API, PASS, pass);

	if (!g_key_file_save_to_file(key_file, CONFIG_FILE, NULL)) {
		printf("-> actions.c: could not save config file %s\n", CONFIG_FILE);
		return -3;
	}

	return 0;
}

int is_config_exists()
{
	return access(CONFIG_FILE, F_OK) == -1 ? 0 : 1;
}

int web_open_doc()
{
	/* probably need to add a check for version of TM1/PA to link to the appropriate
	   documentation */
	system(LINUX_WEB_DOC);

	return 0;
}

int row_clicked(GtkTreeView *treeview, GtkTreePath *path, GtkTreeViewColumn *col, gpointer userdata)
{
	GtkTreeModel *model;
	GtkTreeIter iter;

	model = gtk_tree_view_get_model(treeview);
	if (gtk_tree_model_get_iter(model, &iter, path)) {
		gchar *type, *name;
		gtk_tree_model_get(model, &iter, COL_TYPE, &type, -1);
		gtk_tree_model_get(model, &iter, COL_NAME, &name, -1);
		printf("row clicked->type: %s name: %s\n", type, name);
		g_free(type);
		g_free(name);
	}

	return 0;
}

/***********************
* internal definitions
************************/
static int create_config(GKeyFile *key_file)
{
	const gchar *group = GROUP_API;
	const gchar *ip = IP_DEFAULT;
	const gchar *port = PORT_DEFAULT;
	const gchar *user = USER_DEFAULT;
	const gchar *pass = PASS_DEFAULT;

	g_key_file_set_string(key_file, group, IP, ip);
	g_key_file_set_string(key_file, group, PORT, port);
	g_key_file_set_string(key_file, group, USER, user);
	g_key_file_set_string(key_file, group, PASS, pass);
	g_key_file_set_comment(key_file, group, PASS, PASS_COMMENT, NULL);

	if (!g_key_file_save_to_file(key_file, CONFIG_FILE, NULL)) {
		printf("-> actions.c: could not save config file %s\n", CONFIG_FILE);
		return -3;
	}

	return 0;
}

static GKeyFile *get_key_file()
{
	GKeyFile *key_file;
	GKeyFileFlags flags = G_KEY_FILE_KEEP_COMMENTS | G_KEY_FILE_KEEP_TRANSLATIONS;
	key_file = g_key_file_new();

	if (!g_key_file_load_from_file(key_file, CONFIG_FILE, flags, NULL)) {
		printf("-> actions.c: could not read config file %s\n", CONFIG_FILE);
		return NULL;
	}

	return key_file;
}
