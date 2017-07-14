extends Node2D

var accum=0
var screen_size
const PAD_SPEED = 250
const BORDER = 50

var player_w
var player_h

func _ready():
	screen_size = get_viewport_rect().size
	print(screen_size.width)
	print(screen_size.height)
	
	set_process(true)

func _process(delta):
	accum+=delta;
	get_node("Label").set_text(str(accum));

	if(Input.is_action_pressed("ui_cancel")):
		get_tree().quit();

	var pos = get_node("TileMap").get_node("Sprite_0").get_pos();
	#print("pos_x ", pos.x);

	if(Input.is_action_pressed("ui_left") and pos.x > BORDER):
		pos.x -= PAD_SPEED*delta;
		
	if(Input.is_action_pressed("ui_right") and pos.x < screen_size.width - BORDER):
		pos.x += PAD_SPEED*delta;
		
	if(Input.is_action_pressed("ui_up") and pos.y > BORDER):
		pos.y -= PAD_SPEED*delta;
		
	if(Input.is_action_pressed("ui_down") and pos.y < screen_size.height - BORDER):
		pos.y += PAD_SPEED*delta;
		
	get_node("TileMap").get_node("Sprite_0").set_pos(pos);
	