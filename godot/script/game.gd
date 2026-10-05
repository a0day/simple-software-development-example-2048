extends Node2D

var nums = Nums.new()
@onready var grid := $board/grid
var direction ={
	"left": Nums.Dir.LEFT,
	"right":Nums.Dir.RIGHT,
	"up": Nums.Dir.UP,
	"down": Nums.Dir.DOWN,
}
var state = globalconfig.State.playing
func _ready() -> void:
	$overlay.restart_requested.connect(reset_game)
	grid.refresh(nums)

func reset_game() -> void:
	nums.reset()
	state = globalconfig.State.playing
	grid.refresh(nums)
	$overlay.visible = false

func _unhandled_input(event: InputEvent) -> void:
	if event.is_action_pressed("reset"):
		reset_game()
		return

	if state != globalconfig.State.playing :
		return

	var dir = null
	for action in direction:
		if event.is_action_pressed(action):
			dir = direction.get(action)
			break

	if dir != null && nums.move(dir):
		nums.spawn()
		grid.refresh(nums)
		if nums.isWin():
			state = globalconfig.State.win
			print("you win")
			$overlay.show_result(state)
		elif nums.isLose():
			state = globalconfig.State.lose
			print("you lose")
			$overlay.show_result(state)
