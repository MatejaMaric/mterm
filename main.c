#include <vte/vte.h>
#include "config.h"

#define CLR_R(x)   (((x) & 0xff0000) >> 16)
#define CLR_G(x)   (((x) & 0x00ff00) >>  8)
#define CLR_B(x)   (((x) & 0x0000ff) >>  0)
#define CLR_16(x)  ((double)(x) / 0xff)
#define CLR_GDK(x, y) (const GdkRGBA){ .red = CLR_16(CLR_R(x)), \
                                    .green = CLR_16(CLR_G(x)), \
                                    .blue = CLR_16(CLR_B(x)), \
                                    .alpha = (y) }

static void child_ready(VteTerminal*, GPid, GError*, gpointer);


int main(int argc, char *argv[]) {
    GtkWidget *window, *terminal;
    VteTerminal *vTerminal;

    /* Initialise GTK, the window and the terminal */
    gtk_init(&argc, &argv);
    terminal = vte_terminal_new();
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "mterm");

    vTerminal = VTE_TERMINAL(terminal);

    /* Set colors */
    vte_terminal_set_colors(vTerminal,
        &CLR_GDK(foreground, 0),
        &CLR_GDK(background, transparency),
        (const GdkRGBA[]){
            CLR_GDK(colors[0], 0),
            CLR_GDK(colors[1], 0),
            CLR_GDK(colors[2], 0),
            CLR_GDK(colors[3], 0),
            CLR_GDK(colors[4], 0),
            CLR_GDK(colors[5], 0),
            CLR_GDK(colors[6], 0),
            CLR_GDK(colors[7], 0),
            CLR_GDK(colors[8], 0),
            CLR_GDK(colors[9], 0),
            CLR_GDK(colors[10], 0),
            CLR_GDK(colors[11], 0),
            CLR_GDK(colors[12], 0),
            CLR_GDK(colors[13], 0),
            CLR_GDK(colors[14], 0),
            CLR_GDK(colors[15], 0)
    }, 16);

    /* More settings */
    vte_terminal_set_scrollback_lines(vTerminal, max_lines);
    vte_terminal_set_scroll_on_output(vTerminal, scroll_on_output);
    vte_terminal_set_scroll_on_keystroke(vTerminal, scroll_on_keystroke);
    vte_terminal_set_mouse_autohide(vTerminal, mouse_autohide);
    vte_terminal_set_allow_hyperlink(vTerminal, TRUE);
    vte_terminal_set_font_scale(vTerminal, font_scale);

    /* Start a new shell */
    gchar **envp = g_get_environ();
    gchar **command = (gchar *[]){g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    g_strfreev(envp);
    vte_terminal_spawn_async(vTerminal,
        VTE_PTY_DEFAULT,
        NULL,         /* working directory  */
        command,      /* command */
        NULL,         /* environment */
        0,            /* spawn flags */
        NULL, NULL,   /* child setup */
        NULL,         /* child pid */
        -1,           /* timeout */
        NULL,         /* cancellable */
        child_ready,  /* callback */
        NULL);        /* user_data */

    /* Connect some signals */
    g_signal_connect(window, "delete-event", gtk_main_quit, NULL);
    g_signal_connect(terminal, "child-exited", gtk_main_quit, NULL);

    gtk_widget_set_visual(window, gdk_screen_get_rgba_visual(gtk_widget_get_screen(window)));

    /* Put widgets together and run the main loop */
    gtk_container_add(GTK_CONTAINER(window), terminal);
    gtk_widget_show_all(window);
    gtk_main();
}

static void child_ready(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data) {
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}
