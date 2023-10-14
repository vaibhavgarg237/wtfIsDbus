/*
    Note:
        Sample Code to filter whenever anyone sends some message to DBUS_BUS_SYSTEM (systemwide dbus)
        - Vaibhav Garg
*/
#include <iostream>
#include <dbus/dbus.h>
#include <thread>
using namespace std;

// Function to send a test D-Bus message
void sendTestDBusMessage()
{
  DBusError err;
  dbus_error_init(&err);

  std::this_thread::sleep_for(std::chrono::seconds(4));

  // Connect to the system bus
  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err))
  {
    std::cerr << "[CLIENT] D-Bus connection error: " << err.message << endl;
    dbus_error_free(&err);
    return;
  }

  DBusMessage *testMessage = dbus_message_new_signal("/test/signal/Object", "test.signal.Type", "Test");
  // Add necessary arguments to the test message

  dbus_uint32_t serial = 0;
  dbus_connection_send(conn, testMessage, &serial);

  cout << "[CLIENT]: Message must be sent!!!" << endl;
  dbus_message_unref(testMessage);
  dbus_connection_unref(conn);
}

// Callback function to process intercepted messages
DBusHandlerResult filter_func(DBusConnection *connection, DBusMessage *message, void *user_data)
{
  cout << "[SERVER]: filter_func triggered" << endl;

  if (dbus_message_get_type(message) == DBUS_MESSAGE_TYPE_SIGNAL)
  {
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
  }

  return DBUS_HANDLER_RESULT_NOT_YET_HANDLED;
}

int main()
{
  /* CLIENT */
  // Create a new thread to send the test D-Bus message
  std::thread senderThread(sendTestDBusMessage);

  /* SERVER */
  DBusError err;
  dbus_error_init(&err);

  // Connect to the system bus
  DBusConnection *conn = dbus_bus_get(DBUS_BUS_SYSTEM, &err);
  if (dbus_error_is_set(&err))
  {
    std::cerr << "[SERVER] D-Bus connection error: " << err.message << std::endl;
    dbus_error_free(&err);
    return 1;
  }

  // Add a filter to intercept signals
  //     dbus_bus_add_match(conn, "type='signal',sender='org.example.Sender',path='/org/example/Path',interface='org.example.Interface'", &err);

  // l("/your/test/path", "your.test.interface", "TestSignal");

  dbus_bus_add_match(conn, "type='signal',interface='test.signal.Type'", &err);
  if (dbus_error_is_set(&err))
  {
    std::cerr << "[SERVER] D-Bus match rule error: " << err.message << std::endl;
    dbus_error_free(&err);
    return 1;
  }

  // Set the filter function to process intercepted messages
  // Note: This is a callback function whenever any message is passed through Main system bus, this gets triggered
  dbus_connection_add_filter(conn, filter_func, nullptr, nullptr);

  std::cout << "[SERVER] ->Monitoring D-Bus signals. Press Enter to exit..." << std::endl;
  // senderThread.join();
  std::cin.get(); // Wait for user input

  // Cleanup and close the connection
  dbus_connection_remove_filter(conn, filter_func, nullptr);
  dbus_bus_remove_match(conn, "type='signal'", &err);
  dbus_connection_unref(conn);

  return 0;
}
