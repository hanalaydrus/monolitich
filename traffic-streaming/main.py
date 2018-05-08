from flask import Flask, render_template, Response
from queue import Queue
from camera import VideoCamera

app = Flask(__name__)

camera_1_obj = VideoCamera('./videos/camera_1.mp4')
camera_1_obj.start()

camera_2_obj = VideoCamera('./videos/camera_2.mp4')
camera_2_obj.start()

camera_3_obj = VideoCamera('./videos/camera_3.mp4')
camera_3_obj.start()

camera_4_obj = VideoCamera('./videos/camera_4.mp4')
camera_4_obj.start()

camera_5_obj = VideoCamera('./videos/camera_5.mp4')
camera_5_obj.start()

camera_6_obj = VideoCamera('./videos/camera_6.mp4')
camera_6_obj.start()

def gen_1():
    while True:
        frame = camera_1_obj.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

def gen_2():
    while True:
        frame = camera_2_obj.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

def gen_3():
    while True:
        frame = camera_3_obj.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

def gen_4():
    while True:
        frame = camera_4_obj.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')
def gen_5():
    while True:
        frame = camera_5_obj.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

def gen_6():
    while True:
        frame = camera_6_obj.get_frame()
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + frame + b'\r\n\r\n')

@app.route('/camera_1')
def camera_1():
    return Response(gen_1(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_2')
def camera_2():
    return Response(gen_2(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_3')
def camera_3():
    return Response(gen_3(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_4')
def camera_4():
    return Response(gen_4(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_5')
def camera_5():
    return Response(gen_5(), mimetype='multipart/x-mixed-replace; boundary=frame')

@app.route('/camera_6')
def camera_6():
    return Response(gen_6(), mimetype='multipart/x-mixed-replace; boundary=frame')
                    
if __name__ == "__main__":
    app.run()