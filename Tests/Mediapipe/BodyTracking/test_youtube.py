import time
from pytubefix import YouTube
from pytubefix.cli import on_progress

url = "https://www.youtube.com/watch?v=FWM7yGZDvbk"

yt = YouTube(url, on_progress_callback = on_progress)
print(f"[title]:'{yt.title}'")

ys = yt.streams.get_highest_resolution()
ys.download()


import cv2

# Path to the local video file
video_path = f"{yt.title}.mp4"
cap = cv2.VideoCapture(video_path)

# Display the video
while cap.isOpened():
    ret, frame = cap.read()
    if not ret: break

    cv2.imshow("YouTube Video", frame)
    time.sleep(0.02)     

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

