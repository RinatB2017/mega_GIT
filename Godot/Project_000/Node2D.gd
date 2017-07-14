extends Node2D

# member variables here, example:

# var a=2
# var b=textvar"

var x = 0

func _on_button_pressed():
	var text = 	"HELLO! " + str(x);
	get_node("Label").set_text(text)
	x = x + 10

func _ready():
	get_node("Button").connect("pressed",self,"_on_button_pressed")
