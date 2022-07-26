#ifndef SHELL_CLIENT_H
#define SHELL_CLIENT_H

int start_shell_client();
int stop_shell_client();

void shell_wait_until_run();

void shell_sig_handler(int signum);

#endif // SHELL_CLIENT_H
