led_stripe_array = []

def led_stripe_init(qview_led_stripe):

    global led_stripe_array

    led_stripe_array = [
        # Creates Led Stripe
        qview_led_stripe.canvas.create_rectangle(120, 5, 140, 20, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(150, 5, 170, 20, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(180, 5, 200, 20, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(210, 5, 230, 20, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(240, 5, 260, 20, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(270, 5, 290, 20, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(300, 5, 320, 20, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(330, 5, 350, 20, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(120, 25, 140, 40, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(150, 25, 170, 40, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(180, 25, 200, 40, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(210, 25, 230, 40, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(240, 25, 260, 40, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(270, 25, 290, 40, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(300, 25, 320, 40, outline = "black", fill = '#000000', width = 1),
        qview_led_stripe.canvas.create_rectangle(330, 25, 350, 40, outline = "black", fill = '#000000', width = 1),
    ]




def led_stripe_set(qview_led_stripe, num, r, g, b):

    r_string = "{:02x}".format(r)
    g_string = "{:02x}".format(g)
    b_string = "{:02x}".format(b)

    color = '#' + r_string + g_string + b_string
    qview_led_stripe.canvas.itemconfig(led_stripe_array[num], fill = color)

def led_stripe_set_all(qview_led_stripe, r, g, b):

    r_string = "{:02x}".format(r)
    g_string = "{:02x}".format(g)
    b_string = "{:02x}".format(b)

    color = '#' + r_string + g_string + b_string
    for led in led_stripe_array:
        qview_led_stripe.canvas.itemconfig(led, fill = color)
    
def led_stripe_set_half(qview_led_stripe, is_left, r, g, b):

    r_string = "{:02x}".format(r)
    g_string = "{:02x}".format(g)
    b_string = "{:02x}".format(b)

    color = '#' + r_string + g_string + b_string

    if (is_left == True):
        for led in led_stripe_array[0:8]:
            qview_led_stripe.canvas.itemconfig(led, fill = color)
    else:
        for led in led_stripe_array[8:16]:
            qview_led_stripe.canvas.itemconfig(led, fill = color)