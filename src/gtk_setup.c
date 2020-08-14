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
static void set_store_data(struct store_data *data, char *col_type, char *col_name);

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
        char *args[] = {"Cubes", "Processes"};
	size_t n = sizeof(args)/sizeof(args[0]);
	struct store_data data[n];

	/* init data struct */
	set_store_data(&data[0], CUBE_TYPE, CUBE_PAR);
	set_store_data(&data[1], PROC_TYPE, PROC_PAR);

        store = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING, G_TYPE_STRING);

        gtk_tree_store_append(store, &data[0].iter_parent, NULL);
        gtk_tree_store_set(store, &data[0].iter_parent,
                           COL_TYPE, args[0],
                           COL_NAME, data[0].col_name,
                           -1);

        gtk_tree_store_append(store, &data[1].iter_parent, NULL);
        gtk_tree_store_set(store, &data[1].iter_parent,
                           COL_TYPE, args[1],
                           COL_NAME, data[1].col_name,
                           -1);

	/* if configuration file does not exist, allow load_settings to create the file
	   and report back to the user */
	if (!is_config_exists()) {
		load_settings(NULL, window);
		return GTK_TREE_MODEL(store);
	}

	/* get the python module and init the python interpreter (possibly move this to main) */
	PyObject *pModule = init_py();
	/* load the all of the data from TM1; possibly break this out into a separate function */
	for (int i = 0; i < (int)n; i++) {
		data[i].names = malloc(sizeof(struct cube_names));
		data[i].names = load_py(pModule, args[i]);

		/* if load_py fails, return headers only */
		if (data[i].names == NULL) {
			const gchar *message = ERR_IP;
			display_error(window, message);
			return GTK_TREE_MODEL(store);
		}

		for (int b = 0; b < data[i].names->size; b++) {
			gtk_tree_store_append(store, &data[i].iter_child, &data[i].iter_parent);
			gtk_tree_store_set(store, &data[i].iter_child,
					   COL_TYPE, data[i].col_type,
					   COL_NAME, data[i].names->name[b],
					   -1);
		}

		free(data[i].names);
	}
	/* close the python interpreter */
	close_py();
        return GTK_TREE_MODEL(store);
}

static void set_store_data(struct store_data *data, char *col_type, char *col_name)
{
	data->col_type = col_type;
	data->col_name = col_name;
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

	g_signal_connect(view, "row-activated", G_CALLBACK(row_clicked), NULL); 

        gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
        g_object_unref(model);

        return view;
}

static GtkWidget *build_menu(gpointer window)
{
	GtkWidget *menubar;
	GtkWidget *file_menu, *help_menu;
	GtkWidget *file_sub, *help_sub;
	GtkWidget *refresh_item, *settings_item, *about_item, *doc_item;

	menubar = gtk_menu_bar_new();
	file_menu = gtk_menu_new();
	help_menu = gtk_menu_new();

	file_sub = gtk_menu_item_new_with_label("File");
	refresh_item = gtk_menu_item_new_with_label("Refresh");
	settings_item = gtk_menu_item_new_with_label("Settings");
	help_sub = gtk_menu_item_new_with_label("Help");
	doc_item = gtk_menu_item_new_with_label("Documentation");
	about_item = gtk_menu_item_new_with_label("About");

	gtk_menu_item_set_submenu(GTK_MENU_ITEM(file_sub), file_menu);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(help_sub), help_menu);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), refresh_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(file_menu), settings_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), doc_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(help_menu), about_item);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file_sub);
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), help_sub);

	g_signal_connect(G_OBJECT(settings_item), "activate", G_CALLBACK(display_settings), window);
	g_signal_connect(G_OBJECT(doc_item), "activate", G_CALLBACK(web_open_doc), NULL);
	g_signal_connect(G_OBJECT(about_item), "activate", G_CALLBACK(display_about), window);

	return menubar;
}
