# global config
extends Node
const Cell : int = 90
const gap : int = 10
const N : int = 410
const GRID_COUNT : int = 4

static func get_position(row: int, col: int) -> Vector2: 
	var x = col * globalconfig.Cell + (col+1) * globalconfig.gap
	var y = row * globalconfig.Cell + (row+1)* globalconfig.gap
	return Vector2(x,y)

enum State {
	playing,
	win,
	lose
}
