
#!/usr/bin/env python3
import asyncio
import websockets
import json
import random
import base64
import os

# Function to encode an image to Base64
def encode_image_to_base64(image_path):
    with open(image_path, "rb") as image_file:
        return base64.b64encode(image_file.read()).decode('utf-8')

# Function to send updated image every 500ms
async def send_video_stream(websocket, path):
    image_file = "placeholder.jpg"  # Your image file that will be updated every 500ms

    while True:
        if os.path.exists(image_file):
            # Encode the current image to Base64
            image_base64 = encode_image_to_base64(image_file)

            # Simulate data to send (e.g., height, distance, etc.)
            data = {
                "height": random.uniform(1.0, 10.0),
                "distance": random.uniform(0.5, 5.0),
                "acceleration": random.uniform(0.0, 9.8),
                "image": image_base64  # Base64 encoded image
            }
            # Convert the data to JSON and send to the client
            await websocket.send(json.dumps(data))

        # Wait 500ms before sending the next image
        await asyncio.sleep(0.5)

async def main():
    async with websockets.serve(send_video_stream, "localhost", 8765):
        await asyncio.Future()  # Run forever

if __name__ == "__main__":
    asyncio.run(main())

