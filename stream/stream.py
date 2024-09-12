import socket
from PIL import Image
from flask import Flask, render_template
from flask_socketio import SocketIO
import io
import threading
import logging

app = Flask(__name__)
socketio = SocketIO(app)
image_data = bytearray()

MATRIX_COLS = 64  # Matrix dimensions
MATRIX_ROWS = 64

# Adjustable scaling factor
SCALE_FACTOR = 5


# Disable default Flask request logging
log = logging.getLogger('werkzeug')
log.setLevel(logging.ERROR)  # Only log errors, suppress other messages

def receive_data():
    global image_data
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect(('localhost', 10005))  # Connect to the MatrixStream server

    try:
        while True:
            data = sock.recv(1024)
            if not data:
                break
            image_data.extend(data)

            # Check if we have enough data for a full frame
            if len(image_data) >= MATRIX_COLS * MATRIX_ROWS * 3:
                # Extract image data for the current frame
                frame_data = image_data[:MATRIX_COLS * MATRIX_ROWS * 3]
                image_data = image_data[MATRIX_COLS * MATRIX_ROWS * 3:]  # Remove the processed frame data

                # Create an image from the pixel data
                img = Image.new('RGB', (MATRIX_COLS, MATRIX_ROWS))
                img.putdata([(frame_data[i], frame_data[i+1], frame_data[i+2]) for i in range(0, len(frame_data), 3)])
                
                # Upscale the image
                upscaled_img = img.resize((MATRIX_COLS * SCALE_FACTOR, MATRIX_ROWS * SCALE_FACTOR), Image.NEAREST)
                
                # Save image to a BytesIO object
                img_bytes = io.BytesIO()
                upscaled_img.save(img_bytes, format='PNG')
                img_bytes.seek(0)

                # Emit the image data to all connected clients
                socketio.emit('new_image', {'image': img_bytes.getvalue().hex()})
    except Exception as e:
        print(f"Error receiving data: {e}")
    finally:
        sock.close()

@app.route('/')
def index():
    return render_template('index.html')

if __name__ == "__main__":
    threading.Thread(target=receive_data, daemon=True).start()
    socketio.run(app, host='0.0.0.0', port=5000, allow_unsafe_werkzeug=True)
