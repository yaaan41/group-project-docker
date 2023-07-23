import socket
import mysql.connector
from mysql.connector import Error
from datetime import datetime

PORT = 8084
BUFFER_SIZE = 1024

def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

    server_socket.bind(('', PORT))
    server_socket.listen(3)

    print(f"Server listening on port {PORT}...")

    # Database connection initialization
    conn = mysql.connector.connect(
        host="server01-mysql",
        user="root",
        password="cisco",
        database="project_db"
    )
    cursor = conn.cursor()

    while True:
        client_socket, client_addr = server_socket.accept()
        print(f"New client connected. IP address: {client_addr[0]}, Port: {client_addr[1]}")

        while True:
            try:
                data = client_socket.recv(BUFFER_SIZE).decode('utf-8')
                if not data:
                    break

                data = data.strip()
                choice, username, userpoints = data.split(' ')
                choice = int(choice)
                userpoints = int(userpoints)

                # Process the client's choice
                if choice == 1:
                    # Add User to database (perform SQL insert operation)
                    query = "INSERT INTO usertable (username, userpoints, datetime_stamps) VALUES (%s, %s, NOW())"
                    values = (username, userpoints)
                    cursor.execute(query, values)
                    conn.commit()

                elif choice == 2:
                    # Delete User from database (perform SQL delete operation)
                    query = "DELETE FROM usertable WHERE (username, userpoints) VALUES (%s, %s)"
                    values = (username, userpoints)
                    cursor.execute(query, values)
                    conn.commit()

                elif choice == 3:
                    # End Connection
                    client_socket.close()
                    print(f"Client disconnected. IP address: {client_addr[0]}, Port: {client_addr[1]}")
                    break

                else:
                    print("Invalid choice received from client.")

            except Exception as e:
                print(f"Error processing client request: {str(e)}")

    cursor.close()
    conn.close()

if __name__ == "__main__":
    main()
