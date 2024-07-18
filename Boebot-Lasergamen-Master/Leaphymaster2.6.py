from flask import Flask, request, redirect, jsonify, render_template
import json
import Hit_detection
import os
import socket
import random
import threading
import time
from datetime import datetime, timedelta

#---- Game config ----
Game_mode = 'None'
Hit_points = True

Game_start = False
#Global team scores
Team_blue_score = 0
Team_red_score = 0

# Optional: A list to keep track of IP addresses
client_ips = []
Game_state_curr = {}

# Global variable to store the game end time
game_end_time = None

class Game_state:
    def __init__(self, Mac_address, IP, Team='Not assigned', Team_color='Not assigned', Team_score=0, Enemy_team_score=0, Hit_points=10, Hits=0):
        self.Mac_address = Mac_address
        self.IP = IP
        self.Team = Team
        self.Team_color = Team_color
        self.Team_score = Team_score
        self.Enemy_team_score = Enemy_team_score
        self.Hit_points = Hit_points
        self.Hits = Hits

    def get_all(self):
        return {
            "Mac_address": self.Mac_address,
            "IP": self.IP,
            "Team": self.Team,
            "Team_color": self.Team_color,
            "Team_score": self.Team_score,
            "Enemy_team_score": self.Enemy_team_score,
            "Hit_points": self.Hit_points,
            "Hits": self.Hits
        }

def send_string(String, Leaphy_mac):
    Recieving_Leaphy = Game_state_curr[Leaphy_mac]
    send_ip = Recieving_Leaphy.IP
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        sock.connect((send_ip, 5005))  # Assuming the ESP32s listen on port 5005
        sock.sendall(String.encode())
        print(f"Sent message: {String} to {Leaphy_mac}")
    finally:
        sock.close()

def get_mac_from_IP(ip_address):
    for mac_address, game_data in Game_state_curr.items():
        if game_data.IP == ip_address:
            return mac_address
    return None

def Update_game_state(Leaphy_ip, Leaphy_ID):  # Update_game_state(<Leaphy who shot>, <Leaphy who got hit>)
    Shot_send_mac = get_mac_from_IP(Leaphy_ip)
    Leaphy_Hit(Shot_send_mac)
    Shot_recieve_mac = Hit_detection.get_mac_from_ID(Leaphy_ID)
    Leaphy_Got_Hit(Shot_recieve_mac)

def Leaphy_Got_Hit(Leaphy_mac):
    if Leaphy_mac in Game_state_curr:  
        if Game_mode == 'Free For All':
            if Hit_points == True:
                Leaphy_Got_Hit = Game_state_curr[Leaphy_mac]
                Leaphy_Got_Hit.Hit_points -= 1
                if Leaphy_Got_Hit.Hit_points == 0:
                    send_string('End game', Leaphy_mac)

def Leaphy_Hit(Leaphy_mac):
    if Leaphy_mac in Game_state_curr:
        Hit_Leaphy = Game_state_curr[Leaphy_mac]
        Hit_Leaphy.Hits += 1  # Increment the hits

        if Game_mode == 'Team Deathmatch':  # Check if the game mode is team deathmatch
            global Team_blue_score, Team_red_score 
            if Hit_Leaphy.Team == 'Blue':
                Team_blue_score += 1  # Correctly increment the score
            elif Hit_Leaphy.Team == 'Red': 
                Team_red_score += 1
    else:
        print("Leaphy not found in game state.")

app = Flask(__name__)

@app.route('/')
def home():
    return "Dit is de server van de Leaphy Lasergame!"

# Folder where uploaded images will be stored
UPLOAD_FOLDER = '/home/luclandman/Desktop/Temp'
if not os.path.exists(UPLOAD_FOLDER):
    os.makedirs(UPLOAD_FOLDER)
app.config['UPLOAD_FOLDER'] = UPLOAD_FOLDER

@app.route('/startup', methods=['POST'])
def startup():
    if 'mac_address' not in request.files:
        return jsonify({"error": "No mac_address part in the request"}), 401
    Leaphy_ip = request.remote_addr
    mac_address_file = request.files['mac_address']
    mac_address = mac_address_file.read().decode('utf-8').strip()
    if mac_address:
        if mac_address in Game_state_curr:
            Game_data = Game_state_curr[mac_address]
        else:
            Game_data = Game_state(mac_address,Leaphy_ip)
            Game_state_curr[mac_address] = Game_data
            
        if Game_start == False:
            print('Registered Mac_address is:', Game_data.Mac_address, "\n Registered IP is:", Game_data.IP)
            return jsonify({"message": f"Registered MAC {Game_data.Mac_address} for IP {Game_data.IP}.","mac_address": f"{Game_data.Mac_address}","IP": f"{Game_data.IP}"}), 200
        else:
            print('No registrations allowed while game is running')
            return jsonify({"error": "No registrations allowed while game is running"}), 400   
    else:
        return jsonify({"error": "No mac_address in file"}), 400

@app.route('/shoot', methods=['POST'])
def upload_file():
    if 'image' not in request.files:
        return jsonify({"error": "No image part in the request"}), 400
    
    file = request.files['image']
    if file.filename == '':
        return jsonify({"error": "No selected file"}), 400

    Leaphy_ip = request.remote_addr

    filename = Leaphy_ip
    save_path = os.path.join(app.config['UPLOAD_FOLDER'], filename)
    file.save(save_path)

    Hit,Leaphy_ID = Hit_detection.Check_for_hit(save_path)
    if Hit is None:
        return jsonify({"error": "Error processing the image"}), 500
    elif Hit:
        Update_game_state(Leaphy_ip,Leaphy_ID)
        return jsonify({"message": "Image uploaded successfully and hit!", "filename": filename, "client_ip": Leaphy_ip}), 200
    else:
        return jsonify({"message": "Image uploaded successfully but missed!", "filename": filename, "client_ip": Leaphy_ip}), 201

@app.route('/scoreboard')
def scoreboard():
    global Game_mode
    if Game_mode == 'Free For All':
        scores = {game_data.Mac_address: game_data.Hits for game_data in Game_state_curr.values()}
        return render_template('FFA_scoreboard.html', scores=scores)
    elif Game_mode == 'Team Deathmatch':
        scores = {'scoreTeam1': Team_blue_score, 'scoreTeam2': Team_red_score}
        return render_template('TD_scoreboard.html', scores=scores)
    else:
        print('Error: Game mode selection failed')

@app.route('/latest_scores')
def latest_scores():
    global Game_mode
    if Game_mode == 'Free For All':
        scores = {game_data.Mac_address: game_data.Hits for game_data in Game_state_curr.values()}
    else:
        scores = {'scoreTeam1': Team_blue_score, 'scoreTeam2': Team_red_score}
    return jsonify(scores)

@app.route('/Bluehit')
def blue_hit():
    global Team_blue_score
    Team_blue_score = Team_blue_score + 1
    return "Blue hit!"
    
@app.route('/Redhit')
def red_hit():
    global Team_red_score
    Team_red_score = Team_red_score + 1
    return "Red hit!"   

@app.route('/start')
def start():
    # Render an HTML form for starting the game
    return render_template('start_game.html')

@app.route('/countdown')
def countdown():
    return render_template('countdown.html')

@app.route('/gamestate/<username>', methods=['GET'])
def get_user_data(username):
    print(username)
    game_data = Game_state_curr.get(username)  # This line is slightly modified for safe access
    if game_data:
        data_to_jsonify = game_data.get_all()  # Get the data as a dictionary
        json_string = json.dumps(data_to_jsonify, indent=4)
        print (json_string)
        return jsonify(data_to_jsonify), 200  # This line is modified to correctly return JSON
    else:
        print ("error: user not found")
        return jsonify({"error": "User not found"}), 404

@app.route('/start_game', methods=['POST'])
def start_game():
    global Game_mode, Game_start, game_end_time

    # Retrieve game mode and duration from the form
    Game_mode = request.form.get('game_mode')
    duration = request.form.get('duration')

    # Validate the retrieved values
    if not Game_mode or not duration:
        return "Error: Missing game mode or duration", 400

    try:
        duration = int(duration)
    except ValueError:
        return "Error: Duration must be an integer", 401

    if Game_mode == 'Team Deathmatch':
        TD_init()
    elif Game_mode == 'Free For All':
        FFA_init()
    elif Game_mode == 'None':
        return "Error: No game mode selected", 402

    # Notify each device that the game has started
    for mac_address, game_data in Game_state_curr.items():
        send_string("Game started", mac_address)
        print('Game started for:', mac_address)

    Game_start = True

    # Calculate and store the end time
    game_end_time = datetime.now() + timedelta(minutes=duration)

    # Start the game timer in a new thread
    threading.Thread(target=game_timer).start()

    print(f"Starting game with mode: {Game_mode}, for duration: {duration} minutes.")
    return redirect('/countdown')

# --------Game modes----------
def TD_init():
    global Team_blue_score, Team_red_score

    # Reset the team scores and players counts back to 0
    Team_blue_score = 0
    Team_red_score = 0
    Team_blue_count = 0
    Team_red_count = 0

    # Check the amount of connected devies and shuffle fairly
    devices = list(Game_state_curr.items())
    random.shuffle(devices)

    for mac_address, game_data in devices:
        if Team_blue_count < Team_red_count:
            game_data.Team = 'Blue'
            game_data.Team_color = 'blue'
            Team_blue_count += 1
        else:
            game_data.Team = 'Red'
            game_data.Team_color = 'red'
            Team_red_count += 1

def FFA_init():
    predefined_hit_points = 10  # Set your predefined hit points value here

    for mac_address, game_data in Game_state_curr.items():
        game_data.Team = 'FFA'
        game_data.Hits = 0
        if Hit_points:
            game_data.Hit_points = predefined_hit_points

# New function to handle the game timer
def game_timer():
    global Game_start, game_end_time
    while Game_start and datetime.now() < game_end_time:
        time.sleep(1)  # Sleep for a second and then check the time again

    Game_start = False

    # Send a message to each device that the game is over
    for mac_address, game_data in Game_state_curr.items():
        send_string("Reset", mac_address)
        print(f"Reset for: {mac_address}")

@app.route('/remaining_time')
def remaining_time():
    if not game_end_time:
        return jsonify({"error": "Game has not started"}), 400

    remaining = game_end_time - datetime.now()
    remaining_seconds = int(remaining.total_seconds())
    return jsonify({"remaining_time": remaining_seconds})

@app.route('/ESP_DEBUG', methods=['POST'])
def esp_debug():
    if 'message' not in request.files and 'message' not in request.form:
        return jsonify({"error": "No message part in the request"}), 400
    
    if 'message' in request.files:
        message_file = request.files['message']
        message = message_file.read().decode('utf-8').strip()
    else:
        message = request.form['message']
    
    print(f"ESP_DEBUG message: {message}")
    return jsonify({"message": "Debug message received"}), 200

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
