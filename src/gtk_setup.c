#include "tm1_py.h"
#include "gtk_setup.h"
#include "actions.h"
#include "gtk_err.h"
#include "gtk_menu.h"

/***********************
* internal declarations
************************/
/* internal functions receive a pointer to the window (parent) for traceability and
   in order to correctly attach error messages to the parent window */
static GtkTreeModel *create_and_fill_model(gpointer window);
static GtkWidget *create_view_and_model(gpointer window);
static GtkWidget *build_menu(gpointer window);

/***********************
* public definitions
************************/
int build_frame(GtkApplication *app)
{
	GtkWidget *box;

        GtkWidget *window;
        GtkWidget *view;
        GtkWidget *scrolled_window;

	GtkWidget *menu;

        window = gtk_application_window_new(app);
        //gtk_window_set_position(gtk_window(window), gtk_win_pos_center);
        gtk_window_set_title(GTK_WINDOW(window), "Open Cubes");
        gtk_window_set_default_size(GTK_WINDOW(window), 510, 900);

	box = gtk_box_new(TRUE, 0);
	gtk_container_add(GTK_CONTAINER(window), box);

        scrolled_window = gtk_scrolled_window_new(NULL, NULL);
        gtk_container_set_border_width(GTK_CONTAINER(scrolled_window), 10);

	menu = build_menu(window);

	gtk_box_pack_start(GTK_BOX(box), menu, FALSE, FALSE, 0);

        view = create_view_and_model(window);

        gtk_container_add(GTK_CONTAINER(scrolled_window), view);

	gtk_box_pack_start(GTK_BOX(box), scrolled_window, TRUE, TRUE, 0);

        gtk_widget_show_all(window);

	return 0;
}

/***********************
* internal definitions
************************/
// need to split into 2 functions: 1 - create_store, 2 - fill_store
static GtkTreeModel *create_and_fill_model(gpointer window)
{
        GtkTreeStore *store;
        GtkTreeIter cube_parent_iter;
        GtkTreeIter cube_child_iter;
        GtkTreeIter proc_parent_iter;
        //GtkTreeIter proc_child_iter;
        struct py_data py_init;
        struct cube_names *names = malloc(sizeof(struct cube_names));
        //char **values;
        char *args[] = {"Cubes", "Processes"};

        // def script and func in tm1_py.h
        // remove param from all function calls and declaration in main
        py_init.script = "py_test";
        py_init.func = "get_names";

        store = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

        gtk_tree_store_append(store, &cube_parent_iter, NULL);
        gtk_tree_store_set(store, &cube_parent_iter,
                           COL_TYPE, args[0],
                           COL_NAME, "All Cubes",
                           -1);

        gtk_tree_store_append(store, &proc_parent_iter, NULL);
        gtk_tree_store_set(store, &proc_parent_iter,
                           COL_TYPE, args[1],
                           COL_NAME, "All Processes",
                           -1);

	/* if configuration file does not exist, allow load_settings to create the file
	   and report back to the user */
	if (!is_config_exists()) {
		load_settings(NULL, window);
		free(names);
		return GTK_TREE_MODEL(store);
	}

        names = load_py(&py_init, args[0]);

	/* if load_py fails, return headers only */
	if (names == NULL) {
		const gchar *message = ERR_IP;
		display_error(window, message);
		return GTK_TREE_MODEL(store);
	}

        for (int b = 0; b < names->size; b++) {
                gtk_tree_store_append(store, &cube_child_iter, &cube_parent_iter);
                gtk_tree_store_set(store, &cube_child_iter,
                                   COL_TYPE, "Cube",
                                   COL_NAME, names->name[b],
                                   -1);
        }

	free(names);
        return GTK_TREE_MODEL(store);
}

static GtkWidget *create_view_and_model(gpointer window)
{
        GtkCellRenderer *renderer;
        GtkTreeModel *model;
        GtkWidget *view;

        view = gtk_tree_view_new();
        
        renderer = gtk_cell_renderer_text_new();
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "Type", renderer, "text", COL_TYPE, NULL);
        gtk_tree_view_insert_column_with_attributes(GTK_TREE_VIEW(view), -1, "Name", renderer, "text", COL_NAME, NULL);

        model = create_and_fill_model(window);

        gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
        g_object_unref(model);

        return view;
}

static GtkWidget *build_menu(gpointer window)
{
	GtkWidget *menubar;
	GtkWidget *file_menu;

	GtkWidget *file_item;
	GtkWidget *refresh_item;
	GtkWidget *settings_item;

	menubar = gtk_menu_bar_new();
	file_menu = gtk_menu_new();

	file_item = gtk_menu_item_new_with_label("File");
	refresh_item = gtk_menu_item_new_with_label("Refresh");
	settings_item = gtk_menu_item_new_with_label("Settings");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_item), file_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), refresh_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), settings_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_item);

	//g_signal_connect(G_OBJECT(settings_item), "activate", G_CALLBACK(load_settings), window);
	g_signal_connect(G_OBJECT(settings_item), "activate", G_CALLBACK(display_settings), window);

	return menubar;
}
