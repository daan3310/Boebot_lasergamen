import cv2
import cv2.aruco as aruco
import numpy as np
import json

aruco_dict = aruco.Dictionary_get(aruco.DICT_ARUCO_ORIGINAL)
parameters = aruco.DetectorParameters_create()

def load_config(file_path='config.json'):
    with open(file_path, 'r') as file:
        return json.load(file)

def get_mac_from_ID(Leaphy_ID):
    config = load_config()
    for Leaphy in config['Leaphy']:
        if Leaphy_ID in Leaphy['aruco_ids']:
            return Leaphy['mac_address']
    return None
        
    
def draw_rectangle_between_markers(corners,ids, frame):
    if len(ids) >= 2:
        # Get outer points of the first two detected markers
        points = get_outer_points(corners)


        # Create rectangles using the outer points of both markers
        rect1 = np.array([points[0], points[1], points[2], points[3]], np.int32)

        # Concatenate the rectangles to form a single rectangle
        x_values = points[0,0], points[1,0], points[2,0], points[3,0]

        # Draw a rectangle between the outer points of the two markers
        cv2.polylines(frame, [rect1], True, (0, 255, 0), 2)
        x_min, x_max = min(x_values), max(x_values)

        # Check if the point is inside the rectangle
        x = 320
        if x_min <= x <= x_max:
            Hit = True
        else:
            Hit = False
            ids = 0
        return Hit, ids

def get_outer_points(corners):
    points = np.zeros((4, 2), dtype=np.float32)
    corners1 = corners[0][0]
    corners2 = corners[1][0]
    distance_1_to_4 = np.linalg.norm(corners1[0] - corners2[1])
    distance_4_to_1 = np.linalg.norm(corners2[0] - corners1[1])
    
    if distance_1_to_4 <= distance_4_to_1:
        points[0] = corners1[1]
        points[1] = corners1[2]
        points[2] = corners2[3]
        points[3] = corners2[0]
    else:
        points[0] = corners2[1]
        points[1] = corners2[2]
        points[2] = corners1[3]
        points[3] = corners1[0]
    return points

def Check_for_hit(file_path):
    # Open the camera
    
    frame = cv2.imread(file_path)
        
    # Convert the frame to grayscale (ArUco detection works on grayscale images)
    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    corners, ids, rejected = aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
    # Detect markers
    #corners, ids, rejected = aruco.detectMarkers(gray, aruco_dict, parameters=parameters)
    
    if ids is not None:
        # Draw markers and rectangles
        aruco.drawDetectedMarkers(frame, corners, ids)
        Hit, tank_ID = draw_rectangle_between_markers(corners, ids, frame)
    else:
        Hit = False
        tank_ID = 0

        # Exit when 'q' is pressed
    
    # Release the camera and close OpenCV windows
    return(Hit,tank_ID)