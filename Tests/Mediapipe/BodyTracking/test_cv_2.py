import cv2
import numpy as np 

import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
from matplotlib.animation import FuncAnimation

import utils_display
import utils_mediapipe

bone_connections = [[0,1],  [1,2], [2,3],    # Left eye
                    [0,4],  [4,5], [5,6],    # Right eye
                    [3,7],  [6,8],           # Ear
                    [9,10], [9,10],          # Mouth
                    [11,23],[11,12],[11,13],[12,14],[13,15],[14,16], # Upper body and arm
                    [15,17],[16,18],[17,19],[18,20],[15,21],[16,22], # Hand
                    [23,24],[12,24], # Torso
                    [23,25],[24,26],[25,27],[26,28],[27,29],[28,30],[27,31],[28,32]] # Leg


class DisplayBody2 :

    def __init__(self) :
        self.num_points = 33 
        self.lines = bone_connections #[(i, i + 1) for i in range(self.num_points - 1)]
        self.points = np.random.rand(self.num_points, 3)
        return 


    def create(self, ax, points=None, lines=None) :
        self.ax = ax 

        if lines is not None : self.lines = lines
        if points is not None : self.points = points
        return 


    def scatter_create(self) :
        self.scatters = [] 
        for p in self.points : 
            line, = ax.plot(*p, 'o')  # 'o' for points
            self.scatters.append(line) 
        return 


    def lines_create(self) : 
        self.line_objs = []
        for start, end in self.lines:
            (line,) = self.ax.plot(
                [self.points[start][0], self.points[end][0]],
                [self.points[start][1], self.points[end][1]],
                [self.points[start][2], self.points[end][2]],
                'k-'
            )
            self.line_objs.append(line)


    def update(self, points): 
        x,y,z = 0,1,2 
        x,y,z = 0,2,1 
        for i, (start,end) in enumerate(self.lines):
            self.line_objs[i].set_data([points[start][x], points[end][x]], [points[start][y], points[end][y]])
            self.line_objs[i].set_3d_properties([-points[start][z], -points[end][z]])

        for i, (start,end) in enumerate(self.lines):
            self.scatters[i].set_data([points[start][x], points[end][x]], [points[start][y], points[end][y]])
            self.scatters[i].set_3d_properties([-points[start][z], -points[end][z]])



class  ComputerVision :

    def __init__(self) : 
        cv2.destroyAllWindows()
        return 


    def FromRealTime(self): 
        self.cap = cv2.VideoCapture(0)
        self.__set_up__() 
        return 


    def __set_up__(self) : 
        ret,img = self.cap.read(0)

        img_width  = img.shape[1]
        img_height = img.shape[0]

        intrin = {
            'fx': img_width*0.9, # Approx 0.7w < f < w https://www.learnopencv.com/approximate-focal-length-for-webcams-and-cell-phone-cameras/
            'fy': img_width*0.9,
            'cx': img_width*0.5, # Approx center of image
            'cy': img_height*0.5,
            'width': img_width,
            'height': img_height,
        }

        self.pipe = utils_mediapipe.MediaPipeBody(static_image_mode=False, model_complexity=1, intrin=intrin)
        self.disp = utils_display.DisplayBody(draw3d=False, draw_camera=True, intrin=intrin)
        return 


    def Displayer(self, plotter3d) : 
        ret, img = self.cap.read()
        if not ret:
            print("Failed to capture image, exiting...")
            return True 

        img = cv2.flip(img, 1)
        img.flags.writeable = False

        param = self.pipe.forward(img)
        plotter3d( param['joint'] )

        img.flags.writeable = True
        cv2.imshow('img 2D', self.disp.draw2d(img, param))

        if cv2.waitKey(1) & 0xFF == ord('q'): return True 
        return False 


computerVision = ComputerVision() 
computerVision.FromRealTime()  

fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')

displayBody2 = DisplayBody2() 
displayBody2.create(ax) 
displayBody2.scatter_create() 
displayBody2.lines_create() 

def update(frame) :
    computerVision.Displayer( displayBody2.update )
    #displayBody2.update() 

ani = FuncAnimation(fig, update, frames=100, interval=50)
plt.show()