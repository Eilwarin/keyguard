import pyAesCrypt
import sys
import os


def encrypt_file(input_path, key):
    buffer_size = 64 * 1024  # Adjust the buffer size as needed

    try:
        with open(input_path, 'rb') as inputFile:
            with open(input_path, 'wb') as outputFile:
                pyAesCrypt.encryptStream(inputFile, outputFile, key, buffer_size)
    except FileNotFoundError:
        with open(input_path, 'wb'):
            pyAesCrypt.encryptStream(inputFile, outputFile, key, buffer_size)


def decrypt_file(input_path, key):
    buffer_size = 64 * 1024  # Adjust the buffer size as needed

    decrypted_data = b""
    try:
        with open(input_path, 'rb') as inputFile:
            pyAesCrypt.decryptStream(inputFile, decrypted_data, key, buffer_size)
    except FileNotFoundError:
        with open(input_path, 'wb'):
            pyAesCrypt.decryptStream(inputFile, key, buffer_size)

    return decrypted_data.decode('utf-8')


if __name__ == "__main__":
    args = sys.argv[1:]

    if len(args) != 3:
        sys.exit(1)

    operation, input_file, key = args

    if not os.path.exists(input_file):
        with open(input_file, "wb") as new_file:
            pass

    if operation == "encrypt":
        encrypt_file(input_file, key)
    elif operation == "decrypt":
        decrypt_file(input_file, key)
    else:
        sys.exit(1)
