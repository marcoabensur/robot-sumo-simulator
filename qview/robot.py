class Robot:

    def __init__(self, x, y):

        self.angle = 0
        self.robot_pos_x = x
        self.robot_pos_y = y
        self.mot_esq = 0
        self.mot_dir = 0

    def set_motors(self, vel_esq, vel_dir):
        self.mot_esq = vel_esq
        self.mot_dir = vel_dir

    def get_motors(self):
        return (self.mot_esq, self.mot_dir)
    
    def set_position(self, x, y):
        self.robot_pos_x = x
        self.robot_pos_y = y
    
    def offset_position(self, delta_x, delta_y):
        self.robot_pos_x += delta_x
        self.robot_pos_y += delta_y

    def get_position(self):
        return (self.robot_pos_x, self.robot_pos_y)
    
    def set_angle(self, angle):
        self.angle = angle

    def get_angle(self):
        return self.angle