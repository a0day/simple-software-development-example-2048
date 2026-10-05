extends Control

var number = {
	2: preload("res://asset/pieces/2.svg"),
	4: preload("res://asset/pieces/4.svg"),
	8: preload("res://asset/pieces/8.svg"),
	16: preload("res://asset/pieces/16.svg"),
	32: preload("res://asset/pieces/32.svg"),
	64: preload("res://asset/pieces/64.svg"),
	128: preload("res://asset/pieces/128.svg"),
	256: preload("res://asset/pieces/256.svg"),
	512: preload("res://asset/pieces/512.svg"),
	1024: preload("res://asset/pieces/1024.svg"),
	2048: preload("res://asset/pieces/2048.svg"),
}

func refresh(nums: Nums) -> void :
	_clear()
	for row in range(globalconfig.GRID_COUNT):
		for col in range(globalconfig.GRID_COUNT):
			var thisnum = nums.get_cell(row,col)
			if thisnum == 0:
				continue
			var thistile = TextureRect.new()
			thistile.size = Vector2(globalconfig.Cell,globalconfig.Cell)
			thistile.position = globalconfig.get_position(row,col)
			thistile.expand_mode = TextureRect.EXPAND_IGNORE_SIZE
			thistile.texture = number.get(thisnum)
			add_child(thistile)
	return

func _clear() -> void:
	for child in get_children():
		child.free()
	return
