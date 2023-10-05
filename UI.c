#include <gtk/gtk.h>
#include "util/validator.h"
#include "util/diseases.h"
#include "util/doctors.h"
#include "util/services.h"

#define APPNAME "XYZ Clinic"

GtkApplication *app;

GtkWidget *main_window;
GtkWidget *user_details_input;
GtkWidget *appointment_booked;
GtkWidget *disease_select;
GtkWidget *doctor_select;
GtkWidget *service_select;
GtkWidget *appointment_date;
GtkWidget *alert_dialog;
GtkWidget *user_name, *age, *phone;
int gender = -1;

enum Booking
{
    appointment,
    doctor,
    service,
};

struct BookingData
{
    char *data;
    enum Booking type;
};

typedef struct BookingData BookingData;

static void navigate_back_to_main_window(GtkWidget *, gpointer);
static void display_dialog_window(GtkWindow *, char *);
static void display_choose_problems_window();
static void display_disease_buttons(GtkWidget *, char *);
static void handle_gender_toggle(GtkWindow *, char *);
static void validate_user_data(GtkWindow *, char *);
static void clean_up_memory();
static void quit_app(GtkWidget *widget);
static void display_choose_service_window();
static void display_choose_doctor_window();
static void display_doctors(GtkWidget *);
static void display_main_window(GtkApplication *, gpointer);
static void handle_navigation(int);
static void display_user_details_input_window(GtkWidget *, gpointer);
static void back_to_main(GtkWidget *, gpointer);
static void display_appointment_booked_window(GtkWidget *, gpointer);
static void display_services(GtkWidget *);

int main(int argc, char **argv)
{
    int status = 0;
    app = gtk_application_new("org.c.project", 0);
    g_signal_connect(app, "activate", G_CALLBACK(display_main_window), NULL);
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}

static void quit_app(GtkWidget *widget)
{
    clean_up_memory();
    g_main_context_wakeup(NULL);
    for (GList *iter = gtk_window_list_toplevels(); iter != NULL; iter = iter->next)
        gtk_window_close(GTK_WINDOW(iter->data));
}

static void navigate_back_to_main_window(GtkWidget *widget, gpointer data)
{
    gtk_widget_set_visible(user_details_input, 0);
    gtk_widget_set_visible(main_window, 1);
}

static void handle_navigation(int destination)
{
    if (destination == appointment)
        display_choose_problems_window();
    else if (destination == doctor)
        display_choose_doctor_window();
    else if (destination == service)
        display_choose_service_window();
}

static void display_user_details_input_window(GtkWidget *widget, gpointer data)
{
    GtkWidget *grid, *outer_grid;
    GtkWidget *label;
    GtkWidget *button;

    gtk_widget_set_visible(main_window, 0);
    user_details_input = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(user_details_input), APPNAME);
    g_signal_connect(user_details_input, "destroy", G_CALLBACK(quit_app), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    outer_grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(outer_grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(outer_grid), 16);

    gtk_widget_set_margin_start(outer_grid, 5);
    gtk_widget_set_margin_end(outer_grid, 5);
    gtk_widget_set_margin_bottom(outer_grid, 5);

    gtk_window_set_child(GTK_WINDOW(user_details_input), outer_grid);

    button = gtk_button_new_with_label("Go Back");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 0, 1, 1);

    g_signal_connect(button, "clicked", G_CALLBACK(navigate_back_to_main_window), NULL);

    label = gtk_label_new("Please Provide your Details");
    gtk_grid_attach(GTK_GRID(grid), label, 1, 0, 3, 1);

    label = gtk_label_new("Name");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

    user_name = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(user_name), "Ex: John Doe");
    gtk_grid_attach(GTK_GRID(grid), user_name, 1, 1, 3, 1);

    label = gtk_label_new("Age");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

    age = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(age), "Must be older than 10");
    gtk_grid_attach(GTK_GRID(grid), age, 1, 2, 3, 1);

    label = gtk_label_new("Gender");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);

    GtkWidget *male = gtk_check_button_new_with_label("male");
    GtkWidget *female = gtk_check_button_new_with_label("female");

    gtk_check_button_set_group(GTK_CHECK_BUTTON(female),
                               GTK_CHECK_BUTTON(male));

    g_signal_connect(male, "toggled", G_CALLBACK(handle_gender_toggle), "male");
    g_signal_connect(female, "toggled", G_CALLBACK(handle_gender_toggle), "female");

    gtk_grid_attach(GTK_GRID(grid), male, 1, 3, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), female, 2, 3, 1, 1);

    label = gtk_label_new("Phone");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);
    // TODO: Only ALlow Phone Numbers
    phone = gtk_entry_new();
    gtk_entry_set_placeholder_text(GTK_ENTRY(phone), "10 digit number");
    gtk_grid_attach(GTK_GRID(grid), phone, 1, 4, 3, 1);

    gtk_grid_attach(GTK_GRID(outer_grid), grid, 0, 0, 2, 1);

    if ((enum Booking *)data == appointment || (enum Booking *)data == service)
    {
        label = gtk_label_new("Appointment Date");
        gtk_grid_attach(GTK_GRID(outer_grid), label, 0, 1, 1, 1);
        appointment_date = gtk_calendar_new();
        gtk_grid_attach(GTK_GRID(outer_grid), appointment_date, 1, 1, 1, 1);
    }

    button = gtk_button_new_with_label("Next");
    g_signal_connect(button, "clicked", G_CALLBACK(validate_user_data), (enum Booking *)data);

    gtk_grid_attach(GTK_GRID(outer_grid), button, 0, ((enum Booking *)data == appointment || (enum Booking *)data == service) ? 2 : 1, 2, 1);

    gtk_widget_set_visible(user_details_input, 1);
}

static void display_main_window(GtkApplication *application, gpointer user_data)
{
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button;

    /* create a new window, and set its title */
    main_window = gtk_application_window_new(application);
    gtk_window_set_title(GTK_WINDOW(main_window), APPNAME);
    g_signal_connect(main_window, "destroy", G_CALLBACK(quit_app), NULL);

    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);
    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);

    /* Pack the container in the window */
    gtk_window_set_child(GTK_WINDOW(main_window), grid);

    /* App Welcome */
    label = gtk_label_new("Welcome to XYZ Clinic Booking");

    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 3, 1);

    button = gtk_button_new_with_label("Book Appointment By Date");
    g_signal_connect(button, "clicked", G_CALLBACK(display_user_details_input_window), appointment);

    /* Place the first button in the grid cell (0, 0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

    button = gtk_button_new_with_label("Book Appointment By Doctor");
    g_signal_connect(button, "clicked", G_CALLBACK(display_user_details_input_window), doctor);

    /* Place the second button in the grid cell (1, 0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    gtk_grid_attach(GTK_GRID(grid), button, 1, 1, 1, 1);

    button = gtk_button_new_with_label("Book A Service");
    g_signal_connect(button, "clicked", G_CALLBACK(display_user_details_input_window), service);

    /* Place the second button in the grid cell (1, 0), and make it fill
     * just 1 cell horizontally and vertically (ie no spanning)
     */
    gtk_grid_attach(GTK_GRID(grid), button, 2, 1, 1, 1);

    // button = gtk_button_new();
    // gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    button = gtk_button_new();
    label = gtk_label_new("Exit");
    PangoAttribute *textColor = pango_attr_foreground_new(65535, 0, 0);
    PangoAttrList *const Attrs = pango_attr_list_new();
    pango_attr_list_insert(Attrs, textColor);
    gtk_label_set_attributes((GtkLabel *)label, Attrs);
    gtk_button_set_child(GTK_BUTTON(button), label);
    pango_attr_list_unref(Attrs);
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(quit_app), main_window);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 3, 1);

    gtk_widget_set_visible(main_window, 1);
}

static void display_choose_problems_window()
{
    GtkWidget *grid;
    GtkWidget *label;

    gtk_widget_set_visible(user_details_input, 0);

    disease_select = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(disease_select), APPNAME);
    g_signal_connect(disease_select, "destroy", G_CALLBACK(quit_app), NULL);

    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);
    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);

    /* Pack the container in the window */
    gtk_window_set_child(GTK_WINDOW(disease_select), grid);

    /* App Welcome */
    label = gtk_label_new("Choose you Issue");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 3, 1);

    // Code for loading buttons
    display_disease_buttons(grid, "Appointment");

    // g_signal_connect_swapped(button, "clicked", G_CALLBACK(gtk_window_close), disease_select);

    // gtk_grid_attach(GTK_GRID(grid), button, 0, 3, 3, 1);

    gtk_widget_set_visible(disease_select, 1);
}

static void display_choose_doctor_window()
{
    GtkWidget *grid;
    GtkWidget *label;

    gtk_widget_set_visible(user_details_input, 0);

    doctor_select = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(doctor_select), APPNAME);
    g_signal_connect(doctor_select, "destroy", G_CALLBACK(quit_app), NULL);

    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);
    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);

    /* Pack the container in the window */
    gtk_window_set_child(GTK_WINDOW(doctor_select), grid);

    /* App Welcome */
    label = gtk_label_new("Choose your preferred Doctor");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 3, 1);

    // Code for loading buttons
    display_doctors(grid);

    gtk_widget_set_visible(doctor_select, 1);
}

static void display_choose_service_window()
{
    GtkWidget *grid;
    GtkWidget *label;

    gtk_widget_set_visible(user_details_input, 0);

    service_select = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(service_select), APPNAME);
    g_signal_connect(service_select, "destroy", G_CALLBACK(quit_app), NULL);

    /* Here we construct the container that is going pack our buttons */
    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);
    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);

    /* Pack the container in the window */
    gtk_window_set_child(GTK_WINDOW(service_select), grid);

    /* App Welcome */
    label = gtk_label_new("Choose your Required Service");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 3, 1);

    // Code for loading buttons
    display_services(grid);

    gtk_widget_set_visible(service_select, 1);
}

static void back_to_main(GtkWidget *widget, gpointer data)
{
    gtk_widget_set_visible(appointment_booked, 0);
    gtk_widget_set_visible(main_window, 1);
}

static void display_appointment_booked_window(GtkWidget *widget, gpointer argument_data)
{
    GtkWidget *grid;
    GtkWidget *label;
    GtkWidget *button;
    GtkEntryBuffer *data;

    BookingData *booking = (BookingData *)argument_data;

    if (booking->type == service)
        gtk_widget_set_visible(service_select, 0);
    else if (booking->type == doctor)
        gtk_widget_set_visible(doctor_select, 0);
    else if (booking->type == appointment)
        gtk_widget_set_visible(disease_select, 0);
    appointment_booked = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(appointment_booked), APPNAME);
    g_signal_connect(appointment_booked, "destroy", G_CALLBACK(quit_app), NULL);

    grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_widget_set_margin_start(grid, 5);
    gtk_widget_set_margin_end(grid, 5);
    gtk_widget_set_margin_bottom(grid, 5);

    gtk_window_set_child(GTK_WINDOW(appointment_booked), grid);

    if (booking->type == doctor)
    {
        char date_string[40] = "Appointment Booked for ";
        char *actual_date = g_date_time_format(g_date_time_new_now_local(), "%d-%m-%Y");

        strcat(date_string, actual_date);

        label = gtk_label_new(date_string);
        gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 4, 1);
    }
    else if (booking->type == appointment || booking->type == service)
    {
        char date_string[40] = "Appointment Booked for ";
        char *actual_date = (char *)g_date_time_format(gtk_calendar_get_date(GTK_CALENDAR(appointment_date)), "%d-%m-%Y");

        strcat(date_string, actual_date);

        label = gtk_label_new(date_string);
        gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 4, 1);
    }

    label = gtk_label_new("Name");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 1, 1, 1);

    data = gtk_entry_get_buffer(GTK_ENTRY(user_name));
    label = gtk_label_new(gtk_entry_buffer_get_text(data));
    gtk_grid_attach(GTK_GRID(grid), label, 1, 1, 2, 1);

    label = gtk_label_new("Age");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 2, 1, 1);

    data = gtk_entry_get_buffer(GTK_ENTRY(age));
    label = gtk_label_new(gtk_entry_buffer_get_text(data));
    gtk_grid_attach(GTK_GRID(grid), label, 1, 2, 2, 1);

    label = gtk_label_new("Gender");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 3, 1, 1);

    label = gtk_label_new(gender == 1 ? "Male" : "Female");
    gtk_grid_attach(GTK_GRID(grid), label, 1, 3, 2, 1);

    label = gtk_label_new("Phone");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 4, 1, 1);

    data = gtk_entry_get_buffer(GTK_ENTRY(phone));
    label = gtk_label_new(gtk_entry_buffer_get_text(data));
    gtk_grid_attach(GTK_GRID(grid), label, 1, 4, 2, 1);

    if (booking->type == doctor)
        label = gtk_label_new("Doctor");
    else if (booking->type == service)
        label = gtk_label_new("Service");
    else
        label = gtk_label_new("Issue");
    gtk_grid_attach(GTK_GRID(grid), label, 0, 5, 1, 1);

    label = gtk_label_new(booking->data);
    gtk_grid_attach(GTK_GRID(grid), label, 1, 5, 2, 1);

    button = gtk_button_new_with_label("Ok");
    gtk_grid_attach(GTK_GRID(grid), button, 0, 6, 4, 1);

    g_signal_connect(button, "clicked", G_CALLBACK(back_to_main), appointment_booked);

    gtk_widget_set_visible(appointment_booked, 1);
}

void display_dialog_parent(GtkWindow *parent, gpointer data)
{
    gtk_window_close(alert_dialog);
}

void display_dialog_window(GtkWindow *parent, char *message)
{
    alert_dialog = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(alert_dialog), "Alert!");
    g_signal_connect(alert_dialog, "destroy", G_CALLBACK(display_dialog_parent), NULL);

    GtkWidget *grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 16);
    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
    gtk_grid_set_row_homogeneous(GTK_GRID(grid), TRUE);

    gtk_widget_set_margin_top(grid, 10);
    gtk_widget_set_margin_bottom(grid, 10);
    gtk_widget_set_margin_start(grid, 10);
    gtk_widget_set_margin_end(grid, 10);

    GtkWidget *label = gtk_label_new(message);
    gtk_grid_attach(GTK_GRID(grid), label, 0, 0, 1, 1);

    GtkWidget *button = gtk_button_new_with_label("Ok");
    g_signal_connect_swapped(button, "clicked", G_CALLBACK(display_dialog_parent), NULL);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 1, 1, 1);

    gtk_window_set_child(GTK_WINDOW(alert_dialog), grid);
    gtk_widget_set_visible(alert_dialog, 1);

    // GtkWidget *dialog, *label, *content_area;
    // GtkDialogFlags flags;

    // // Create the widgets
    // flags = GTK_DIALOG_DESTROY_WITH_PARENT;
    // dialog = gtk_dialog_new_with_buttons("Alert!",
    //                                      parent,
    //                                      flags,
    //                                      ("I understand"),
    //                                      GTK_RESPONSE_NONE,
    //                                      NULL);

    // content_area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
    // label = gtk_label_new(message);

    // gtk_widget_set_margin_start(content_area, 10);
    // gtk_widget_set_margin_end(content_area, 10);
    // gtk_widget_set_margin_top(content_area, 10);
    // gtk_widget_set_margin_bottom(content_area, 10);

    // // Ensure that the dialog box is destroyed when the user responds
    // g_signal_connect_swapped(dialog,
    //                          "response",
    //                          G_CALLBACK(gtk_window_destroy),
    //                          dialog);

    // // Add the label, and show everything we’ve added
    // gtk_box_append(GTK_BOX(content_area), label);
    // gtk_box_append(GTK_BOX(content_area), label);
    // gtk_widget_set_visible(dialog, 1);
}

static void validate_user_data(GtkWindow *parent, char *message)
{
    GtkEntryBuffer *data = gtk_entry_get_buffer(GTK_ENTRY(user_name));
    int valid = validate_name(gtk_entry_buffer_get_text(data), gtk_entry_buffer_get_length(data));
    if (valid)
        g_print("Valid Name\n");
    else
    {
        display_dialog_window(parent, "Invalid Name");
        return;
    }
    data = gtk_entry_get_buffer(GTK_ENTRY(age));
    valid = validate_age(gtk_entry_buffer_get_text(data), gtk_entry_buffer_get_length(data));
    if (valid)
        g_print("Valid Age\n");
    else
    {
        display_dialog_window(parent, "Invalid Age");
        return;
    }
    if (gender == 1 || gender == 2)
        g_print("Valid Gender\n");
    else
    {
        display_dialog_window(parent, "Invalid Gender");
        return;
    }
    data = gtk_entry_get_buffer(GTK_ENTRY(phone));
    valid = validate_mobile_number(gtk_entry_buffer_get_text(data), gtk_entry_buffer_get_length(data));
    if (valid)
        g_print("Valid Number\n");
    else
    {
        display_dialog_window(parent, "Invalid Phone Number");
        return;
    }
    if ((enum Booking *)message == appointment || (enum Booking *)message == service)
    {
        char *date_string = g_date_time_format(gtk_calendar_get_date(GTK_CALENDAR(appointment_date)), "%d-%m-%Y");
        valid = isValidDate(date_string, g_date_time_format(g_date_time_new_now_local(), "%d-%m-%Y"));
        if (valid)
            g_print("Valid Date\n");
        else
        {
            display_dialog_window(parent, "Invalid Date");
            return;
        }
    }
    printf("%d\n", (enum Booking)message);
    handle_navigation((enum Booking)message);
}

static void handle_gender_toggle(GtkWindow *parent, char *message)
{

    if (strcmp(message, "male") == 0)
        gender = 1;
    else if (strcmp(message, "female") == 0)
        gender = 2;
    else
        gender = -1;
}

static void display_disease_buttons(GtkWidget *grid, char *message)
{
    load_diseases();
    GtkWidget *button;

    for (int i = 0; i < DISEASES; i++)
    {
        button = gtk_button_new_with_label(DISEASES_ARRAY[i]);
        BookingData *data = (BookingData *)malloc(sizeof(BookingData));
        data->data = DISEASES_ARRAY[i];
        data->type = appointment;
        g_signal_connect(button, "clicked", G_CALLBACK(display_appointment_booked_window), data);
        gtk_grid_attach(GTK_GRID(grid), button, i % 3, i / 3 + 1, 1, 1);
    }
}

static void clean_up_memory()
{
    free_diseases_array();
    free_doctors_array();
    free_services_array();
    free_up_validator()
}

static void inject_data()
{
}

static void display_doctors(GtkWidget *grid)
{
    load_doctors("data/doctors.csv");
    GtkWidget *button;

    for (int i = 0; i < DOCTORS; i++)
    {
        button = gtk_button_new_with_label(doctors_array[i].name);
        BookingData *data = malloc(sizeof(BookingData));
        data->data = doctors_array[i].name;
        data->type = doctor;
        g_signal_connect(button, "clicked", G_CALLBACK(display_appointment_booked_window), data);
        gtk_grid_attach(GTK_GRID(grid), button, i % 3, i / 3 + 1, 1, 1);
    }
}

static void display_services(GtkWidget *grid)
{
    load_services("data/services.csv");
    GtkWidget *button;

    for (int i = 0; i < SERVICES; i++)
    {
        button = gtk_button_new_with_label(SERVICES_ARRAY[i]);
        BookingData *data = malloc(sizeof(BookingData));
        data->data = SERVICES_ARRAY[i];
        data->type = service;
        g_signal_connect(button, "clicked", G_CALLBACK(display_appointment_booked_window), data);
        gtk_grid_attach(GTK_GRID(grid), button, i % 3, i / 3 + 1, 1, 1);
    }
}