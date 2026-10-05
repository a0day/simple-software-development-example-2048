extends CanvasLayer

signal restart_requested

func _ready() -> void:
	visible = false

func show_result(state : globalconfig.State) -> void :
	if state == globalconfig.State.win:
		$finaltex.text = "you win!"
	elif state == globalconfig.State.lose:
		$finaltex.text = "you lose!"
	else:
		return
	visible = true


func _on_button_pressed() -> void:
	restart_requested.emit()
