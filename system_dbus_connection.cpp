#include <iostream>
#include <dbus/dbus.h>
using namespace std;

// code establishes a connection to the D-Bus system bus, creates and sends a D-Bus message with arguments to the "org.freedesktop.Notifications" service, and then closes the connection
int main()
{
  DBusError err;
  dbus_error_init(&err);

  // Connect to the system bus
  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err))
  {
    std::cerr << "D-Bus connection error: " << err.message << std::endl;
    dbus_error_free(&err);
    return 1;
  }

  // Create a new dbus message (Notify is method called on service org.freedesktop.Notifications)
  DBusMessage *msg = dbus_message_new_method_call("org.freedesktop.Notifications",
                                                  "/org/freedesktop/Notifications",
                                                  "org.freedesktop.Notifications",
                                                  "Notify");

  if (!msg)
  {
    std::cerr << "D-Bus message creation failed." << std::endl;
    return 1;
  }

  // Add message arguments
  // dbus_message_append_args(msg, DBUS_TYPE_STRING, "Hello", DBUS_TYPE_STRING);

  // Send the message
  dbus_uint32_t serial = 0;
  if (!dbus_connection_send(conn, msg, &serial))
  {
    std::cerr << "D-Bus message sending failed." << std::endl;
    return 1;
  }

  // Free the message and close the connection
  dbus_message_unref(msg);
  dbus_connection_flush(conn);
  dbus_connection_unref(conn);
  cout << "3";
  return 0;
}
