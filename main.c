#include <gtk/gtk.h>
#include <evince-view.h> /* this is for EvDocumentModel */
#include <evince-document.h>

static void
next_page (GtkWidget *widget,
           gpointer data)
{
        ev_view_next_page (EV_VIEW (data));
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
        GtkWidget *window;
        GtkWidget *presentation;
        GtkWidget *scrolled;
        GtkWidget *scrolled_next;
        GtkWidget *scrolled_presentation;
        GtkWidget *grid;
        GtkWidget *button;
        EvDocument *document;
        EvDocumentModel *model;
        EvDocumentModel *model_next;
        GtkWidget *view;
        GtkWidget *next;
        GtkWidget *presentation_view;
        GFile *file;
        GError *err = NULL;

        window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
        presentation = gtk_window_new (GTK_WINDOW_TOPLEVEL);

        grid = gtk_grid_new ();

        scrolled = gtk_scrolled_window_new (NULL, NULL);
        scrolled_next = gtk_scrolled_window_new (NULL, NULL);
        scrolled_presentation = gtk_scrolled_window_new (NULL, NULL);

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
                        button = gtk_button_new_with_label ("Next!");

                        model = ev_document_model_new_with_document (document);
                        ev_document_model_set_continuous (EV_DOCUMENT_MODEL (model), FALSE);
                        model_next = ev_document_model_new_with_document (document);
                        ev_document_model_set_continuous (EV_DOCUMENT_MODEL (model_next), FALSE);

                        view = ev_view_new ();
                        next = ev_view_new ();
                        presentation_view = ev_view_new ();
                        ev_view_set_model (EV_VIEW (view), model);
                        ev_view_set_model (EV_VIEW (presentation_view), model);
                        ev_view_set_model (EV_VIEW (next), model_next);
                        ev_view_next_page (EV_VIEW (next));

                        g_signal_connect (button, "clicked", G_CALLBACK (next_page), view);
                        g_signal_connect (button, "clicked", G_CALLBACK (next_page), next);

                        gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW (scrolled),
                                                                   800);
                        gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW (scrolled_next),
                                                                   400);

                        gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW (scrolled),
                                                                   600);
                        gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW (scrolled_next),
                                                                   400);

                        gtk_scrolled_window_set_min_content_width (GTK_SCROLLED_WINDOW (scrolled_presentation),
                                                                   800);
                        gtk_scrolled_window_set_min_content_height (GTK_SCROLLED_WINDOW (scrolled_presentation),
                                                                    400);

                        gtk_container_add (GTK_CONTAINER (scrolled),
                                           GTK_WIDGET (view));
                        gtk_container_add (GTK_CONTAINER (scrolled_next),
                                           GTK_WIDGET (next));
                        gtk_container_add (GTK_CONTAINER (scrolled_presentation),
                                           GTK_WIDGET (presentation_view));

                        gtk_grid_attach (GTK_GRID (grid),
                                         GTK_WIDGET (scrolled),
                                         0, 0, 1, 1);
                        gtk_grid_attach (GTK_GRID (grid),
                                         GTK_WIDGET (scrolled_next),
                                         1, 0, 1, 1);

                        gtk_grid_attach (GTK_GRID (grid),
                                         GTK_WIDGET (button),
                                         1, 2, 1, 1);

                        gtk_container_add (GTK_CONTAINER (window),
                                           GTK_WIDGET (grid));

                        gtk_container_add (GTK_CONTAINER (presentation),
                                           GTK_WIDGET (scrolled_presentation));
                }

        }

        gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);

        gtk_widget_show_all (GTK_WIDGET (window));

        gtk_window_set_default_size (GTK_WINDOW (presentation), 800, 600);


        gtk_widget_show_all (GTK_WIDGET (presentation));
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
