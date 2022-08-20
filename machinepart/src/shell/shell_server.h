#ifndef SHELL_SERVER_H
#define SHELL_SERVER_H

int start_shell_server();
int stop_shell_server();

int add_command(void(*func), char *name, char *description);

#endif // SHELL_SERVER_H
