#include <gtk/gtk.h>
#include <evince-view.h> /* this is for EvDocumentModel */
#include <evince-document.h>

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
        GtkWidget *window;
        GtkWidget *scrolled;
        GtkWidget *scrolled_next;
        GtkWidget *grid;
        cairo_surface_t *surface;
        EvDocument *document;
        EvDocumentModel *model;
        GtkWidget *view;
        GtkWidget *next;
        EvRenderContext *rc;
        EvPage *page;
        GFile *file;
        GError *err = NULL;
        GFileInfo *info;
        /* GtkWidget *viewport; */

        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);

        grid = gtk_grid_new ();
        gtk_grid_insert_row (GTK_GRID (grid),
                             0);
        gtk_grid_insert_column (GTK_GRID (grid),
                                0);
        gtk_grid_insert_column (GTK_GRID (grid),
                                1);

        scrolled = gtk_scrolled_window_new (NULL, NULL);
        scrolled_next = gtk_scrolled_window_new (NULL, NULL);

        gtk_window_set_application (GTK_WINDOW (window), GTK_APPLICATION (app));
        gtk_window_set_title (GTK_WINDOW (window), "Evince Presenter View sketches");

        if (ev_init ())
        {
                fprintf (stdout, "Yay! we got backends!\n");
       
                file = g_file_new_for_path ("presentazione.pdf");

                document = ev_document_factory_get_document_for_gfile (file,
                                                                       EV_DOCUMENT_LOAD_FLAG_NONE,
                                                                       NULL,
                                                                       &err);
                ev_document_load_gfile (document, file, EV_DOCUMENT_LOAD_FLAG_NONE,
                                        NULL, &err);

                g_assert ( (document != NULL) || (err != NULL));

                if (err) 
                {
                        fprintf (stderr, "Error loading file: %s\n", err->message); 
                }

                if (EV_IS_DOCUMENT (document))
                {
                        model = ev_document_model_new_with_document (document);
                        view = ev_view_new ();
                        next = ev_view_new ();
                        ev_view_set_model (EV_VIEW (view), model);
                        ev_view_set_model (EV_VIEW (next), model);

                        gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW (scrolled),
                                                                   200);
                        gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW (scrolled_next),
                                                                   200);

                        gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW (scrolled),
                                                                   200);
                        gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW (scrolled_next),
                                                                   200);

                        gtk_container_add (GTK_CONTAINER (scrolled),
                                           GTK_WIDGET (view));
                        gtk_container_add (GTK_CONTAINER (scrolled_next),
                                           GTK_WIDGET (next));

                        gtk_grid_attach (GTK_GRID (grid),
                                         GTK_WIDGET (scrolled),
                                         0, 0, 1, 1);
                        gtk_grid_attach (GTK_GRID (grid),
                                         GTK_WIDGET (scrolled_next),
                                         1, 0, 1, 1);

                        gtk_container_add (GTK_CONTAINER (window),
                                           GTK_WIDGET (grid));
                }

        }

        gtk_window_set_default_size (GTK_WINDOW (window), 600, 400);

        gtk_widget_show_all (GTK_WIDGET (window));
}

int
main (int argc, char **argv)
{
        GtkApplication *app;
        int status;

        app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
        g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
        status = g_application_run (G_APPLICATION (app), argc, argv);
        g_object_unref (app);

        return status;
}
