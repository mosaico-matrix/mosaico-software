import base64
import os
import logging
import socket
import subprocess

logger = logging.getLogger(__name__)


def extract_archive_from_base64_bytes(archive_bytes_b64, output_path):
    """
    Extract the archive from the given bytes to the output directory.
    """
    try:
        # Convert the base64 to bytes
        archive_bytes = base64.b64decode(archive_bytes_b64)

        # Path where to store the archive
        archive_path = os.path.join(output_path, "archive.tar.gz")

        # Write the decoded bytes to a file
        with open(archive_path, "wb") as file:
            file.write(archive_bytes)

        # Command to extract the archive
        command = ["tar", "-xzf", archive_path, "-C", output_path]
        logger.debug(f"Extracting with command: {' '.join(command)}")

        # Run the tar extraction command
        result = subprocess.run(command, check=True, capture_output=True, text=True)

        # Log the output of the command
        logger.debug(f"Extraction stdout: {result.stdout}")
        logger.debug(f"Extraction stderr: {result.stderr}")

    except base64.binascii.Error as e:
        logger.error(f"Failed to decode base64: {e}")
        raise
    except subprocess.CalledProcessError as e:
        logger.error(f"Extraction failed: {e}")
        logger.error(f"Extraction stderr: {e.stderr}")
        raise
    except Exception as e:
        logger.error(f"An unexpected error occurred: {e}")
        raise


def get_local_ip():
    # Create a UDP socket
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # Connect to an external address (Google's DNS server)
        s.connect(("8.8.8.8", 80))
        # Get the local IP address from the socket
        local_ip = s.getsockname()[0]
    except Exception as e:
        print(f"An error occurred: {e}")
        local_ip = None
    finally:
        s.close()
    return local_ip


def add_wifi_network(ssid, password):

    # Generate PSK using wpa_passphrase
    command = f'wpa_passphrase "{ssid}" "{password}"'
    result = subprocess.run(command, shell=True, capture_output=True, text=True)

    psk = None
    for line in result.stdout.split('\n'):
        if "psk=" in line and "#" not in line:
            psk = line.split('=')[1].strip()
            break

    if psk:
        # Append the new network configuration to wpa_supplicant.conf
        config = f'\nnetwork={{\n\tssid="{ssid}"\n\tscan_ssid=1\n\tkey_mgmt=WPA-PSK\n\tpsk={psk}\n}}\n'
        with open('/etc/wpa_supplicant/wpa_supplicant.conf', 'a') as file:
            file.write(config)

        # Restart the wpa_supplicant service and the network interface
        subprocess.run('sudo systemctl restart wpa_supplicant', shell=True)
        subprocess.run('sudo ifdown wlan0 && sudo ifup wlan0', shell=True)

        print(f"Network {ssid} added and services restarted.")
    else:
        print("Failed to generate PSK.")