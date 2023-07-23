// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define PORT 8082
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in serv_addr;
    char buffer[BUFFER_SIZE] = {0};

    // Socket initialization
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket creation failed");
        return 1;
    }
    
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    
    //Server address follows server docker image using command docker inspect <container>
    if (inet_pton(AF_INET, "172.19.0.3", &serv_addr.sin_addr) <= 0) {
        perror("invalid address");
        return 1;
    }
    
    if (connect(client_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connection failed");
        return 1;
    }

    int continue_running = 1;

    while (continue_running) {
        int choice;
        char username[50];
        int userpoints;

        printf("1. Add User to database\n");
        printf("2. Delete User from database\n");
        printf("3. End Connection\n");
        printf("Enter your choice (1/2/3): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter user points: ");
                scanf("%d", &userpoints);
                snprintf(buffer, BUFFER_SIZE, "%d %s %d", choice, username, userpoints);
                write(client_fd, buffer, strlen(buffer));
                break;

            case 2:
                printf("Enter username to delete: ");
                scanf("%s", username);
                snprintf(buffer, BUFFER_SIZE, "%d %s", choice, username);
                write(client_fd, buffer, strlen(buffer));
                break;

            case 3:
                continue_running = 0;
                printf("Closing the connection...\n");
                write(client_fd, "3", 1);
                break;

            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }

        if (choice != 3) {
            printf("Do you want to continue using the system? (1: Yes / 0: No): ");
            scanf("%d", &continue_running);

            // Clear the input buffer after reading the integer choice
            while ((getchar()) != '\n');
        }
    }

    close(client_fd);
    return 0;
}
