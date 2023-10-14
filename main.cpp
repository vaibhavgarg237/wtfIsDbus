#include <iostream>
#include <dbus/dbus.h>
using namespace std;

int main()
{
    DBusError error;
    dbus_error_init(&error);

    // Connect to the D-Bus system bus
    DBusConnection *connection = dbus_bus_get(DBUS_BUS_SYSTEM, &error);
    if (dbus_error_is_set(&error))
    {
        std::cerr << "D-Bus connection error: " << error.message << std::endl;
        dbus_error_free(&error);
        return 1;
    }

    // Define a message filter function
    DBusHandleMessageFunction filter_function = [](DBusConnection *connection, DBusMessage *message, void *user_data) -> DBusHandlerResult
    {
        // Your filter logic here
        // For this example, we simply log the received message's path

        const char *sender = dbus_message_get_sender(message);
        const char *path = dbus_message_get_path(message);
        const char *iface = dbus_message_get_interface(message);
        const char *member = dbus_message_get_member(message);

        std::cout << "[SERVER] Received D-Bus signal:" << std::endl;
        std::cout << "Sender: " << sender << std::endl;
        std::cout << "Path: " << path << std::endl;
        std::cout << "Interface: " << iface << std::endl;
        std::cout << "Member: " << member << std::endl;
        std::cout << std::endl;

        // Return DBUS_HANDLER_RESULT_HANDLED if you want to stop further processing
        return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
    };

    // Add the message filter to the connection
    dbus_connection_add_filter(connection, filter_function, nullptr, nullptr);

    // Loop to keep the program running (you may have other code here)
    while (true)
    {
        cout << "[Server] While loop running" << endl;
        dbus_connection_read_write_dispatch(connection, -1);
    }

    return 0;
}
