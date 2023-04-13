#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>

#define MAX_WORD_LENGTH 8

typedef struct {
    GtkApplication *pApp;
    GtkEntry *pEntry;
} CallbackData;

void get_word_from_file(const char *pFilename, char *pWord) {
    FILE *pFile = fopen(pFilename, "r");

    if (pFile == NULL) {
        printf("Error opening file: %s\n", pFilename);
        exit(1);
    }

    int num_lines = 0;
    while (!feof(pFile)) {
        fgets(pWord, MAX_WORD_LENGTH, pFile);
        if (strlen(pWord) > 1) {
            num_lines++;
        }
    }

    fseek(pFile, 0, SEEK_SET);

    int random_line = rand() % num_lines;

    for (int i = 0, lines_read = 0; i < random_line; lines_read++) {
        fgets(pWord, MAX_WORD_LENGTH, pFile);
        if (strlen(pWord) > 1) {
            i++;
        }
    }

    strtok(pWord, "\n");

    fclose(pFile);
}

static void on_close_button_clicked(GtkButton *pButton, gpointer pUserData) {
    GtkWidget *pDialog = gtk_widget_get_ancestor(GTK_WIDGET(pButton), GTK_TYPE_DIALOG);
    gtk_window_destroy(GTK_WINDOW(pDialog));
}

GtkWidget *create_error_dialog(GtkApplication *pApp, const gchar *pMessage) {
    GtkWidget *pDialog = gtk_application_window_new(pApp);
    gtk_window_set_title(GTK_WINDOW(pDialog), "Error");
    gtk_window_set_modal(GTK_WINDOW(pDialog), TRUE);
    gtk_window_set_default_size(GTK_WINDOW(pDialog), 300, 200);

    GtkWidget *pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_window_set_child(GTK_WINDOW(pDialog), pBox);

    GtkWidget *pLabel = gtk_label_new(pMessage);
    gtk_label_set_wrap(GTK_LABEL(pLabel), TRUE);
    gtk_box_append(GTK_BOX(pBox), pLabel);

    GtkWidget *pButtonBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(pBox), pButtonBox);

    GtkWidget *pCloseButton = gtk_button_new_with_label("Close");
    gtk_box_append(GTK_BOX(pButtonBox), pCloseButton);
    g_signal_connect(pCloseButton, "clicked", G_CALLBACK(on_close_button_clicked), NULL);

    return pDialog;
}

static void on_generate_password_button_clicked(GtkButton *pButton, gpointer pUserData) {
    CallbackData *pData = (CallbackData *)pUserData;
    GtkEntry *pEntry = pData->pEntry;
    GtkApplication *pApp = pData->pApp;

    const gchar *pNumWordsStr = gtk_editable_get_text(GTK_EDITABLE(pEntry));
    int num_words = atoi(pNumWordsStr);

    if (num_words < 3 || num_words > 4) {
        GtkWidget *pDialog = create_error_dialog(pApp, "Invalid input! Please enter either 3 or 4.");
        gtk_window_present(GTK_WINDOW(pDialog));
        return;
    }

    char password[4 * MAX_WORD_LENGTH] = "";

    for (int i = 0; i < num_words; i++) {
        char word[MAX_WORD_LENGTH];

        int random_dict = rand() % 4;
        char filename[128];

        char current_path[FILENAME_MAX];
        strncpy(current_path, __FILE__, sizeof(current_path));
        char *last_separator = strrchr(current_path, '/');
        if (last_separator == NULL) {
            last_separator = strrchr(current_path, '\\');
        }
        *last_separator = '\0';

        snprintf(filename, sizeof(filename), "%s/dictionaries/%d.txt", current_path, random_dict + 4);

        get_word_from_file(filename, word);

        strcat(password, word);

        if (i < num_words - 1) {
            strcat(password, "-");
        }

        printf("%d %s\n", i + 1, word);
    }

    GtkWidget *pOutputLabel = g_object_get_data(G_OBJECT(pButton), "output_label");
    gchar *pOutputText = g_strdup_printf("Generated Password: %s", password);
    gtk_label_set_text(GTK_LABEL(pOutputLabel), pOutputText);
    g_free(pOutputText);
}


static void activate(GtkApplication *pApp, gpointer pUserData) {
    GtkWidget *pWindow;
    GtkWidget *pLabel;
    GtkWidget *pEntry;
    GtkWidget *pOutputLabel;
    GtkWidget *pButton;
    GtkWidget *pBox;
    GtkWidget *pLabelBox;

    pWindow = gtk_application_window_new(pApp);
    gtk_window_set_title(GTK_WINDOW(pWindow), "e-t-r Password Generator");
    gtk_window_set_default_size(GTK_WINDOW(pWindow), 300, 200);

    pBox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_window_set_child(GTK_WINDOW(pWindow), pBox);

    pLabelBox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_append(GTK_BOX(pBox), pLabelBox);

    pLabel = gtk_label_new("Enter number of words (3 or 4):");
    gtk_box_append(GTK_BOX(pLabelBox), pLabel);

    pEntry = gtk_entry_new();
    gtk_box_append(GTK_BOX(pLabelBox), pEntry);

    CallbackData *pData = g_new(CallbackData, 1);
    pData->pApp = pApp;
    pData->pEntry = GTK_ENTRY(pEntry);

    pOutputLabel = gtk_label_new("Generated Password: ");
    gtk_box_append(GTK_BOX(pBox), pOutputLabel);

    pButton = gtk_button_new_with_label("Generate Password");
    gtk_box_append(GTK_BOX(pBox), pButton);
    g_signal_connect(pButton, "clicked", G_CALLBACK(on_generate_password_button_clicked), pData);
    g_object_set_data(G_OBJECT(pButton), "output_label", pOutputLabel);

    gtk_window_present((GtkWindow *) pWindow);
}

int main(int argc, char **argv) {
    GtkApplication *pApp;
    int status;
    srand(time(NULL));

    pApp = gtk_application_new("com.example.etr_password_generator", G_APPLICATION_DEFAULT_FLAGS);
    g_signal_connect(pApp, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(pApp), argc, argv);
    g_object_unref(pApp);

    return status;
}
