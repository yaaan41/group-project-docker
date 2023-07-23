// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <mysql/mysql.h>

#define PORT 8082
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd, read_size;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE] = {0};

    // Database connection initialization
    MYSQL *conn = mysql_init(NULL);
    if (conn == NULL) {
        fprintf(stderr, "mysql_init() failed\n");
        return 1;
    }

    if (mysql_real_connect(conn, "server01-mysql", "root", "cisco", "project_db", 0, NULL, 0) == NULL) {
        fprintf(stderr, "MySQL connection error: %s\n", mysql_error(conn));
        return 1;
    }

    // Socket initialization
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        return 1;
    }

    printf("Server listening on port %d...\n", PORT);

    while (1) {
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0) {
            perror("accept failed");
            return 1;
        }

        printf("New client connected. IP address: %s, Port: %d\n",
               inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));

        while ((read_size = read(client_fd, buffer, BUFFER_SIZE)) > 0) {
            buffer[read_size] = '\0'; // Null-terminate the received data

            int choice;
            char username[50];
            int userpoints;

            sscanf(buffer, "%d %s %d", &choice, username, &userpoints);

            // Process the client's choice
            switch (choice) {
                case 1:
                    // Add User to database (perform SQL insert operation)
                    snprintf(buffer, BUFFER_SIZE, "INSERT INTO usertable (username, userpoints, datetime_stamps) VALUES ('%s', %d, NOW())", username, userpoints);
                    if (mysql_query(conn, buffer) != 0) {
                        fprintf(stderr, "Error: %s\n", mysql_error(conn));
                    }
                    break;

                case 2:
                    // Delete User from database (perform SQL delete operation)
                    snprintf(buffer, BUFFER_SIZE, "DELETE FROM usertable WHERE username = '%s'", username);
                    if (mysql_query(conn, buffer) != 0) {
                        fprintf(stderr, "Error: %s\n", mysql_error(conn));
                    }
                    break;

                case 3:
                    // End Connection
                    close(client_fd);
                    printf("Client disconnected. IP address: %s, Port: %d\n",
                           inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
                    break;

                default:
                    printf("Invalid choice received from client.\n");
                    break;
            }

            if (choice == 3) {
                break; // Exit the inner loop if the client requested to end the connection
            }

            // Clear the buffer for the next iteration
            memset(buffer, 0, BUFFER_SIZE);
        }
    }

    mysql_close(conn);
    return 0;
}
