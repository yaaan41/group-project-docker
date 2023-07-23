import socket

PORT = 8084
BUFFER_SIZE = 1024

def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    
    #Server address follows server docker image using command docker inspect <container>
    serv_addr = ("172.19.0.4", PORT)

    try:
        client_socket.connect(serv_addr)
        print("Connected to the server.")

        continue_running = True

        while continue_running:
            print("1. Add User to database")
            print("2. Delete User from database")
            print("3. End Connection")

            choice = int(input("Enter your choice (1/2/3): "))

            if choice == 1:
                username = input("Enter username: ")
                userpoints = int(input("Enter user points: "))
                data = f"{choice} {username} {userpoints}"
                client_socket.send(data.encode('utf-8'))

            elif choice == 2:
                username = input("Enter username to delete: ")
                userpoints = int(input("Enter user points to delete: "))
                data = f"{choice} {username} {userpoints}"
                client_socket.send(data.encode('utf-8'))

            elif choice == 3:
                continue_running = False
                print("Closing the connection...")
                client_socket.send(b"3")

            else:
                print("Invalid choice. Please try again.")

            if choice != 3:
                continue_running = int(input("Do you want to continue using the system? (1: Yes / 0: No): "))

    except Exception as e:
        print(f"Error: {str(e)}")

    finally:
        client_socket.close()

if __name__ == "__main__":
    main()
