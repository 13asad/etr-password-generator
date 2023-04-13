# e-t-r Password Generator

This is an easy-to-remember password generator that creates passwords by concatenating 3 or 4 random words from a set of dictionaries. The program is implemented using the GTK4 library.

## Dependencies

To compile and run the program, you need to have the following dependencies installed:

- GTK4
- GLib

## Installation

1. Clone the repository and navigate to the project directory.
2. Ensure that you have GTK4 and GLib development libraries installed on your system.
3. Compile the program using the following command (replace the paths to the GTK4 libraries as needed):

```sh
gcc `pkg-config --cflags gtk4` e-t-r.c -o etr_password_generator.exe `pkg-config --libs gtk4`
```

## How to run
After successfully compiling the program, run it using the following command:
```sh
./etr_password_generator.exe
```

## Directory structure
`Dictionaries`: Contains the dictionary files, each containing a list of words.

`error_dialog.ui`: Contains the UI definition for the error dialog.

`e-t-r.c`: Contains the main C source code for the program.

## How it works

### Main function
The main() function initializes the GTK application and seeds the random number generator with the current time. It then connects the "activate" signal to the activate function, runs the application, and unreferences the application object.

### Activate function
The activate function creates the main application window and sets its title and default size. It then creates a vertical box and sets it as the child of the window.

A horizontal box containing a label and an entry widget is created and appended to the vertical box. The label instructs the user to enter the number of words (3 or 4) for the password.

A CallbackData struct is created and assigned to hold the GtkApplication and GtkEntry pointers.

An output label is created and appended to the vertical box. This label displays the generated password.

A button with the label "Generate Password" is created and appended to the vertical box. The "clicked" signal is connected to the on_generate_password_button_clicked function, and the CallbackData struct is passed as user data.

The output label is set as data for the button so that it can be accessed in the on_generate_password_button_clicked function.

Finally, the window is presented.

### On Generate Password Button Clicked
The on_generate_password_button_clicked function is called when the "Generate Password" button is clicked. It retrieves the number of words from the entry widget and converts it to an integer. If the number of words is not 3 or 4, it displays an error dialog and returns without generating a password.

The function then generates the requested number of words by selecting a random dictionary file, reading a random word from the file, and appending the word to the password buffer. A hyphen is added between words, except for the last word.

The generated password is printed to the console and displayed in the output label.

### on_close_button_clicked
The on_close_button_clicked function is called when the "Close" button of the error dialog is clicked. It retrieves the dialog window from the button and destroys the dialog window.

### Create Error Dialog
The create_error_dialog function creates a new dialog window with the specified message. It sets the dialog's title, makes it modal, and sets its default size. A vertical box is created and set as the child of the dialog window. The message is displayed in a label, and a close button is created and connected to the on_close_button_clicked function.

### Get Word from File
The get_word_from_file function reads a random word from the specified file. It first opens the file for reading and checks if it was opened successfully.

It then counts the number of non-empty lines in the file and generates a random line number.

The function reads the word at the random line number, skipping empty lines. The newline character at the end of the word is removed, and the file is closed.

## GTK Syntax Explanation

1. `GtkApplication *app;`: Defines a pointer to a GtkApplication object, representing the application itself. A single instance of GtkApplication is typically used for each application.


2. `GtkWidget *window;`: Defines a pointer to a GtkWidget object, representing a window in your application.


3. `GtkWidget *label;`: Defines a pointer to a GtkWidget object, representing a label.


4. `GtkWidget *entry;`: Defines a pointer to a GtkWidget object, representing a text entry field.


5. `GtkWidget *output_label;`: Defines a pointer to a GtkWidget object, representing a label to display the generated password.


6. `GtkWidget *button;`: Defines a pointer to a GtkWidget object, representing a button.


7. `GtkWidget *box;`: Defines a pointer to a GtkWidget object, representing a container box.


8. `GtkWidget *label_box;`: Defines a pointer to a GtkWidget object, representing another container box for the label and the entry.


9. `gtk_application_window_new(app);`: Creates a new GtkApplicationWindow object and assigns it to the window pointer.


10. `gtk_window_set_title(GTK_WINDOW(window), "e-t-r Password Generator");`: Sets the title of the window.


11. `gtk_window_set_default_size(GTK_WINDOW(window), 300, 200);`: Sets the default size of the window.


12. `gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);`: Creates a new GtkBox object with a vertical orientation and 6 pixels of spacing between child widgets.


13. `gtk_window_set_child(GTK_WINDOW(window), box);`: Adds the box as a child of the window.


14. `gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);`: Creates a new GtkBox object with a horizontal orientation and 6 pixels of spacing between child widgets.


15. `gtk_box_append(GTK_BOX(box), label_box);`: Appends the label_box to the box.


16. `gtk_label_new("Enter number of words (3 or 4):");`: Creates a new GtkLabel object with the specified text.


17. `gtk_box_append(GTK_BOX(label_box), label);`: Appends the label to the label_box.


18. `gtk_entry_new();`: Creates a new GtkEntry object, which is a single-line text entry field.


19. `gtk_box_append(GTK_BOX(label_box), entry);`: Appends the entry to the label_box.


20. `gtk_label_new("Generated Password: ");`: Creates a new GtkLabel object with the specified text.


21. `gtk_box_append(GTK_BOX(box), output_label);`: Appends the output_label to the box.


22. `gtk_button_new_with_label("Generate Password");`: Creates a new GtkButton object with the specified label text.


23. `gtk_box_append(GTK_BOX(box), button);`: Appends the button to the box.


24. `g_signal_connect(button, "clicked", G_CALLBACK(on_generate_password_button_clicked), data);`: Connects the "clicked" signal emitted by the button to the on_generate_password_button_clicked callback function. The CallbackData struct is passed as user data to the callback function.


25. `g_object_set_data(G_OBJECT(button), "output_label", output_label);`: Associates the output_label with the button using the key "output_label". This allows the output_label to be accessed in the on_generate_password_button_clicked function.


26. `gtk_window_present((GtkWindow *) window);`: Presents the window and all its child widgets.

27. `g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);`: Connects the "activate" signal emitted by the app to the activate callback function. This function is called when the application is activated, which typically happens when it starts or when a new instance is requested.


28. `g_application_run(G_APPLICATION(app), argc, argv);`: Runs the application and enters the main event loop, which handles user input and other events. The loop continues running until the application is terminated. The function returns an integer status code.


29. `g_object_unref(app);`: Decrements the reference count of the GtkApplication object. When its reference count drops to 0, the object is finalized (i.e., its memory is freed).

## Error Dialog UI
The `error_dialog.ui` file is an XML-formatted UI definition file for the error dialog that appears when a user enters an invalid number of words. It uses the GTK Builder format to define the layout, styling, and properties of the error dialog and its child widgets, such as the message label and the close button.

This file is loaded and used by the `create_error_dialog` function in the main C code to construct the error dialog at runtime. When the function is called, it loads the UI definition from the `error_dialog.ui` file, gets the objects (widgets) defined in it, and connects signals to their respective callback functions.

To customize the error dialog, edit the `error_dialog.ui` file using a text editor or a dedicated UI designer tool like Glade (https://glade.gnome.org/).

## Usage
Run the e-t-r Password Generator application by executing the compiled executable:
```sh
./e-t-r
```

The application window will open. Enter the number of words (either 3 or 4) for the password in the text entry field.

Click the "Generate Password" button. The generated password will be displayed in the output label below the button. The individual words of the password will also be printed to the console.

If you enter a number other than 3 or 4, an error dialog will appear, informing you of the invalid input. Click "Close" to dismiss the error dialog.

## Customization
To use custom dictionaries, replace the existing dictionary files in the specified path with your own dictionary files. Each dictionary file should contain one word per line. Update the file path in the on_generate_password_button_clicked function to match the location of your dictionaries.

```c
snprintf(filename, sizeof(filename), "/path/to/your/dictionaries/%d.txt", random_dict + 4);
```

## Troubleshooting
If the application fails to compile or run, ensure that you have the required dependencies installed and that the paths to the GTK4 libraries are correct.

If the application is unable to read from the dictionary files, make sure that the file paths are correct and that the files are readable.

## License
This project is open-source and available under the MIT License.