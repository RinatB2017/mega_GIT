extends Node2D

var accum=0
const PAD_SPEED = 250
const FIELD = 1
var screen_size
var map_w
var map_h

func create_map():
	for y in range(0, map_h):
		for x in range(0, map_w):
			get_node("TileMap").set_cell(x, y, 0);

	for y in range(1, map_h - 1):
		for x in range(1, map_w - 1):
			get_node("TileMap").set_cell(x, y, 1);

func _ready():
	screen_size = get_viewport_rect().size
	map_w = screen_size.width / 32
	map_h = screen_size.height / 32

	set_process(true)

func _check_collision(pos):
	var c0 = get_node("TileMap").get_cell((pos.x - 14) / 32, (pos.y - 14) / 32)
	var c1 = get_node("TileMap").get_cell((pos.x + 14) / 32, (pos.y - 14) / 32)
	var c2 = get_node("TileMap").get_cell((pos.x - 14) / 32, (pos.y + 14) / 32)
	var c3 = get_node("TileMap").get_cell((pos.x + 14) / 32, (pos.y + 14) / 32)
	
	if c0 == FIELD and c1 == FIELD and c2 == FIELD and c3 == FIELD:
		return false
	return true

func _process(delta):
	accum+=delta;

	if(Input.is_action_pressed("ui_cancel")):
		get_tree().quit();

	var pos = get_node("TileMap").get_node("player").get_pos();
	#print("pos_x ", pos.x);

	if(Input.is_action_pressed("ui_left")):
		pos.x -= PAD_SPEED*delta;
		
	if(Input.is_action_pressed("ui_right") and pos.x < screen_size.width):
		pos.x += PAD_SPEED*delta;
		
	if(Input.is_action_pressed("ui_up")):
		pos.y -= PAD_SPEED*delta;
		
	if(Input.is_action_pressed("ui_down") and pos.y < screen_size.height):
		pos.y += PAD_SPEED*delta;
		
	var c0 = get_node("TileMap").get_cell((pos.x - 14) / 32, (pos.y - 14) / 32)
	var c1 = get_node("TileMap").get_cell((pos.x + 14) / 32, (pos.y - 14) / 32)
	var c2 = get_node("TileMap").get_cell((pos.x - 14) / 32, (pos.y + 14) / 32)
	var c3 = get_node("TileMap").get_cell((pos.x + 14) / 32, (pos.y + 14) / 32)
	
	if _check_collision(pos) == false :
		get_node("TileMap").get_node("player").set_pos(pos);
