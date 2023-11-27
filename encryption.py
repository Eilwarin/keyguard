import pyAesCrypt
import sys
import os


def encrypt_file(key):
    buffer_size = 64 * 1024  # Adjust the buffer size as needed

    pyAesCrypt.encryptFile(".logins.json", ".logins.enc", key, buffer_size)
    os.remove(".logins.json")


def decrypt_file(key):
    buffer_size = 64 * 1024  # Adjust the buffer size as needed

    pyAesCrypt.decryptFile(".logins.enc", ".logins.json", key, buffer_size)
    os.remove(".logins.enc")


if __name__ == "__main__":
    args = sys.argv[1:]

    if len(args) != 2:
        sys.exit(1)

    operation, key = args

    if operation == "encrypt":
        encrypt_file(key)
    elif operation == "decrypt":
        decrypt_file(key)
